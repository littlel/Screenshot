
// screen_test.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cscreen_testApp:
// �йش����ʵ�֣������ screen_test.cpp
//

class Cscreen_testApp : public CWinApp
{
public:
	Cscreen_testApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cscreen_testApp theApp;