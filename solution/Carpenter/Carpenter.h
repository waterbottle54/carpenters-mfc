
// Carpenter.h : Carpenter ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CCarpenterApp:
// �� Ŭ������ ������ ���ؼ��� Carpenter.cpp�� �����Ͻʽÿ�.
//

class CCarpenterApp : public CWinApp
{
public:
	CCarpenterApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCarpenterApp theApp;
