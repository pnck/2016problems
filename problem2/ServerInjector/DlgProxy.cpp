
// DlgProxy.cpp : ʵ���ļ�
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
	
	// ΪʹӦ�ó������Զ��������ڻ״̬ʱһֱ���� 
	//	���У����캯������ AfxOleLockApp��
	AfxOleLockApp();

	// ͨ��Ӧ�ó����������ָ��
	//  �����ʶԻ���  ���ô�����ڲ�ָ��
	//  ָ��Ի��򣬲����öԻ���ĺ���ָ��ָ��
	//  �ô���
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
	// Ϊ������ OLE �Զ����������ж������ֹӦ�ó���
	//	������������ AfxOleUnlockApp��
	//  ���������������⣬�⻹���������Ի���
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CServerInjectorDlgAutoProxy::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease��  ���ཫ�Զ�
	// ɾ���ö���  �ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CServerInjectorDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CServerInjectorDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// ע��: ��������˶� IID_IServerInjector ��֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡�  �� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {037FE15E-C13A-4D45-AF7E-13BF1A20B43A}
static const IID IID_IServerInjector =
{ 0x37FE15E, 0xC13A, 0x4D45, { 0xAF, 0x7E, 0x13, 0xBF, 0x1A, 0x20, 0xB4, 0x3A } };

BEGIN_INTERFACE_MAP(CServerInjectorDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CServerInjectorDlgAutoProxy, IID_IServerInjector, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 ���ڴ���Ŀ�� StdAfx.h �ж���
// {83493616-0F02-4DA9-A429-1D3F1EF9FD08}
IMPLEMENT_OLECREATE2(CServerInjectorDlgAutoProxy, "ServerInjector.Application", 0x83493616, 0xf02, 0x4da9, 0xa4, 0x29, 0x1d, 0x3f, 0x1e, 0xf9, 0xfd, 0x8)


// CServerInjectorDlgAutoProxy ��Ϣ�������
