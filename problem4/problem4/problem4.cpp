// problem4.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>

const char *repltable = "PJ4MhwxQ8vE}CiX0tRIoc-AekSj6l1gY7LNOprT{nKaBFGyDHsbW93+qVUZ5fd2zmu";
__declspec(noinline) void  RealProc(char *pc, DWORD64 index/*actually dupsz*/);


int findpos(char c)
{
	int i = 66;
	while (i--)
	{
		if (repltable[i] == c)
			return i;
	}
	return -1;
}

__declspec(noinline) void FakeProc(char *pc, DWORD64 index)
{
	DWORD64 dw[3] = { 0xdeadbeefaaaabbbb,0x1956201619940627,0xcccccccacedecade };
	char *p = (char *)dw;
	*pc = repltable[index - 1];
	*pc ^= p[index];
}

__declspec(noinline) bool CheckMatch(PDWORD64 p)
{
	//(1) 0xfa98cfd7ae9fe4a5
	//(2) 0x4b13545c6ff42f73
	//(3) 0xf4a586faeef5b39d
	//(x) 0xc9a63bd0244431fe
	/*
	1^x = a
	2^x = b
	3^x = c
	1^2^3 = d

	a^b^d = 1^x^2^x^1^2^3 = 3
	a^c^d = 1^x^3^x^1^2^3 = 2
	b^c^d = 2^x^3^x^1^2^3 = 1

	*/
#define D 0x452e1d712f9e784b
#define A 0x333ef4078adbd55b
#define B 0x82b56f8c4bb01e8d
#define C 0x3d03bd2acab18263
	DWORD64 dw[4] = { D,A,B,C };
	if ((dw[0] ^ dw[2] ^ dw[3]) == *p)
	{
		p++;
		if ((dw[0] ^ dw[1] ^ dw[3]) == *p)
		{
			p++;
			if ((dw[0] ^ dw[1] ^ dw[2]) == *p)
			{
				return true;
			}
		}
	}
	return false;
}
void RealMain(const char* s, size_t len)
{
	char face[3] = { ":(" };
	char * dupsz = _strdup(s);
	HANDLE h = GetModuleHandle(NULL);
	PIMAGE_DOS_HEADER pdosh = (PIMAGE_DOS_HEADER)h;
	PIMAGE_NT_HEADERS ppe = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<PBYTE>(pdosh) + pdosh->e_lfanew);
	PIMAGE_RUNTIME_FUNCTION_ENTRY prtfe = reinterpret_cast<PIMAGE_RUNTIME_FUNCTION_ENTRY>(reinterpret_cast<PBYTE>(pdosh) + ppe->OptionalHeader.DataDirectory[3].VirtualAddress);

	DWORD oldProtect;
	VirtualProtect(prtfe, ppe->OptionalHeader.DataDirectory[3].Size, PAGE_READWRITE, &oldProtect);
	if (GetLastError() == ERROR_SUCCESS)
	{
		//static CONTEXT ctx;
		//RtlCaptureContext(&ctx);
		DWORD64 imageBase = 0;

		PRUNTIME_FUNCTION pFuncEntry = RtlLookupFunctionEntry((DWORD64)RealMain, &imageBase, NULL);
		PUNWIND_INFO pUnwindInfo = reinterpret_cast<PUNWIND_INFO>(reinterpret_cast<PBYTE>(imageBase) + pFuncEntry->UnwindData);
		PBYTE pExceptionHandler = reinterpret_cast<PBYTE>(pUnwindInfo->UnwindCode + pUnwindInfo->CountOfCodes);
		if (pUnwindInfo->CountOfCodes % 2)//align
		{
			pExceptionHandler += 2;
		}
		PSCOPE_TABLE pScopeTable = reinterpret_cast<PSCOPE_TABLE>(pExceptionHandler + 4);


		DWORD64 dw = (DWORD64)RealProc;
		if (*reinterpret_cast<PBYTE>(dw) == 0xe9)//with ILT,the first byte will be 0xe9
		{
			LONG offset = *reinterpret_cast<PLONG>(dw + 1);
			dw += 5 + offset;
		}

		PRUNTIME_FUNCTION pTargetFuncEntry = RtlLookupFunctionEntry(dw, &imageBase, NULL);
		PUNWIND_INFO pTargetFuncUnwindInfo = reinterpret_cast<PUNWIND_INFO>(reinterpret_cast<PBYTE>(imageBase) + pTargetFuncEntry->UnwindData);
		pExceptionHandler = reinterpret_cast<PBYTE>(pTargetFuncUnwindInfo->UnwindCode + pTargetFuncUnwindInfo->CountOfCodes);
		if (pTargetFuncUnwindInfo->CountOfCodes % 2)//align
		{
			pExceptionHandler += 2;
		}
		ULONG excpHandler = *reinterpret_cast<ULONG*>(pExceptionHandler);
		PSCOPE_TABLE pTargetFuncScopeTable = reinterpret_cast<PSCOPE_TABLE>(pExceptionHandler + 4);

		PUNWIND_INFO pConstructedUnwindInfo = reinterpret_cast<PUNWIND_INFO>(prtfe);
		pConstructedUnwindInfo->Flags = pTargetFuncUnwindInfo->Flags;
		pConstructedUnwindInfo->Version = pTargetFuncUnwindInfo->Version;
		pConstructedUnwindInfo->SizeOfProlog = pTargetFuncUnwindInfo->SizeOfProlog;
		pConstructedUnwindInfo->FrameRegister = pTargetFuncUnwindInfo->FrameRegister;
		pConstructedUnwindInfo->FrameOffset = pTargetFuncUnwindInfo->FrameOffset;
		BYTE CountOfCodes = pConstructedUnwindInfo->CountOfCodes = pUnwindInfo->CountOfCodes;
		memcpy_s(pConstructedUnwindInfo->UnwindCode, sizeof(UNWIND_CODE)*CountOfCodes, pUnwindInfo->UnwindCode, sizeof(UNWIND_CODE)*CountOfCodes);
		pExceptionHandler = reinterpret_cast<PBYTE>(pConstructedUnwindInfo->UnwindCode + pConstructedUnwindInfo->CountOfCodes);
		if (pConstructedUnwindInfo->CountOfCodes % 2)//align
		{
			pExceptionHandler += 2;
		}
		*reinterpret_cast<ULONG*>(pExceptionHandler) = excpHandler;
		PSCOPE_TABLE pConstructedScopeTable = reinterpret_cast<PSCOPE_TABLE>(pExceptionHandler + 4);
		memcpy_s(pConstructedScopeTable, sizeof(SCOPE_TABLE), pTargetFuncScopeTable, sizeof(SCOPE_TABLE));
		pTargetFuncEntry->UnwindInfoAddress = reinterpret_cast<DWORD>(pConstructedUnwindInfo) - imageBase;

		VirtualProtect(pScopeTable,4096, PAGE_READWRITE, &oldProtect);
		if (GetLastError() == ERROR_SUCCESS)
		{
			ULONG newJumpTarget = (DWORD64)RealProc - imageBase;
			//ULONG oldJumpTarget = pScopeTable->ScopeRecord[0].JumpTarget;
#ifdef _DEBUG
			pScopeTable->ScopeRecord[0].JumpTarget = newJumpTarget;
#else
			pScopeTable->ScopeRecord[0].JumpTarget = newJumpTarget - 6;
#endif
#ifdef _DEBUG
			newJumpTarget = pConstructedScopeTable->ScopeRecord[0].JumpTarget = pScopeTable->ScopeRecord[1].JumpTarget;
			PBYTE  pModifyingCode = reinterpret_cast<PBYTE>(newJumpTarget + imageBase);
			VirtualProtect(pModifyingCode, 12, PAGE_EXECUTE_READWRITE, &oldProtect);
			if (GetLastError() == ERROR_SUCCESS)
			{

				BYTE codes[13] = { 0x48,0x81,0xc4,0x70,0x01,0x00,0x00,0x48,0x8d,0x6c,0x24,0x20,0x90 };
				/*
				add rsp, 0x170;
				lea rbp, [rsp+0x20]
				*/
				memcpy_s(pModifyingCode, 13, codes, 13);
		}
#endif
			//printf("before try; handler:%p", pRoutine);
			for (DWORD64 i(0);i < len&&dupsz[i] != 0;i++)
			{
				__try
				{
					//puts("trying");
					//RtlCaptureContext(&ctx);//preparing to fly
					FakeProc(dupsz + i, (DWORD64)dupsz);//oh a fake bug
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					//never come here
				}
				__try//never come here
				{
					throw(0);
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
#ifdef _DEBUG
					puts("should return to here");
#endif
					//puts("should return to here");
			}
	}
			PDWORD64 tp = (PDWORD64)dupsz;
			if (CheckMatch(tp))
			{
				face[1] ++;
			}
			puts(face);
}
	}

}

