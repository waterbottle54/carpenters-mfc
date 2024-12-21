
// CarpenterDoc.h : CCarpenterDoc 클래스의 인터페이스
//


#pragma once
#include "includes\surface.h"



class CCarpenterDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CCarpenterDoc();
	DECLARE_DYNCREATE(CCarpenterDoc)

// 특성입니다.
public:
	struct SURFACE_PAIR { HSURFACE hsur; SURFACE_CREATION_INFO sci; };
	CList<SURFACE_PAIR> m_surfaceList;
	CList<POSITION>		m_selectList;

	HSURFACE GetSurface(POSITION pos)
	{
		return m_surfaceList.GetAt(pos).hsur;
	}
	POSITION GetHeadSurfacePos()
	{
		return m_surfaceList.GetHeadPosition();
	}
	POSITION GetTailSurfacePos()
	{
		return m_surfaceList.GetTailPosition();
	}
	HSURFACE GetNextSurface(POSITION& pos)
	{
		return m_surfaceList.GetNext(pos).hsur;
	}
	void GetSurfaceCreationInfo
	(
		POSITION pos,
		CString& ex, CString& ey, CString& ez,
		double& u_0, double& u_1, double& v_0, double& v_1,
		int& n_u, int& n_v, double org[3], double mat[16], COLORREF& clr
	)
	{
		SURFACE_CREATION_INFO sci;
		if (pos != NULL)
		{
			sci = m_surfaceList.GetAt(pos).sci;
			ex = ((CString*)sci.param)[0];
			ey = ((CString*)sci.param)[1];
			ez = ((CString*)sci.param)[2];
			u_0 = sci.u_0; u_1 = sci.u_1;
			v_0 = sci.v_0; v_1 = sci.v_1;
			n_u = sci.n_u; n_v = sci.n_v;
			memcpy(org, sci.org, sizeof(sci.org));
			memcpy(mat, sci.mat, sizeof(sci.mat));
			clr = sci.clr;
		}
	}
	int GetSurfaceNumber()
	{
		return m_surfaceList.GetSize();
	}
	int GetSelectedSurfaceNumber()
	{
		return m_selectList.GetSize();
	}
	bool IsSelectedSurface(POSITION pos)
	{
		return (m_selectList.Find(pos) != NULL);
	}

// 작업입니다.
public:
	void AddSurface
	(
		LPCWSTR ex, LPCWSTR ey, LPCWSTR ez,
		double u_0, double u_1, double v_0, double v_1,
		int n_u, int n_v, double org[3], double mat[16], COLORREF clr
	);
	void EditSurface
	(
		POSITION pos,
		LPCWSTR ex, LPCWSTR ey, LPCWSTR ez,
		double u_0, double u_1, double v_0, double v_1,
		int n_u, int n_v, double org[3], double mat[16], COLORREF clr
	);
	void CopySurface();
	void RemoveSurface();

	void SelectSurface(POSITION pos);
	void SelectAllSurface();
	void UnselectSurface(POSITION pos);
	void UnselectAllSurface();

	void TranslateSurface(double x, double y, double z);
	void ScaleSurface(double x, double y, double z, bool centered);
	void RotateSurface(double angle, char axis, bool centered);
	void TranslateAllSurface(double x, double y, double z);
	void RotateAllSurface(double angle, char axis);
	void ScaleAllSurface(double x, double y, double z);

	POSITION SearchSurface(double x, double y, double z);

	bool BuildSurfaceDataFile(const wchar_t* filename)
	{
		POSITION pos;
		HSURFACE ahsur[1024];
		int i;

		pos = m_surfaceList.GetHeadPosition();
		for (i = 0; pos != NULL && i < 1024; i++)
			ahsur[i] = m_surfaceList.GetNext(pos).hsur;
		
		return SaveSurface(ahsur, m_surfaceList.GetSize(), filename);
	}

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CCarpenterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
