TEST_BEGIN(Test64)
{
    TEST_NAME(L"64. Minimum Path Sum");
    TEST_DESC(L"Given a m x n grid filled with non-negative numbers, find a path from top left to bottom right which minimizes the sum of all numbers along its path.\n"
        L"Note: You can only move either down or right at any point in time.");

    TEST_RUN()
    {
        using namespace std;
        #include "64. Minimum Path Sum.hpp"

        DEBUG_FUNCTION();
        vector<int> vec = { 9, 4, 2,5,6,3,1,7,8,0 };
        auto grid = vector<vector<int>>(10, vec);

        Solution s;
        int result = s.minPathSum(grid);
        PRINT_INFO_COLOR_GREEN(L"Result: %d\n", result);

        grid = vector<vector<int>>({ { 0 } });
        result = s.minPathSum(grid);
        PRINT_INFO_COLOR_GREEN(L"Result: %d\n", result);

        grid = vector<vector<int>>({ { 1,2,5 },{ 3,2,1 } });
        result = s.minPathSum(grid);
        PRINT_INFO_COLOR_GREEN(L"Result: %d\n", result);

        return TRUE;
    }
}TEST_END(Test64);
