class Solution {
public:
#if 0
    // dp[i][j] Ϊ�����ϵ���(i,j)����Сֵ
    // ����ʽ��
    // dp[i][j] = min(dp[i-1][j], dp[i][j-1]) + a[i][j];
    // ��ֵ���±��0��ʼ��
    //     dp[0][0] = a[0][0];
    //     dp[0][j>0] = dp[0][j-1] + a[0][j];
    //     dp[i>0][0] = dp[i-1][0] + a[i][0];
    // ��ά����ʵ�֣� ʱ�临�ӶȺͿռ临�Ӷ�ΪO(m*n)
    int minPathSum(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();

        vector<vector<int>> dp(m, vector<int>(n));

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i == 0)
                {
                    if (j == 0)
                    {
                        dp[i][j] = grid[i][j];
                    }
                    else
                    {
                        dp[i][j] = dp[i][j - 1] + grid[i][j];
                    }
                }
                else if (j == 0)
                {
                    dp[i][j] = dp[i - 1][j] + grid[i][j];
                }
                else
                {
                    dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
                }
            }
        }

        return dp[m - 1][n - 1];
    }
#endif

#if 1
    // ȥ��һά����һά����ʵ�֣� ʱ�临�Ӷ�O(m*n) �ռ临�Ӷ�ΪO(n)
    int minPathSum(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();

        vector<int> dp(n);

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i == 0)
                {
                    if (j == 0)
                    {
                        dp[j] = grid[i][j];
                    }
                    else
                    {
                        dp[j] = dp[j-1] + grid[i][j];
                    }
                }
                else if (j == 0)
                {
                    dp[j] = dp[j] + grid[i][j];
                }
                else
                {
                    dp[j] = min(dp[j-1], dp[j]) + grid[i][j];
                }
            }
        }

        return dp[n - 1];
    }
#endif

};