// ---
// 题意：有$n$堆魔法珠，第$i$堆有$a_i$颗。选择$n$堆魔法珠中数量大于$1$的任意一堆。记该堆魔法珠数量为$p$，$p$有$b_1、b_2、b_3\ ...\ b_m$这$m$个小于$p$的约数。然后将这一堆魔法珠变成$m$堆，每堆各有$b_1、b_2、b_3\ ...\ b_m$颗魔法珠。最后选择这$m$堆的任意一堆，令其消失，不可操作者输，问谁能获胜。$(1\leq n\leq 100,1\leq a_i\leq 1000)$\\
// \\
// 思路：不难发现整个游戏就是由多个魔法珠堆组成的，因此$SG[x]$表示某一堆魔法珠，个数为$x$时的$SG$值。求$SG[x]$时，若$x$有$b_1、b_2、b_3\ ...\ b_m$这$m$个小于$x$的约数，则可以达到的后继状态一共有$m$个，即去掉任意一堆达到的状态。而每一个状态中仍有$m-1$个堆，即$m-1$个子游戏，因此每一个状态的$SG$值为这$m-1$个子游戏$SG$值的异或和。具体过程见代码。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 1000+10;
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;

int sg[N],n;

int solve(int x)
{
	if(sg[x] != -1) return sg[x];
	int vis[2010]; memset(vis,0,sizeof vis);
	int ans[2010]; //记录每一个约数
	int tp = 0, ct = 0;
	rep(i,1,x-1){
		if(x%i == 0){
			ans[++ct] = solve(i);
			tp ^= ans[ct];
		}
	}
	rep(i,1,ct) vis[tp^ans[i]] = 1; //枚举每一个子状态
	rep(i,0,2000)
		if(vis[i] == 0) return sg[x] = i;
}

int main()
{
	memset(sg,-1,sizeof sg);
	sg[1] = 0;
	while(~scanf("%d",&n)){
		int ans = 0;
		rep(i,1,n){
			int xx; scanf("%d",&xx);
			ans ^= solve(xx);
		}
		if(ans == 0) printf("rainbow\n");
		else printf("freda\n");
	}
	return 0;
}