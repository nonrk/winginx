
// wnLauncher.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CwnLauncherApp: 
// �йش����ʵ�֣������ wnLauncher.cpp
//

class CwnLauncherApp : public CWinApp
{
public:
	CwnLauncherApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CwnLauncherApp theApp;