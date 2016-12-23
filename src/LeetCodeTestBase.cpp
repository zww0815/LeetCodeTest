//
// Copyright (c) zww0815@foxmail.com.  All rights reserved.
//
//
#include "stdafx.h"
#include "LeetCodeTestBase.h"

using namespace LeetCodeTest;
std::map<std::wstring, VectorTestsType> Test::_tests;
std::wstring Test::_curTestInstName;

std::shared_ptr<Test> Test::GetInstance()
{
    return std::make_shared<Test>();
}

void Test::SetCategoryName(const std::wstring& categoryName)
{
    _curTestInstName = categoryName;
}

void Test::Add(SpTestBaseType test)
{
    _tests[_curTestInstName].push_back(test);
}

void Test::Run(const std::wstring& testInstName)
{
    BOOL result = 0;
    auto foundResult = _tests.find(testInstName);
    if (foundResult == _tests.end())
    {
        PRINT_INFO_COLOR_RED(L"No test instance is found.\n");
        return;
    }

    PRINT_INFO_COLOR_YELLOW(L"==============\nCurrent test instance name: %s\n", testInstName.c_str());

    for (auto item : foundResult->second)
    {
        if (item != nullptr)
        {
            std::wstring name = item->Name();
            std::wstring desc = item->Desc();

            PRINT_INFO_COLOR_YELLOW(L"LeetCode Name: \n%s\n\n", name.c_str());
            PRINT_INFO_COLOR_YELLOW(L"LeetCode Desc: \n%s\n\n", desc.c_str());

            result = item->Run();
            if (FALSE == result)
            {
                PRINT_INFO_COLOR_RED(L"Failed, Please check your code.\n");
            }
        }
    }

    PRINT_INFO_COLOR_GREEN(L"\n\n");
}

void Test::Run()
{
    for (auto item : _tests)
    {
        Run(item.first);
    }
}