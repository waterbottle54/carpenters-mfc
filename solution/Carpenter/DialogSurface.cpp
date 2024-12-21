// DialogSurface.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Carpenter.h"
#include "DialogSurface.h"
#include "afxdialogex.h"



// CDialogSurface 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSurface, CDialogEx)

CDialogSurface::CDialogSurface(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SURFACE, pParent)
	, m_strName(_T(""))
	, m_strRed(_T(""))
	, m_strGreen(_T(""))
	, m_strBlue(_T(""))
	, m_strEX(_T(""))
	, m_strEY(_T(""))
	, m_strEZ(_T(""))
	, m_strUMin(_T(""))
	, m_strUMax(_T(""))
	, m_strUNum(_T(""))
	, m_strVMin(_T(""))
	, m_strVMax(_T(""))
	, m_strVNum(_T(""))
	, m_strMat11(_T(""))
	, m_strMat12(_T(""))
	, m_strMat13(_T(""))
	, m_strMat14(_T(""))
	, m_strMat21(_T(""))
	, m_strMat22(_T(""))
	, m_strMat23(_T(""))
	, m_strMat24(_T(""))
	, m_strMat31(_T(""))
	, m_strMat32(_T(""))
	, m_strMat33(_T(""))
	, m_strMat34(_T(""))
	, m_strMat41(_T(""))
	, m_strMat42(_T(""))
	, m_strMat43(_T(""))
	, m_strMat44(_T(""))
	, m_strOrgX(_T(""))
	, m_strOrgY(_T(""))
	, m_strOrgZ(_T(""))
{
	double org[3] = { 0, 0, 0 };
	double mat[16] =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	SetSurfaceCreationInfo(L"0", L"0", L"0", 0, 0, 0, 0, 0, 0, org, mat, RGB(0, 255, 0));
}

CDialogSurface::~CDialogSurface()
{
}

void CDialogSurface::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 32);
	DDX_Text(pDX, IDC_EDIT_RED, m_strRed);
	DDV_MaxChars(pDX, m_strRed, 3);
	DDX_Text(pDX, IDC_EDIT_GREEN, m_strGreen);
	DDV_MaxChars(pDX, m_strGreen, 3);
	DDX_Text(pDX, IDC_EDIT_BLUE, m_strBlue);
	DDV_MaxChars(pDX, m_strBlue, 3);
	DDX_Text(pDX, IDC_EDIT_X, m_strEX);
	DDV_MaxChars(pDX, m_strEX, 256);
	DDX_Text(pDX, IDC_EDIT_Y, m_strEY);
	DDV_MaxChars(pDX, m_strEY, 256);
	DDX_Text(pDX, IDC_EDIT_Z, m_strEZ);
	DDV_MaxChars(pDX, m_strEZ, 256);
	DDX_Text(pDX, IDC_EDIT_U_MIN, m_strUMin);
	DDV_MaxChars(pDX, m_strUMin, 8);
	DDX_Text(pDX, IDC_EDIT_U_MAX, m_strUMax);
	DDV_MaxChars(pDX, m_strUMax, 8);
	DDX_Text(pDX, IDC_EDIT_U_NUM, m_strUNum);
	DDV_MaxChars(pDX, m_strUNum, 8);
	DDX_Text(pDX, IDC_EDIT_V_MIN, m_strVMin);
	DDV_MaxChars(pDX, m_strVMin, 8);
	DDX_Text(pDX, IDC_EDIT_V_MAX, m_strVMax);
	DDV_MaxChars(pDX, m_strVMax, 8);
	DDX_Text(pDX, IDC_EDIT_V_NUM, m_strVNum);
	DDV_MaxChars(pDX, m_strVNum, 8);
	DDX_Text(pDX, IDC_EDIT_MAT11, m_strMat11);
	DDV_MaxChars(pDX, m_strMat11, 32);
	DDX_Text(pDX, IDC_EDIT_MAT12, m_strMat12);
	DDV_MaxChars(pDX, m_strMat12, 32);
	DDX_Text(pDX, IDC_EDIT_MAT13, m_strMat13);
	DDV_MaxChars(pDX, m_strMat13, 32);
	DDX_Text(pDX, IDC_EDIT_MAT14, m_strMat14);
	DDV_MaxChars(pDX, m_strMat14, 32);
	DDX_Text(pDX, IDC_EDIT_MAT21, m_strMat21);
	DDV_MaxChars(pDX, m_strMat21, 32);
	DDX_Text(pDX, IDC_EDIT_MAT22, m_strMat22);
	DDV_MaxChars(pDX, m_strMat22, 32);
	DDX_Text(pDX, IDC_EDIT_MAT23, m_strMat23);
	DDV_MaxChars(pDX, m_strMat23, 32);
	DDX_Text(pDX, IDC_EDIT_MAT24, m_strMat24);
	DDV_MaxChars(pDX, m_strMat24, 32);
	DDX_Text(pDX, IDC_EDIT_MAT31, m_strMat31);
	DDV_MaxChars(pDX, m_strMat31, 32);
	DDX_Text(pDX, IDC_EDIT_MAT32, m_strMat32);
	DDV_MaxChars(pDX, m_strMat32, 32);
	DDX_Text(pDX, IDC_EDIT_MAT33, m_strMat33);
	DDV_MaxChars(pDX, m_strMat33, 32);
	DDX_Text(pDX, IDC_EDIT_MAT34, m_strMat34);
	DDV_MaxChars(pDX, m_strMat34, 32);
	DDX_Text(pDX, IDC_EDIT_MAT41, m_strMat41);
	DDV_MaxChars(pDX, m_strMat41, 32);
	DDX_Text(pDX, IDC_EDIT_MAT42, m_strMat42);
	DDV_MaxChars(pDX, m_strMat42, 32);
	DDX_Text(pDX, IDC_EDIT_MAT43, m_strMat43);
	DDV_MaxChars(pDX, m_strMat43, 32);
	DDX_Text(pDX, IDC_EDIT_MAT44, m_strMat44);
	DDV_MaxChars(pDX, m_strMat44, 32);
	DDX_Text(pDX, IDC_EDIT_ORG_X, m_strOrgX);
	DDV_MaxChars(pDX, m_strOrgX, 32);
	DDX_Text(pDX, IDC_EDIT_ORG_Y, m_strOrgY);
	DDV_MaxChars(pDX, m_strOrgY, 32);
	DDX_Text(pDX, IDC_EDIT_ORG_Z, m_strOrgZ);
	DDV_MaxChars(pDX, m_strOrgZ, 32);
}



