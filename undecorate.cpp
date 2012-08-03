/*
* Simple app to undecorate a MSVC symbol
*
* Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
* Contact: http://www.qt-project.org/
*
* You may use this file under the terms of the 3-clause BSD license.
*
* To compile (MSVC), do
*    cl /c /MD undecorate.cpp && link undecorate.obj dbghelp.lib
*
* Alternatively, if you have UPX (http://upx.sourceforge.net/), you
* can compress it to make it even smaller by
*    upx --stip-loadconf undecorate.exe
*/

#define UNICODE
#define DBGHELP_TRANSLATE_TCHAR

#include <windows.h>
#include <Dbghelp.h>
#include <stdio.h>

int wmain(int argc, wchar_t *argv[], wchar_t *[])
{
    if (argc > 1) {
        wchar_t undecorated[2048];
        if(UnDecorateSymbolName(reinterpret_cast<PCWSTR>(argv[1]),
                                reinterpret_cast<PWSTR>(undecorated),
                                sizeof(undecorated),
                                UNDNAME_COMPLETE)) {
            printf("%S\n", undecorated);
        } else {
            LPWSTR msg;
            DWORD dw = GetLastError();
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                          NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                          (LPWSTR) &msg, 0, NULL);
            printf("** Couldn't undecorate symbol!\n** Error (%d): %S", dw, msg);
            LocalFree(msg);
            return dw;
        }
    } else {
        printf("usage: %S <decorated symbol>\n", argv[0]);
    }
    return 0;
}
