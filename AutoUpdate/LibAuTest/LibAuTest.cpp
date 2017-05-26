// LibAuTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "libau.h"
int main()
{
	CLibAu * sa = new CLibAu;
	sa->check("test.exe", 1, "127.0.0.1");
	char aa[128];
	scanf("%s",aa);
    return 0;
}

