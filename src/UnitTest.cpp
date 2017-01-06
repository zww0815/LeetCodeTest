//
// Copyright (c) zww0815@foxmail.com.  All rights reserved.
//
//
#include "stdafx.h"
#include "UnitTest.h"

using namespace WWL::UnitTest;

Test& Test::GetInstance()
{
    static Test testInst;
    return testInst;
}

WWL::UnitTest::Test::Test()
    : _passCount(0)
    , _totalCount(0)
{
}

void Test::Add(const TestCase& tcInfo)
{
    _tests.push_back(tcInfo);
    _tcResults[tcInfo._tcName] = true;
}

void WWL::UnitTest::Test::AddPassCount()
{
    ++_passCount;
}

void WWL::UnitTest::Test::Run()
{
    _totalCount = _tests.size();
    if (_totalCount <= 0)
    {
        return;
    }

    for (auto tc : _tests)
    {
        _curRunningTesCaseName = tc._tcName;
        if (tc.Run() && IsPassed(_curRunningTesCaseName))
        {
            AddPassCount();
        }
    }

    if (_totalCount != _passCount)
    {
        PRINT_INFO_COLOR(ConsoleColorType::dark_red, L"\n============================================================\n"
            L"Total run: %d, passed %d, failed %d, pass rate: %.1f%%\n",
            _totalCount, _passCount, _totalCount - _passCount, (_passCount*1.00)*100 / _totalCount);
        
        ULONG ulTestCount = 0;
        PRINT_INFO_COLOR(ConsoleColorType::dark_red, L"---------------------------------------------------------------\n"
            L"Detail:\n"
            L"------------------------------------------------------------------------\n"
            L"%-6s%-50s%s\n",L"No.", L"Name", L"Result");
        for (auto item: _tcResults)
        {
            bool fResultPassed = item.second;
            PRINT_INFO_COLOR(fResultPassed ? ConsoleColorType::green: ConsoleColorType::red,
                L"%03d   %-50s%s\n", 
                ++ulTestCount, item.first.c_str(), fResultPassed ? L"Passed" : L"Failed");
        }
    }
    else
    {
        PRINT_INFO_COLOR_YELLOW(L"\nTotal run: %d, All passed, Congratulations!\n",
            _totalCount)
    }
}

void WWL::UnitTest::Test::PrintInfo(const std::wstring& info, UnitTestInfoType resultType)
{
    std::wstring infoTemp;
    ConsoleColorType colorType = ConsoleColorType::green;
    switch (resultType)
    {
    case WWL::UnitTest::UnitTestInfoType::Warning:
        colorType = ConsoleColorType::yellow;
        infoTemp = L"WARNING>>  ";
        break;
    case WWL::UnitTest::UnitTestInfoType::Error:
        colorType = ConsoleColorType::red;
        infoTemp = L"ERROR>>  ";
        break;
    case WWL::UnitTest::UnitTestInfoType::Normal:
        colorType = ConsoleColorType::dark_green;
        break;
    default:
        colorType = ConsoleColorType::green;
        break;
    }

    infoTemp.append(info);
    PRINT_INFO_COLOR(colorType, infoTemp.c_str());
}

bool WWL::UnitTest::Test::Check(bool condition, const std::wstring& desc)
{
    PrintInfo(desc);
    if (!condition)
    {
        Test::GetInstance().SetResult(_curRunningTesCaseName, false);
        PRINT_INFO_COLOR_RED(L"Failed.\n")
    }
    else
    {
        PRINT_INFO_COLOR_GREEN(L"Passed.\n")
    }

    return condition;
}

void WWL::UnitTest::Test::SetResult(const std::wstring & name, bool result)
{
    if (_tcResults.find(name) != _tcResults.end())
    {
        _tcResults[name] = result;
    }
}

bool WWL::UnitTest::Test::IsPassed(const std::wstring & name)
{
    if (_tcResults.find(name) != _tcResults.end())
    {
        return _tcResults[name];
    }

    return true;
}

bool WWL::UnitTest::TestCase::Run()
{
    bool result = true;
    if (_pfnCallBack)
    {
        try
        {
            _pfnCallBack();
        }
        catch (...)
        {
            result = false;
        }
    }

    return result;
}
