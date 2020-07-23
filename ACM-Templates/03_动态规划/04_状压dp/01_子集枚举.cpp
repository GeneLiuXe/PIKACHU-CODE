// ---
// 题意：\\
// 一共$N$个人，给出任意两个人之间的胜负关系，你的编号是$M$。现在需要安排一棵竞赛树使得$M$能够胜出，问使竞赛树高度最小且$M$获胜的安排方案一共有多少个。$(1\leq N\leq 16)$\\
// \\
// \\
// 思路：\\
// 根据题意以及数据范围，可以很明显的发现这是一个状压$dp$，因此我们来考虑$dp$的状态。\\
// \\
// 既然是状态，那肯定要记录当前的状态，即选了哪些人，然后还要记录当前的胜利者，以及当前树的高度，因此$dp[i][j][k]$表示$i$状态下，胜出者为$j$，树高度为$k$的安排方案数。\\
// \\
// 然后采用记忆化搜索，求取$dp[i][j][k]$时将$i$分为两个子状态$x$、$y$，然后递归求取$dp[x][j][k-1]$，枚举$m$为$y$状态下的胜利者并且会输给$j$，求取$dp[y][m][k]$。具体细节见代码。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 1e5+100;
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;

template<class T> int getbit(T s, int i) { return (s >> i) & 1; }
template<class T> T onbit(T s, int i) { return s | (T(1) << i); }
template<class T> T offbit(T s, int i) { return s & (~(T(1) << i)); }
template<class T> int cntbit(T s) { return __builtin_popcount(s);}

int c[20][20],n,win,h[20];
ll dp[1<<16][17][7];

ll solve(int stat,int m,int height){
	if(dp[stat][m][height] != -1) return dp[stat][m][height];
	if(cntbit(stat) == 1){
		if(getbit(stat,m-1)) return dp[stat][m][height] = 1ll;
		else return dp[stat][m][height] = 0;
	}
	for(int i = (stat-1)&stat; i >= 1; i = (i-1)&(stat)){ //枚举子集
		int x = i, y = stat-i;
		if(!getbit(x,m-1) || h[cntbit(x)] > height-1 || h[cntbit(y)] > height-1) continue;
		// if(height-1 > cntbit(x) || height-1 > cntbit(y)) continue;
		ll ans1 = solve(x,m,height-1);
		ll ans2 = 0;
		rep(j,1,n)
			if(getbit(y,j-1) && c[m][j] == 1) ans2 += solve(y,j,height-1);
		if(dp[stat][m][height] == -1) dp[stat][m][height] = ans1*ans2; 
		else dp[stat][m][height] += ans1*ans2;
	}
	return max(0ll,dp[stat][m][height]);
}

int main()
{
	freopen("f.in","r",stdin);
	freopen("f.out","w",stdout);
	scanf("%d%d",&n,&win);
	rep(i,1,n)
		rep(j,1,n) scanf("%d",&c[i][j]);
	rep(i,1,16) h[i] = (log(i-0.5)/log(2))+2;
	memset(dp,-1,sizeof dp);
	printf("%lld\n",max(0ll,solve((1<<n)-1,win,h[n])));
	return 0;
} 