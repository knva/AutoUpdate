/*****************************************
*封装Libcurl下载库
*author：knva
*date：2017年5月22日
*/
#pragma once  
#include <curl/curl.h>  
#include <string>  
using std::string;





class CLibcurlCallback
{
public:
	virtual void Progress(void* lpParam, double dTotal, double bLoaded) = 0;
};

enum LibcurlFlag
{
	Lf_None = 0,
	Lf_Download,
	Lf_Post,
	Lf_Get,
};

class CLibcurl
{
public:
	CLibcurl(void);
	~CLibcurl(void);
	/******************************************************************************
	*封装类的外部调用接口
	*/
	bool SetPort(LONG port);                                            //设置连接端口号  
	bool SetTimeout(int nSecond);                                       //设置执行超时（秒）  
	bool SetConnectTimeout(int nSecond);                                //设置连接超时（秒）  
	bool SetUserAgent(LPCSTR lpAgent);                                  //设置用户代理  
	bool SetResumeFrom(LONG lPos);                                      //设置断点续传起始位置  
	bool SetResumeFromLarge(LONGLONG llPos);                            //设置断点续传起始位置，针对大文件  
	bool AddHeader(LPCSTR lpKey, LPCSTR lpValue);                       //添加自定义头  
	void ClearHeaderList();                                             //清理HTTP列表头  
	bool SetCookie(LPCSTR lpCookie);                                    //设置HTTP请求cookie  
	bool SetCookieFile(LPCSTR lpFilePath);                              //设置HTTP请求cookie文件  
	const char* GetError()const;                                        //获取错误详细信息  
	void SetCallback(CLibcurlCallback* pCallback, void* lpParam);       //设置下载进度回调  
	bool DownloadToFile(LPCSTR lpUrl, LPCSTR lpFile);                   //下载文件到磁盘  
	bool Post(LPCSTR lpUrl, LPCSTR lpData);                             //Post 字符串数据  
	bool Post(LPCSTR lpUrl, unsigned char* lpData, unsigned int nSize); //Post 字符串或者二进制数据  
	bool Get(LPCSTR lpUrl);                                             //Get 请求  
	const string& GetRespons()const;                                    //获取Post/Get请求返回数据  
	const char* GetResponsPtr()const;                                   //获取Post/Get请求返回数据  

protected:
	static size_t   WriteCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam);
	static size_t   HeaderCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam);
	static int      ProgressCallback(void *pParam, double dltotal, double dlnow, double ultotal, double ulnow);

private:
	CURL    *m_pCurl;
	LONG    m_nPort;
	HANDLE  m_hFile;
	CURLcode m_curlCode;
	string  m_strRespons;
	LibcurlFlag m_lfFlag;
	curl_slist *m_curlList;
	void    *m_pCallbackParam;
	CLibcurlCallback    *m_pCallback;
};