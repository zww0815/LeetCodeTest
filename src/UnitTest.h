//
// Copyright (c) zww0815@foxmail.com.  All rights reserved.
//
//
#pragma once

#include <map>
#include <string>
#include <sstream>
#include <memory>
#include <vector>

namespace WWL
{
    namespace UnitTest
    {
        enum class UnitTestInfoType
        {
            Normal,
            Warning,
            Error,
        };

        struct UTException
        {
            explicit UTException(const std::wstring& excp)
                : _excptInfo(excp)
            {
            }

            const std::wstring& What()
            {
                return _excptInfo;
            }

        private:
            std::wstring _excptInfo;
        };

        struct TestCase
        {
            typedef void(*TestCaseProc)();
            explicit TestCase(const std::wstring& name, TestCaseProc pfnCallBack)
                : _tcName(name)
                , _pfnCallBack(pfnCallBack)
            {                
            }

            bool Run();

            TestCaseProc _pfnCallBack;
            std::wstring _tcName;
        };

        class Test
        {
        public:
            static Test& GetInstance();

        public:
            Test();
            void Add(const TestCase& tcInfo);
            void AddPassCount();

            void Run();
            void PrintInfo(const std::wstring& info, UnitTestInfoType resultType = UnitTestInfoType::Normal);

            bool Check(bool condition, const std::wstring& desc);

            template<class T>
            bool CheckEqual(T a, T b, const std::wstring& desc)
            {
                bool result = Check(a == b, desc.c_str());
                if (!result)
                {
                    wstringstream ss;
                    ss << L"Left: " << a << L" ";
                    ss << L"Right: " << b << L"\n";

                    PRINT_INFO_COLOR_RED(L"%s\n", ss.str().c_str());
                }

                return result;
            }

            void SetResult(const std::wstring& name, bool result);

            bool IsPassed(const std::wstring& name);

        private:
            std::vector<TestCase> _tests;
            std::wstring _curRunningTesCaseName;
            std::map<std::wstring, bool> _tcResults;
            size_t _passCount;
            size_t _totalCount;
        };
    }
}

#define TEST_CASE(name)\
    extern void TC_##name();\
    namespace WWL_UT_INST\
    {\
        class TC_RUNNER_##name\
        {\
            public:\
                static void RunUT()\
                {\
                    PRINT_INFO_COLOR_YELLOW(L"## Test Case Name: "#name##" ##\n");\
                    TC_##name();\
                }\
                TC_RUNNER_##name()\
                {\
                    TestCase tc(L""#name, &TC_RUNNER_##name::RunUT);\
                    WWL::UnitTest::Test::GetInstance().Add(tc);\
                }\
        } TC_RUNNER_##name##_INST;\
    }\
    void TC_##name()


#define TEST_RUN_ALL() \
    WWL::UnitTest::Test::GetInstance().Run()

#define TEST_NAME(name) \
    WWL::UnitTest::Test::GetInstance().PrintInfo(L"Name: "#name##"\n");

#define TEST_DESC(desc) \
    WWL::UnitTest::Test::GetInstance().PrintInfo(L"Desc: "#desc##"\n");

#define TEST_CHECK(condition) \
    WWL::UnitTest::Test::GetInstance().Check((condition), L"Check ("#condition##") ...")

#define TEST_EQUAL(a, b) \
    WWL::UnitTest::Test::GetInstance().CheckEqual(a, b, L"Check ("#a##")==("#b##") ...")