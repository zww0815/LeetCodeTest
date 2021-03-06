﻿//
// Copyright (c) zww0815@foxmail.com.  All rights reserved.
//
//
#ifndef LogHelperUtil_h__
#define LogHelperUtil_h__

#include "ColorConsole.h"
#include <ctime>
#include <functional>

namespace LogHelperUtil
{
    static const int sc_fileNameLen = 128;
    static const int sc_nStrBufLength = 1024;
    
    //! LOG Level
    enum LogLevelType
    {
        LogLevelType_All = -1,
        LogLevelType_Trace = 0,
        LogLevelType_Debug,
        LogLevelType_Info,
        LogLevelType_Warning,
        LogLevelType_Error,
        LogLevelType_NoLog = 99,
    };

    enum LogOutType
    {
        LogOutType_ToFile = (1 << 1),
        LogOutType_ToStdout = (1 << 2),
        LogOutType_ToStderr = (1 << 3),
        LogOutType_ToStdoutAndFile = (LogOutType_ToFile | LogOutType_ToStdout),
        LogOutType_NoOut = (1 << 5),
    };

    class LogHelper
    {
    public:
        LogHelper();
        ~LogHelper();

    public:
        bool IsDebugState() const;

        void SetOutHandle(LogOutType eOut);

        void SetLogLevel(int iLogLevel);

        void Initialize();

        bool IsLogLevel(int iLogLevel) const;

        void SetConsoleOutColor(int logLevel);

        void ResetColor();

        int SetLogName(const wchar_t* logFileName);
        std::wstring GetLogName();
        int SetLogDir(const wchar_t* logDir);
        std::wstring GetLogDir();
        std::wstring GetLogFullPath();

        int Loger(const char* pStrBuf, LogOutType outType = LogOutType::LogOutType_ToFile, const wchar_t* pMode = L"a");//Default append add to log

        int Loger(const wchar_t* pStrBuf, LogOutType outType = LogOutType::LogOutType_ToFile, const wchar_t* pMode = L"a");

        int Write(int logLevel,
            const char* logBuf);

        int Write(int logLevel,
            const wchar_t* logBuf);

        int WriteWithColor(const char* buf, ConsoleColorType color);
        int WriteWithColor(const wchar_t* buf, ConsoleColorType color);

        int WriteWithDetailInfo(int logLevel,
            const char* pSrcFileName,
            const char* pFuncName,
            int LineNo,
            const char* logBuf);

        int WriteWithDetailInfo(int logLevel,
            const wchar_t* pSrcFileName,
            const wchar_t* pFuncName,
            int LineNo,
            const wchar_t* logBuf);
        
        int Open(const std::wstring& fileName, const std::wstring& mode);
        void Close();

    private:
        ConsoleColorType sNormalTextColor;
        ConsoleColorType sWarnTextColor;
        ConsoleColorType sErrorTextColor;
        ConsoleColorType sDebugTextColor;

        ConsoleColorType sTextColor;
        ConsoleColorType sBackColor;

        std::wstring _wstrLogFileFullPath;
        std::wstring _wstrLogName;
        std::wstring _wstrLogDir;
        LogOutType _eMsgOutType;
        int _logLevel;
        bool _fIsDebugStateFlag;
        FILE* _fp;
    };

    class LogHelperSingleton
    {
    public:
        static LogHelper& GetInstance();
        static char c_rgcsLogInfoBuffer[sc_nStrBufLength];
        static wchar_t c_rgwcsLogInfoBuffer[sc_nStrBufLength];

    private:
        static LogHelper g_LogInstance;
    };

#ifdef NO_USE_LOG4K

    #define LOG_INIT() {}
    #define LOG_SET_OUT(outHandle) {}
    #define LOG_SET_LOG_LEVEL(level) {}
    #define Log4k_LOG(level, log, fileName, FuncName, LineNo){}

    #define LOGT(log) {}
    #define LOGD(log) {}
    #define LOGI(log) {}
    #define LOGW(log) {}
    #define LOGE(log) {}

    #define LOGFMTT_A( fmt, ...){}
    #define LOGFMTD_A( fmt, ...){}
    #define LOGFMTI_A( fmt, ...){}
    #define LOGFMTW_A( fmt, ...){}
    #define LOGFMTE_A( fmt, ...){}

    #define LOGFMTT( fmt, ...) {}
    #define LOGFMTD( fmt, ...) {}
    #define LOGFMTI( fmt, ...) {}
    #define LOGFMTW( fmt, ...) {}
    #define LOGFMTE( fmt, ...) {}

