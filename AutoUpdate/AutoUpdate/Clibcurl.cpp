#include "StdAfx.h"  
#include "Clibcurl.h"  
#include <assert.h>  

#ifdef _DEBUG  
#pragma comment(lib, "lib/libcurld")  
#else  
#pragma comment(lib, "lib/libcurl")  
#endif  
#pragma comment(lib, "lib/libeay32")  
#pragma comment(lib, "lib/ssleay32")  
#pragma comment(lib, "ws2_32")  
#pragma comment(lib, "Iphlpapi")  
#pragma comment(lib, "Wldap32")  


CLibcurl::CLibcurl(void)
	: m_pCurl(NULL)
	, m_nPort(80)
	, m_hFile(INVALID_HANDLE_VALUE)
	, m_pCallback(NULL)
	, m_pCallbackParam(NULL)
	, m_curlCode(CURLE_OK)
	, m_lfFlag(Lf_None)
	, m_curlList(NULL)
{
	m_pCurl = curl_easy_init();
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this);
}


CLibcurl::~CLibcurl(void)
{
	ClearHeaderList();
	curl_easy_cleanup(m_pCurl);
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
	}
}

bool CLibcurl::SetPort(LONG port)
{
	if (port == m_nPort)
		return true;
	m_nPort = port;
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_PORT, m_nPort);
	return CURLE_OK == m_curlCode;
}

bool CLibcurl::SetTimeout(int nSecond)
{
	if (nSecond<0)
		return false;
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, nSecond);
	return CURLE_OK == m_curlCode;
}

bool CLibcurl::SetConnectTimeout(int nSecond)
{
	if (nSecond<0)
		return false;
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, nSecond);
	return CURLE_OK == m_curlCode;
}

bool CLibcurl::SetUserAgent(LPCSTR lpAgent)
{
	if (NULL == lpAgent)
		return false;
	int nLen = strlen(lpAgent);
	if (nLen == 0)
		return false;
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_USERAGENT, lpAgent);
	return CURLE_OK == m_curlCode;
}

bool CLibcurl::SetResumeFrom(LONG lPos)
{
	if (lPos<0)
		return false;
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_RESUME_FROM, lPos);
	return CURLE_OK == m_curlCode;
}

bool CLibcurl::SetResumeFromLarge(LONGLONG llPos)
{
	if (llPos<0)
		return false;
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_RESUME_FROM_LARGE, llPos);
	return CURLE_OK == m_curlCode;
}

bool CLibcurl::AddHeader(LPCSTR lpKey, LPCSTR lpValue)
{
	assert(lpKey != NULL && lpValue != NULL);
	int nLen1 = strlen(lpKey), nLen2 = strlen(lpValue);
	assert(nLen1>0 && nLen2>0);
	string strHeader(lpKey);
	strHeader.append(": ");
	strHeader.append(lpValue);
	m_curlList = curl_slist_append(m_curlList, strHeader.c_str());
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, m_curlList);
	return CURLE_OK == m_curlCode;
}

void CLibcurl::ClearHeaderList()
{
	if (m_curlList)
	{
		curl_slist_free_all(m_curlList);
		m_curlList = NULL;
	}
}

bool CLibcurl::SetCookie(LPCSTR lpCookie)
{
	assert(lpCookie != NULL);
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_COOKIE, lpCookie);
	return CURLE_OK == m_curlCode;
}

bool CLibcurl::SetCookieFile(LPCSTR lpFilePath)
{
	assert(lpFilePath != NULL);
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_COOKIEFILE, lpFilePath);
	return CURLE_OK == m_curlCode;
}

const char* CLibcurl::GetError() const
{
	return curl_easy_strerror(m_curlCode);
}

void CLibcurl::SetCallback(CLibcurlCallback* pCallback, void* lpParam)
{
	m_pCallbackParam = lpParam;
	m_pCallback = pCallback;
}

