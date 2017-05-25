// AutoUpdate.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Clibcurl.h"  
#include "configjson.h"
#include "MD5.h"
#include <time.h>
using namespace std;


#ifdef _WIN32 
#define MKDIR(a) _mkdir((a))
#define ACCESS _access
#elif _LINUX
#define MKDIR(a) mkdir((a),0755)
#define ACCESS access

#endif 
#define ERR_RESULT "{\"result\":\"failed\"}"
//GETMODE 
//0 post
//1 get
#define GETMODE 0
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
///LC:下载文件  参数1 :下载地址   参数2 :保存文件名
void Download(string pUrl, string filename, int ver);
///LC:提交POST 参数1:程序名  参数2:版本号
string PostTest(string name, int ver, string url);
///LC:使用GET方式获取信息 参数1:程序名  参数2:版本号
string GetUpdate(string name, int ver, string url);
///LC:创建批处理文件 参数1:程序名  参数2:版本号
void MakeBat(string app, int ver);
///LC:支持多文件更新 参数1:下载地址  参数2:下载文件名 参数3:下载版本号
void MoreDownload(vector<string>, vector<string>, int ver);
///UP:存在json时,使用--u命令直接更新
void checkUpdate();
///UP:创建目录
int CreatDir(char *);
///TODO 主函数
int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		if (argc == 2)
		{
			if (strcmp(argv[1], "--u")==0)
			{
				checkUpdate();
				return 0;
			}
		}
		printf("请输入软件名称,软件版本及更新地址,例如:update.exe MyApp.exe 1 update.com\n");
		printf("运行软件之后,软件会生成AppConfig.json ,其中包含了软件名称及版本号.");
		
		return 0;
	}

	string name = "";
	int ver = 0;
	int nver = 0;
	string json = "";
	vector<string> downloadurl;
	vector<string> filename;
	string exename = argv[1];
	
	string url = argv[3];
	configjson *rj = new configjson(exename, atoi(argv[2]),url);
	rj->readjson(name, ver);

	if (GETMODE) {
		json = GetUpdate(name, ver, url);
	}
	else
	{
		json = PostTest(name, ver, url);
	}
	if (json == ERR_RESULT || json == "")
		return 0;
	rj->readupjson(json, downloadurl, filename, nver);

	printf("发现新版本，版本号:%d\n", nver);

	MoreDownload(downloadurl, filename, nver);

	MakeBat(name, nver);

	//system("pause");

	return 0;
}
void MoreDownload(vector<string>downloadurl, vector<string>filename, int ver)
{
	char updatedir[128] = "";
	sprintf_s(updatedir, 128, "update//%d//", ver);
	CreatDir(updatedir);
	for (unsigned int i = 0; i < downloadurl.size(); i++)
	{
		Download(downloadurl.at(i), filename.at(i), ver);
	}
}

void Download(string pUrl, string filename, int ver)
{
	CLibcurl libcurl;
	CLibcurlCallbackEx cb;
	libcurl.SetUserAgent("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.86 Safari/537.36");
	libcurl.SetConnectTimeout(2);
	libcurl.SetResumeFrom(0);
	libcurl.SetCallback(&cb, NULL);
	char updatedir[128] = "";
	sprintf_s(updatedir, 128, "update//%d//%s", ver, filename.c_str());

	libcurl.DownloadToFile(pUrl.c_str(), updatedir);
}

