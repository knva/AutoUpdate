#pragma once
#include <string>
using namespace std;
class configjson
{
public:
	configjson();
	~configjson();
	int readjson(string &name, int &ver);
	int readjson(string json, string &url, int &ver);
private:
	
	void writejson();
	void createjson();
};

