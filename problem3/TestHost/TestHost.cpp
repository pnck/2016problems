// TestHost.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

typedef WCHAR * (*  fntype)(void);
typedef int (*  fntype2)(LPCWSTR);

int main()
{
	WCHAR *p = L"D:\\documents\\Visual Studio 2015\\Projects\\2016problems\\problem3\\x64\\Debug\\problem3.dll";
	HMODULE h = LoadLibrary(p);
	fntype GetPluginName = (fntype)GetProcAddress(h, "GetPluginName");
	std::wcout << GetPluginName() << std::endl;
	WCHAR *p2 = L"C:\\Users\\misaki\\Pictures\\test4.bmp";
	WCHAR *p3 = L"C:\\Users\\misaki\\Pictures\\test4_processed.bmp";
	fntype2 FnProcess = (fntype2)GetProcAddress(h, "Process");
	std::wcout << FnProcess(p2) << std::endl;
	std::wcout << FnProcess(p3) << std::endl;
    return 0;
}

