/*****************************************
*��װLibcurl���ؿ�
*author��knva
*date��2017��5��22��
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
	*��װ����ⲿ���ýӿ�
	*/
	bool SetPort(LONG port);                                            //�������Ӷ˿ں�  
	bool SetTimeout(int nSecond);                                       //����ִ�г�ʱ���룩  
	bool SetConnectTimeout(int nSecond);                                //�������ӳ�ʱ���룩  
	bool SetUserAgent(LPCSTR lpAgent);                                  //�����û�����  
	bool SetResumeFrom(LONG lPos);                                      //���öϵ�������ʼλ��  
	bool SetResumeFromLarge(LONGLONG llPos);                            //���öϵ�������ʼλ�ã���Դ��ļ�  
	bool AddHeader(LPCSTR lpKey, LPCSTR lpValue);                       //����Զ���ͷ  
	void ClearHeaderList();                                             //����HTTP�б�ͷ  
	bool SetCookie(LPCSTR lpCookie);                                    //����HTTP����cookie  
	bool SetCookieFile(LPCSTR lpFilePath);                              //����HTTP����cookie�ļ�  
	const char* GetError()const;                                        //��ȡ������ϸ��Ϣ  
	void SetCallback(CLibcurlCallback* pCallback, void* lpParam);       //�������ؽ��Ȼص�  
	bool DownloadToFile(LPCSTR lpUrl, LPCSTR lpFile);                   //�����ļ�������  
	bool Post(LPCSTR lpUrl, LPCSTR lpData);                             //Post �ַ�������  
	bool Post(LPCSTR lpUrl, unsigned char* lpData, unsigned int nSize); //Post �ַ������߶���������  
	bool Get(LPCSTR lpUrl);                                             //Get ����  
	const string& GetRespons()const;                                    //��ȡPost/Get���󷵻�����  
	const char* GetResponsPtr()const;                                   //��ȡPost/Get���󷵻�����  

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