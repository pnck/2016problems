
// ServerInjectorDlg.h : ͷ�ļ�
//

#pragma once

class CServerInjectorDlgAutoProxy;


// CServerInjectorDlg �Ի���
class CServerInjectorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServerInjectorDlg);
	friend class CServerInjectorDlgAutoProxy;

// ����
public:
	CServerInjectorDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CServerInjectorDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVERINJECTOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	CServerInjectorDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
