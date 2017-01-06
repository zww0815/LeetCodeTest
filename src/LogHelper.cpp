//
// Copyright (c) zww0815@foxmail.com.  All rights reserved.
//
//
#include "stdafx.h"
#include "LogHelper.h"
#include <memory>
#include <queue>
#include <assert.h>
#include <tchar.h>
#include <iostream>
#include <strstream>
#include <strsafe.h>
#include "ColorConsole.h"

#define FILE_NAME_LEN 128

#ifndef NO_USE_LOG4K
#pragma comment(lib, "user32.lib")
#endif

using namespace std;
using namespace LogHelperUtil;

char LogHelperUtil::LogHelperSingleton::c_rgcsLogInfoBuffer[sc_nStrBufLength] = {0};
wchar_t LogHelperUtil::LogHelperSingleton::c_rgwcsLogInfoBuffer[sc_nStrBufLength] = {0};

static const int sc_nSevertiesCount = 5;
static const char* const sc_pszLogSeverityNames[sc_nSevertiesCount] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR"
};

static const wchar_t* const sc_pwcsLogSeverityNames[sc_nSevertiesCount] = {
    L"TRACE",
    L"DEBUG",
    L"INFO",
    L"WARN",
    L"ERROR"
};

static const ConsoleColorType sc_eLogConsoleOutColor[sc_nSevertiesCount] = {
    dark_cyan,
    dark_red,
    green,
    yellow,
    red,
};

typedef CRITICAL_SECTION MutexType;

//mutex class
class Mutex
{
public:
    inline Mutex()
    {
        InitializeCriticalSection(&m_Mutex);
        SetIsSafe();
    }

    inline ~Mutex()
    {
        DeleteCriticalSection(&m_Mutex);
    }

    inline void Lock()
    {
        if (is_safe_) EnterCriticalSection(&m_Mutex);
    }

    inline bool TryLock()
    {
        return is_safe_ ? TryEnterCriticalSection(&m_Mutex) != 0 : true;
    }

    inline void UnLock()
    {
        if (is_safe_) LeaveCriticalSection(&m_Mutex);
    }

    inline void ReaderLock() { Lock(); }
    inline void ReaderUnlock() { UnLock(); }

    inline void WriterLock() { Lock(); }
    inline void WriterUnlock() { UnLock(); }

private:
    volatile bool is_safe_;
    inline void SetIsSafe() { is_safe_ = true; }
    MutexType m_Mutex;

private:
    Mutex(Mutex* /*ignored*/) {}
    Mutex(const Mutex&);
    void operator=(const Mutex&);
};

class MutexLock
{
public:
    explicit MutexLock(Mutex* m)
        : m_Mutex(m)
    {
        m_Mutex->Lock();
    }

    ~MutexLock()
    {
        m_Mutex->UnLock();
    }

private:
    Mutex* const m_Mutex;
    MutexLock(const MutexLock&);
    void operator=(const MutexLock&);
};


class ReaderMutexLock
{
public:
    explicit ReaderMutexLock(Mutex *mu) : m_Mutex(mu) { m_Mutex->ReaderLock(); }
    ~ReaderMutexLock() { m_Mutex->ReaderUnlock(); }

private:
    Mutex * const m_Mutex;
    // Disallow "evil" constructors
    ReaderMutexLock(const ReaderMutexLock&);
    void operator=(const ReaderMutexLock&);
};

class WriterMutexLock
{
public:
    explicit WriterMutexLock(Mutex *mu) : mu_(mu) { mu_->WriterLock(); }
    ~WriterMutexLock() { mu_->WriterUnlock(); }
private:
    Mutex * const mu_;
    // Disallow "evil" constructors
    WriterMutexLock(const WriterMutexLock&);
    void operator=(const WriterMutexLock&);
};

Mutex g_Mutex;

LogHelper::LogHelper()
{
    _wsetlocale(LC_ALL, L"chs");
    SetOutHandle(LogOutType_ToStdout);
    SetLogLevel(LogLevelType_All);
}

LogHelper::~LogHelper()
{
    Close();
}

void LogHelper::SetOutHandle(LogOutType outType)
{
        _eMsgOutType = outType;
        Initialize();
}

