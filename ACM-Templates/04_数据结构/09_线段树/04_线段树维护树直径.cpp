// ---
// 题意：$n$ 个点的一棵树，每个点初始为 $0$，支持两种操作，第一种操作 $C \ x$，表示将第 $x$ 个点取反，即 $1$ 变 $0$，$0$ 变 $1$。第二种操作为 $G$，表示查询两个相距最远的 $0$ 点距离。$(1\leq n\leq 10^5,1\leq m\leq 5*10^5)$\\
// \\
// 思路：这道题的做法有括号序列、动态点分治、线段树维护直径。此处只介绍线段树维护直径的做法。\\
// \\
// 首先我们求一个 $dfs$ 序，然后在 $dfs$ 序上建线段树，对于线段树的每个区间，我们维护两个点表示在这个区间中相距最远的两个 $0$ 点。区间合并时我们只需要取出这两个区间所维护的点，然后对这四个点两两求距离更新答案即可。\\
// \\
// 这样做的原因在于对于两个子树来说，每个子树都有一条属于该子树的直径，则两个子树合并后的新直径必定是从原来两个子树中的 $4$ 个点中选取 $2$ 个点作为答案。\\
// \\
// 大致思路就是这样，具体细节见代码。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
const int N = 1e5+100;
using namespace std;

int n,m,dis[N],tot,head[N],f[N][25],dfn[N],rk[N],flag[N],T;
pair<int,int> sgt[4*N];
struct Edge{
	int to,next;
}e[2*N];

inline void add(int x,int y){
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot;
}

void dfs(int x,int fa){
	dis[x] = dis[fa]+1; f[x][0] = fa; dfn[x] = ++tot; rk[tot] = x;
	for(int i = 1; (1<<i) <= dis[x]; i++)
		f[x][i] = f[f[x][i-1]][i-1];
	for(int i = head[x]; i; i = e[i].next){
		int y = e[i].to;
		if(y == fa) continue;
		dis[y] = dis[x]+1; dfs(y,x);
	}
}

inline int lca(int x,int y){
	if(dis[x] > dis[y]) swap(x,y);
	for(int i = T; i >= 0; i--)
		if(dis[f[y][i]] >= dis[x]) y = f[y][i];
	if(x == y) return x;
	for(int i = T; i >= 0; i--)
		if(f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];
	return f[x][0];
}

inline int dist(int x,int y){
	if(x == 0 && y == 0) return -1;
	if(x == 0 || y == 0) return 0;
	return dis[x]+dis[y]-2*dis[lca(x,y)];
}

inline pair<int,int> pushUp(pair<int,int> x,pair<int,int> y){
	int a[5],res = 0,cnt = 0,tmp; pair<int,int> base = make_pair(0,0); a[0] = 0;
	if(x.first && flag[x.first]) a[++cnt] = x.first;
	if(x.second && flag[x.second]) a[++cnt] = x.second;
	if(y.first && flag[y.first]) a[++cnt] = y.first;
	if(y.second && flag[y.second]) a[++cnt] = y.second;
	rep(i,1,cnt-1)
		rep(j,i+1,cnt)
			if((tmp=dist(a[i],a[j])) > res)
				res = tmp, base = make_pair(a[i],a[j]);
	if(res == 0) base = make_pair(a[cnt],a[cnt]);
	return base;
}

inline void build(int now,int l,int r){
	if(l == r) sgt[now] = make_pair(rk[l],rk[l]), flag[rk[l]] = 1;
	else{
		int mid = (l+r)>>1;
		build(now<<1,l,mid); build(now<<1|1,mid+1,r);
		sgt[now] = pushUp(sgt[now<<1],sgt[now<<1|1]);
	}
}

inline void update(int now,int l,int r,int pos){
	if(l == r){
		flag[rk[l]] ^= 1;
		if(flag[rk[l]]) sgt[now] = make_pair(rk[l],rk[l]);
		else sgt[now] = make_pair(0,0);
		return;
	}
	int mid = (l+r)>>1;
	if(pos <= mid) update(now<<1,l,mid,pos);
	else update(now<<1|1,mid+1,r,pos);
	sgt[now] = pushUp(sgt[now<<1],sgt[now<<1|1]);
}

int main()
{
	scanf("%d",&n); 
	tot = 1; T = (int)(log(n)/log(2))+1;
	rep(i,1,n-1){
		int a,b; scanf("%d%d",&a,&b);
		add(a,b); add(b,a);
	}
	tot = 0; dfs(1,0); build(1,1,n);
	scanf("%d",&m);
	while(m--){
		char op[10]; scanf("%s",op);
		if(op[0] == 'C'){
			int x; scanf("%d",&x); 
			update(1,1,n,dfn[x]);
		}
		else printf("%d\n",dist(sgt[1].first,sgt[1].second));
	}
	return 0;
}