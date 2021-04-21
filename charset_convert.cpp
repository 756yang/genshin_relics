#include "charset_convert.h"
//经验证，无问题
//GBC到UTF-8的转换
int GBCToUtf8(const char* ansi, char* utf8, unsigned int utf8_bufsize)
{
	assert(NULL != ansi && (NULL != utf8 || 0 == utf8_bufsize));
	int len = MultiByteToWideChar(CP_GBC, 0, ansi, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	MultiByteToWideChar(CP_GBC, 0, ansi, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	if(len > utf8_bufsize) return -len;
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8, len, NULL, NULL);
	delete[] wstr;
	return len;
}

//UTF-8到GBC的转换
int Utf8ToGBC(const char* utf8, char* ansi, unsigned int ansi_bufsize)
{
	assert(NULL != utf8 && (NULL != ansi || 0 == ansi_bufsize));
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_GBC, 0, wstr, -1, NULL, 0, NULL, NULL);
	if(len > ansi_bufsize) return -len;
	WideCharToMultiByte(CP_GBC, 0, wstr, -1, ansi, len, NULL, NULL);
	delete[] wstr;
	return len;
}

//GBC到Unicode的转换
int GBCToUnicode(const char* ansi, wchar_t* unicode, unsigned int uni_bufsize)
{
	assert(NULL != ansi && (NULL != unicode || 0 == uni_bufsize));
	int len = MultiByteToWideChar(CP_GBC, 0, ansi, -1, NULL, 0);
	if(len > uni_bufsize) return -len;
	MultiByteToWideChar(CP_GBC, 0, ansi, -1, unicode, len);
	return len;
}

//Unicode到GBC的转换
int UnicodeToGBC(const wchar_t* unicode, char* ansi, unsigned int ansi_bufsize)
{
	assert(NULL != unicode && (NULL != ansi || 0 == ansi_bufsize));
	int len = WideCharToMultiByte(CP_GBC, 0, unicode, -1, NULL, 0, NULL, NULL);
	if(len > ansi_bufsize) return -len;
	WideCharToMultiByte(CP_GBC, 0, unicode, -1, ansi, len, NULL, NULL);
	return len;
}

//UTF-8到Unicode的转换
int Utf8ToUnicode(const char* utf8, wchar_t* unicode, unsigned int uni_bufsize)
{
	assert(NULL != utf8 && (NULL != unicode || 0 == uni_bufsize));
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	if(len > uni_bufsize) return -len;
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, unicode, len);
	return len;
}

//Unicode到UTF-8的转换
int UnicodeToUtf8(const wchar_t* unicode, char* utf8, unsigned int utf8_bufsize)
{
	assert(NULL != unicode && (NULL != utf8 || 0 == utf8_bufsize));
	int len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	if(len > utf8_bufsize) return -len;
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, utf8, len, NULL, NULL);
	return len;
}