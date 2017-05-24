#include "stdafx.h"
#include "configjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>

using namespace rapidjson;
using namespace std;
configjson::configjson()
{
}


configjson::~configjson()
{
}

int configjson::readjson(string &name, int &ver)
{
	// 1. 把 JSON 解析至 DOM。
	const char* json = "{\"name\":\"update.exe\",\"version\":3}";
	Document d;
	d.Parse(json);
	// 2. 利用 DOM 作出修改。
	Value& ns = d["name"];
	Value& s = d["version"];
	//s.SetInt(s.GetInt() + 1);
	//// 3. 把 DOM 转换（stringify）成 JSON。
	//StringBuffer buffer;
	//Writer<StringBuffer> writer(buffer);
	//d.Accept(writer);
	// Output {"project":"rapidjson","stars":11}
	//std::cout << buffer.GetString() << std::endl;
	name = ns.GetString();
	ver = s.GetInt();
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
void configjson::writejson()
{
}
void configjson::createjson()
{
}