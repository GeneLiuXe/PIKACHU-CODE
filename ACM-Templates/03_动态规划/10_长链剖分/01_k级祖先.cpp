// ---
// 题意：\\
// $N$ 个点的一棵树，$M$ 次询问，每次询问给出一个 $x$、$k$，表示询问点 $x$ 的 $k$ 级祖先。  $(1\leq N\leq 3*10^5, 1\leq M\leq 1800000)$\\
// \\
// 思路：\\
// $k$ 级祖先是长链剖分的经典问题，主要应用的性质是 “任意一个点的 $k$ 级祖先 $y$ 所在的长链的长度大于等于 $k$”。\\
// \\
// 因此我们对于每个长链的端点维护两个 $vector$，$down[x]$ 中存储了点 $x$ 所属长链上的所有节点，$up[x]$ 中存储了点 $x$ 向上寻找能够寻找到的 $min(d[x],dep[x])$ 个点。此处 $dep[x]$ 表示节点 $x$ 在树中的深度，$d[x]$ 表示以 $x$ 为端点的长链的长度。\\
// \\
// 因此我们查询点 $x$ 的 $k$ 级祖先时，只需要判断点 $x$ 所在长链的端点距 $x$ 的距离是否大于 $k$，如果大于 $k$，则需要使用点 $x$ 长链端点的 $up$ 数组，否则使用 $down$ 数组。大致思路是这样，具体的实现过程中还有一些其它的细节，详见代码。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
const int N = 3*1e5+100;
const int M = 6*1e5+100;
const db EPS = 1e-9;
using namespace std;

int n,m,tot,head[N],top[N],son[N],len[N],fa[N],f[N][25],d[N],hbit[N];
vector<int> up[N], down[N];
struct Edge{
	int to,next;
}e[M];

void add(int x,int y){
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot;
}

void dfs1(int x,int y){
	len[x] = 1; d[x] = d[y]+1; f[x][0] = fa[x];
	for(int i = 1; (1<<i) <= d[x]; i++)
		f[x][i] = f[f[x][i-1]][i-1];
	for(int v,i = head[x]; i; i = e[i].next)
		if((v = e[i].to) != y){
			fa[v] = x;
			dfs1(v,x);
			if(len[son[x]] < len[v]) son[x] = v;
		}
	len[x] = len[son[x]]+1;
}

void dfs2(int x,int y){
	top[x] = y;
	if(son[x]) dfs2(son[x],y);
	for(int v,i = head[x]; i; i = e[i].next)
		if((v = e[i].to) != fa[x] && v != son[x]) dfs2(v,v);
}

int query(int x,int k){
	if(k == 0) return x;
	if(d[x] < k+1) return 0;
	int r = f[x][hbit[k]];
	if(d[r] == d[x]-k) return r;
	else{
		int an = top[r];
		if(d[an] <= d[x]-k){
			int len = d[x]-k-d[an];
			return down[an][len];
		}
		else{
			int len = d[an]-d[x]+k;
			return up[an][len];
		}
	}
}

int main()
{
	scanf("%d",&n); tot = 1;
	rep(i,1,n-1){
		int x,y; scanf("%d%d",&x,&y);
		add(x,y); add(y,x);
	}
	scanf("%d",&m);
	dfs1(1,0);
	dfs2(1,1);
	int ans = 0;
	rep(i,1,n)
		if(top[i] == i){
			int x = i;
			while(x) {down[i].push_back(x); x = son[x];}
			int ct = len[i];
			x = i;
			while(x && ct) {up[i].push_back(x); x = fa[x]; ct--;}
		}
	rep(i,1,300000)
		rep(j,0,20)
			if(i & (1<<j)) hbit[i] = max(hbit[i],j);
	rep(i,1,m){
		int x,y; scanf("%d%d",&x,&y);
		x ^= ans; y ^= ans;
		ans = query(x,y);
		printf("%d\n",ans);
	}
	return 0;
}