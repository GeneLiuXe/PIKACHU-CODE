// ---
// 题意：\\
//        给定一颗树，单向边，给出每个点的价值。然后任选树中一个点 i 进入，从 j 点出来，获得的价值为val[i]*1+val[i+1]*2+...+val[j]*(j-i+1)，也可以选择不进入，节点价值有正有负，求最多可以获得多少价值。\\
// \\
// 思路：\\
//         这题可以简化成在一个序列上，找出一段 [x,y]，求SUM(val[i]*(i-x+1))的最大值，即1*val[1]+2*val[2]+3*val[3]+...的问题。\\
// \\
//         我们可以列一下dp方程，dp[i]表示以i为右端点的区间和最大值，假设dp[i]的最优解是[j+1, i]这一段，则dp[i] = f[i]-f[j]-(sum[i]-sum[j])*j，sum[i]表示从根节点到i路径上所有点的权值之和，f[i]表示从根节点到i路径上所有点按照1*val[1]+2*val[2]+3*val[3]的方式得到的累加和。\\
// \\
//         将dp方程拆成斜率优化的形式，可以得到f[j]-j*sum[j] = -sum[i]*j+f[i]-dp[i]，而dp[i]就是答案。因此令横坐标为j，纵坐标为f[j]-j*sum[j]，在图中标出这些点，维护一个下凸壳，然后在下凸壳中二分左边斜率最接近-sum[i]的点即为最优点。因此问题变成了如何维护下凸壳，正常的维护方法是用单调栈，添加一个点之后就将其他的点一一弹出，直到该点到达合理位置。但由于现在是在树上维护凸壳，因此下凸壳的序列会不断变化，因此我们需要进行动态维护。\\
// \\
//         可以发现每次往下凸壳中加入一个点，这个点最终都会到达原凸壳中的某一个位置，因此每次添加一个点，只是O(1)修改，然后我们在修改完之后再改回来，然后再维护原来的凸壳长度，即可实现动态维护。如何求该点最终到达的位置，只需要进行二分，找到点x与x+1之间斜率最接近x与新点之间斜率的点x，x+1处即为新点最终的更新位置。因此本题就是两个二分+dfs结束。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
typedef long long ll;
const int N = 1e5+1000;
 
int n,q[N],fa[N],head[N],tot,qh,qt;
ll val[N],ans,x0[N],y0[N];
struct Edge{
	int to,next;
}e[N];
 
void init()
{	
	tot = 1;
	rep(i,0,n) head[i] = 0;
}
 
void add(int x,int y)
{
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot; 
}
 
bool jud(int a,int b,int c)
{
	return ((y0[a]-y0[b])*(x0[c]-x0[b]) > (y0[c]-y0[b])*(x0[a]-x0[b]));
}
 
int calc1(int l,int r,int x)	//维护凸壳
{
	int tp = r+1; r = r-1;
	while(l <= r)
	{
		int mid = (l+r)>>1;
		if(jud(q[mid+1],q[mid],x)) tp = mid+1, r = mid-1;
		else l = mid+1;
	}
	return tp;
}
 
void calc2(int l,int r,ll k,ll f)	//斜率优化dp,找到第一个右边斜率比k大的点
{
	int tp = q[l]; r--;
	while(l <= r)
	{
		int mid = (l+r)>>1;
		if(y0[q[mid+1]]-y0[q[mid]] < k*(x0[q[mid+1]]-x0[q[mid]])) tp = q[mid+1], l = mid+1;
		else r = mid-1;
	}
	ans = max(ans,-y0[tp]+k*x0[tp]+f);
}
 
void dfs(int x,int dep,ll sum,ll f)	//从1号点往下dfs
{
	x0[x] = dep, y0[x] = f-dep*sum;
	calc2(qh,qt,-sum,f);
	int oldh = qh, oldt = qt;
	int pos = calc1(qh,qt,x), old = q[pos]; q[pos] = x, qt = pos;
	for(int i = head[x]; i; i = e[i].next)
	{
		int y = e[i].to;
		dfs(y,dep+1,sum+val[y],f+(dep+1)*val[y]);
	}
	qh = oldh, qt = oldt, q[pos] = old;
}
 
int main()
{
	int T; scanf("%d",&T);
	while(T--)
	{
		init();
		ans = 0;
		scanf("%d",&n);
		rep(i,1,n) scanf("%lld",&val[i]);
		rep(i,2,n) scanf("%d",&fa[i]), add(fa[i],i);
		qh = qt = 1; q[1] = 0;
		dfs(1,1,val[1],val[1]);
		printf("%lld\n",ans);
	}
	return 0;
}