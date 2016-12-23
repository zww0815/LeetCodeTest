//
// Copyright (c) zww0815@foxmail.com.  All rights reserved.
//
//
#include "stdafx.h"
#include "ColorConsole.h"

void ColorConsole::ShowLastSystemError()
{
    LPSTR messageBuffer;
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        0,          // source
        GetLastError(),
        0,          // lang
        (LPSTR)&messageBuffer,
        0,
        NULL);

    LogHelperUtil::LogHelperSingleton::GetInstance().Loger(messageBuffer);
    LocalFree(messageBuffer);
}