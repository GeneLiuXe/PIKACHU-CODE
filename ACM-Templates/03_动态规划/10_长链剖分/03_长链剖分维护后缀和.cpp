// ---
// 题意：\\
// $n$ 个点的一棵树，有 $q$ 组询问，每组询问给出一个 $p$、$k$，询问有多少个有序三元组 $(p,b,c)$ 满足要求。其中 $p$、$b$、$c$ 分别为树上三个不同的点，其中 $p$ 和 $b$ 都是 $c$ 的祖先，且 $p$ 与 $b$ 之间的距离小于等于 $k$。$(1\leq n\leq 3*10^5,1\leq q\leq 3*10^5,1\leq k\leq n)$\\
// \\
// 思路：\\
// 首先我们来分析一下这个三元组的性质。$(p,b,c)$，$p$ 和 $b$ 是 $c$ 的祖先，$p$ 与 $b$ 之间距离小于等于 $k$。因此 $b$ 可能是 $p$ 的儿子，也可能是 $p$ 的祖先。当 $b$ 是 $p$ 的祖先时，$ans=ans+min(dep[p]-1,k)*(sz[p]-1)$；当 $b$ 是 $p$ 的儿子时，$ans=ans+sz[b]-1$。\\
// \\
// 因此我们可以发现，$b$ 是 $p$ 的祖先的情况好处理，因此我们只需要处理 $b$ 是 $p$ 的儿子时情况即可。而 $b$ 是 $p$ 的儿子时，对答案的贡献就是 $p$ 子树中所有节点 $sz[b]-1$ 的和。\\
// \\
// 因此我们定义 $dp[u][j]$ 表示节点 $u$ 中所有距 $u$ 距离大于等于 $j$ 的节点 $size-1$ 之和。
// $$
// dp[u][j+1]=dp[u][j+1]+dp[v][j] \\
// $$
// \\
// $$
// dp[u][0]=dp[u][1]+sz[u]-1
// $$
// 然后我们再将所有查询离线，接着进行长链剖分的常规操作，$O(1)$ 继承重儿子，暴力继承轻儿子即可完成此题。\\
// \\
// 这里需要注意，由于长链剖分的数组只能暂时保存每个节点状态的值，因此我们需要将所有查询离线进行预先处理。\\
// ---
#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 3*1e5+100;
const db EPS = 1e-9;
using namespace std;

int n,q,tot,head[N],son[N],d[N],dep[N];
ll *f[N],tmp[N],*id=tmp,sz[N],ans[N];
struct Node{
	int to,next;
}e[2*N];
vector<pair<int,int> > Q[N];

void add(int x,int y){
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot;
}

void dfs1(int x,int fa){
	sz[x] = 1; dep[x] = dep[fa]+1;
	for(int i = head[x]; i; i = e[i].next){
		int y = e[i].to;
		if(y == fa) continue;
		dfs1(y,x); sz[x] += sz[y];
		if(d[y] > d[son[x]]) son[x] = y;
	}
	d[x] = d[son[x]]+1;
}

void dfs2(int x,int fa){
	if(son[x]){
		f[son[x]] = f[x]+1;
		dfs2(son[x],x);
	}
	for(int i = head[x]; i; i = e[i].next){
		int y = e[i].to;
		if(y == son[x] || y == fa) continue;
		f[y] = id; id += d[y];
		dfs2(y,x);
		for(int j = 0; j < d[y]; j++)
			f[x][j+1] += f[y][j];
	}
	for(auto &v:Q[x]){
		int num = v.first, k = v.second;
		ans[num] = (sz[x]-1ll)*(ll)min(dep[x]-1,k);
		if(k >= d[x]-1) ans[num] += f[x][1]-f[x][d[x]-1];
		else ans[num] += (ll)f[x][1]-(ll)f[x][k+1];
	}
	f[x][0] = f[x][1]+sz[x]-1ll;
}

int main()
{
	scanf("%d%d",&n,&q); tot = 1;
	rep(i,1,n-1){
		int u,v; scanf("%d%d",&u,&v);
		add(u,v); add(v,u);
	}
	dfs1(1,0);
	f[1] = id; id += d[1];
	rep(i,1,q){
		int p,k; scanf("%d%d",&p,&k);
		Q[p].push_back(make_pair(i,k));
	}
	dfs2(1,0);
	rep(i,1,q) printf("%lld\n",ans[i]);
	return 0;
}