void LogHelper::SetLogLevel(int iLogLevel)
{
    size_t requiredSize = 0;

    _logLevel = iLogLevel;

    _wgetenv_s(&requiredSize, NULL, 0, L"DEBUG_FLAG");
    if (0 != requiredSize)
    {
        //_fIsDebugStateFlag = true;
        _logLevel = LogLevelType_Debug;
    }
}

bool LogHelper::IsLogLevel(int iLogLevel) const
{
    return (iLogLevel >= _logLevel);
}

bool LogHelper::IsDebugState() const
{
    return _fIsDebugStateFlag;
}

void LogHelper::SetConsoleOutColor(int logLevel)
{
    if (_eMsgOutType&LogOutType_ToStderr
        || _eMsgOutType&LogOutType_ToStdout)
    {
        SetTextColor(sc_eLogConsoleOutColor[logLevel]);
    }
}

void LogHelper::ResetColor()
{
    if (_eMsgOutType&LogOutType_ToStderr
        || _eMsgOutType&LogOutType_ToStdout)
    {
        sTextColor = sNormalTextColor;
        sBackColor = g_defBackCol;
        SetTextColor(g_defTextCol);
    }
}

void LogHelper::Initialize()
{
    if (LogOutType_NoOut&_eMsgOutType)
    {
        return;
    }

    if (LogOutType_ToStdout&_eMsgOutType
        || LogOutType_ToStderr&_eMsgOutType)
    {
        sNormalTextColor = green;
        sWarnTextColor = yellow;
        sErrorTextColor = red;
        sDebugTextColor = pink;

        sTextColor = sNormalTextColor;
        sBackColor = g_defBackCol;

        _fIsDebugStateFlag = false;

        ColorConsole consoleInst;
        consoleInst.Initialize();
        return;
    }

    Open(GetLogFullPath(), L"wt, ccs=UTF-8");
}

int LogHelper::Open(const wstring& fileName, const wstring& mode)
{
    Close();
    return _wfopen_s(&_fp, fileName.c_str(), mode.c_str());
}

void LogHelper::Close()
{
    if (_fp != nullptr)
    {
        fflush(_fp);
        fclose(_fp);
        _fp = nullptr;
    }
}

int LogHelper::Loger(const wchar_t* pStrBuf, LogOutType outType, const wchar_t* pMode)
{
    LogOutType outTypeTmp = outType;
    if (outTypeTmp&LogOutType_ToFile)
    {
        if (_fp)
        {
            fwprintf_s(_fp, L"%s", pStrBuf);
        }
    }

    if (outTypeTmp&LogOutType_ToStderr
        || outTypeTmp&LogOutType_ToStdout)
    {
        fwprintf_s(stdout, L"%s", pStrBuf);
    }

    return 0;
}
    

int LogHelper::Loger(const char* pStrBuf, LogOutType outType, const wchar_t* pMode)
{
    LogOutType outTypeTmp = outType;
    if (outTypeTmp&LogOutType_ToFile)
    {
        if (_fp)
        {
            fprintf_s(_fp, "%s", pStrBuf);
        }
    }

    if (outTypeTmp&LogOutType_ToStderr
        || outTypeTmp&LogOutType_ToStdout)
    {
        fprintf_s(stdout, "%s", pStrBuf);
    }

    return 0;
}

int LogHelper::SetLogDir(const wchar_t* logDir)
{
    _wstrLogDir = logDir;
    _wstrLogFileFullPath = _wstrLogDir + L"\\" + _wstrLogName;
    return 0;
}

wstring LogHelper::GetLogDir()
{
    DWORD attr = 0;
    attr = GetFileAttributes(_wstrLogDir.c_str());
    if (_wstrLogDir.empty()
        || (!((attr != (DWORD)(-1)) && (attr & FILE_ATTRIBUTE_DIRECTORY))))
    {
        wchar_t buf[MAX_PATH];
        ZeroMemory(buf, sizeof(buf));
        GetCurrentDirectory(MAX_PATH, buf);
        _wstrLogDir = (wstring)buf + L"\\log";
        _wmkdir(L".\\log");
    }

    return _wstrLogDir;
}

