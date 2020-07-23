// ---
// 题意：\\
// 一个序列，选手 $A$、$B$ 轮流从序列中从左端或者右端选一段区间，然后区间和加到自己的权值中。两个选手都会按照最优的方式进行选取，问先手 $A$ 最多可以比 $B$ 多拿多少。 $(1\leq n\leq 100)$\\
// \\
// 思路：\\
// 很明显这是一道 $DP$ 问题，又因为只能从左端点或右端点拿，因此不难想到用区间 $DP$ 的方法来解决此题。\\
// \\
// 既然是区间 $DP$，那么最常见的状态就是 $DP[i][j]$ 表示对于区间 $[i,j]$ ，先手最多领先后手多少。又因为区间和是一定的，因此已知选手 $A$ 获得的价值就可以知道选手 $B$ 获得的价值，因此修改状态为 $DP[i][j]$ 表示区间 $[i,j]$，先手最多可以获得多少价值。\\
// \\
// 因此 $DP[i][j] = max(sum[i][j]-DP[x][j],sum[i][j]-DP[i][y])，i < x,y < j$，由于 $n$ 比较小，直接枚举区间长度，从小区间到大区间进行转移即可。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
typedef long long ll;
typedef double db;
const db EPS = 1e-9;
using namespace std;
const int N = 200;

int n,a[N],sum[N],dp[N][N];
//dp[i][j]: 表示区间[i,j]先手能够获得的最大价值

int main()
{
	while(~scanf("%d",&n))
	{
		if(!n) break;
		memset(dp,0,sizeof dp);
		rep(i,1,n) scanf("%d",&a[i]);
		rep(i,1,n) sum[i] = sum[i-1]+a[i];
		rep(len,1,n)
			rep(i,1,n-len+1){
				int j = i+len-1;
				dp[i][j] = sum[j]-sum[i-1];
				rep(k,1,len-1){
					dp[i][j] = max(sum[j]-sum[i-1]-dp[i+k][j],dp[i][j]);
					dp[i][j] = max(sum[j]-sum[i-1]-dp[i][j-k],dp[i][j]);
				} 
			}
		int ans = dp[1][n]-(sum[n]-sum[0]-dp[1][n]);
		printf("%d\n",ans);
	}
	return 0;
}
