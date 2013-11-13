/*
    TLS GL hook by Dominic Tobias
    www.dominictobias.com
    Licence: WTFPL (http://www.wtfpl.net/)
*/
 
#pragma once
#pragma warning(disable: 4311)
#pragma warning(disable: 4312)
 
#include <windows.h>
#include <stdio.h>
 
int GLInit();
DWORD GLHook(char *funcName, void *funcHook);
bool GLUnhook(char *funcName, void *funcOrig);