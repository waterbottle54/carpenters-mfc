
// CarpenterView.cpp : CCarpenterView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Carpenter.h"
#endif

#include "CarpenterDoc.h"
#include "CarpenterView.h"
#include "DialogSurface.h"
#include "includes\utility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CCarpenterView

IMPLEMENT_DYNCREATE(CCarpenterView, CView)

BEGIN_MESSAGE_MAP(CCarpenterView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_EDIT_CREATE, &CCarpenterView::OnEditCreate)
	ON_COMMAND(ID_EDIT_COPY, &CCarpenterView::OnEditCopy)
	ON_COMMAND(ID_EDIT_REMOVE, &CCarpenterView::OnEditRemove)
	ON_COMMAND(ID_EDIT_SELECT_ALL, &CCarpenterView::OnEditSelectAll)
	ON_COMMAND(ID_TRANSFORM_TRANSLATION, &CCarpenterView::OnTransformTranslation)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_TRANSLATION, &CCarpenterView::OnUpdateTransformTranslation)
	ON_COMMAND(ID_TRANSFORM_ROTATION, &CCarpenterView::OnTransformRotation)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_ROTATION, &CCarpenterView::OnUpdateTransformRotation)
	ON_COMMAND(ID_TRANSFORM_SCALING, &CCarpenterView::OnTransformScaling)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_SCALING, &CCarpenterView::OnUpdateTransformScaling)
	ON_COMMAND(ID_TRANSFORM_CENTERED, &CCarpenterView::OnTransformCentered)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_CENTERED, &CCarpenterView::OnUpdateTransformCentered)
	ON_COMMAND(ID_TRANSFORM_PRECISELY, &CCarpenterView::OnTransformPrecisely)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_PRECISELY, &CCarpenterView::OnUpdateTransformPrecisely)
	ON_COMMAND(ID_BUILD_DATA_FILE, &CCarpenterView::OnBuildDataFile)
	ON_COMMAND(ID_BUTTON_CREATE, &CCarpenterView::OnEditCreate)
	ON_COMMAND(ID_BUTTON_COPY, &CCarpenterView::OnEditCopy)
	ON_COMMAND(ID_BUTTON_REMOVE, &CCarpenterView::OnEditRemove)
	ON_COMMAND(ID_BUTTON_TRANSLATION, &CCarpenterView::OnTransformTranslation)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_TRANSLATION, &CCarpenterView::OnUpdateTransformTranslation)
	ON_COMMAND(ID_BUTTON_ROTATION, &CCarpenterView::OnTransformRotation)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ROTATION, &CCarpenterView::OnUpdateTransformRotation)
	ON_COMMAND(ID_BUTTON_SCALING, &CCarpenterView::OnTransformScaling)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SCALING, &CCarpenterView::OnUpdateTransformScaling)
	ON_COMMAND(ID_BUTTON_CENTERED, &CCarpenterView::OnTransformCentered)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CENTERED, &CCarpenterView::OnUpdateTransformCentered)
	ON_COMMAND(ID_BUTTON_PRECISELY, &CCarpenterView::OnTransformPrecisely)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_PRECISELY, &CCarpenterView::OnUpdateTransformPrecisely)
	ON_COMMAND(ID_BUTTON_BUILD, &CCarpenterView::OnBuildDataFile)
END_MESSAGE_MAP()



// CCarpenterView 생성/소멸

CCarpenterView::CCarpenterView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CCarpenterView::~CCarpenterView()
{
}

BOOL CCarpenterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

int CCarpenterView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CFrameWnd* frame;
	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat;

	// adjust position and size of frame window
	frame = GetParentFrame();
	frame->SetWindowPos(NULL, 150, 50, 1000, 650, SWP_NOZORDER);

	// get hDC of this view
	m_hDC = ::GetDC(m_hWnd);

	// set pixel format of hDC for opengl rendering
	pfd.nVersion = 0;
	pfd.nSize = sizeof(pfd);
	pfd.cColorBits = 32;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	SetPixelFormat(m_hDC, nPixelFormat, &pfd);

	// create hGLRC (rendering context of opengl)
	m_hGLRC = wglCreateContext(m_hDC);

	// make hGLRC to be current of hDC
	wglMakeCurrent(m_hDC, m_hGLRC);

	return 0;
}

void CCarpenterView::OnDestroy()
{
	CView::OnDestroy();

	// remove hGLRC from current of hDC
	wglMakeCurrent(m_hDC, NULL);
	// Delete hGLRC
	wglDeleteContext(m_hGLRC);
	// Release hDC
	::ReleaseDC(m_hWnd, m_hDC);
}

void CCarpenterView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_curSurface = NULL;
	m_tranMode = TRANSFORM_TRANSLATION;
	m_tranCentered = false;
	m_tranPrecisely = false;
}



