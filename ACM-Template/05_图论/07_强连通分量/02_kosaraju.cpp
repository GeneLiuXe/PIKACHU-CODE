// ---
// 用两次 dfs 求解 SCC 问题 \\
// \\
// 补充一个 DAG 性质：\\
// 1. 选出最小的顶点集合，使得任意点该集合可达 = 入度为 0 的点\\
// 2. 至少加几条边，使得 DAG 成为一个 SCC = max(入度为 0 的点个数，出度为 0 的点个数)\\
// 3. 至多加几条边，使得 DAG 不是一个 SCC = 极大非连通子图\\
// \\
// 极大非连通子图：\\
// 图由 x 和 y 两部分构成，其中 x 是一个完全有向图，y 也是一个完全有向图，然后 x 中的点全部连向 y \\
// 因此极大非连通子图的边数为 n*(n-1)-x*y，且 x+y=n，因此 x 和 y 的差值越大，xy 越小，总边数越多\\
// ---
#include <bits/stdc++.h>
const int N = 2e5;
using namespace std;

int n,c[N],dfn[N],vis[N],dcnt,scnt;
// dcnt - dfs序计数，scnt - scc计数
// dfn[i] - dfs后序列中第i个点
// c[i] - i 号点所在 scc 编号
vector<int> G1[N], G2[N]; // G1-原图, G2-反图

void dfs1(int x){
	vis[x] = 1;
	for(auto y:G1[x])
		if(!vis[y]) dfs1(y);
	dfn[++dcnt] = x;
}

void dfs2(int x){
	c[x] = scnt;
	for(auto y:G2[x])
		if(!c[y]) dfs2(y);
}

void kosaraju(){
	// 初始化
	dcnt = scnt = 0;
	memset(c,0,sizeof c);
	memset(vis,0,sizeof vis);
	// 第一遍 dfs
	for(int i = 1; i <= n; i++)
		if(!vis[i]) dfs1(i);
	// 第二遍 dfs
	for(int i = n; i >= 1; i--)
		if(!c[dfn[i]]) ++scnt, dfs2(dfn[i]);
}

int main()
{
	int m; scanf("%d%d",&n,&m);
	for(int i = 1; i <= m; i++){
		int a,b; scanf("%d%d",&a,&b);
		G1[a].push_back(b);
		G2[b].push_back(a);
	}
	kosaraju();
	return 0;
}