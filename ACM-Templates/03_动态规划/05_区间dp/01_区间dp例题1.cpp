// ---
// 题意：\\
//        给定一个序列。将序列中的一个数字消去的代价是与这个数字相邻的两个数字的gcd，问将所有数字消去的最小代价。注意这个序列是环形的。\\
// \\
// 思路：\\
//         首先我们比较容易发现这是一个区间DP问题，于是问题就变成了如何列区间DP状态。一开始考虑的是dp[i][j]表示区间[i,j]全部消去的最小代价，然后在区间[i,j]中枚举第一个消去的k进行更新。然后会发现一个问题，如何先消的是 j ，那么 j 是右端点，因此 j 两端的元素是不确定的，因此这个转移方程不对。\\
// \\
//         因此我们来重新考虑这道题。由于左右端点不确定，因此我们重新定义dp状态，dp[i][j]表示区间 [i, j] 中所有数全部消除，最后剩下 i 和 j 的最小代价。则在区间中枚举 k ，dp[i][j] = dp[i][k]+dp[k][j]+gcd(i,j) 。由于是个环形序列，因此需要将长度扩展两倍进行dp。最后的答案就是枚举最后剩下的两个点，然后找最小值即可。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
typedef long long ll;
typedef double db;
const db EPS = 1e-9;
using namespace std;
const int N = 300;
 
int n,a[N],dp[N][N];
 
int gcd(int a,int b)
{
	return b == 0 ? a:gcd(b,a%b);
}
 
int main()
{
	while(~scanf("%d",&n))
	{
		if(n == 0) break;
		rep(i,1,n) scanf("%d",&a[i]);
		rep(i,n+1,2*n) a[i] = a[i-n];
		rep(len,1,n)
			rep(j,1,2*n){
				int x = j, y = x+len-1;
				if(y > 2*n) continue;
				if(y == x+1 || y == x) dp[x][y] = 0;
				else{
					dp[x][y] = 10000;
					rep(k,x+1,y-1) dp[x][y] = min(dp[x][y],dp[x][k]+dp[k][y]+gcd(a[x],a[y]));
				}
				// printf("dp[%d][%d]:%d\n",x,y,dp[x][y]);
			}
		int ans = 10000;
		rep(i,1,n)
			rep(j,i+1,i+n-1){
				if(ans > dp[i][j]+dp[j][i+n]+gcd(a[i],a[j])){
					ans = dp[i][j]+dp[j][i+n]+gcd(a[i],a[j]);
					// LOG1("ans",ans);
					// LOG2("i",i,"j",j);
				}
			}
		printf("%d\n",ans);
	}
	return 0;
}