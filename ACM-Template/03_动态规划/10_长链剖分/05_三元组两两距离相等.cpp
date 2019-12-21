// ---
// 题意：\\
// $n$ 个点的一棵树，求树上的三元无序集合 $(a,b,c)$，满足集合中任意两点距离相同的条件的集合个数。$(1\leq n\leq 10^5)$\\
// \\
// 思路：\\
// 这是一道树形 $dp$ 用长链剖分优化的典型问题，主要难点在于树形 $dp$ 的状态方程如何设置。\\
// \\
// 我们首先要思考如何才能不重不漏地计算所有的集合个数。而想要不重不漏地计算个数，就必须要找到一个不变量。而在一棵树中，最常见的不变量就是 $lca$。因此我们对于树上每一个点，枚举其作为 $lca$ 时对答案的贡献。由于 $lca$ 是固定的，所以枚举 $lca$ 可以避免重复计算。\\
// \\
// 然后我们考虑对于一个点作为 $lca$ 时，它对答案的贡献是什么？首先对于树形 $dp$，我们一定是从左往右枚举子节点的一个过程，当枚举到节点 $v$ 时，对答案的贡献要么是左边子树中的两个节点 + $v$子树中的一个节点，不然就是左边子树中的一个节点 + $v$子树中的两个节点。只有这两种情况需要进行枚举。\\
// \\
// 思考到这一步，我们的 $dp$ 状态方程就好定了。首先令 $f[u][j]$ 表示距节点 $u$ 距离为 $j$ 的节点个数，由此我们可以解决子树中一个节点的情况。然后考虑子树中两个节点时，如果设置状态？\\
// \\
// 由于我们需要两个子节点之间的距离，因此我们令 $g[u][j]$ 表示 $u$ 子树中，$(a,b)$ 点对数量。点对满足条件为 $a$、$b$ 到其 $lca$ 距离均为 $d$，而其 $lca$ 到 $u$ 距离为 $d-j$，$d$为任意值。这样设置状态的原因是，$d-j+j=d$，这样就可以满足三点距离相同了。至此，状态便设置完成，然后考虑如何书写转移方程。\\
// \\
// $$
// f[u][j]=f[u][j]+f[v][j-1]
// $$
// \\
// $$
// g[u][j] = g[u][j]+g[v][j+1]
// $$
// \\
// 上述两条转移式子比较显然，但是对于 $g$ 数组来说，我们只考虑了 $d-j$ 不为 $0$ 的情况，即 $u$ 不为 $lca(a,b)$ 的情况，因此还需要考虑 $u=lca(a,b)$ 的情况，即 $(a,b)$ 分别位于 $u$ 的两个子节点的子树中。\\
// \\
// $$
// g[u][j] = g[u][j]+f[u][j]*f[v][j-1]
// $$
// \\
// 至此，我们考虑完了 $f$、$g$ 两个数组的转移方程。接下来我们需要考虑对答案的贡献如何表示。如上所述，每一个节点作为三点 $lca$ 时，对答案的贡献为左子树两点+当前子节点子树中一点，或左子树一点+当前子节点子树中两点。由此可以得到下述方程。\\
// $$
// ans = ans+f[u][j-1]*g[v][j]+g[u][j+1]*f[v][j]
// $$
// \\
// 现在，我们得到了此题完整的四个转移方程。接下来只有最后一个注意点就是，由于 $g[u][j] = g[u][j]+g[v][j+1]$，因此 $g$ 数组我们需要开两倍，避免子节点转移时下标 $-1$ 影响到了其它节点的区间造成越界。\\
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
const int N = 1e5+100;
using namespace std;

int n,tot,head[N],son[N],d[N];
ll *f[N], *g[N], tf[N], tg[2*N], *id1 = tf+1, *id2 = tg+1, ans;
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
		f[son[x]] = f[x]+1; g[son[x]] = g[x]-1;
		dfs2(son[x],x);
	}
	ans += g[x][0];
	for(int i = head[x]; i; i = e[i].next){
		int y = e[i].to;
		if(y == son[x] || y == fa) continue;
		f[y] = id1, g[y] = id2+d[y];
		id1 += d[y], id2 += d[y]*2;
		dfs2(y,x);
		rep(j,0,d[y]-1){
			ans += g[x][j+1]*f[y][j];
			if(j >= 1) ans += f[x][j-1]*g[y][j];
		}
		rep(j,0,d[y]-1){
			g[x][j+1] += f[x][j+1]*f[y][j]; 
			f[x][j+1] += f[y][j];
			if(j >= 1) g[x][j-1] += g[y][j];
		}
	}
}

int main()
{
	scanf("%d",&n); tot = 1;
	rep(i,1,n-1){
		int a,b; scanf("%d%d",&a,&b);
		add(a,b); add(b,a);
	}
	dfs1(1,0);
	f[1] = id1, g[1] = id2+d[1]; id1 += d[1], id2 += d[1]*2;
	dfs2(1,0);
	printf("%lld\n",ans);
	return 0;
}