    #define DEBUG_FUNCTION()
    #define PRINT_INFO_COLOR_GREEN
    #define PRINT_INFO_COLOR_GREEN_A
    #define PRINT_INFO_COLOR_RED
    #define PRINT_INFO_COLOR_RED_A
    #define PRINT_INFO_COLOR_YELLOW
    #define PRINT_INFO_COLOR_YELLOW_A

#else

    #define LOG_INIT() \
    LogHelperUtil::LogHelperSingleton::GetInstance().Initialize();

    #define LOG_SET_OUT(outHandle) \
    LogHelperUtil::LogHelperSingleton::GetInstance().SetOutHandle(outHandle);

    #define LOG_SET_LOG_LEVEL(level) \
    LogHelperUtil::LogHelperSingleton::GetInstance().SetLogLevel(level);

    #define LOG_SET_LOG_NAME(logName) \
    LogHelperUtil::LogHelperSingleton::GetInstance().SetLogName(logName);

    #define LOG_SET_LOG_DIR(dirName) \
    LogHelperUtil::LogHelperSingleton::GetInstance().SetLogDir(dirName);

    //define yourself color out macro.
    #define PRINT_INFO_COLOR(color, strformat, ...) do {\
        ZeroMemory(LogHelperUtil::LogHelperSingleton::c_rgwcsLogInfoBuffer, LogHelperUtil::sc_nStrBufLength);\
        _snwprintf_s(LogHelperUtil::LogHelperSingleton::c_rgwcsLogInfoBuffer, LogHelperUtil::sc_nStrBufLength, _TRUNCATE, strformat, ##__VA_ARGS__); \
        LogHelperUtil::LogHelperSingleton::GetInstance().WriteWithColor(LogHelperUtil::LogHelperSingleton::c_rgwcsLogInfoBuffer, color); \
    } while (0);

    #define PRINT_INFO_COLOR_A(color, strformat, ...) do {\
        ZeroMemory(LogHelperUtil::LogHelperSingleton::c_rgcsLogInfoBuffer, LogHelperUtil::sc_nStrBufLength);\
        _snprintf_s(LogHelperUtil::LogHelperSingleton::c_rgcsLogInfoBuffer, LogHelperUtil::sc_nStrBufLength, _TRUNCATE, strformat, ##__VA_ARGS__); \
        LogHelperUtil::LogHelperSingleton::GetInstance().WriteWithColor(LogHelperUtil::LogHelperSingleton::c_rgcsLogInfoBuffer, color); \
    } while (0);

