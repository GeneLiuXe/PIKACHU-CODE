// ---
// 题意：\\
// 给定一棵$n$个点的树，以及一张$k$个点的图。树中每一个节点都控制图中的一条边，问：对于树中每一个节点，将其子树 (包括自己) 中所有节点控制的边加到图中，图中连通块个数即为这个节点的答案。\\
// \\
// 思路：\\
// 树上启发式合并裸题，但是比赛的时候没有写过启发式合并以为这样写会$T$...就没有尝试...\\
// \\
// 先具体讲一下这题的解法，再分析一下复杂度。`用并查集维护图中的连通关系，对于当前节点，最后求其重儿子子树所形成的并查集，并将重儿子的并查集直接复用在求父节点上。对于非重儿子节点再对其子树中的边进行添加。\\
// \\
// 非常暴力的做法...但是为什么能够快速通过呢...我们先考虑一下序列上的启发式合并，由于每次选择将小的合并进大的，所以每个元素的大小每次至少$*2$，因此最多被合并$log$次就可以达到$n$的大小，所以复杂度是$nlogn$。\\
// \\
// 而对于树上的启发式合并，每次将小的子树合并进大的子树中，因此子树的大小每次至少$*2$，因此最多$logn$次子树的大小就可以达到$n$，因此复杂度也是$nlogn$。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>
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

int sz[N],n,k,l[N],r[N],ans[N],f[N],num;
vector<int> p[N];

int find(int x){
	return x == f[x] ? x : (f[x] = find(f[x]));
}

void clear(int x){
	f[l[x]] = l[x], f[r[x]] = r[x];
	for(int u : p[x]) clear(u);
}

void unite(int x,int y){
	int fx = find(x), fy = find(y);
	if(fx != fy) f[fx] = fy, num--;
}

void solve(int x){
	unite(l[x],r[x]);
	for(int u : p[x]) solve(u);
}

void dfs(int x){
	int y = -1;
	for(int u : p[x])
		if(y == -1 || sz[y] < sz[u]) y = u;
	for(int u : p[x]){
		if(u == y) continue;
		num = k, dfs(u), clear(u);
	}
	num = k;
	if(y != -1) dfs(y);
	for(int u : p[x]){
		if(u != y) solve(u);
	}
	unite(l[x],r[x]);
	ans[x] = num;
}

int main()
{
	scanf("%d%d",&n,&k);
	rep(i,2,n){
		int pp; scanf("%d",&pp);
		p[pp].push_back(i);
	}
	rep(i,1,n) scanf("%d%d",&l[i],&r[i]);
	for(int i = n; i >= 1; i--){
		for(int y : p[i]) sz[i] += sz[y];
		sz[i]++;
	}
	rep(i,0,k) f[i] = i;
	dfs(1);
	rep(i,1,n) printf("%d\n",ans[i]);
	return 0;
}
