// LibAuTest.cpp : �������̨Ӧ�ó������ڵ㡣
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

