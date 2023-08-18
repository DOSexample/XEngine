#pragma once

#include <Windows.h>
#include <stdio.h>

#include <string>

inline void DebugA( const char* _Format, ...)
{
    CHAR _Buffer[4096];
    int _Result;
    va_list _ArgList;
    __crt_va_start(_ArgList, _Format);

    _Result = vsprintf_s(_Buffer, sizeof(_Buffer), _Format, _ArgList);

    __crt_va_end(_ArgList);

    OutputDebugStringA(_Buffer);
}

inline void DebugW(const wchar_t* _Format, ...)
{
    WCHAR _Buffer[4096];
    int _Result;
    va_list _ArgList;
    __crt_va_start(_ArgList, _Format);

    _Result = vswprintf_s(_Buffer, sizeof(_Buffer)/2, _Format, _ArgList);

    __crt_va_end(_ArgList);

    OutputDebugStringW(_Buffer);
}
