// LibAu.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "LibAu.h"
#include "windows.h"
#include "shellapi.h"
#include "resource.h"
#include "stdio.h"

// ���ǵ���������һ��ʾ��
LIBAU_API int nLibAu=0;

// ���ǵ���������һ��ʾ����
LIBAU_API int fnLibAu(void)
{
    return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� LibAu.h
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