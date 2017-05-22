#include "stdafx.h"
#include "configjson.h"

#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"
#include <iostream>

using namespace rapidjson;
using namespace std;
configjson::configjson()
{
}


configjson::~configjson()
{
}

int configjson::readjson(string &name,int &ver)
{
	// 1. �� JSON ������ DOM��
	const char* json = "{\"name\":\"update.exe\",\"version\":3}";
	Document d;
	d.Parse(json);
	// 2. ���� DOM �����޸ġ�
	Value& ns = d["name"];
	Value& s = d["version"];
	//s.SetInt(s.GetInt() + 1);
	//// 3. �� DOM ת����stringify���� JSON��
	//StringBuffer buffer;
	//Writer<StringBuffer> writer(buffer);
	//d.Accept(writer);
	// Output {"project":"rapidjson","stars":11}
	//std::cout << buffer.GetString() << std::endl;
	name = ns.GetString();
	ver = s.GetInt();
	return 0;
}
int configjson::readjson(string json ,string &url, int &ver)
{
	// 1. �� JSON ������ DOM��
	const char* mjson = json.c_str();
	Document d;
	d.Parse(mjson);
	// 2. ���� DOM �����޸ġ�
	Value& ns = d["url"];
	Value& s = d["ver"];
	//s.SetInt(s.GetInt() + 1);
	//// 3. �� DOM ת����stringify���� JSON��
	//StringBuffer buffer;
	//Writer<StringBuffer> writer(buffer);
	//d.Accept(writer);
	// Output {"project":"rapidjson","stars":11}
	//std::cout << buffer.GetString() << std::endl;
	url = ns.GetString();
	ver = s.GetInt();
	return 0;
}
void configjson::writejson()
{
}
void configjson::createjson()
{
}