// CCarpenterView 그리기

void CCarpenterView::OnDraw(CDC* /*pDC*/)
{
	CCarpenterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	GLfloat light[4] = { 0.0, 5.0, 0.0, 1.0 };
	GLfloat ambient[4] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat diffuse[4] = { 0.8, 0.8, 0.8, 1.0 };
	POSITION pos;
	HSURFACE sur;
	COLORREF clr;

	// enable rendering option of opengl
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// clear rendering buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set viewing matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(4, 4, 10, 0, 0, 0, 0, 1, 0);

	// set a light
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	
	// render axis
	glColor3ub(100, 255, 0);
	glBegin(GL_LINES);
	glVertex3d(-2, 0, 0);
	glVertex3d(2, 0, 0);
	glVertex3d(0, 2, 0);
	glVertex3d(0, -2, 0);
	glVertex3d(0, 0, 2);
	glVertex3d(0, 0, -2);
	glEnd();

	// render surfaces
	pos = pDoc->GetHeadSurfacePos();
	for (; pos != NULL; pDoc->GetNextSurface(pos))
	{
		sur = pDoc->GetSurface(pos);
		clr = GetSurfaceColor(sur);
		if (pos == m_curSurface)
			SetSurfaceColor(sur, RGB(96, 96, 96));
		else if (pDoc->IsSelectedSurface(pos))
			SetSurfaceColor(sur, RGB(128, 128, 128));
		RenderSurface(sur);
		SetSurfaceColor(sur, clr);
	}
	
	// flush buffer
	glFinish();
}

void CCarpenterView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	Invalidate(false);
}



// CCarpenterView 진단

#ifdef _DEBUG
void CCarpenterView::AssertValid() const
{
	CView::AssertValid();
}

void CCarpenterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCarpenterDoc* CCarpenterView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCarpenterDoc)));
	return (CCarpenterDoc*)m_pDocument;
}
#endif //_DEBUG



// CCarpenterView 메시지 처리기

void CCarpenterView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// modify viewport matrix of opengl
	glMatrixMode(GL_VIEWPORT);
	glLoadIdentity();
	glViewport(0, 0, cx, cy);

	// modify projection matrix of opengl
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLdouble)cx/cy, 0.01, 100);
}

void CCarpenterView::OnMouseMove(UINT nFlags, CPoint point)
{
	CCarpenterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	// search which surface is indicated by cursor
	if (Get3DCursorPos(point.x, point.y, m_cursor))
		m_curSurface = pDoc->SearchSurface(m_cursor[0], m_cursor[1], m_cursor[2]);
	else m_curSurface = NULL;

	// update view
	OnUpdate(NULL, 0, NULL);

	CView::OnMouseMove(nFlags, point);
}

void CCarpenterView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CCarpenterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	// select or unselect surface
	if (m_curSurface == NULL)
		pDoc->UnselectAllSurface();
	else if (pDoc->IsSelectedSurface(m_curSurface))
		pDoc->UnselectSurface(m_curSurface);
	else if ((nFlags & MK_CONTROL) == 0)
	{
		pDoc->UnselectAllSurface();
		pDoc->SelectSurface(m_curSurface);
	}
	else pDoc->SelectSurface(m_curSurface);

	// update view
	OnUpdate(NULL, 0, NULL);

	CView::OnLButtonDown(nFlags, point);
}

void CCarpenterView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CCarpenterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	// select all surface
	if (m_curSurface != NULL)
		pDoc->SelectAllSurface();

	// update view
	OnUpdate(NULL, 0, NULL);

	CView::OnLButtonDblClk(nFlags, point);
}

void CCarpenterView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CCarpenterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	CDialogSurface		dialog;
	CString				ex, ey, ez;
	double				u_min, u_max;
	double				v_min, v_max;
	int					u_num, v_num;
	double				org[3];
	double				mat[16];
	COLORREF			clr;

	// edit indicated surface
	if (m_curSurface != NULL)
	{
		pDoc->GetSurfaceCreationInfo(
			m_curSurface, ex, ey, ez, u_min, u_max, v_min, v_max, u_num, v_num, org, mat, clr);
		dialog.SetSurfaceCreationInfo(
			ex, ey, ez, u_min, u_max, v_min, v_max, u_num, v_num, org, mat, clr);

		if (dialog.DoModal() == IDOK)
		{
			dialog.GetSurfaceCreationInfo(
				ex, ey, ez, u_min, u_max, v_min, v_max, u_num, v_num, org, mat, clr);
			pDoc->EditSurface(
				m_curSurface, ex, ey, ez, u_min, u_max, v_min, v_max, u_num, v_num, org, mat, clr);
		}
	}
	// create new surface
	else OnEditCreate();

	// update view
	OnUpdate(NULL, 0, NULL);

	CView::OnRButtonDown(nFlags, point);
}

