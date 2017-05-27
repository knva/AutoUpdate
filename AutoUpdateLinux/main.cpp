#include <iostream>
#include "configjson.h"
#include "string.h"
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

#ifdef _WIN32
#define MKDIR(a) _mkdir((a))
#define ACCESS _access
#else
#define MKDIR(a) mkdir((a),0755)
#define ACCESS access

#endif
#define ERR_RESULT "{\"result\":\"failed\"}"
///UP:创建目录
int CreatDir(char *);
///LC:支持多文件更新 参数1:下载地址  参数2:下载文件名 参数3:下载版本号
void MoreDownload(vector<string>, vector<string>, int ver);
int main() {
    //cout << "!!!Hello Wor123123ld!!!" << endl; // prints !!!Hello World!!!
    vector<string> downloadurl;
    vector<string> filename;
    int nver;
    configjson *cj = new configjson("update.exe",1,"192.168.1.100");
    cj->readupjson("{\"status\":\"ok\",\"ver\":30,\"url\":[\"http://127.0.0.1/update.exe\",\"http://127.0.0.1/update.dll\"],\"file\":[\"update.exe\",\"update.dll\"]}",downloadurl,filename,nver);
    MoreDownload(downloadurl,filename,3);
    return 0;
}

void MoreDownload(vector<string>downloadurl, vector<string>filename, int ver)
{
    char updatedir[128] = "";
    snprintf(updatedir, 128, "update//%d//", ver);
    CreatDir(updatedir);
    for (unsigned int i = 0; i < downloadurl.size(); i++)
    {
        cout <<downloadurl.at(i)<<"\n"<< filename.at(i)<<"\n"<< ver <<endl;
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

    pszDir = strdup(pDir);
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
