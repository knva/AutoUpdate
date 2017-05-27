#pragma once
#include <string>
#include <vector>
using namespace std;
class configjson
{
public:
	configjson(string app ="", int ver=0,string url ="");
	~configjson();
	int readjson(string &name, int &ver);
	int readupjson(string json, vector<string> &url, vector<string> &filename ,int &ver);
	int checkUp(string & name, int & ver,string &url);
	
private:
	void createjson(string filename,string app ,  int ver,string url);
	int checkJson();
	string appname;
	int version;
	string upurl;
};