string PostTest(string name, int ver, string updateurl)
{
	char url[128] = "";
	const char *mname = name.data();
	char strver[10] = "";
	sprintf_s(strver, 10, "%d", ver);
	char md5str[128] = "";
	time_t tt = time(NULL);//这句返回的只是一个时间戳
	sprintf_s(md5str, 128, "595902716@qq.com%lld", tt / 100);
	string mmd5 = MD5(md5str).toString();
	const char * mmkey = mmd5.c_str();
	sprintf_s(url, 128, "name=%s&version=%02d&publicKey=%s", mname, ver, mmkey);
	CLibcurl libcurl;
	libcurl.SetUserAgent("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.86 Safari/537.36");
	libcurl.SetPort(80);
	libcurl.SetConnectTimeout(2);
	libcurl.AddHeader("name", mname);
	libcurl.AddHeader("version", strver);
	libcurl.AddHeader("publicKey", mmkey);
	libcurl.SetCookieFile("c:\\cookie");
	//char* pData = "maintype=10001&subtype=100&appver=2.5.19.3753&sysver=Microsoft Windows 7&applist=100:15,200:2&sign=2553d888bc922275eca2fc539a5f0c1b";
	char postUrl[128];
	sprintf_s(postUrl, 128, "http://%s/autoupdate.php", updateurl.c_str());
	libcurl.Post(postUrl, url);
	const char* pHtml = libcurl.GetResponsPtr();
	const char* pError = libcurl.GetError();
	if (strlen(pHtml) == 0)
	{
		return ERR_RESULT;
	}
	return pHtml;
}

string GetUpdate(string name, int ver, string updateurl)
{
	char url[512] = "";
	const char *mname = name.data();
	char md5str[128] = "";
	time_t tt = time(NULL);//这句返回的只是一个时间戳
	sprintf_s(md5str, 128, "595902716@qq.com%lld", tt / 100);
	string mmd5 = MD5(md5str).toString();
	const char * mmkey = mmd5.c_str();
	sprintf_s(url, 512, "http://%s/autoupdate.php?name=%s&version=%02d&publicKey=%s", updateurl.c_str(), mname, ver, mmkey);
	//printf(url);
	CLibcurl libcurl;
	libcurl.SetUserAgent("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.86 Safari/537.36");
	libcurl.Get(url);
	const char* pHtml = libcurl.GetResponsPtr();
	const char* pError = libcurl.GetError();
	if (strlen(pHtml) == 0)
	{
		return ERR_RESULT;
	}
	return pHtml;
}
void MakeBat(string app, int ver)
{

	char tmp[256];
	FILE *fbat;
	printf("准备更新，请不要操作!\n");
	snprintf(tmp, sizeof(tmp), "%s\\update.bat", "update");
	errno_t err;
	if (err = fopen_s(&fbat, tmp, "w+") != 0)
	{
		printf("操作失败.");
		return;
	}
	fprintf(fbat, "@echo off\n");
	fprintf(fbat, "taskkill /F /IM %s\n", app.c_str());
	fprintf(fbat, "ping -n 2 127.1>nul\n");
	fprintf(fbat, "copy /Y .\\update\\%d\\* .\\\n", ver);

	fprintf(fbat, "start %s",app.c_str());
	if (fbat)
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

void checkUpdate()
{
	string name = "";
	int ver = 0;
	int nver = 0;
	string json = "";
	string url = "";
	vector<string> downloadurl;
	vector<string> filename;
	configjson *cj = new configjson();
	if (cj->checkUp(name, nver, url))
	{
		if (GETMODE) {
			json = GetUpdate(name, ver, url);
		}
		else
		{
			json = PostTest(name, ver, url);
		}
		if (json == ERR_RESULT || json == "")
			return;

		cj->readupjson(json, downloadurl, filename, nver);

		printf("发现新版本，版本号:%d\n", nver);

		MoreDownload(downloadurl, filename, nver);

		MakeBat(name, nver);
	}
	else
	{
		return;
	}
}

int CreatDir(char *pDir)
{
	int i = 0;
	int iRet;
	int iLen;
	char* pszDir;
	if (NULL == pDir)
	{
		return 0;
	}

	pszDir = _strdup(pDir);
	iLen = strlen(pszDir);
	// 创建中间目录
	for (i = 0; i < iLen; i++)
	{
		if (pszDir[i] == '\\' || pszDir[i] == '/')
		{
			pszDir[i] = '\0';
			//如果不存在,创建
			iRet = ACCESS(pszDir, 0);
			if (iRet != 0)
			{
				iRet = MKDIR(pszDir);
				if (iRet != 0)
				{
					return -1;
				}
			}
			//支持linux,将所有\换成/
			pszDir[i] = '/';
		}
	}
	iRet = MKDIR(pszDir);
	free(pszDir);
	return iRet;
}