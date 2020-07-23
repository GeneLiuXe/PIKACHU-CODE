// ---
// 题意：\\
//         给定k与b，求出所有k在0~($2^b$-1)范围内的倍数，将这些倍数二进制中的1求sum和，模1e9+9输出。\\
// \\
// 思路：\\
//         首先，这是一个在数位上的dp，重点就在于如何描述每个数的状态。\\
// \\
//         发现数的范围很大，想要直接描述是不可能的。但是k的范围很小，只有1000，因此考虑存储这个数%k之后的值。\\
// \\
//         然后就可以列出dp方程，dp[i][j] 表示前 i 个二进制位，mod k = j 的个数，再用ans[i][j] 表示前 i 个二进制位，mod k = j 的每一种情况二进制拆分后 1 的总和。\\
// \\
//         因为在mod意义下的加减都是可以的。因此对于第 i 个位置，我们只需考虑此处为0还是1，只有两个状态，然后就可以列出转移方程。\\
// \\
//         dp[i][j] = dp[i-1][j]+dp[i-1][(j-poww[i]+k)%k]，poww[i] = $2^(i-1)$。\\
// \\
//         ans[i][j] = ans[i-1][j]+ans[i-1][(j-poww[i]+k)%k]+dp[i-1][(j-poww[i]+k)%k]，即当第 i 位填1时，还需要加上dp[i-1][(j-poww[i]+k)%k]个 1 。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
const int mod = 1e9+9;
 
int k,b,dp[150][1100],poww[200],ans[150][1100];
 
int main()
{
	scanf("%d%d",&k,&b);
	poww[1] = 1%k;
	rep(i,2,130) poww[i] = (poww[i-1]*2)%k;
	dp[1][0] = 1, dp[1][1] = 1;
	ans[1][0] = 0, ans[1][1] = 1;
	if(k == 1) dp[1][0] = 2, dp[1][1] = 0, ans[1][0] = 1, ans[1][1] = 0;
	rep(i,2,b)
		rep(j,0,k-1){
			dp[i][j] = (dp[i-1][j]+dp[i-1][(j-poww[i]+k)%k])%mod;
			ans[i][j] = ((ans[i-1][j]+ans[i-1][(j-poww[i]+k)%k])%mod+dp[i-1][(j-poww[i]+k)%k])%mod;
		}
	printf("%d\n",ans[b][0]);
	return 0;
}