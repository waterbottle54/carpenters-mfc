
// CarpenterDoc.cpp : CCarpenterDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Carpenter.h"
#endif

#include "CarpenterDoc.h"
#include <propkey.h>
#include "includes\expression.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// surface function defined by runtime expression

void Graph(double u, double v, double point[3], void* param)
{
	CString*	astr;
	double		result;
	bool		valid;
	int			i;

	astr = (CString*)param;
	for (i = 0; i < 3; i++)
	{
		result = function(astr[i], &valid, L"uv", u, v);
		point[i] = valid ? result : 0;
	}
}



// CCarpenterDoc

IMPLEMENT_DYNCREATE(CCarpenterDoc, CDocument)

BEGIN_MESSAGE_MAP(CCarpenterDoc, CDocument)
END_MESSAGE_MAP()



// CCarpenterDoc 생성/소멸

CCarpenterDoc::CCarpenterDoc()
{
}

CCarpenterDoc::~CCarpenterDoc()
{
}

void CCarpenterDoc::DeleteContents()
{
	// clear surface list
	SelectAllSurface();
	RemoveSurface();

	CDocument::DeleteContents();
}

BOOL CCarpenterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	return TRUE;
}

BOOL CCarpenterDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}



// CCarpenterDoc serialization

void CCarpenterDoc::Serialize(CArchive& ar)
{
	POSITION	pos;
	int			size, i;
	CString		ex, ey, ez;
	double		u_0, u_1;
	double		v_0, v_1;
	int			n_u, n_v;
	double		org[3];
	double		mat[16];
	COLORREF	clr;

	if (ar.IsStoring())
	{
		ar << m_surfaceList.GetSize();
		pos = m_surfaceList.GetHeadPosition();
		for (; pos != NULL; m_surfaceList.GetNext(pos))
		{
			GetSurfaceCreationInfo(pos, ex, ey, ez, u_0, u_1, v_0, v_1, n_u, n_v, org, mat, clr);
			ar << ex << ey << ez << u_0 << u_1 << v_0 << v_1 << n_u << n_v << clr;
			ar.Write(org, sizeof(org));
			ar.Write(mat, sizeof(mat));
		}
	}
	else
	{
		ar >> size;
		for (i = 0; i < size; i++)
		{
			ar >> ex >> ey >> ez >> u_0 >> u_1 >> v_0 >> v_1 >> n_u >> n_v >> clr;
			ar.Read(org, sizeof(org));
			ar.Read(mat, sizeof(mat));
			AddSurface(ex, ey, ez, u_0, u_1, v_0, v_1, n_u, n_v, org, mat, clr);
		}
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CCarpenterDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CCarpenterDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CCarpenterDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS



// CCarpenterDoc 진단

#ifdef _DEBUG
void CCarpenterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCarpenterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG



// CCarpenterDoc 명령

void CCarpenterDoc::AddSurface
(
	LPCWSTR ex, LPCWSTR ey, LPCWSTR ez,
	double u_0, double u_1, double v_0, double v_1,
	int n_u, int n_v, double org[3], double mat[16], COLORREF clr
)
{
	SURFACE_PAIR sp;

	// set surface creation info
	sp.sci.func = Graph;
	sp.sci.param = new CString[3];
	((CString*)sp.sci.param)[0] = ex;
	((CString*)sp.sci.param)[1] = ey;
	((CString*)sp.sci.param)[2] = ez;
	sp.sci.u_0 = u_0; sp.sci.u_1 = u_1;
	sp.sci.v_0 = v_0; sp.sci.v_1 = v_1;
	sp.sci.n_u = n_u; sp.sci.n_v = n_v;
	memcpy(sp.sci.org, org, sizeof(sp.sci.org));
	memcpy(sp.sci.mat, mat, sizeof(sp.sci.mat));
	sp.sci.clr = clr;

	// create surface using its creation info
	sp.hsur = CreateSurface(sp.sci);

	// add surface and its creation info to list
	m_surfaceList.AddTail(sp);
}

void CCarpenterDoc::EditSurface
(
	POSITION pos,
	LPCWSTR ex, LPCWSTR ey, LPCWSTR ez,
	double u_0, double u_1, double v_0, double v_1,
	int n_u, int n_v, double org[3], double mat[16], COLORREF clr
)
{
	SURFACE_PAIR* psp;

	if (pos != NULL)
	{
		// get pointer of surface pair from list
		psp = &m_surfaceList.GetAt(pos);

		// modify surface creation info
		((CString*)psp->sci.param)[0] = ex;
		((CString*)psp->sci.param)[1] = ey;
		((CString*)psp->sci.param)[2] = ez;
		psp->sci.u_0 = u_0; psp->sci.u_1 = u_1;
		psp->sci.v_0 = v_0; psp->sci.v_1 = v_1;
		psp->sci.n_u = n_u; psp->sci.n_v = n_v;
		memcpy(psp->sci.org, org, sizeof(psp->sci.org));
		memcpy(psp->sci.mat, mat, sizeof(psp->sci.mat));
		psp->sci.clr = clr;

		// re-create surface using modified creation info
		DeleteSurface(psp->hsur);
		psp->hsur = CreateSurface(psp->sci);
	}
}

void CCarpenterDoc::CopySurface()
{
	POSITION pos_sel, pos_sur;
	SURFACE_CREATION_INFO sci;

	pos_sel = m_selectList.GetHeadPosition();
	while (pos_sel != NULL)
	{
		// get creation info of selected surface
		pos_sur = m_selectList.GetAt(pos_sel);
		sci = m_surfaceList.GetAt(pos_sur).sci;

		// add new surface using the creation info 
		AddSurface(
			((CString*)sci.param)[0], ((CString*)sci.param)[1], ((CString*)sci.param)[2],
			sci.u_0, sci.u_1, sci.v_0, sci.v_1, sci.n_u, sci.n_v, sci.org, sci.mat, sci.clr);

		// alter selection to new surface
		m_selectList.GetAt(pos_sel) = m_surfaceList.GetTailPosition();
		m_selectList.GetNext(pos_sel);
	}
}

void CCarpenterDoc::RemoveSurface()
{
	POSITION pos;
	SURFACE_PAIR sp;

	while (!m_selectList.IsEmpty())
	{
		// get selected surface
		pos = m_selectList.GetHead();
		sp = m_surfaceList.GetAt(pos);

		// unselect the surface
		m_selectList.RemoveHead();

		// delete the surface and its creation info
		DeleteSurface(sp.hsur);
		delete[](CString*)sp.sci.param;

		// remove them from list
		m_surfaceList.RemoveAt(pos);
	}
}


void CCarpenterDoc::SelectSurface(POSITION pos)
{
	if (pos != NULL)
		m_selectList.AddTail(pos);
}

void CCarpenterDoc::SelectAllSurface()
{
	POSITION pos_sur;
	m_selectList.RemoveAll();
	pos_sur = m_surfaceList.GetHeadPosition();
	while (pos_sur != NULL)
	{
		m_selectList.AddTail(pos_sur);
		m_surfaceList.GetNext(pos_sur);
	}
}

void CCarpenterDoc::UnselectSurface(POSITION pos)
{
	if (pos != NULL)
	{
		if (m_selectList.Find(pos))
			m_selectList.RemoveAt(m_selectList.Find(pos));
	}
}

void CCarpenterDoc::UnselectAllSurface()
{
	m_selectList.RemoveAll();
}


void CCarpenterDoc::TranslateSurface(double x, double y, double z)
{
	POSITION				pos_sel;
	POSITION				pos_sur;
	SURFACE_PAIR*			psp;
	double					mat[16];

	pos_sel = m_selectList.GetHeadPosition();
	while (pos_sel != NULL)
	{
		pos_sur = m_selectList.GetNext(pos_sel);
		psp = &m_surfaceList.GetAt(pos_sur);

		TranslationMatrix(mat, x, y, z);
		TransformSurface(psp->hsur, mat);
		MultiplyMatrix(mat, psp->sci.mat, psp->sci.mat, true);
		MultiplyMatrix(mat, psp->sci.org, psp->sci.org, false);
	}
}

void CCarpenterDoc::RotateSurface(double angle, char axis, bool centered)
{
	POSITION				pos_sel;
	POSITION				pos_sur;
	SURFACE_PAIR*			psp;
	double					mat[16];
	double					org[3];



	pos_sel = m_selectList.GetHeadPosition();
	while (pos_sel != NULL)
	{
		pos_sur = m_selectList.GetNext(pos_sel);
		psp = &m_surfaceList.GetAt(pos_sur);
		GetSurfaceOrigin(psp->hsur, org);

		if (centered == true) RotationMatrix(mat, 0, 0, 0, angle, axis);
		else RotationMatrix(mat, org[0], org[1], org[2], angle, axis);
		TransformSurface(psp->hsur, mat);
		MultiplyMatrix(mat, psp->sci.mat, psp->sci.mat, true);
		MultiplyMatrix(mat, psp->sci.org, psp->sci.org, false);
	}
}

void CCarpenterDoc::ScaleSurface(double x, double y, double z, bool centered)
{
	POSITION				pos_sel;
	POSITION				pos_sur;
	SURFACE_PAIR*			psp;
	double					mat[16];
	double					org[3];

	pos_sel = m_selectList.GetHeadPosition();
	while (pos_sel != NULL)
	{
		pos_sur = m_selectList.GetNext(pos_sel);
		psp = &m_surfaceList.GetAt(pos_sur);
		GetSurfaceOrigin(psp->hsur, org);

		if (centered == true) ScalingMatrix(mat, 0, 0, 0, x, y, z);
		else ScalingMatrix(mat, org[0], org[1], org[2], x, y, z);
		TransformSurface(psp->hsur, mat);
		MultiplyMatrix(mat, psp->sci.mat, psp->sci.mat, true);
		MultiplyMatrix(mat, psp->sci.org, psp->sci.org, false);
	}
}

void CCarpenterDoc::TranslateAllSurface(double x, double y, double z)
{
	POSITION				pos;
	SURFACE_PAIR*			psp;
	double					mat[16];

	pos = m_surfaceList.GetHeadPosition();
	while (pos != NULL)
	{
		psp = &m_surfaceList.GetNext(pos);
		TranslationMatrix(mat, x, y, z);
		TransformSurface(psp->hsur, mat);
		MultiplyMatrix(mat, psp->sci.mat, psp->sci.mat, true);
		MultiplyMatrix(mat, psp->sci.org, psp->sci.org, false);
	}
}

void CCarpenterDoc::RotateAllSurface(double angle, char axis)
{
	POSITION				pos;
	SURFACE_PAIR*			psp;
	double					mat[16];

	pos = m_surfaceList.GetHeadPosition();
	while (pos != NULL)
	{
		psp = &m_surfaceList.GetNext(pos);
		RotationMatrix(mat, 0, 0, 0, angle, axis);
		TransformSurface(psp->hsur, mat);
		MultiplyMatrix(mat, psp->sci.mat, psp->sci.mat, true);
		MultiplyMatrix(mat, psp->sci.org, psp->sci.org, false);
	}
}

void CCarpenterDoc::ScaleAllSurface(double x, double y, double z)
{
	POSITION				pos;
	SURFACE_PAIR*			psp;
	double					mat[16];

	pos = m_surfaceList.GetHeadPosition();
	while (pos != NULL)
	{
		psp = &m_surfaceList.GetNext(pos);
		ScalingMatrix(mat, 0, 0, 0, x, y, z);
		TransformSurface(psp->hsur, mat);
		MultiplyMatrix(mat, psp->sci.mat, psp->sci.mat, true);
		MultiplyMatrix(mat, psp->sci.org, psp->sci.org, false);
	}
}


POSITION CCarpenterDoc::SearchSurface(double x, double y, double z)
{
	POSITION pos;
	HSURFACE hsur;

	// search which surface indicated by given point
	pos = m_surfaceList.GetHeadPosition();
	for (; pos != NULL; m_surfaceList.GetNext(pos))
	{
		hsur = m_surfaceList.GetAt(pos).hsur;
		if (PtOnSurface(hsur, x, y, z, 0.2))
			return pos;
	}
	return NULL;
}


