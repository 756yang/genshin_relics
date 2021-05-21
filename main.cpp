//UINCODE环境
#ifndef UNICODE
#undef _UNICODE
#else
#ifndef _UNICODE
#define _UNICODE
#endif
#endif
#include <tchar.h>
#include <windows.h>
#include "genshin_relics.h"
#include <QtWidgets/QApplication>

/* this is municode qt application */

char *utf16_to_utf8(const wchar_t *input)
{
    char *Buffer;
    int BuffSize = 0, Result = 0;
    BuffSize = WideCharToMultiByte(CP_UTF8, 0, input, -1, NULL, 0, NULL, NULL);
    Buffer = (char*) malloc(sizeof(char) * BuffSize);
    if(Buffer)
    {
        Result = WideCharToMultiByte(CP_UTF8, 0, input, -1, Buffer, BuffSize, NULL, NULL);
    }
    return ((Result > 0) && (Result <= BuffSize)) ? Buffer : NULL;
}

void init_arguments_utf8(int argc, _TCHAR **argv, char **argv_utf8)
{
    for(int i = 0; i < argc; i++)
    {
#ifdef UNICODE
        argv_utf8[i] = utf16_to_utf8(argv[i]);
#else
        int len_plus = strlen(argv[i]) + 1;
        argv_utf8[i] = (char*) malloc(len_plus);
        memcpy(argv_utf8[i], argv[i], len_plus);
#endif
    }
}

void free_arguments_utf8(int argc, char **argv)
{
    if(argv != NULL)
    {
        for(int i = 0; i < argc; i++)
        {
            if(argv[i] != NULL)
            {
                free(argv[i]);
                argv[i] = NULL;
            }
        }
    }
}


extern "C" int _tmain(int argc, _TCHAR **argv)
{
    int exit_code;
    char **argv_utf8 = (char**) malloc(sizeof(char*) * argc);
    init_arguments_utf8(argc, argv, argv_utf8);
    QApplication a(argc, argv_utf8);
    genshin_relics w;
    w.show();
    exit_code = a.exec();
    free_arguments_utf8(argc, argv_utf8);
    free(argv_utf8);
    return exit_code;
}