int main()
{
	std::string s;
	std::cin >> s;

	//hdu{YouMakeChoicesRight}
	//FAKE(-1):MfmTgIm4KeA}MICoAHtC1M0E
	//0x346d4967546d664d ^ 0xdeadbeefaaaabbbb = 0xeac0f788fec7ddf6
	//0x6f43494d7d41654b ^ 0x1956201619940627 = 0x7615695b64d5636c
	//0x45304d3143744841 ^ 0xcccccccacedecade = 0x89fc81fb8daa829f

	//REAL(-3):JZ2plt2P{-cvJtERcyXE6Ji8
	//0x5032746c70325a4a ^ 0xaaaabbbbdeadbeef = 0xfa98cfd7ae9fe4a5
	//0x5245744a76632d7b ^ 0x1956201619970208 = 0x4b13545c6ff42f73
	//0x38694a3645587963 ^ 0xccccccccabadcafe = 0xf4a586faeef5b39d

	DWORD oldProtect = 0;
	VirtualProtect(reinterpret_cast<PBYTE>(RealProc) - 6, 20, PAGE_EXECUTE_READWRITE, &oldProtect);
	if (GetLastError() == ERROR_SUCCESS)
	{
#ifdef _DEBUG
		BYTE codes[12] = { 0x59,0x5a,0x48,0x83,0xec,0x28,0x90,0x90 ,0x90 ,0x90 ,0x90 ,0x90 };
		/*
		pop rcx;
		pop rdx;
		sub rsp,0x28
		*/
		memcpy_s(RealProc, 12, codes, 12);
#else
		PBYTE pModifyingCode = reinterpret_cast<PBYTE>(RealProc) - 6;
		pModifyingCode[0] = 0x48;
		pModifyingCode[1] = 0x89;
		pModifyingCode[2] = 0xf1;//mov rcx,rsi
		pModifyingCode[3] = 0x48;
		pModifyingCode[4] = 0x89;
		pModifyingCode[5] = 0xfa;//mov rdx,rdi
		pModifyingCode[9] = 0x30;//sub rsp, 0x30
		VirtualProtect(reinterpret_cast<PBYTE>(RealProc) - 6, 20, PAGE_EXECUTE_READ, &oldProtect);
#endif
		RealMain(s.c_str(), s.length());
}

	getchar();
	getchar();
	return 0;
}

__declspec(noinline) void  RealProc(char *pc, DWORD64 index/*actually dupsz*/)
{
	DWORD64 dw[3] = { 0xaaaabbbbdeadbeef, 0x1956201619970208,0xccccccccabadcafe };
	__try
	{
		index = reinterpret_cast<DWORD64>(pc) - index;//calculate the real index
		const char *ts = "XZcmkfo1586end";
		char *p = (char *)dw;
		*pc = repltable[findpos(*pc) - 3];
		*pc ^= p[index];
		const_cast<char*>(ts)[0] = 'Y';
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//never come here
	}
}
