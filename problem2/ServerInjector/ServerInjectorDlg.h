
// ServerInjectorDlg.h : 头文件
//

#pragma once

class CServerInjectorDlgAutoProxy;


// CServerInjectorDlg 对话框
class CServerInjectorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServerInjectorDlg);
	friend class CServerInjectorDlgAutoProxy;

// 构造
public:
	CServerInjectorDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CServerInjectorDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVERINJECTOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	CServerInjectorDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// 生成的消息映射函数
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
