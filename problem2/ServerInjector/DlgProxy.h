
// DlgProxy.h: ͷ�ļ�
//

#pragma once

class CServerInjectorDlg;


// CServerInjectorDlgAutoProxy ����Ŀ��

class CServerInjectorDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CServerInjectorDlgAutoProxy)

	CServerInjectorDlgAutoProxy();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��

// ����
public:
	CServerInjectorDlg* m_pDialog;

// ����
public:

// ��д
	public:
	virtual void OnFinalRelease();

// ʵ��
protected:
	virtual ~CServerInjectorDlgAutoProxy();

	// ���ɵ���Ϣӳ�亯��

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CServerInjectorDlgAutoProxy)

	// ���ɵ� OLE ����ӳ�亯��

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