    #define PRINT_INFO_COLOR_RED(strformat, ...) \
        PRINT_INFO_COLOR(ConsoleColorType::red, strformat, ##__VA_ARGS__)

    #define PRINT_INFO_COLOR_RED_A(strformat, ...) \
        PRINT_INFO_COLOR_A(ConsoleColorType::red, strformat, ##__VA_ARGS__)

    #define PRINT_INFO_COLOR_GREEN(strformat, ...) \
        PRINT_INFO_COLOR(ConsoleColorType::green, strformat, ##__VA_ARGS__)

    #define PRINT_INFO_COLOR_GREEN_A(strformat, ...) \
        PRINT_INFO_COLOR_A(ConsoleColorType::green, strformat, ##__VA_ARGS__)

    #define PRINT_INFO_COLOR_YELLOW(strformat, ...) \
        PRINT_INFO_COLOR(ConsoleColorType::yellow, strformat, ##__VA_ARGS__)

    #define PRINT_INFO_COLOR_YELLOW_A(strformat, ...) do {\
        PRINT_INFO_COLOR_A(ConsoleColorType::yellow, strformat, ##__VA_ARGS__)

    #define Log4k_LOG(level, log, fileName, FuncName, LineNo) \
    if (LogHelperUtil::LogHelperSingleton::GetInstance().IsLogLevel(level))\
    {\
        LogHelperUtil::LogHelperSingleton::GetInstance().WriteWithDetailInfo(level, fileName, FuncName, LineNo, log);\
    }\


    #define LOGT(log) Log4k_LOG(LogHelperUtil::LogLevelType::LogLevelType_Trace, log, __FILEW__, __FUNCTIONW__, __LINE__);
    #define LOGD(log) Log4k_LOG(LogHelperUtil::LogLevelType::LogLevelType_Debug, log, __FILEW__, __FUNCTIONW__, __LINE__);
    #define LOGI(log) Log4k_LOG(LogHelperUtil::LogLevelType::LOG4K_INFO, log, __FILEW__, __FUNCTIONW__, __LINE__);
    #define LOGW(log) Log4k_LOG(LogHelperUtil::LogLevelType::LOG4K_WARNING, log, __FILEW__, __FUNCTIONW__, __LINE__);
    #define LOGE(log) Log4k_LOG(LogHelperUtil::LogLevelType::LOG4K_ERROR, log, __FILEW__, __FUNCTIONW__, __LINE__);

    #define LOG_FORMAT_A(level, logformat, ...) \
    {\
        ZeroMemory(LogHelperUtil::LogHelperSingleton::c_rgcsLogInfoBuffer, LogHelperUtil::sc_nStrBufLength);\
        _snprintf_s(LogHelperUtil::LogHelperSingleton::c_rgcsLogInfoBuffer, LogHelperUtil::sc_nStrBufLength, _TRUNCATE, logformat, ##__VA_ARGS__); \
        Log4k_LOG(level, LogHelperUtil::LogHelperSingleton::c_rgcsLogInfoBuffer, __FILE__, __FUNCTION__, __LINE__);\
    }

    #define LOG_FORMAT(level, logformat, ...) \
    {\
        ZeroMemory(LogHelperUtil::LogHelperSingleton::c_rgwcsLogInfoBuffer, LogHelperUtil::sc_nStrBufLength);\
        _snwprintf_s(LogHelperUtil::LogHelperSingleton::c_rgwcsLogInfoBuffer, LogHelperUtil::sc_nStrBufLength, _TRUNCATE, logformat, ##__VA_ARGS__); \
        Log4k_LOG(level, LogHelperUtil::LogHelperSingleton::c_rgwcsLogInfoBuffer, __FILEW__, __FUNCTIONW__, __LINE__);\
    }

    //!format string
    #define LOGFMTT_A( fmt, ...)   LOG_FORMAT_A(LogHelperUtil::LogLevelType::LogLevelType_Trace, fmt, ##__VA_ARGS__)
    #define LOGFMTD_A( fmt, ...)   LOG_FORMAT_A(LogHelperUtil::LogLevelType::LogLevelType_Debug, fmt, ##__VA_ARGS__)
    #define LOGFMTI_A( fmt, ...)   LOG_FORMAT_A(LogHelperUtil::LogLevelType::LogLevelType_Info, fmt, ##__VA_ARGS__)
    #define LOGFMTW_A( fmt, ...)   LOG_FORMAT_A(LogHelperUtil::LogLevelType::LogLevelType_Warning, fmt, ##__VA_ARGS__)
    #define LOGFMTE_A( fmt, ...)   LOG_FORMAT_A(LogHelperUtil::LogLevelType::LogLevelType_Error, fmt, ##__VA_ARGS__)

    #define LOGFMTT( fmt, ...)   LOG_FORMAT(LogHelperUtil::LogLevelType::LogLevelType_Trace, fmt, ##__VA_ARGS__)
    #define LOGFMTD( fmt, ...)   LOG_FORMAT(LogHelperUtil::LogLevelType::LogLevelType_Debug, fmt, ##__VA_ARGS__)
    #define LOGFMTI( fmt, ...)   LOG_FORMAT(LogHelperUtil::LogLevelType::LogLevelType_Info, fmt, ##__VA_ARGS__)
    #define LOGFMTW( fmt, ...)   LOG_FORMAT(LogHelperUtil::LogLevelType::LogLevelType_Warning, fmt, ##__VA_ARGS__)
    #define LOGFMTE( fmt, ...)   LOG_FORMAT(LogHelperUtil::LogLevelType::LogLevelType_Error, fmt, ##__VA_ARGS__)

    template<typename ActionType>
    class ScopeOptions
    {
    public:
        explicit ScopeOptions(ActionType const& action, ULONG ulLineNo, bool fIsEnableEntryAction = false)
            : m_strFuncName(L"")
            , m_ulLineNo(ulLineNo)
            , m_Action(action)
            , _fIsEnableEntryAction(fIsEnableEntryAction)
        {
            ScopeEntryOpts();
        }

        explicit ScopeOptions(const std::wstring& funcName, ULONG ulLineNo, bool fIsEnableEntryAction = false)
            : m_strFuncName(funcName)
            , m_ulLineNo(ulLineNo)
            , m_Action(nullptr)
            , _fIsEnableEntryAction(fIsEnableEntryAction)
        {
            ScopeEntryOpts();
        }

        explicit ScopeOptions(const std::wstring& funcName, ULONG ulLineNo, ActionType const& action, bool fIsEnableEntryAction = false)
            : m_strFuncName(funcName)
            , m_ulLineNo(ulLineNo)
            , m_Action(action)
            , _fIsEnableEntryAction(fIsEnableEntryAction)
        {
            ScopeEntryOpts();
        }

        ~ScopeOptions()
        {
            ScopeExitOpts();
        }

    private:
        void ScopeEntryOpts()
        {
            wchar_t tmpbuf[sc_nStrBufLength];
            ZeroMemory(tmpbuf, sizeof(tmpbuf));
            _snwprintf_s(tmpbuf, sc_nStrBufLength, _TRUNCATE, L"[L%05d, %-30s]  Entry...\n",
                m_ulLineNo, m_strFuncName.c_str());
            if (LogHelperSingleton::GetInstance().IsLogLevel(LogHelperUtil::LogLevelType_Debug))
            {
                LogHelperSingleton::GetInstance().Write(LogHelperUtil::LogLevelType_Debug, tmpbuf);
            }
            startTime = clock();

            if (_fIsEnableEntryAction && m_Action)
            {
                m_Action();
            }
        }

        void ScopeExitOpts()
        {
            if (_fIsEnableEntryAction)
            {
                return;
            }

            if (m_Action)
            {
                m_Action();
            }

            wchar_t tmpbuf[sc_nStrBufLength];
            ZeroMemory(tmpbuf, sizeof(tmpbuf));

            endTime = clock();

            _snwprintf_s(tmpbuf, sc_nStrBufLength, _TRUNCATE, L"[L%05d, %-30s]  Exit.(Used time %ld.%lds)\n",
                m_ulLineNo, m_strFuncName.c_str(),
                (ULONG)((endTime - startTime) / CLOCKS_PER_SEC),
                (ULONG)((endTime - startTime) % CLOCKS_PER_SEC));
            if (LogHelperSingleton::GetInstance().IsLogLevel(LogHelperUtil::LogLevelType_Debug))
            {
                LogHelperSingleton::GetInstance().Write(LogHelperUtil::LogLevelType_Debug, tmpbuf);
            }
        }

    private:
        std::wstring m_strFuncName;
        ActionType m_Action;
        ULONG m_ulLineNo;
        clock_t startTime;
        clock_t endTime;
        bool _fIsEnableEntryAction; // Default is false.
    };

    #define _TSTRINGIZE(x)  _T(_STRINGIZE(x))

    #define SCOPE_LINENAME_CAT(name, line) name##line
    #define SCOPE_INSTANCE_END_LINENAME(name, line) SCOPE_LINENAME_CAT(name, line)

    #define SCOPE_LAMBDA_LINENAME(name, line) (std::wstring)L"LambdaAction@" \
        + (name) + L"@line" + _TSTRINGIZE(line)

    #define SCOPE_EXIT_ACTION(expr)                                                                    \
        LogHelperUtil::ScopeOptions<std::function<void()>>                                             \
        SCOPE_INSTANCE_END_LINENAME(instance, __LINE__)(SCOPE_LAMBDA_LINENAME(__FUNCTIONW__, __LINE__),\
        __LINE__, [&](){expr;});

    #define GLOBAL_SCOPE_ENTRY_ACTION(expr)                                                               \
        LogHelperUtil::ScopeOptions<std::function<void()>>                                                \
        SCOPE_INSTANCE_END_LINENAME(instance, __LINE__)(SCOPE_LAMBDA_LINENAME(L"_globalFunc_", __LINE__), \
        __LINE__, [&](){expr;}, true);

    #define GLOBAL_SCOPE_EXIT_ACTION(expr)                                                                \
        LogHelperUtil::ScopeOptions<std::function<void()>>                                                \
        SCOPE_INSTANCE_END_LINENAME(instance, __LINE__)(SCOPE_LAMBDA_LINENAME(L"_globalFunc_", __LINE__), \
        __LINE__, [&](){expr;});


#ifdef _DEBUG
#define DEBUG_FLAG
#endif

    #ifdef DEBUG_FLAG
        #define DEBUG_INFO  LOGFMTD
        #define DEBUG_FUNCTION()                                                                          \
                const LogHelperUtil::ScopeOptions<std::function<void()>>                                  \
                SCOPE_INSTANCE_END_LINENAME(instance, __LINE__)(std::wstring(__FUNCTIONW__), __LINE__);
    #else
        #define DEBUG_INFO(Fmt, ...)
        #define DEBUG_FUNCTION()
    #endif
#endif //NO_UES_LOG4K

#ifdef _DEBUG
    #define DEBUG_MSG wprintf_s
#else
    #define DEBUG_MSG
#endif
}


#endif // LogHelperUtil_h__