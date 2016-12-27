class Solution {
public:
#if 0
    // 分治
    int maxSubArray(int a[], int n)
    {
        if (1 == n)
        {
            return nums[0];
        }

        int mid = n >> 1;
        // 0...mid-1  mid...n-1
        int answer = max(maxSubArray(a, mid), maxSubArray(a+mid, n-mid));
        int now = a[mid - 1];
        int may = now;
        
        // Merge
        for (int i=mid-2;i>=0;--i)
        {
            may = max(may, now += a[i]);
        }

        now = may;
        for (int i = mid; i < n; ++i)
        {
            may = max(may, now += a[i]);
        }

        return max(answer, may);
    }

    int maxSubArray(vector<int>& nums) 
    {
        int n = nums.size();
        
    }
#endif

    // DP
    int maxSubArray(vector<int>& nums)
    {
        int n = nums.size();
        if (1 == n)
        {
            return nums[0];
        }
        
        vector<int> dp(n, 0);

        for (int i=0; i<n; ++i)
        {
            if (i == 0)
            {
                dp[i] = nums[i];
            }
            else
            {
                dp[i] = max(dp[i - 1] + nums[i], nums[i]);
            }
        }

        return dp[n - 1];
    }

    // 线性枚举
};