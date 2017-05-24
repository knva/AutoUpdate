// AutoUpdate.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Clibcurl.h"  
#include "configjson.h"
#include "MD5.h"
#include <time.h>
using namespace std;


#ifdef WIN32 
#define MKDIR _mkdir
#elif   (defined   (UNIX)   &&   defined(_LARGEFILE64_SOURCE)) 


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
		//printf("���ؽ��ȣ�%0.2lf%%\n", bPercent);
		printf(".");
	}
};
///
///
///LC:�����ļ�  ����1 :���ص�ַ   ����2 :�����ļ���
void Download(const char *pUrl, const char* filename,int ver);
///LC:�ύPOST ����1:������  ����2:�汾��
string PostTest(string name, int ver);
///LC:ʹ��GET��ʽ��ȡ��Ϣ ����1:������  ����2:�汾��
string GetUpdate(string name, int ver);
///LC:�����������ļ� ����1:������  ����2:�汾��
void MakeBat(const char* app,int ver);
///LC:֧�ֶ��ļ����� ����1:���ص�ַ  ����2:�����ļ��� ����3:���ذ汾��
void MoreDownload(vector<string>, vector<string>,int ver);
///
///TODO ������
int _tmain(int argc, _TCHAR* argv[])
{
	//DownloadTest();  
	//PostTest();  
	string name="";
	int ver=0;
	int nver = 0;
	string json = "";

	vector<string> downloadurl;
	vector<string> filename;
	configjson *rj = new configjson();
	rj->readjson(name, ver);
	if(GETMODE){
		json = GetUpdate(name, ver);
	}
	else
	{
		json = PostTest(name, ver);
	}
	if (json == ERR_RESULT||json =="")
		return 0;
	rj->readjson(json, downloadurl, filename,nver);

	printf("�����°汾���汾��:%d\n", nver);

	MoreDownload(downloadurl, filename,nver);

	MakeBat(name.c_str(),nver);
	system("pause");
	return 0;
}
void MoreDownload(vector<string>downloadurl, vector<string>filename,int ver)
{
	char updatedir[128] = "";
	sprintf_s(updatedir, 128, "update//%d//", ver );
	MKDIR(updatedir);
	for (unsigned int i = 0; i < downloadurl.size(); i++)
	{
		Download(downloadurl.at(i).c_str(), filename.at(i).c_str(),ver);
	}
}

void Download(const char *pUrl,const char* filename,int ver)
{
	CLibcurl libcurl;
	CLibcurlCallbackEx cb;
	libcurl.SetUserAgent("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.86 Safari/537.36");
	libcurl.SetConnectTimeout(2);
	libcurl.SetResumeFrom(0);
	libcurl.SetCallback(&cb, NULL);
	char updatedir[128] = "";
	sprintf_s(updatedir, 128,"update//%d//%s",ver, filename);
	
	libcurl.DownloadToFile(pUrl, updatedir);
}

string PostTest(string name, int ver)
{
	char url[512] = "";
	const char *mname = name.data();
	char strver[10] = "";
	sprintf_s(strver, 10, "%d", ver);
	char md5str[128] = "";
	time_t tt = time(NULL);//��䷵�ص�ֻ��һ��ʱ���
	sprintf_s(md5str, 128, "595902716@qq.com%lld",tt/100);
	string mmd5 = MD5(md5str).toString();
	const char * mmkey = mmd5.c_str();
	sprintf_s(url, 512, "name=%s&version=%02d&publicKey=%s", mname, ver, mmkey);
	CLibcurl libcurl;
	libcurl.SetUserAgent("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.86 Safari/537.36");
	libcurl.SetPort(80);
	libcurl.SetConnectTimeout(2);
	libcurl.AddHeader("name", mname);
	libcurl.AddHeader("version", strver);
	libcurl.AddHeader("publicKey", mmkey);
	libcurl.SetCookieFile("c:\\cookie");
	//char* pData = "maintype=10001&subtype=100&appver=2.5.19.3753&sysver=Microsoft Windows 7&applist=100:15,200:2&sign=2553d888bc922275eca2fc539a5f0c1b";
	libcurl.Post("http://127.0.0.1/autoupdate.php", url);
	const char* pHtml = libcurl.GetResponsPtr();
	const char* pError = libcurl.GetError();
	if (strlen(pHtml) == 0)
	{
		return ERR_RESULT;
	}
	return pHtml;
}

string GetUpdate(string name,int ver)
{
	char url[512] = "";
	const char *mname = name.data();
	char md5str[128] = "";
	time_t tt = time(NULL);//��䷵�ص�ֻ��һ��ʱ���
	sprintf_s(md5str, 128, "595902716@qq.com%lld", tt / 100);
	string mmd5 = MD5(md5str).toString();
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
		return ERR_RESULT;
	}
	return pHtml;
}  
void MakeBat(const char* app,int ver)
{
	
	char tmp[256];
	FILE *fbat;
	printf("׼�����£��벻Ҫ����!\n");
	snprintf(tmp, sizeof(tmp), "%s\\update.bat", "update");
	errno_t err;
	if (err = fopen_s(&fbat, tmp, "w+")!=0)
	{
		printf("����ʧ��.");
		return;
	}
	fprintf(fbat, "@echo off\n");
    fprintf(fbat, "taskkill /F /IM %s\n", app);
	fprintf(fbat, "ping -n 2 127.1>nul\n");
	fprintf(fbat, "copy /Y .\\update\\%d\\* .\\\n",ver);
	
	//fprintf(fbat, "%s",app);
	if(fbat)
	{
	 err = fclose(fbat);
	 if (err != 0)
	 {
		 printf("����ʧ��.");
		 return;
	 }
	}

	snprintf(tmp, sizeof(tmp), "%s\\update.bat", "update");
	WinExec(tmp, SW_HIDE); //���ش�������a.bat
}