int LogHelper::SetLogName(const wchar_t* logName)
{
    _wstrLogName = logName;

    _wstrLogFileFullPath = _wstrLogDir + L"\\" + _wstrLogName;

    if (LogOutType::LogOutType_ToFile&_eMsgOutType)
    {
        //Clear the log.
        Open(_wstrLogFileFullPath, L"w");
    }

    return 0;
}

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
wstring LogHelper::GetLogName()
{
    if (_wstrLogName.empty())
    {
        SYSTEMTIME stCurSysTime;
            TCHAR szLogFileName[sc_fileNameLen] = { 0 };
            TCHAR curLogFileWithSysTime[MAX_PATH + 1];
            wstring wstrFileName;

            ZeroMemory(curLogFileWithSysTime, sizeof(curLogFileWithSysTime));

            GetModuleFileName((HINSTANCE)&__ImageBase, szLogFileName, sc_fileNameLen);
            wstrFileName = szLogFileName;
            size_t pos = wstrFileName.find_last_of(L"\\");
            if (std::wstring::npos != pos)
            {
                wstrFileName = wstrFileName.substr(pos + 1);
                pos = wstrFileName.find_last_of(L".");
                if (std::wstring::npos != pos)
                {
                    wstrFileName = wstrFileName.substr(0, pos);
                }
            }

            GetLocalTime(&stCurSysTime);
            StringCbPrintf(curLogFileWithSysTime, MAX_PATH, L"%s_%4d%02d%02d_%02d%02d%02d_%03d.log",
                wstrFileName.c_str(),
                stCurSysTime.wYear, stCurSysTime.wMonth, stCurSysTime.wDay,
                stCurSysTime.wHour, stCurSysTime.wMinute, stCurSysTime.wSecond,
                stCurSysTime.wMilliseconds);
            _wstrLogName = curLogFileWithSysTime;
        }

    return _wstrLogName;
}

wstring LogHelper::GetLogFullPath()
{
    _wstrLogFileFullPath = GetLogDir() + L"\\" + GetLogName();
    return _wstrLogFileFullPath;
}

int LogHelper::Write(int logLevel, const char* logBuf)
{
    static SYSTEMTIME stCurLocalTime;
    static char szLogBuf[sc_nStrBufLength];

    GetLocalTime(&stCurLocalTime);
    ZeroMemory(szLogBuf, sc_nStrBufLength);

    sprintf_s(szLogBuf, "[%4d-%02d-%02d %02d:%02d:%02d.%03d] [%-5s] %s",
        stCurLocalTime.wYear, stCurLocalTime.wMonth, stCurLocalTime.wDay,
        stCurLocalTime.wHour, stCurLocalTime.wMinute, stCurLocalTime.wSecond,
        stCurLocalTime.wMilliseconds,
        sc_pszLogSeverityNames[logLevel],
        logBuf);

    MutexLock lock(&g_Mutex);
    SetConsoleOutColor(logLevel);
    Loger(szLogBuf, _eMsgOutType);
    ResetColor();

    return 0;
}

int LogHelper::Write(int logLevel, const wchar_t* logBuf)
{
    static SYSTEMTIME stCurLocalTime;
    static wchar_t szLogBuf[sc_nStrBufLength];

    GetLocalTime(&stCurLocalTime);
    ZeroMemory(szLogBuf, sc_nStrBufLength);

    swprintf_s(szLogBuf, L"[%4d-%02d-%02d %02d:%02d:%02d.%03d] [%-5s] %s",
        stCurLocalTime.wYear, stCurLocalTime.wMonth, stCurLocalTime.wDay,
        stCurLocalTime.wHour, stCurLocalTime.wMinute, stCurLocalTime.wSecond,
        stCurLocalTime.wMilliseconds,
        sc_pwcsLogSeverityNames[logLevel],
        logBuf);

    MutexLock lock(&g_Mutex);
    SetConsoleOutColor(logLevel);
    Loger(szLogBuf, _eMsgOutType);
    ResetColor();

    return 0;
}

int LogHelper::WriteWithColor(const char* buf, ConsoleColorType color)
{
    SetTextColor(color);
    Loger(buf, _eMsgOutType);
    ResetColor();
    return 0;
}

