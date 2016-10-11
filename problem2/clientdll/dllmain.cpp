// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <cstdio>
#include <assert.h>
#include <string>
#include "../rpcdef.h"

#define  CMD_NEVER 0x4999
#define  CMD_CHECKING 0x4627

void _Main();
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	static RPC_WSTR pszStringBinding = NULL;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		_Main();
		//FreeLibrary(GetModuleHandle(NULL));
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

BOOL CALLBACK EnumProc(HWND hwnd, LPARAM lparam);
LRESULT NewWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

static WNDPROC pfnOldWndProc;
static bool bGettingCmd = false;

void _Main()
{
	//MessageBox(NULL, L"done", L"done", MB_OK); 

	HWND hMainWnd;
	if (!EnumWindows(EnumProc, (LPARAM)&hMainWnd) && (GetLastError() == ERROR_SUCCESS))
	{

		pfnOldWndProc = (WNDPROC)GetWindowLongPtr(hMainWnd, GWLP_WNDPROC);
		SetWindowLongPtr(hMainWnd, GWLP_WNDPROC, (LONG_PTR)NewWndProc);
		HMENU hMenu = GetMenu(hMainWnd);
		WCHAR buff[256] = {};
		//wsprintf(buff, L"window handle=0x%x", hMainWnd);
		//MessageBox(hMainWnd, buff, L"test", MB_OK);
		HMENU hNewPopupMenu = CreatePopupMenu();
		AppendMenu(hNewPopupMenu, MF_UNCHECKED, CMD_CHECKING, L"Checking Flag");
		AppendMenu(hNewPopupMenu, MF_ENABLED, CMD_NEVER, L"never click this");
		AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hNewPopupMenu, L"?");
		UpdateWindow(hMainWnd);
	}

}
BOOL CALLBACK EnumProc(HWND hwnd, LPARAM lparam)
{
	DWORD dwProcessId = 0;
	HWND * p = (HWND *)lparam;
	GetWindowThreadProcessId(hwnd, &dwProcessId);
	if (dwProcessId == GetCurrentProcessId())
	{
		SetLastError(ERROR_SUCCESS);
		*p = hwnd;
		return FALSE;
	}
	return TRUE;
}

