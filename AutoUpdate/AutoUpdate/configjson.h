#pragma once
#include <string>
#include "stdafx.h"
using namespace std;
class configjson
{
public:
	configjson(string app , int ver);
	~configjson();
	int readjson(string &name, int &ver);
	int readjson(string json, vector<string> &url, vector<string> &filename ,int &ver);
private:
	void createjson(string filename,string app ,  int ver);
	string appname;
	int version;
};

