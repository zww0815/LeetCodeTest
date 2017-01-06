
TEST_CASE(Test53)
{
    TEST_NAME(L"53. Maximum Subarray");
    TEST_DESC(L"Find the contiguous subarray within an array (containing at least one number) which has the largest sum.\n\
            For example, given the array[-2, 1, -3, 4, -1, 2, 1, -5, 4],\n\
            the contiguous subarray[4, -1, 2, 1] has the largest sum = 6.");

    using namespace std;
    #include "53. Maximum Subarray.hpp"

    DEBUG_FUNCTION();

    vector<int> vec = { 9, 4, 2,5,6,3,1,7,8,0 };
    Solution s;
    int result = s.maxSubArray(vec);
    TEST_EQUAL(result, 45);

    vec = { 0 };
    result = s.maxSubArray(vec);
    TEST_EQUAL(result, 0);

    vec = { -2,1,-3,4,-1,2,1,-5,4 };
    result = s.maxSubArray(vec);
    TEST_EQUAL(result, 5);
}