// CDialogSurface Attributes

void CDialogSurface::SetSurfaceCreationInfo
(
	LPCWSTR x, LPCWSTR y, LPCWSTR z,
	double u_min, double u_max, double v_min, double v_max, 
	int u_num, int v_num, double org[3], double mat[16], COLORREF clr
)
{
	//m_strName.SetString(name);
	m_strRed.Format(L"%d", GetRValue(clr));
	m_strGreen.Format(L"%d", GetGValue(clr));
	m_strBlue.Format(L"%d", GetBValue(clr));
	m_strEX.SetString(x);
	m_strEY.SetString(y);
	m_strEZ.SetString(z);
	m_strUMin.Format(L"%.2lf", u_min);
	m_strUMax.Format(L"%.2lf", u_max);
	m_strVMin.Format(L"%.2lf", v_min);
	m_strVMax.Format(L"%.2lf", v_max);
	m_strUNum.Format(L"%d", u_num);
	m_strVNum.Format(L"%d", v_num);
	m_strOrgX.Format(L"%.2lf", org[0]);
	m_strOrgY.Format(L"%.2lf", org[1]);
	m_strOrgZ.Format(L"%.2lf", org[2]);
	m_strMat11.Format(L"%.2lf", mat[0]);
	m_strMat12.Format(L"%.2lf", mat[1]);
	m_strMat13.Format(L"%.2lf", mat[2]);
	m_strMat14.Format(L"%.2lf", mat[3]);
	m_strMat21.Format(L"%.2lf", mat[4]);
	m_strMat22.Format(L"%.2lf", mat[5]);
	m_strMat23.Format(L"%.2lf", mat[6]);
	m_strMat24.Format(L"%.2lf", mat[7]);
	m_strMat31.Format(L"%.2lf", mat[8]);
	m_strMat32.Format(L"%.2lf", mat[9]);
	m_strMat33.Format(L"%.2lf", mat[10]);
	m_strMat34.Format(L"%.2lf", mat[11]);
	m_strMat41.Format(L"%.2lf", mat[12]);
	m_strMat42.Format(L"%.2lf", mat[13]);
	m_strMat43.Format(L"%.2lf", mat[14]);
	m_strMat44.Format(L"%.2lf", mat[15]);
}

void CDialogSurface::GetSurfaceCreationInfo
(
	CString& x, CString& y, CString& z,
	double& u_min, double& u_max, double& v_min, double& v_max, 
	int& u_num, int& v_num, double org[3], double mat[16], COLORREF& clr
) const
{
	//name = m_strName;
	clr = RGB(_wtoi(m_strRed), _wtoi(m_strGreen), _wtoi(m_strBlue));
	x.SetString(m_strEX);
	y.SetString(m_strEY);
	z.SetString(m_strEZ);
	u_min = _wtof(m_strUMin);
	u_max = _wtof(m_strUMax);
	v_min = _wtof(m_strVMin);
	v_max = _wtof(m_strVMax);
	u_num = _wtoi(m_strUNum);
	v_num = _wtoi(m_strVNum);
	org[0] = _wtof(m_strOrgX);
	org[1] = _wtof(m_strOrgY);
	org[2] = _wtof(m_strOrgZ);
	mat[0] = _wtof(m_strMat11);
	mat[1] = _wtof(m_strMat12);
	mat[2] = _wtof(m_strMat13);
	mat[3] = _wtof(m_strMat14);
	mat[4] = _wtof(m_strMat21);
	mat[5] = _wtof(m_strMat22);
	mat[6] = _wtof(m_strMat23);
	mat[7] = _wtof(m_strMat24);
	mat[8] = _wtof(m_strMat31);
	mat[9] = _wtof(m_strMat32);
	mat[10] = _wtof(m_strMat33);
	mat[11] = _wtof(m_strMat34);
	mat[12] = _wtof(m_strMat41);
	mat[13] = _wtof(m_strMat42);
	mat[14] = _wtof(m_strMat43);
	mat[15] = _wtof(m_strMat44);
}



BEGIN_MESSAGE_MAP(CDialogSurface, CDialogEx)
END_MESSAGE_MAP()



// CDialogSurface 메시지 처리기입니다.

void CDialogSurface::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	UpdateData(true);

	CDialogEx::OnOK();
}


