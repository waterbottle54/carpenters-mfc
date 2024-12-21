
// CarpenterView.h : CCarpenterView Ŭ������ �������̽�
//

#pragma once
#pragma comment(lib, "opengl32")
#include <gl/GLU.h>

#define TRANSFORM_TRANSLATION		0x0001
#define TRANSFORM_ROTATION			0x0002
#define TRANSFORM_SCALING			0x0003



class CCarpenterView : public CView
{
protected: // serialization������ ��������ϴ�.
	CCarpenterView();
	DECLARE_DYNCREATE(CCarpenterView)

// Ư���Դϴ�.
public:
	HDC			m_hDC;
	HGLRC		m_hGLRC;
	GLdouble	m_cursor[3];
	POSITION	m_curSurface;

	SHORT		m_tranMode;
	bool		m_tranCentered;
	bool		m_tranPrecisely;
	
	CCarpenterDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
protected:

// �����Դϴ�.
public:
	virtual ~CCarpenterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditCreate();
	afx_msg void OnEditCopy();
	afx_msg void OnEditRemove();
	afx_msg void OnEditSelectAll();
	afx_msg void OnTransformTranslation();
	afx_msg void OnUpdateTransformTranslation(CCmdUI *pCmdUI);
	afx_msg void OnTransformRotation();
	afx_msg void OnUpdateTransformRotation(CCmdUI *pCmdUI);
	afx_msg void OnTransformScaling();
	afx_msg void OnUpdateTransformScaling(CCmdUI *pCmdUI);
	afx_msg void OnTransformCentered();
	afx_msg void OnUpdateTransformCentered(CCmdUI *pCmdUI);
	afx_msg void OnTransformPrecisely();
	afx_msg void OnUpdateTransformPrecisely(CCmdUI *pCmdUI);
	afx_msg void OnBuildDataFile();
};

#ifndef _DEBUG  // CarpenterView.cpp�� ����� ����
inline CCarpenterDoc* CCarpenterView::GetDocument() const
   { return reinterpret_cast<CCarpenterDoc*>(m_pDocument); }
#endif

