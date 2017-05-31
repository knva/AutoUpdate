#include "configjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "base64.h"
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
#ifdef _WIN32
		FILE* fp;
        fopen_s(&fp, FILENAME, "rb");
#else
        FILE* fp = fopen(FILENAME, "r");
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
    for (SizeType i = 0; i < ns.Size(); i++)
	{
		url.insert(url.begin(), ns[i].GetString());
		filename.insert(filename.begin(), fs[i].GetString());
	}
	return 1;
}

int configjson::checkUp(string &name, int &ver,string &url)
{
	ZBase64 zBase;
	if(checkJson()){
#ifdef WIN32 
		FILE* fp;
        fopen_s(&fp, FILENAME, "rb");
#else
        FILE* fp = fopen(FILENAME, "r");
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
		string murl = u.GetString();
		int OutByte = 0;
		string strTmpResult = zBase.Decode(murl.c_str(), murl.length(), OutByte);
		url = strTmpResult;
		
		return 1;
	}
	else{
		return 0;
	}
}

void configjson::createjson(string filename, string app, int ver,string url)
{
	ZBase64 zBase;
	Document document;
	document.SetObject();
	Value appname;
	char buffer[32];
    int len = snprintf(buffer, 32, "%s", app.c_str());
	appname.SetString(buffer, len, document.GetAllocator());
	memset(buffer, 0, sizeof(buffer));
	Value aupurl;
	string encoded = zBase.Encode(reinterpret_cast<const unsigned char*>(url.c_str()), url.length());
    len = snprintf(buffer, 32, "%s", encoded.c_str());
    aupurl.SetString(buffer,len,document.GetAllocator());
	memset(buffer, 0, sizeof(buffer));
	Value version;
	version.SetInt(ver);
	document.AddMember("name", appname, document.GetAllocator());
	document.AddMember("ver", version, document.GetAllocator());
	document.AddMember("url", aupurl, document.GetAllocator());


#ifdef WIN32 
	FILE* fp;
    fopen_s(&fp, filename, "wb");
#else
    FILE* fp = fopen(filename.c_str(), "w");
#endif 
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	document.Accept(writer);
	fclose(fp);

}

int configjson::checkJson()
{
#ifdef _WIN32
	FILE *fp;
    fopen_s(&fp,FILENAME, "r");
#else
	FILE *fp;
	fp = fopen(FILENAME, "r");
#endif
	if (fp == NULL)
        return 0;
	else
	{
        fclose(fp);
        return 1;
	}
}