void CCarpenterView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CCarpenterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	double precise;

	precise = 1;
	if (m_tranPrecisely == true)
		precise = 10;

	switch (nChar)
	{
	case VK_SPACE: m_tranCentered = true; break;
	case VK_CONTROL: m_tranPrecisely = true; break;
	case VK_NUMPAD8:
		if (pDoc->GetSelectedSurfaceNumber() > 0)
		{
			if (m_tranMode == TRANSFORM_TRANSLATION) pDoc->TranslateSurface(0, 0.1/precise, 0);
			else if (m_tranMode == TRANSFORM_ROTATION) pDoc->RotateSurface(-10/precise, 'x', m_tranCentered);
			else if (m_tranMode == TRANSFORM_SCALING) pDoc->ScaleSurface(1, 1+0.1/precise, 1, m_tranCentered);
		}
		else
		{
			if (m_tranMode == TRANSFORM_TRANSLATION) pDoc->TranslateAllSurface(0, 0.1/precise, 0);
			else if (m_tranMode == TRANSFORM_ROTATION) pDoc->RotateAllSurface(-10/precise, 'x');
			else if (m_tranMode == TRANSFORM_SCALING) pDoc->ScaleAllSurface(1, 1+0.1/precise, 1);
		}
		break;
	case VK_NUMPAD2:
		if (pDoc->GetSelectedSurfaceNumber() > 0)
		{
			if (m_tranMode == TRANSFORM_TRANSLATION) pDoc->TranslateSurface(0, -0.1/precise, 0);
			else if (m_tranMode == TRANSFORM_ROTATION) pDoc->RotateSurface(10/precise, 'x', m_tranCentered);
			else if (m_tranMode == TRANSFORM_SCALING) pDoc->ScaleSurface(1, 1-0.091/precise, 1, m_tranCentered);
		}
		else
		{
			if (m_tranMode == TRANSFORM_TRANSLATION) pDoc->TranslateAllSurface(0, -0.1/precise, 0);
			else if (m_tranMode == TRANSFORM_ROTATION) pDoc->RotateAllSurface(10/precise, 'x');
			else if (m_tranMode == TRANSFORM_SCALING) pDoc->ScaleAllSurface(1, 1-0.091/precise, 1);
		}
		break;
	case VK_NUMPAD6:
		if (pDoc->GetSelectedSurfaceNumber() > 0)
		{
			if (m_tranMode == TRANSFORM_TRANSLATION) pDoc->TranslateSurface(0.1/precise, 0, 0);
			else if (m_tranMode == TRANSFORM_ROTATION) pDoc->RotateSurface(-10/precise, 'z', m_tranCentered);
			else if (m_tranMode == TRANSFORM_SCALING) pDoc->ScaleSurface(1+0.1/precise, 1, 1, m_tranCentered);
		}
		else
		{
			if (m_tranMode == TRANSFORM_TRANSLATION) pDoc->TranslateAllSurface(0.1/precise, 0, 0);
			else if (m_tranMode == TRANSFORM_ROTATION) pDoc->RotateAllSurface(-10/precise, 'z');
			else if (m_tranMode == TRANSFORM_SCALING) pDoc->ScaleAllSurface(1+0.1/precise, 1, 1);
		}
		break;
	case VK_NUMPAD4:
		if (pDoc->GetSelectedSurfaceNumber() > 0)
		{
			if (m_tranMode == TRANSFORM_TRANSLATION) pDoc->TranslateSurface(-0.1/precise, 0, 0);
			else if (m_tranMode == TRANSFORM_ROTATION) pDoc->RotateSurface(10/precise, 'z', m_tranCentered);
			else if (m_tranMode == TRANSFORM_SCALING) pDoc->ScaleSurface(1-0.091/precise, 1, 1, m_tranCentered);
		}
		else
		{
			if (m_tranMode == TRANSFORM_TRANSLATION) pDoc->TranslateAllSurface(-0.1/precise, 0, 0);
			else if (m_tranMode == TRANSFORM_ROTATION) pDoc->RotateAllSurface(10/precise, 'z');
			else if (m_tranMode == TRANSFORM_SCALING) pDoc->ScaleAllSurface(1-0.091/precise, 1, 1);
		}
		break;
	case VK_NUMPAD9:
		if (pDoc->GetSelectedSurfaceNumber() > 0)
		{
			if (m_tranMode == TRANSFORM_TRANSLATION) pDoc->TranslateSurface(0, 0, -0.1/precise);
			else if (m_tranMode == TRANSFORM_ROTATION) pDoc->RotateSurface(-10/precise, 'y', m_tranCentered);
			else if (m_tranMode == TRANSFORM_SCALING) pDoc->ScaleSurface(1, 1, 1+0.1/precise, m_tranCentered);
		}
		else
		{
			if (m_tranMode == TRANSFORM_TRANSLATION) pDoc->TranslateAllSurface(0, 0, -0.1/precise);
			else if (m_tranMode == TRANSFORM_ROTATION) pDoc->RotateAllSurface(-10/precise, 'y');
			else if (m_tranMode == TRANSFORM_SCALING) pDoc->ScaleAllSurface(1, 1, 1+0.1/precise);
		}
		break;
	case VK_NUMPAD3:
		if (pDoc->GetSelectedSurfaceNumber() > 0)
		{
			if (m_tranMode == TRANSFORM_TRANSLATION) pDoc->TranslateSurface(0, 0, 0.1/precise);
			else if (m_tranMode == TRANSFORM_ROTATION) pDoc->RotateSurface(10/precise, 'y', m_tranCentered);
			else if (m_tranMode == TRANSFORM_SCALING) pDoc->ScaleSurface(1, 1, 1-0.091/precise, m_tranCentered);
		}
		else
		{
			if (m_tranMode == TRANSFORM_TRANSLATION) pDoc->TranslateAllSurface(0, 0, 0.1/precise);
			else if (m_tranMode == TRANSFORM_ROTATION) pDoc->RotateAllSurface(10/precise, 'y');
			else if (m_tranMode == TRANSFORM_SCALING) pDoc->ScaleAllSurface(1, 1, 1-0.091/precise);
		}
		break;
	}

	// update view
	OnUpdate(NULL, 0, NULL);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCarpenterView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_SPACE: m_tranCentered = false; break;
	case VK_CONTROL: m_tranPrecisely = false; break;
	}

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CCarpenterView::OnEditCreate()
{
	CCarpenterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	CDialogSurface		dialog;
	CString				ex, ey, ez;
	double				u_min, u_max;
	double				v_min, v_max;
	int					u_num, v_num;
	double				org[3];
	double				mat[16];
	COLORREF			clr;

	if (dialog.DoModal() == IDOK)
	{
		dialog.GetSurfaceCreationInfo(
			ex, ey, ez, u_min, u_max, v_min, v_max, u_num, v_num, org, mat, clr);
		pDoc->AddSurface(
			ex, ey, ez, u_min, u_max, v_min, v_max, u_num, v_num, org, mat, clr);
	}
	pDoc->UpdateAllViews(NULL, 0, NULL);
}

