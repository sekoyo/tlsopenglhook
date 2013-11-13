#include "glhook.h"
 
// Vars
int hookMethod = 0;
DWORD glTLS = 0;
 
// Declarations
DWORD GetTblOffset(DWORD funcAddr);
DWORD HookGLNative(DWORD funcTblAddr, void *funcHook);
void UnhookGLNative(DWORD funcTblAddr, void *funcOrig);
 
int GLInit()
{
    __asm 
    {
        push eax
        mov eax, dword ptr fs:[0x18]
        mov glTLS, eax
        pop eax
    }
}
 
DWORD GLHook(char *funcName, void *funcHook)
{
    DWORD funcAddr = 0;
    DWORD funcTblAddr = 0;
    DWORD funcNativeOrig = 0;
 
    if (!glTLS || !funcHook) 
        return 0;
 
    if (!(funcAddr = (DWORD)GetProcAddress(GetModuleHandle("opengl32.dll"), funcName)))
        return 0;
 
    if (!(funcTblAddr = GetTblOffset(funcAddr)))
        return 0;
 
    if (!(funcNativeOrig = HookGLNative(funcTblAddr, funcHook)))
        return 0;
 
    return funcNativeOrig;
}
 
bool GLUnhook(char *funcName, void *funcOrig)
{
    DWORD funcAddr = 0;
    DWORD funcTblAddr = 0;
    DWORD funcNativeOrig = 0;
 
    if (!glTLS || !funcOrig) 
        return false;
 
    if (!(funcAddr = (DWORD)GetProcAddress(GetModuleHandle("opengl32.dll"), funcName)))
        return false;
 
    if (!(funcTblAddr = GetTblOffset(funcAddr)))
        return false;
 
    UnhookGLNative(funcTblAddr, funcOrig);
 
    return true;
}
 
DWORD GetTblOffset(DWORD funcAddr)
{
    if (*(BYTE*)(funcAddr + 0x07) == 0x83) {
        hookMethod = 0;
        return ((*(DWORD*)(glTLS + *(DWORD*)(*(DWORD*)(funcAddr + 0x14)))) + (*(DWORD*)(funcAddr + 0x1C)));
    }
 
    else if (*(BYTE*)(funcAddr + 0x07) == 0xA0) {
        hookMethod = 1;
        return (*(DWORD*)(funcAddr + 0x08));
    }
 
    return 0;
}
 
DWORD HookGLNative(DWORD funcTblAddr, void *funcHook)
{
    DWORD funcNativeOrig = 0;
 
    switch(hookMethod)
    {
        case 0:
            funcNativeOrig = *(DWORD*)funcTblAddr;
            *(DWORD*)funcTblAddr = (DWORD)funcHook;
            break;
 
        case 1:
            funcNativeOrig = *(DWORD*)(glTLS + funcTblAddr);
            *(DWORD*)(glTLS + funcTblAddr) = (DWORD)funcHook;
            break;
 
        default:
            break;
    }
 
    return funcNativeOrig;
}
 
void UnhookGLNative(DWORD funcTblAddr, void *funcOrig)
{
    switch(hookMethod)
    {
        case 0:
            *(DWORD*)funcTblAddr = (DWORD)funcOrig;
            break;
 
        case 1:
            *(DWORD*)(glTLS + funcTblAddr) = (DWORD)funcOrig;
            break;
 
        default:
            break;
    }
}