LRESULT NewWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		if (!HIWORD(wparam))
		{
			HMENU hMenu = GetMenu(hwnd);
			WORD resid = LOWORD(wparam);
			if (resid == CMD_NEVER)
			{
				for (int i(0);i < 10;i++)
				{
					RemoveMenu(hMenu, 0, MF_BYPOSITION);
				}
				for (int i(0);i < 1000;i++)
				{
					AppendMenu(hMenu, MF_DISABLED, 0x4444, L"?");
				}
				UpdateWindow(hwnd);
				ShowWindow(hwnd, SW_SHOW);
			}
			if (resid == CMD_CHECKING)
			{
				HMENU hSubMenu = GetSubMenu(hMenu, 5);
				MENUITEMINFO mii;
				mii.cbSize = sizeof(mii);
				mii.fMask = MIIM_STATE;
				GetMenuItemInfo(hSubMenu, 0, TRUE, &mii);
				mii.fMask |= MIIM_STATE;
				static RPC_WSTR pszStringBinding = NULL;
				if (mii.fState & MFS_CHECKED)
				{
					mii.fState &= ~MFS_CHECKED;
					bGettingCmd = false;
					RpcTryExcept
					{
						Shutdown();
					}
						RpcExcept(1)
					{
					}
					RpcEndExcept
						RpcStringFree(&pszStringBinding);
					RpcBindingFree(&Client_Binding);
				}
				else
				{
					mii.fState |= MFS_CHECKED;
					bGettingCmd = true;
					RpcStringBindingCompose(NULL, (RPC_WSTR)L"ncalrpc", NULL, (RPC_WSTR)L"xxRPCxx", NULL, &pszStringBinding);
					RpcBindingFromStringBinding(pszStringBinding, &Client_Binding);

				}
				SetMenuItemInfo(hSubMenu, 0, TRUE, &mii);
			}
		}
		if (HIWORD(wparam) == EN_UPDATE)
		{
			if (bGettingCmd)
			{
				HWND hEdit = (HWND)lparam;
				WCHAR szCheck[256];
				GetWindowText(hEdit, szCheck, 255);
				if (szCheck[0] == L'$' && szCheck[1] == L'!')
				{
					UINT i(0);
					for (;i < 255;i++)
					{
						if (szCheck[i] == L'$' && szCheck[i + 1] == L'$')
						{
							szCheck[i] = L'\0';
							break;
						}
					}
					if (i < 255)
					{
						//##############here check flag
						//SetWindowText(hwnd, szCheck + 2);
						RpcTryExcept
						{
							HCRYPTMSG hMsg;
							BYTE* pbContent;     // a byte pointer to the message
							DWORD cbContent;     // the size of message
							DWORD cbEncodedBlob;
							BYTE *pbEncodedBlob;

							pbContent = (BYTE*)&szCheck[2];
							cbContent = (wcslen(szCheck+2)+1)*sizeof(WCHAR);

							//-------------------------------------------------------------------
							// Get the size of the encoded message BLOB.

							cbEncodedBlob = CryptMsgCalculateEncodedLength(
								PKCS_7_ASN_ENCODING,       // message encoding type
								0,                      // flags
								CMSG_DATA,              // message type
								NULL,                   // pointer to structure
								NULL,                   // inner content object ID
								cbContent);             // size of content

							//-------------------------------------------------------------------
							// Allocate memory for the encoded BLOB.

							pbEncodedBlob = (BYTE *)malloc(cbEncodedBlob);

							//-------------------------------------------------------------------
							// Open a message to encode.

							hMsg = CryptMsgOpenToEncode(
								PKCS_7_ASN_ENCODING,        // encoding type
								0,                       // flags
								CMSG_DATA,               // message type
								NULL,                    // pointer to structure
								NULL,                    // inner content object ID
								NULL);                  // stream information (not used)

							//-------------------------------------------------------------------
							// Update the message with the data.

							CryptMsgUpdate(
								hMsg,         // handle to the message
								pbContent,    // pointer to the content
								cbContent,    // size of the content
								TRUE);        // last call
							//-------------------------------------------------------------------
							// Get the resulting message.

							CryptMsgGetParam(
								hMsg,                      // handle to the message
								CMSG_BARE_CONTENT_PARAM,   // parameter type
								0,                         // index
								pbEncodedBlob,             // pointer to the BLOB
								&cbEncodedBlob);           // size of the BLOB

							//-------------------------------------------------------------------
							// pbEncodedBlob now points to the encoded, signed content.

							//-------------------------------------------------------------------
							// Close the message.

							if (hMsg)CryptMsgClose(hMsg);
							WCHAR szEncryptStr[1024];
							DWORD cch = 1024;
							CryptBinaryToString(pbEncodedBlob, cbEncodedBlob, CRYPT_STRING_BASE64, szEncryptStr, &cch);
							if (CheckFlag(szEncryptStr))
							{
								SetWindowText(hwnd, L"Congratulations! You got the right flag.");
								//SetWindowText(hwnd, szCheck + 2);
								//SetWindowText(hEdit, (LPWSTR)szEncryptStr);
								
							}
							free(pbEncodedBlob);
						}
							RpcExcept(1)
						{
							WCHAR buff[256];
							wsprintf(buff, L"RPC Exception: %d", RpcExceptionCode());
							MessageBox(hwnd, buff, L"failed", MB_ICONERROR);
						}
						RpcEndExcept
					}
				}
			}
		}
	}
	break;
	default:
		break;
	}
	return CallWindowProc(pfnOldWndProc, hwnd, msg, wparam, lparam);
}
//flag{s1mpleLogic_with_unfami1iarTech}