void CCarpenterView::OnEditCopy()
{
	CCarpenterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	pDoc->CopySurface();
	pDoc->UpdateAllViews(NULL, 0, NULL);
}

void CCarpenterView::OnEditRemove()
{
	CCarpenterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	pDoc->RemoveSurface();
	pDoc->UpdateAllViews(NULL, 0, NULL);
}

void CCarpenterView::OnEditSelectAll()
{
	CCarpenterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	if (pDoc->GetSelectedSurfaceNumber() < pDoc->GetSurfaceNumber())
		pDoc->SelectAllSurface();
	else pDoc->UnselectAllSurface();

	pDoc->UpdateAllViews(NULL, 0, NULL);
}

void CCarpenterView::OnTransformTranslation()
{
	m_tranMode = TRANSFORM_TRANSLATION;
}

void CCarpenterView::OnUpdateTransformTranslation(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_tranMode == TRANSFORM_TRANSLATION);
}

void CCarpenterView::OnTransformRotation()
{
	m_tranMode = TRANSFORM_ROTATION;
}

void CCarpenterView::OnUpdateTransformRotation(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_tranMode == TRANSFORM_ROTATION);
}

void CCarpenterView::OnTransformScaling()
{
	m_tranMode = TRANSFORM_SCALING;
}

void CCarpenterView::OnUpdateTransformScaling(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_tranMode == TRANSFORM_SCALING);
}

void CCarpenterView::OnTransformCentered()
{
	m_tranCentered = !m_tranCentered;
}

void CCarpenterView::OnUpdateTransformCentered(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_tranCentered == true);
}

void CCarpenterView::OnTransformPrecisely()
{
	m_tranPrecisely = !m_tranPrecisely;
}

void CCarpenterView::OnUpdateTransformPrecisely(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_tranPrecisely == true);
}

void CCarpenterView::OnBuildDataFile()
{
	CCarpenterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	CFileDialog dialog(false);
	CString filename;

	if (dialog.DoModal() == IDOK)
	{
		filename = dialog.GetPathName();
		if (pDoc->BuildSurfaceDataFile(filename))
			MessageBox(L"Surface data file has builded succesfully");
		else MessageBox(L"Fail to build surface data file");
	}
}


