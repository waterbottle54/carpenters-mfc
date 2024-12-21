#pragma once
#include "afxwin.h"


// CDialogSurface 대화 상자입니다.

class CDialogSurface : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSurface)

protected:
	CString m_strName;
	CString m_strRed;
	CString m_strGreen;
	CString m_strBlue;
	CString m_strEX;
	CString m_strEY;
	CString m_strEZ;
	CString m_strUMin;
	CString m_strUMax;
	CString m_strUNum;
	CString m_strVMin;
	CString m_strVMax;
	CString m_strVNum;
	CString m_strMat11;
	CString m_strMat12;
	CString m_strMat13;
	CString m_strMat14;
	CString m_strMat21;
	CString m_strMat22;
	CString m_strMat23;
	CString m_strMat24;
	CString m_strMat31;
	CString m_strMat32;
	CString m_strMat33;
	CString m_strMat34;
	CString m_strMat41;
	CString m_strMat42;
	CString m_strMat43;
	CString m_strMat44;
	CString m_strOrgX;
	CString m_strOrgY;
	CString m_strOrgZ;

public:
	void SetSurfaceCreationInfo
	(
		LPCWSTR x, LPCWSTR y, LPCWSTR z,
		double u_min, double u_max, double v_min, double v_max, 
		int u_num, int v_num, double org[3], double mat[16], COLORREF clr
	);
	void GetSurfaceCreationInfo
	(
		CString& x, CString& y, CString& z,
		double& u_min, double& u_max, double& v_min, double& v_max,
		int& u_num, int& v_num, double org[3], double mat[16], COLORREF& clr
	) const;

public:
	CDialogSurface(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSurface();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SURFACE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
};
