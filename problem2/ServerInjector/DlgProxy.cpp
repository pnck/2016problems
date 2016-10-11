
// DlgProxy.cpp : 实现文件
//

#include "stdafx.h"
#include "ServerInjector.h"
#include "DlgProxy.h"
#include "ServerInjectorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerInjectorDlgAutoProxy

IMPLEMENT_DYNCREATE(CServerInjectorDlgAutoProxy, CCmdTarget)

CServerInjectorDlgAutoProxy::CServerInjectorDlgAutoProxy()
{
	EnableAutomation();
	
	// 为使应用程序在自动化对象处于活动状态时一直保持 
	//	运行，构造函数调用 AfxOleLockApp。
	AfxOleLockApp();

	// 通过应用程序的主窗口指针
	//  来访问对话框。  设置代理的内部指针
	//  指向对话框，并设置对话框的后向指针指向
	//  该代理。
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CServerInjectorDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CServerInjectorDlg)))
		{
			m_pDialog = reinterpret_cast<CServerInjectorDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CServerInjectorDlgAutoProxy::~CServerInjectorDlgAutoProxy()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	//	析构函数调用 AfxOleUnlockApp。
	//  除了做其他事情外，这还将销毁主对话框
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CServerInjectorDlgAutoProxy::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CServerInjectorDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CServerInjectorDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_IServerInjector 的支持
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {037FE15E-C13A-4D45-AF7E-13BF1A20B43A}
static const IID IID_IServerInjector =
{ 0x37FE15E, 0xC13A, 0x4D45, { 0xAF, 0x7E, 0x13, 0xBF, 0x1A, 0x20, 0xB4, 0x3A } };

BEGIN_INTERFACE_MAP(CServerInjectorDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CServerInjectorDlgAutoProxy, IID_IServerInjector, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 宏在此项目的 StdAfx.h 中定义
// {83493616-0F02-4DA9-A429-1D3F1EF9FD08}
IMPLEMENT_OLECREATE2(CServerInjectorDlgAutoProxy, "ServerInjector.Application", 0x83493616, 0xf02, 0x4da9, 0xa4, 0x29, 0x1d, 0x3f, 0x1e, 0xf9, 0xfd, 0x8)


// CServerInjectorDlgAutoProxy 消息处理程序
