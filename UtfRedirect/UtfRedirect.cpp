/*
 * NeoSmart UtfRedirect
 * Author: Mahmoud Al-Qudsi <mqudsi@neosmart.net>
 * Copyright (C) 2011 by NeoSmart Technologies
 * This code is released under the terms of the MIT License
*/

#include "stdafx.h"
#include <string>
#include <Windows.h>
#include <atlstr.h>
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	typedef BOOL (WINAPI * LPWOW64DISABLEWOW64FSREDIRECTION)(PVOID *);
	LPWOW64DISABLEWOW64FSREDIRECTION lpWow64DisableWow64FsRedirection = NULL;
	HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));
	lpWow64DisableWow64FsRedirection = (LPWOW64DISABLEWOW64FSREDIRECTION) GetProcAddress(hKernel32, "Wow64DisableWow64FsRedirection");
	LPVOID oldValue;
	if (lpWow64DisableWow64FsRedirection != NULL)
	{
		lpWow64DisableWow64FsRedirection(&oldValue);
	}
	FreeLibrary(hKernel32);

 	SetConsoleCP(65001);
 	SetConsoleOutputCP(65001);

	HANDLE hOld = GetStdHandle(STD_OUTPUT_HANDLE);
	SetStdHandle(STD_OUTPUT_HANDLE, NULL);

	string application, arguments;
	getline(cin, application);
	getline(cin, arguments);

	CString wApplication, wArguments;
	MultiByteToWideChar(CP_UTF8, 0, application.c_str(), -1, CStrBuf(wApplication, MAX_PATH), MAX_PATH);
	MultiByteToWideChar(CP_UTF8, 0, arguments.c_str(), -1, CStrBuf(wArguments, MAX_PATH), MAX_PATH);

	SetStdHandle(STD_OUTPUT_HANDLE, hOld);
	
	CString commandLine;
	commandLine.Format(_T("\"%s\" %s"), wApplication, wArguments);

	STARTUPINFO si = {0};
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi = {0};

	CreateProcess(NULL, CStrBuf(commandLine), NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD returnCode;
	GetExitCodeProcess(pi.hProcess, &returnCode);
	CloseHandle(pi.hProcess);

	return returnCode;
}
