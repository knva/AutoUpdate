// LibAu.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "LibAu.h"
#include "windows.h"
#include "shellapi.h"
#include "resource.h"
#include "stdio.h"

// 这是导出变量的一个示例
LIBAU_API int nLibAu=0;

// 这是导出函数的一个示例。
LIBAU_API int fnLibAu(void)
{
    return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 LibAu.h
CLibAu::CLibAu()
{
	HMODULE hExe = GetModuleHandle(L"LibAu.dll");
	//HMODULE hExe = NULL;
	HRSRC hRes = FindResource(hExe, MAKEINTRESOURCE(IDR_EXE1), L"exe");
	if (NULL == hRes) {
		int ierr = GetLastError();
		return;
	}

	HGLOBAL hgRes = LoadResource(hExe, hRes);
	DWORD nResSize = SizeofResource(hExe, hRes);

	LPVOID pRes = LockResource(hgRes);
	FILE *fp;
	fopen_s(&fp, "auto.exe", "wb");
	fwrite(pRes, sizeof(char), nResSize, fp);
	fclose(fp);
	//ShellExecute(NULL, "open",".\auto.exe", NULL, NULL, SW_SHOWNORMAL);

}

void CLibAu::check(const char *softname, int ver, const char *url)
{
	char runshell[64];
	sprintf_s(runshell, 64, ".\\auto.exe %s %d %s", softname, ver, url);
	WinExec(runshell, SW_SHOW);
	return;
}