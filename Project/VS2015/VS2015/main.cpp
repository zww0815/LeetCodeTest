// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "LeetcodeInc.hpp"

int main()
{
    LOG_SET_LOG_LEVEL(LogHelperUtil::LogLevelType_Info);
    LOG_SET_OUT(LogHelperUtil::LogOutType_ToStdout);

    TEST_RUN_ALL();

    return 0;
}

