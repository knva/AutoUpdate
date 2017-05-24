#pragma once
#include <string>
#include "stdafx.h"
using namespace std;
class configjson
{
public:
	configjson();
	~configjson();
	int readjson(string &name, int &ver);
	int readjson(string json, vector<string> &url, vector<string> &filename ,int &ver);
private:
	
	void writejson();
	void createjson();
};

