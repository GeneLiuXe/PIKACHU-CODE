// ---
// 题意：\\
// $n$个点，$m$个操作，操作共两类。$①\ 1\ u\  v$ 表示在图中加一条边连接$u、v$，$②\ 2\ u\ v$ 表示查询 $u$ 与 $v$ 最早是在哪一次加边操作后连通，不连通输$-1$。 $(1\leq n,m\leq 10^5)$\\
// \\
// 思路：\\
// 维护连通性，最直观的想法就是用并查集来维护连通性。但是如何通过并查集来查看两点最早什么时候连通呢？\\
// \\
// 首先可以知道并查集维护的其实是一个森林，假如我们不破坏树的结构，即不进行路径压缩，则并查集每次加边，则将边权定义为操作编号，那么两点树上边权最大值就是两点最早连通的加边操作。\\
// \\
// 因此问题变成如何维护树的结构进行并查集合并，方法就是按秩合并，将小的树合并到大的树上，这样可以保证每个节点最多被合并 $log(n)$ 次，因此每个节点的高度最多为$log(n)$。因此对于每次查询，我们可以将两个点到根节点的路径直接取出，然后查询第一次遇到的位置，输出达到这个位置之前的边权最大值即可。\\
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
const int N = 1e5+100;
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;

int f[N],n,m,siz[N],d[N],flag[N],vis[N];

int find(int x){
	if(x == f[x]) return x;
	else return find(f[x]);
}

int solve(int u,int v){
	vis[u] = 1; flag[u] = 0; int w = 0;
	while(f[u] != u){
		w = max(w,d[u]);
		vis[f[u]] = 1; flag[f[u]] = w;
		u = f[u];
	}
	vis[u] = 1, flag[u] = w;
	w = 0;
	if(vis[v] == 1) return flag[v];
	while(f[v] != v){
		w = max(w,d[v]);
		if(vis[f[v]] == 1) return max(w,flag[f[v]]);
		v = f[v];
	}
	if(vis[v] == 1) return max(flag[v],w);
}

void clear(int u){
	while(f[u] != u) vis[u] = 0, flag[u] = 0, u = f[u];
	vis[u] = 0, flag[u] = 0;
}

int main()
{
	int _; scanf("%d",&_);
	while(_--){
		scanf("%d%d",&n,&m);
		rep(i,0,n) f[i] = i, d[i] = 0, siz[i] = 1;
		rep(i,1,m){
			int op,u,v; scanf("%d%d%d",&op,&u,&v);
			int xu = find(u), xv = find(v);
			if(op == 1 && xu != xv){
				if(siz[xu] < siz[xv]) f[xu] = xv, d[xu] = i, siz[xv] += siz[xu];
				else f[xv] = xu, d[xv] = i, siz[xu] += siz[xv];
			}
			else if(op == 2){
				if(xu != xv) printf("-1\n");
				else{
					printf("%d\n",solve(u,v));
					clear(u);
				}
			}
		}
	}
	return 0;
}
