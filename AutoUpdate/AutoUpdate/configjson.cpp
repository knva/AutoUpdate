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
configjson::configjson(string app, int ver)
{
	appname = app;
	version = ver;
}


configjson::~configjson()
{
}

int configjson::readjson(string &name, int &ver)
{

		this->createjson(FILENAME, appname, version);
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

	
	return 0;
}
int configjson::readjson(string json, vector<string> &url, vector<string> &filename, int &ver)
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
	return 0;
}

void configjson::createjson(string filename, string app, int ver)
{
	Document document;
	document.SetObject();
	Value appname;
	char buffer[32];
	int len = sprintf_s(buffer, 32, "%s", app.c_str()); // 动态创建的字符串。
	appname.SetString(buffer, len, document.GetAllocator());
	memset(buffer, 0, sizeof(buffer));
	Value version;
	version.SetInt(ver);
	document.AddMember("name", appname, document.GetAllocator());
	document.AddMember("ver", version, document.GetAllocator());


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