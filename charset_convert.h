#ifndef __CHARSET_CONVERT_H__
#define __CHARSET_CONVERT_H__

#include <windows.h>
#include <assert.h>
#define CP_GBC 936

//GBC到UTF-8的转换
int GBCToUtf8(const char* ansi, char* utf8, unsigned int utf8_bufsize);
//UTF-8到GBC的转换
int Utf8ToGBC(const char* utf8, char* ansi, unsigned int ansi_bufsize);
//GBC到Unicode的转换
int GBCToUnicode(const char* ansi, wchar_t* unicode, unsigned int uni_bufsize);
//Unicode到GBC的转换
int UnicodeToGBC(const wchar_t* unicode, char* ansi, unsigned int ansi_bufsize);
//UTF-8到Unicode的转换
int Utf8ToUnicode(const char* utf8, wchar_t* unicode, unsigned int uni_bufsize);
//Unicode到UTF-8的转换
int UnicodeToUtf8(const wchar_t* unicode, char* utf8, unsigned int utf8_bufsize);


#endif