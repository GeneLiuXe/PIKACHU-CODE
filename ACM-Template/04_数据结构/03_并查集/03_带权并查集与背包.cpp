// ---
// 题意：给出 $p1$ 个天使，$p2$ 个魔鬼，一共有 $n$ 个问题。每个问题的格式为 $x\  y\  (yes\  or\  no)$ 表示问 $x$ $y$ 是否为天使，如果 $x$ 为天使，则会说真话，如果 $x$ 为魔鬼，则会说假话。问根据这 $n$ 个问题，是否可以确定哪些人为天使，如果可以确定，按编号大小输出天使编号。$(n\leq 1000,\ \ p1,p2\leq 300)$\\
// \\
// 思路：遇到这样的 $yes$ $or$ $no$ 问题，显然我们需要对于可能出现的情况进行模拟。\\
// $1.$ 假如 $x$ 为天使，$y$ 为天使，则回答 $yes$ 。\\
// $2.$ 假如 $x$ 为天使，$y$ 为魔鬼，则回答 $no$ 。\\
// $3.$ 假如 $x$ 为魔鬼，$y$ 为天使，则回答 $no$ 。\\
// $4.$ 假如 $x$ 为魔鬼，$y$ 为魔鬼，则回答 $yes$ 。\\
// \\
// 可以发现，如果回答是 $yes$，则 $x$ 与 $y$ 属于同一类。如果回答 $no$，则 $x$ 与 $y$ 类别相反。因此一个模 $2$ 剩余系的带权并查集合并就可以维护所有人之间的关系。\\
// \\
// 因此不难发现，处理完 $n$ 个问题之后，我们会拥有若干个并查集，每个并查集中都会分为两批人，两批人类别不同。\\
// \\
// 因此问题变成了，从这若干个并查集中随机选一部分，使得最后选中的人数恰好为 $p1$，并且这种选择方式唯一，则我们可以确定哪些人为天使。因此本题就变成了一个类似背包的问题，$dp[i][j]$ 表示前 $i$ 个并查集选取人数为 $j$ 一共有多少种选择方案，$pre[i][j]=x$ 表示 $dp[i][j]$ 由 $dp[i-1][x]$ 更新而来。到此，本题即可顺利解决。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <map>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
typedef long long ll;
typedef double db;
const db EPS = 1e-9;
using namespace std;
const int N = 700;

int n,p1,p2,fa[N],d[N],base[N][2],tot,dp[N][600],pre[N][600],vis[N];
map<int,int> mp;

int find(int x)
{
	if(x == fa[x]) return x;
	int root = find(fa[x]);
	d[x] = (d[x]+d[fa[x]]+2)%2;
	return fa[x] = root;
}

void solve()
{
	memset(base,0,sizeof base);
	memset(vis,0,sizeof vis);
	mp.clear();
	tot = 0;
	rep(i,1,p1+p2){
		fa[i] = find(i);
		if(mp.find(fa[i]) == mp.end()) mp[fa[i]] = ++tot;
		int pos = mp[fa[i]];
		base[pos][d[i]]++;	//0: 相同	1：不同
	}
	memset(dp,0,sizeof dp);
	memset(pre,0,sizeof pre);
	dp[1][base[1][1]]++;
	dp[1][base[1][0]]++;
	rep(i,2,tot){
		int minn = min(base[i][0],base[i][1]);
		rep(j,minn,p1){
			if(j >= base[i][0] && dp[i-1][j-base[i][0]] != 0)
				dp[i][j] += dp[i-1][j-base[i][0]], pre[i][j] = j-base[i][0];
			if(j >= base[i][1] && dp[i-1][j-base[i][1]] != 0)
				dp[i][j] += dp[i-1][j-base[i][1]], pre[i][j] = j-base[i][1];
		}
	}
	if(dp[tot][p1] != 1) printf("no\n");
	else{
		int xx = tot, yy = p1;
		while(xx > 1){
			if(pre[xx][yy] == yy-base[xx][0]) vis[xx] = 0, yy -= base[xx][0];
			else if(pre[xx][yy] == yy-base[xx][1]) vis[xx] = 1, yy -= base[xx][1];
			xx--;
		}
		if(xx == 1){
			if(base[1][0] == yy) vis[1] = 0;
			else vis[1] = 1;
		}
		rep(i,1,p1+p2){
			int pos = mp[fa[i]];
			int dd = d[i];
			if(vis[pos] == 1 && dd == 1) printf("%d\n",i);
			else if(vis[pos] == 0 && dd == 0) printf("%d\n",i);
		}
		printf("end\n");
	}
}

int main()
{
	while(~scanf("%d%d%d",&n,&p1,&p2))
	{
		if((n+p1+p2) == 0) break; 
		rep(i,0,p1+p2) fa[i] = i, d[i] = 0;
		rep(i,1,n){
			int x,y; char op[10];
			scanf("%d%d",&x,&y);
			scanf("%s",op);
			int fx = find(x), fy = find(y);
			// LOG2("x",x,"y",y);
			// LOG2("fx",fx,"fy",fy);
			if(fx != fy){
				fa[fx] = fy;
				if(op[0] == 'n') d[fx] = (2+1+d[y]-d[x])%2;
				else d[fx] = (d[y]-d[x]+2)%2;
			}
		}
		solve();
	}
	return 0;
}