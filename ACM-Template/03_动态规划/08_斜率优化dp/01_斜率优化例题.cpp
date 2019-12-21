// ---
// 题意：\\
//        n个需要被处理的任务，机器启动时间为s。每个任务都有时间Ti和花费Ci，计算方法为完成这批任务所需的时间是各个任务需要时间的总和。注意，同一批任务将在同一时刻完成，新的一批任务开始时，机器需要重新启动。确定一个分组方案，使得总费用最小。【每批任务包含相邻的若干任务】\\
// \\
// 思路：\\
//        1  <= n <= 3*1e5，0 <= S,Ci <= 512，-512 <= Ti <= 512。\\
// \\
//        首先我们需要列出dp方程，dp[i]表示完成前i个任务的最小费用。此处由于涉及到s，因此我们需要用到一个叫做“费用提前计算”的思想。因为s对于后续的每个任务都有影响，因此我们应当将s对后续任务的影响提前计算。\\
// \\
//        假设dp[i]由dp[j]更新而来，则dp[i] = dp[j] + sumT[i]*(sumC[i]-sumC[j]) + s*(sumC[N]-sumC[j]); 将转移方程拆开，则可以得到dp[j] - s*sumC[j] = sumT[i]*sumC[j]+dp[i]-s*sumC[N]-sumT[i]*sumC[i]，由此我们可以发现令横坐标x = sumC[j]，纵坐标y = dp[j]-s*sumC[j]，则对于每一个j，都可以在平面中确定一个(x,y)坐标，而直线的斜率也是固定的，为sumT[i]。\\
// \\
//        因此在平面中的这么多点中，我们需要维护一个下凸壳，更新dp[i]的时候，只需要在下凸壳中，二分x点与x+1点的斜率是否小于sumT[i]，找到一个点，该点左边的斜率小于k，右边斜率大于k，则该点即为该下凸壳中的最优点。\\
// \\
//        再谈一下如何维护下凸壳，用一个数组，末尾为qt，则判断qt-1与x的斜率是否小于等于qt-1与qt的斜率，如果小于等于，则需要弹出qt，然后不断继续往下更新。注意此处如果是等于也需要弹出。\\
// \\
//        本题在维护凸壳时，可能会爆long long，需要转成long double。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
const int N = 1e6+100;
typedef long long ll;
 
int n,qt;
ll s,T[N],C[N],sumT[N],sumC[N],dp[N],q[N],xx[N],yy[N];
 
void calc(int x)	//在凸壳中二分最优点，斜率优化
{
	int l = 1, r = qt-1, ans = q[l];
	ll k = sumT[x];
	while(l <= r)
	{
		int mid = (l+r)>>1;
		if((long double)(yy[q[mid+1]]-yy[q[mid]]) <= (long double)(xx[q[mid+1]]-xx[q[mid]])*(long double)k)
			ans = q[mid+1], l = mid+1;
		else r = mid-1;
	}
	dp[x] = yy[ans]-k*xx[ans]+s*sumC[n]+sumT[x]*sumC[x];
}
 
void solve()
{
	qt = 1, q[1] = 0;
	xx[0] = yy[0] = 0;
	rep(i,0,n) xx[i] = 0, yy[i] = 0;
	rep(i,1,n)
	{
		calc(i);
		xx[i] = sumC[i], yy[i] = dp[i]-s*sumC[i];
		while(qt >= 2){
			//重点在于 >=，> 会wa
			if((long double)(yy[q[qt]]-yy[q[qt-1]])*(long double)(xx[i]-xx[q[qt-1]]) >= (long double)(yy[i]-yy[q[qt-1]])*(long double)(xx[q[qt]]-xx[q[qt-1]]))
				qt--;
			else break;
		}
		qt++; q[qt] = i;
	} 
	printf("%lld\n",dp[n]);
}
 
int main()
{
	while(~scanf("%d%lld",&n,&s))
	{
		sumT[0] = sumC[0] = 0;
		rep(i,1,n){
			scanf("%lld%lld",&T[i],&C[i]);
			sumT[i] = sumT[i-1]+T[i];
			sumC[i] = sumC[i-1]+C[i];
		}
		solve();
	}
	return 0;
}