
// ServerInjector.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "../rpcdef.h"

// CServerInjectorApp: 
// �йش����ʵ�֣������ ServerInjector.cpp
//

class CServerInjectorApp : public CWinApp
{
public:
	CServerInjectorApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CServerInjectorApp theApp;