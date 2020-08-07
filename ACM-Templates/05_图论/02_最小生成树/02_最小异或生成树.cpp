// ---
// 题意：给定一颗 $n$ 个点的树，每条边的权重为 $w_i$。$(2\leq n\leq 10^5,0\leq w_i< 2^{30})$ \\
// \\
// 可以任意删除一些边或者增加一些边，但是在删增过程中必须保证图连通并且任意一个环的异或和为 $0$。\\
// \\
// 在此基础上，求最小生成树。\\
// \\
// 思路：\\
// 观察题目要求，删增过程中保证图连通且任意一个环的异或和为 $0$，所以其实 $(u,v)$ 边的权重是已经确定的，即两点之间边的异或和。\\
// \\
// 确定完边权后，问题就转换为了对于一个 $n$ 个点的完全图，求最小生成树。\\
// \\
// 为了解决这个问题，我们先了解一下 $Boruvka$ 算法：\\
// $Boruvka$ 算法主要针对这些问题：$n$ 个点，每个点有点权，任意两个点之间有边权，边权为两个点权用某种计算方式得出，求最小生成树。\\
// \\
// 对于这些问题，我们通常用 $O(log(n))$ 的时间找到与点 $i$ 连边的边权最小的 $j$，具体生成树算法如下：\\
// 1. 考虑维护当前的连通块（初始每个点为独立的一个连通块）\\
// 2. 对每个连通块，找到一条与该连通块相连的，且另一端点不在此连通块中的边权最小的边 \\
// 3. 将所有的这些边都加入最小生成树，注意，当加入一条边时需判断该边的两端点是否在同一连通块 \\
// 4. 重复若干遍上述操作，直到图连通 \\
// \\
// 上述算法即为 $Boruvka$ 算法，每次操作完，连通块个数至少减半，则复杂度为 $O((n+m)log(n))$（并查集复杂度为常数）\\
// \\
// 在本题中，我们可以先以 $1$ 为起点，求出 $1$ 号点到每一个点的异或和 $d[i]$，任意两点 $u,v$ 的边权即为 $d[u]\ xor\ d[v]$。因此题目转换成，每一个点都有一个权值 $d[i]$，任意一条边 $(u,v)$ 的边权为 $d[u] \ xor \ d[v]$，求最小生成树。\\
// \\
// 这个问题即为最小异或生成树问题，可以采用 $Boruvka$ 思想来解决。\\
// \\
// 首先，由于涉及到了异或，因此引入字典树是非常常见的操作。我们将每一个点的权值从高位到低位依次插入字典树中，然后遍历字典树求答案。对于字典树节点 $x$，当前深度为 $step$，我们令其 $0$ 号儿子为 $lx$，$1$ 号儿子为 $rx$。\\
// \\
// 若 $lx$ 与 $rx$ 只有一个存在，则一定不存在一条边，其边权中包含 $1<<step$，即继续往下递归即可。\\
// \\
// 若 $lx$ 与 $rx$ 均存在，则一定存在一条边，连接左右两部分，即边权中包含 $1<<step$，因此我们还需要从 $lx$ 和 $rx$ 中分别找到一个数字，使其异或值最小。此处可以直接采用贪心的思想，尽可能令每一位的数相同，贪心向下搜索，平均复杂度为 $O(log(n))$，具体过程见下述代码。\\
// \\
// 因此本题总复杂度为遍历字典树 $+$ 每个节点找最小值，即 $O(nlog^2(n))$。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
const int N = 3e6+10;
using namespace std;

int n, tot, trie[N][3];
ll d[N];
vector<pair<int, ll> > G[N];

void dfs1(int x, int fa) {
	for(auto it:G[x]) {
		int y = it.first;
		ll w = it.second;
		if(y == fa) continue;
		d[y] = d[x]^w;
		dfs1(y,x);
	}
}

void insert(ll w) {
	int cur = 0;
	for(int i = 30; i >= 0; i--) {
		int v = ((w&(1<<i)) > 0);
		if(!trie[cur][v]) trie[cur][v] = ++tot;
		cur = trie[cur][v];		
	}
}

ll find(int x, int y, int step) {
	if(step < 0) return 0;
	int lx = trie[x][0], rx = trie[x][1];
	int ly = trie[y][0], ry = trie[y][1];
	ll minn = 1e15;
	if(lx) {
		if(ly) minn = min(minn, find(lx, ly, step-1));
		else minn = min(minn, find(lx, ry, step-1) + (1<<step));
	}
	if(rx) {
		if(ry) minn = min(minn, find(rx, ry, step-1));
		else minn = min(minn, find(rx, ly, step-1) + (1<<step));
	}
	return minn;
}

ll dfs2(int cur, int step) {
	int ls = trie[cur][0], rs = trie[cur][1];
	ll ans = 0;
	if(ls && rs) ans += find(ls, rs, step-1) + (1<<step);
	if(ls) ans += dfs2(ls, step-1);
	if(rs) ans += dfs2(rs, step-1);
	return ans;
}

int main()
{
	scanf("%d",&n);
	rep(i,1,n-1) {
		int u,v,w; scanf("%d%d%d",&u,&v,&w);
		G[u].push_back({v,w});
		G[v].push_back({u,w});
	}
	dfs1(0, -1);
	rep(i,0,n-1) insert(d[i]);
	printf("%lld\n", dfs2(0, 30));
	return 0;	
}