bool CLibcurl::DownloadToFile(LPCSTR lpUrl, LPCSTR lpFile)
{
	CURLcode code = curl_easy_setopt(m_pCurl, CURLOPT_URL, lpUrl);
	DeleteFileA(lpFile);
	m_hFile = CreateFileA(lpFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == m_hFile)
	{
		return FALSE;
	}
	curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSFUNCTION, ProgressCallback);
	curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSDATA, this);
	m_lfFlag = Lf_Download;
	//开始执行请求  
	m_curlCode = curl_easy_perform(m_pCurl);
	return CURLE_OK == m_curlCode;
}

bool CLibcurl::Post(LPCSTR lpUrl, LPCSTR lpData)
{
	assert(lpData != NULL);
	curl_easy_setopt(m_pCurl, CURLOPT_POST, 1);
	curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, lpData);
	//curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDSIZE, lpData);  
	curl_easy_setopt(m_pCurl, CURLOPT_URL, lpUrl);
	m_lfFlag = Lf_Post;
	m_strRespons.clear();
	m_curlCode = curl_easy_perform(m_pCurl);
	return CURLE_OK == m_curlCode;
}

bool CLibcurl::Post(LPCSTR lpUrl, unsigned char* lpData, unsigned int nSize)
{
	assert(lpData != NULL && nSize>0);
	curl_easy_setopt(m_pCurl, CURLOPT_POST, 1);
	curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, lpData);
	curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDSIZE, nSize);
	curl_easy_setopt(m_pCurl, CURLOPT_URL, lpUrl);
	m_lfFlag = Lf_Post;
	m_strRespons.clear();
	m_curlCode = curl_easy_perform(m_pCurl);
	return CURLE_OK == m_curlCode;
}

bool CLibcurl::Get(LPCSTR lpUrl)
{
	assert(lpUrl != NULL);
	curl_easy_setopt(m_pCurl, CURLOPT_HTTPGET, 1);
	curl_easy_setopt(m_pCurl, CURLOPT_URL, lpUrl);
	curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1);//支持重定向  
	curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
	m_lfFlag = Lf_Get;
	m_strRespons.clear();
	m_curlCode = curl_easy_perform(m_pCurl);
	return CURLE_OK == m_curlCode;
}

const string& CLibcurl::GetRespons() const
{
	return m_strRespons;
}

const char* CLibcurl::GetResponsPtr() const
{
	return m_strRespons.c_str();
}

size_t CLibcurl::WriteCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam)
{
	//把下载到的数据以追加的方式写入文件(一定要有a，否则前面写入的内容就会被覆盖了)  
	CLibcurl* pThis = (CLibcurl*)pParam;
	DWORD dwWritten = 0;
	switch (pThis->m_lfFlag)
	{
	case Lf_Download://下载  
	{
		if (pThis->m_hFile == INVALID_HANDLE_VALUE)
			return 0;
	//	printf("size1:%d size2:%d", nSize, nMemByte);
		if (!WriteFile(pThis->m_hFile, pBuffer, nSize*nMemByte, &dwWritten, NULL))
			return 0;
		
	}
	break;
	case Lf_Post://Post数据  
	case Lf_Get://Get数据  
	{
		pThis->m_strRespons.append((const char*)pBuffer, nSize*nMemByte);
		dwWritten = nSize*nMemByte;
	}
	break;
	case Lf_None://未定义  
		break;
	}
	return dwWritten;
}

size_t CLibcurl::HeaderCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam)
{
	CLibcurl* pThis = (CLibcurl*)pParam;
	return 0;
}

int CLibcurl::ProgressCallback(void *pParam, double dltotal, double dlnow, double ultotal, double ulnow)
{
	CLibcurl* pThis = (CLibcurl*)pParam;
	if (pThis->m_pCallback)
	{
		pThis->m_pCallback->Progress(pThis->m_pCallbackParam, dltotal, dlnow);
	}
	return 0;
}