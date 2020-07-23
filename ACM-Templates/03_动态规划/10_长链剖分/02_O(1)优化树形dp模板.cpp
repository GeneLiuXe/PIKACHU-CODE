// ---
// 题意：\\
// $n$ 个点的一棵树，对于树上每个点 $x$，求出一个 $ans[x]$，表示在 $x$ 的子树中，$ans[x]$ 深度处点的个数最多，若个数相同，令下标最小。$(1\leq N\leq 10^6)$\\
// \\
// 思路：\\
// 这是一道长链剖分的经典例题，也可以说是模板题。可以查看这道题的代码，来学会长链剖分的基础操作。\\
// \\
// 这道题思路比较经典，就是 $O(1)$ 继承重儿子，暴力继承轻儿子，更新的时候记录最大值即可。\\
// ---
#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const db EPS = 1e-9;
const int N = 1e6+100;
using namespace std;

int n,tot,head[N],ans[N],d[N],son[N];
int *f[N],tmp[N],*id=tmp;
struct Node{
	int to,next;
}e[2*N];

void add(int x,int y){
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot;
}

void dfs1(int x,int fa){
	for(int i = head[x]; i; i = e[i].next){
		int y = e[i].to;
		if(y == fa) continue;
		dfs1(y,x);
		if(d[y] > d[son[x]]) son[x] = y;
	}
	d[x] = d[son[x]]+1;
}

void dfs2(int x,int fa){
	f[x][0] = 1;
	if(son[x]){
		f[son[x]] = f[x]+1;
		dfs2(son[x],x);
		ans[x] = ans[son[x]]+1;
	}
	for(int i = head[x]; i; i = e[i].next){
		int y = e[i].to;
		if(y == fa || y == son[x]) continue;
		f[y] = id; id += d[y];
		dfs2(y,x);
		rep(j,0,d[y]-1){
			f[x][j+1] += f[y][j];
			if(f[x][j+1] > f[x][ans[x]] || (f[x][j+1] == f[x][ans[x]] && j+1 < ans[x])) ans[x] = j+1;
		}
	}
	if(f[x][ans[x]] == 1) ans[x] = 0;
}

int main()
{
	scanf("%d",&n); tot = 1;
	rep(i,1,n-1){
		int x,y; scanf("%d%d",&x,&y);
		add(x,y); add(y,x);
	}
	dfs1(1,0);
	f[1] = id; id += d[1];
	dfs2(1,0);
	rep(i,1,n) printf("%d\n",ans[i]);
	return 0;
}
/*
题意：n个点的一棵树，对于树上每个点x，求出一个ans[x]，表示在x的子树中，ans[x]深度处点的个数最多。
*/