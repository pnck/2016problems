
// ServerInjectorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ServerInjector.h"
#include "ServerInjectorDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerInjectorDlg �Ի���


IMPLEMENT_DYNAMIC(CServerInjectorDlg, CDialogEx);

CServerInjectorDlg::CServerInjectorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVERINJECTOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CServerInjectorDlg::~CServerInjectorDlg()
{
	// ����öԻ������Զ���������
	//  ���˴���ָ��öԻ���ĺ���ָ������Ϊ NULL���Ա�
	//  �˴���֪���öԻ����ѱ�ɾ����
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CServerInjectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CServerInjectorDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CServerInjectorDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CServerInjectorDlg ��Ϣ�������

BOOL CServerInjectorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerInjectorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CServerInjectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ���û��ر� UI ʱ������������Ա���������ĳ��
//  �������Զ�����������Ӧ�˳���  ��Щ
//  ��Ϣ�������ȷ����������: �����������ʹ�ã�
//  ������ UI�������ڹرնԻ���ʱ��
//  �Ի�����Ȼ�ᱣ�������

void CServerInjectorDlg::OnClose()
{
	if (CanExit())
		CDialogEx::OnClose();
}

void CServerInjectorDlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CServerInjectorDlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL CServerInjectorDlg::CanExit()
{
	// �����������Ա�����������Զ���
	//  �������Իᱣ�ִ�Ӧ�ó���
	//  ʹ�Ի���������������� UI ����������
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

UINT RPCServerProc(LPVOID pParam)
{
	BOOL *bAlive = (BOOL*)pParam;
	*bAlive = TRUE;
	RpcServerUseProtseqEp((RPC_WSTR)L"ncalrpc", RPC_C_PROTSEQ_MAX_REQS_DEFAULT, (RPC_WSTR)L"xxRPCxx", NULL);
	RpcServerRegisterIf(IRpcCheck_v1_0_s_ifspec, NULL, NULL);
	RpcServerListen(1, 20, FALSE);
	*bAlive = FALSE;
	return 0;
}

void CServerInjectorDlg::OnBnClickedOk()
{
	HANDLE hToken;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		LUID luid;
		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
		{
			TOKEN_PRIVILEGES tp;
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = luid;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			AdjustTokenPrivileges(hToken, FALSE, &tp, NULL, NULL, NULL);

			DWORD pid = 0;
			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			PROCESSENTRY32 pe = { sizeof PROCESSENTRY32 };
			BOOL fContinue = Process32First(hSnap, &pe);
			while (fContinue)
			{
				if (wcscmp(pe.szExeFile, L"notepad.exe") == 0)
					pid = pe.th32ProcessID;
				fContinue = Process32Next(hSnap, &pe);
			}
			if (!pid) return;
			HANDLE hNotepadProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, pid);
			LPVOID lpAllocatedMem = VirtualAllocEx(hNotepadProcess, NULL, 1024, MEM_COMMIT, PAGE_READWRITE);
			WCHAR szDllName[500] = { L"D:\\documents\\Visual Studio 2015\\Projects\\2016problems\\problem2\\x64\\Release\\clientdll.dll" };
			if (WriteProcessMemory(hNotepadProcess, lpAllocatedMem, szDllName, sizeof(szDllName), NULL))
			{
				PTHREAD_START_ROUTINE pfn_LoadLibraryW = (PTHREAD_START_ROUTINE)LoadLibraryW;
				HANDLE hThread = CreateRemoteThread(hNotepadProcess, NULL, 0, pfn_LoadLibraryW, lpAllocatedMem, 0, NULL);
				ASSERT(hThread);
				//ResumeThread(hThread);
				static BOOL bAlive = FALSE;
				if (!bAlive) AfxBeginThread(RPCServerProc, &bAlive);
			}

		}
	}

}
int CheckFlag(const wchar_t *pflgstr)
{
	//flag{s1mpleLogic_with_unfami1iarTech}
	const WCHAR * szFlagEncryped = L"BExmAGwAYQBnAHsAcwAxAG0AcABsAGUATABvAGcAaQBjAF8AdwBpAHQAaABfAHUA\r\nbgBmAGEAbQBpADEAaQBhAHIAVABlAGMAaAB9AAAA\r\n";
	return wcscmp(pflgstr, szFlagEncryped)==0;
/*
	HCRYPTMSG hMsg;
	DWORD cbEncodedBlob = 1024;
	BYTE *pbEncodedBlob = (BYTE *)malloc(1024);
	CryptStringToBinary(pflgstr, wcslen(pflgstr)*sizeof(wchar_t), CRYPT_STRING_BASE64, (BYTE*)pbEncodedBlob, &cbEncodedBlob, NULL, NULL);

	DWORD cbDecoded;
	BYTE *pbDecoded;

	hMsg = CryptMsgOpenToDecode(
		PKCS_7_ASN_ENCODING,      // encoding type.
		0,                     // flags.
		CMSG_DATA,             // look for a data message.
		NULL,                  // cryptographic provider.
		NULL,                  // recipient information.
		NULL);                 // stream information.

	//-------------------------------------------------------------------
	// Update the message with an encoded BLOB.
	// Both pbEncodedBlob, the encoded data, 
	// and cbEncodedBlob, the length of the encoded data,
	// must be available. 

	CryptMsgUpdate(
		hMsg,                 // handle to the message
		pbEncodedBlob,        // pointer to the encoded BLOB
		cbEncodedBlob,        // size of the encoded BLOB
		TRUE);                // last call
	//-------------------------------------------------------------------
	// Get the size of the content.

	CryptMsgGetParam(
		hMsg,                  // handle to the message
		CMSG_CONTENT_PARAM,    // parameter type
		0,                     // index
		NULL,                  // address for returned 
							   // information
		&cbDecoded);           // size of the returned
							   // information
	//-------------------------------------------------------------------
	// Allocate memory.
	pbDecoded = (BYTE *)malloc(cbDecoded);
	//-------------------------------------------------------------------
	// Get a pointer to the content.

	CryptMsgGetParam(
		hMsg,                  // handle to the message
		CMSG_CONTENT_PARAM,    // parameter type
		0,                     // index
		pbDecoded,             // address for returned 
							   // information
		&cbDecoded);           // size of the returned 
							   // information
	//-------------------------------------------------------------------
	// Clean up.
	AfxMessageBox((LPWSTR)pbDecoded);
	free(pbEncodedBlob);
	free(pbDecoded);
	CryptMsgClose(hMsg);
*/
}


void Shutdown(void)
{
	RpcMgmtStopServerListening(NULL);
	RpcServerUnregisterIf(NULL, NULL, FALSE);
	//AfxMessageBox(L"Shutdown!");
}