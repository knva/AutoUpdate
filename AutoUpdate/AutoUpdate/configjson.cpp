#include "stdafx.h"
#include "configjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include <fstream>
using namespace rapidjson;
using namespace std;
#define FILENAME "./AppConfig.json"
configjson::configjson(string app, int ver,string url)
{
	appname = app;
	version = ver;
	upurl = url;
}


configjson::~configjson()
{
}

int configjson::readjson(string &name, int &ver)
{

		this->createjson(FILENAME, appname, version,upurl);
#ifdef WIN32 
		FILE* fp;
		fopen_s(&fp, FILENAME, "rb"); // 非 Windows 平台使用 "r"
#elif   (defined   (UNIX)   &&   defined(_LARGEFILE64_SOURCE)) 
		FILE* fp = fopen(FILENAME, "r"); // 非 Windows 平台使用 "r"
#endif 
		char readBuffer[512];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		Document d;
		d.ParseStream(is);
		fclose(fp);
		Value& n = d["name"];
		Value& v = d["ver"];

		name = n.GetString();
		ver = v.GetInt();
	return 1;
}
int configjson::readupjson(string json, vector<string> &url, vector<string> &filename, int &ver)
{
	const char* mjson = json.c_str();
	Document d;
	d.Parse(mjson);

	Value& s = d["ver"];
	Value& ns = d["url"];
	Value& fs = d["file"];

	ver = s.GetInt();
	for (SizeType i = 0; i < ns.Size(); i++) // 使用 SizeType 而不是 size_t
	{
		url.insert(url.begin(), ns[i].GetString());
		filename.insert(filename.begin(), fs[i].GetString());
	}
	return 1;
}

int configjson::checkUp(string &name, int &ver,string &url)
{
	if(checkJson()){
#ifdef WIN32 
		FILE* fp;
		fopen_s(&fp, FILENAME, "rb"); // 非 Windows 平台使用 "r"
#elif   (defined   (UNIX)   &&   defined(_LARGEFILE64_SOURCE)) 
		FILE* fp = fopen(FILENAME, "r"); // 非 Windows 平台使用 "r"
#endif 
		char readBuffer[512];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		Document d;
		d.ParseStream(is);
		fclose(fp);
		Value& n = d["name"];
		Value& v = d["ver"];
		Value& u = d["url"];

		name = n.GetString();
		ver = v.GetInt();
		url = u.GetString();
		return 1;
	}
	else{
		return 0;
	}
}

void configjson::createjson(string filename, string app, int ver,string url)
{
	Document document;
	document.SetObject();
	Value appname;
	char buffer[32];
	int len = sprintf_s(buffer, 32, "%s", app.c_str()); // 动态创建的字符串。
	appname.SetString(buffer, len, document.GetAllocator());
	memset(buffer, 0, sizeof(buffer));
	Value aupurl;
	len = sprintf_s(buffer, 32, "%s", url.c_str()); // 动态创建的字符串。
	aupurl.SetString(buffer, len, document.GetAllocator());
	memset(buffer, 0, sizeof(buffer));
	Value version;
	version.SetInt(ver);
	document.AddMember("name", appname, document.GetAllocator());
	document.AddMember("ver", version, document.GetAllocator());
	document.AddMember("url", aupurl, document.GetAllocator());


#ifdef WIN32 
	FILE* fp;
	fopen_s(&fp, FILENAME, "wb"); // 非 Windows 平台使用 "r"
#elif   (defined   (UNIX)   &&   defined(_LARGEFILE64_SOURCE)) 
	FILE* fp = fopen(FILENAME, "w"); // 非 Windows 平台使用 "r"
#endif 
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	document.Accept(writer);
	fclose(fp);

}

int configjson::checkJson()
{

	FILE *fp;
	fopen_s(&fp,FILENAME, "r");   //fopenC库函数用于打文件,"r"读模式种模式文件存则能功读模式打fopen返非0文件描述符文件存则fopen返NULL（NULL意思空）利用点判断文件否存
	if (fp == NULL)
		return 0;   //存返0
	else
	{
		fclose(fp);  //存要先前打文件关掉
		return 1;    //返1
	}
}
