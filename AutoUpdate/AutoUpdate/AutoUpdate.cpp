// AutoUpdate.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Clibcurl.h"  
#include "configjson.h"
#include <direct.h>
#include "MD5.h"
using namespace std;
class CLibcurlCallbackEx
	: public CLibcurlCallback
{
public:
	virtual void Progress(void* lpParam, double dTotal, double dLoaded)
	{
		//if (dTotal == 0.0)
		//	return;
		//double bPercent = (dLoaded / dTotal) * 100;
		//
		//printf("下载进度：%0.2lf%%\n", bPercent);
		printf(".");
	}

};
///
///
///TODO:下载文件  参数1 :下载地址   参数2 :保存文件名
void Download(const char *pUrl, const char* filename);
///TODO:提交POST
void PostTest();
///TODO:使用GET方式获取信息 参数1:程序名  参数2:版本号
string GetUpdate(string name, int ver);
///TODO:创建批处理文件 参数1:程序名
void MakeBat(const char* app);
///TODO 主函数
int _tmain(int argc, _TCHAR* argv[])
{
	//DownloadTest();  
	//PostTest();  
	string name="";
	int ver=0;
	configjson *rj = new configjson();
	rj->readjson(name, ver);
	//printf(name.c_str());
	//printf("%d",ver);
	string downloadUrl = "";
	int nver = 0;
	string json = GetUpdate(name, ver);
	if (json == "{\"code\":\"0\"}")
		return 0;
	rj->readjson(json, downloadUrl, nver);

	printf("发现新版本，版本号:%d\n", nver);
	_mkdir("update");
	Download(downloadUrl.c_str(), name.c_str());
	MakeBat(name.c_str());
	//system("pause");
	return 0;
}

void Download(const char *pUrl,const char* filename)
{
	CLibcurl libcurl;
	CLibcurlCallbackEx cb;
	libcurl.SetUserAgent("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.86 Safari/537.36");
	libcurl.SetConnectTimeout(2);
	libcurl.SetResumeFrom(0);
	libcurl.SetCallback(&cb, NULL);
	char updatedir[128] = "";
	sprintf_s(updatedir, 128,"update//%s", filename);
	libcurl.DownloadToFile(pUrl, updatedir);
}

void PostTest()
{
	CLibcurl libcurl;
	libcurl.SetUserAgent("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.86 Safari/537.36");
	libcurl.SetPort(80);
	libcurl.SetConnectTimeout(2);
	libcurl.AddHeader("name", "Jelin");
	libcurl.AddHeader("sex", "man");
	libcurl.SetCookieFile("c:\\cookie");
	char* pData = "maintype=10001&subtype=100&appver=2.5.19.3753&sysver=Microsoft Windows 7&applist=100:15,200:2&sign=2553d888bc922275eca2fc539a5f0c1b";
	libcurl.Post("http://interface.***********.com/v2/stat/index/jingpin", pData);
	string strRet = libcurl.GetRespons();

}

string GetUpdate(string name,int ver)
{
	char url[512] = "";
	const char *mname = name.data();
	string mmd5 = MD5("595902716@qq.com").toString();
	const char * mmkey = mmd5.c_str();
	sprintf_s(url,512, "http://127.0.0.1/autoupdate.php?name=%s&version=%02d&publicKey=%s", mname, ver,mmkey);
	//printf(url);
	CLibcurl libcurl;
	libcurl.SetUserAgent("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.86 Safari/537.36");
	libcurl.Get(url);
	const char* pHtml = libcurl.GetResponsPtr();
	const char* pError = libcurl.GetError();
	if (strlen(pHtml)==0)
	{
		return "{\"code\":\"0\"}";
	}
	return pHtml;
}  
void MakeBat(const char* app)
{
	
	char tmp[256];
	FILE *fbat;
	printf("准备更新，请不要操作!\n");
	snprintf(tmp, sizeof(tmp), "%s\\update.bat", "update");
	errno_t err;
	if (err = fopen_s(&fbat, tmp, "w+")!=0)
	{
		printf("操作失败.");
		return;
	}
	fprintf(fbat, "@echo off\n");
    fprintf(fbat, "taskkill /F /IM %s\n", app);
	fprintf(fbat, "ping -n 2 127.1>nul\n");
	fprintf(fbat, "copy /Y .\\update\\* .\\\n");
	fprintf(fbat, "del .\\update.bat\n");
	//fprintf(fbat, "%s",app);
	if(fbat)
	{
	 err = fclose(fbat);
	 if (err != 0)
	 {
		 printf("操作失败.");
		 return;
	 }
	}

	snprintf(tmp, sizeof(tmp), "%s\\update.bat", "update");
	WinExec(tmp, SW_HIDE); //隐藏窗口运行a.bat
}