int LogHelper::WriteWithColor(const wchar_t* buf, ConsoleColorType color)
{
    SetTextColor(color);
    Loger(buf, _eMsgOutType);
    ResetColor();

    return 0;
}

int LogHelper::WriteWithDetailInfo(int logLevel,
    const char* pSrcFileName,
    const char* pFuncName,
    int LineNo,
    const char* logBuf)
{
    static char buffer[sc_nStrBufLength];

    ZeroMemory(buffer, sc_nStrBufLength);

    sprintf_s(buffer, sc_nStrBufLength, "[L%05d, %-30s]  %s\n",
        LineNo, pFuncName, logBuf);

    return Write(logLevel, buffer);
}

int LogHelper::WriteWithDetailInfo(int logLevel,
    const wchar_t* pSrcFileName,
    const wchar_t* pFuncName,
    int LineNo,
    const wchar_t* logBuf)
{
    static wchar_t buffer[sc_nStrBufLength];

    ZeroMemory(buffer, sc_nStrBufLength);

    swprintf_s(buffer, sc_nStrBufLength, L"[L%05d, %-30s]  %s\n",
        LineNo, pFuncName, logBuf);

    return Write(logLevel, buffer);
}

LogHelper LogHelperSingleton::g_LogInstance;
LogHelper& LogHelperSingleton::GetInstance()
{
    return g_LogInstance;
}


#if 0
static int g_Cnt = 0;
void LogTestThread1(LPVOID lpPara)
{
    DEBUG_FUNCTION();

    LOGFMTT(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);
    LOGFMTD(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);
    LOGFMTI(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);
    LOGFMTW(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);
    LOGFMTE(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);

    for (int i = 0; i < 10;)
    {
        LOGFMTT(L"[%d] %s", i++, __FUNCTIONW__);
        printf("%s : i=%d\n", __FUNCTION__, i++);
        //Sleep(10);
    }
}

void LogTestThread2(LPVOID lpPara)
{
    DEBUG_FUNCTION();

    LOGFMTT_A("----------Function:%s, Line:%d", __FUNCTION__, __LINE__);
    LOGFMTD(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);
    LOGFMTI_A("----------Function:%s, Line:%d", __FUNCTION__, __LINE__);
    LOGFMTW(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);
    LOGFMTE_A("----------Function:%s, Line:%d", __FUNCTION__, __LINE__);

    for (int i = 0; i < 10;)
    {
        LOGFMTI(L"[%d] %s", i++, __FUNCTIONW__);
        printf("%s : i=%d\n", __FUNCTION__, i++);
        //Sleep(10);
    }
}

int main()
{
    //Sleep(3000);
    LOG4K_SET_LOG_LEVEL(LOG4K_TRACE);
    LOG4K_SET_OUT(LogOutType_ToStdout);

    DEBUG_FUNCTION();

    LOGFMTT(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);
    LOGFMTD(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);
    LOGFMTI(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);
    LOGFMTW(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);
    LOGFMTE(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);

    LOGFMTT_A("----------Function:%s, Line:%d", __FUNCTION__, __LINE__);
    LOGFMTD_A("----------Function:%s, Line:%d", __FUNCTION__, __LINE__);
    LOGFMTI_A("----------Function:%s, Line:%d", __FUNCTION__, __LINE__);
    LOGFMTW_A("----------Function:%s, Line:%d", __FUNCTION__, __LINE__);
    LOGFMTE_A("----------Function:%s, Line:%d", __FUNCTION__, __LINE__);

    HANDLE hThread[2];
    ZeroMemory(hThread, sizeof(HANDLE) * 2);

    hThread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LogTestThread1, NULL, 0, NULL);
    hThread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LogTestThread2, NULL, 0, NULL);


    for (int i = 0; i < 100; i++)
    {
        LOGFMTD(L"[%d]======%s", i, __FUNCTIONW__);
    }

    WaitForMultipleObjects(2, hThread, true, INFINITE);
    //LOG4K_SET_OUT(LogOutType_ToStdout);

    LOGFMTT(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);
    LOGFMTD(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);
    LOGFMTI(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);
    LOGFMTW(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);
    LOGFMTE(L"----------Function:%s, Line:%d", __FUNCTIONW__, __LINE__);

    return 0;
}
#endif