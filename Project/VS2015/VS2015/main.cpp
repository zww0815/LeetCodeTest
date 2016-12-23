// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "test_64. Minimum Path Sum.hpp"

using namespace LeetCodeTest;
using namespace std;

int main()
{
    LOG_SET_LOG_LEVEL(LogHelperUtil::LogLevelType_All);
    LOG_SET_OUT(LogHelperUtil::LogOutType_ToStdout);

    TEST_INIT(L"dp");
    TEST_ADD(Test64);
    TEST_RUN_ALL();

    return 0;
}

