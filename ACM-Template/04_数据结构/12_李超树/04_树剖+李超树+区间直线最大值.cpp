// ---
// 题意：\\
// 一棵 $n$ 个点的树，两个操作。操作$1$，选择两个点 $s$ 和 $t$，对于该路径上的一个点 $r$，在该点上添加的数字是 $a*dis+b$，$dis$ 为点 $r$ 到点 $s$ 的距离。操作$2$，给出两个点 $s$ 和 $t$，在$s$到$t$路径上选择一个点，再在这个点上选择一个数字，要求选的数字最小。每个点都有一个初始数字，为 $123456789123456789$。$(n\leq 10^5,m\leq 10^5)$\\
// \\
// 思路：\\
// 树链剖分后维护李超树。第二种操作好处理，只需在线段树每个节点上维护一个最小值，每次插入直线都动态维护即可。第一种操作需要分类讨论，$lca=lca(s,t)$。对于 $s$ 到 $lca$ 上的节点 $x$，每次添加数字为 $a*(dis[s]-dis[x])+b$，因此 $dis[x]$ 为横坐标，$k$ 为 $-a$，$b$ 为 $a*dis[s]+b$。对于 $lca$ 到 $t$ 上的节点 $x$，每次添加数字为 $a*(dis[s]+dis[x]-2*dis[lca])+b$，因此 $dis[x]$ 为横坐标，$k$ 为 $a$，$b$ 为 $a*(dis[s]-2*dis[lca])+b$。\\
// \\
// 因此插入直线时对两种情况分类讨论插入直线即可完成。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
const int N = 1e5+100;
const ll inf = 123456789123456789ll;
using namespace std;

int n,m,tot,head[N],fa[N],d[N],son[N],siz[N],top[N],id[N],rk[N];
ll dis[N];
struct Edge{
	int to,next;
	ll w;
}e[2*N];
struct line { ll k,b; int l,r;};
struct Node{
	line t;
	ll mn;
}sgt[2*N];
inline ll calc(line t, ll pos) {return t.k*pos+t.b;}
inline int get_id(int l, int r) { return (l + r) | (l != r); } //将叶子结点赋成偶数, 其余节点赋成奇数, 编号最大为2n

void add(int x,int y,ll w){
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot, e[tot].w = w;
}

void dfs1(int x){
	siz[x] = 1, d[x] = d[fa[x]]+1, son[x] = 0;
	for(int v,i = head[x]; i; i = e[i].next){
		if((v = e[i].to) != fa[x]){
			dis[v] = dis[x]+e[i].w; fa[v] = x; dfs1(v); siz[x] += siz[v];
			if(siz[son[x]] < siz[v]) son[x] = v;
		} 
	}
}

void dfs2(int x,int tp){
	top[x] = tp; id[x] = ++tot; rk[tot] = x;
	if(son[x]) dfs2(son[x],tp);
	for(int v,i = head[x]; i; i = e[i].next)
		if((v = e[i].to) != fa[x] && v != son[x]) dfs2(v,v);
}

void build(int l,int r){
	int now = get_id(l,r);
	sgt[now].t = {0,inf,l,r}; sgt[now].mn = inf;
	if(l < r){
		int mid = (l+r)>>1;
		build(l,mid); build(mid+1,r);
	}
}

void insert(int l,int r,line k){
	int now = get_id(l,r);
	if(k.l <= l && r <= k.r){
		if(calc(k,dis[rk[l]]) <= calc(sgt[now].t,dis[rk[l]]) && calc(k,dis[rk[r]]) <= calc(sgt[now].t,dis[rk[r]])){
			sgt[now].t = k;
			sgt[now].mn = min(calc(k,dis[rk[l]]),calc(k,dis[rk[r]]));
			int mid = (l+r)>>1;
			if(l < r) sgt[now].mn = min(sgt[now].mn,min(sgt[get_id(l,mid)].mn,sgt[get_id(mid+1,r)].mn));
		}
		else if(calc(k,dis[rk[l]]) < calc(sgt[now].t,dis[rk[l]]) || calc(k,dis[rk[r]]) < calc(sgt[now].t,dis[rk[r]])){
			int mid = (l+r)>>1;
			if(calc(k,dis[rk[mid]]) < calc(sgt[now].t,dis[rk[mid]])) swap(k,sgt[now].t);
			if(calc(k,dis[rk[l]]) < calc(sgt[now].t,dis[rk[l]])) insert(l,mid,k);
			else insert(mid+1,r,k);
			sgt[now].mn = min(sgt[get_id(l,mid)].mn,sgt[get_id(mid+1,r)].mn);
			sgt[now].mn = min(sgt[now].mn,min(calc(sgt[now].t,dis[rk[l]]),calc(sgt[now].t,dis[rk[r]])));
		}
	}
	else{
		int mid = (l+r)>>1;
		if(k.l <= mid) insert(l,mid,k);
		if(mid < k.r) insert(mid+1,r,k);
		sgt[now].mn = min(sgt[get_id(l,mid)].mn,sgt[get_id(mid+1,r)].mn);
		sgt[now].mn = min(sgt[now].mn,min(calc(sgt[now].t,dis[rk[l]]),calc(sgt[now].t,dis[rk[r]])));
	}
}

int getlca(int x,int y){
	while(top[x] != top[y]){
		if(d[top[x]] < d[top[y]]) swap(x,y);
		x = fa[top[x]];
	}
	return (d[x]<d[y]?x:y);
}

void modify(int x,int y,ll a,ll b){
	int u = x, v = y, lca = getlca(x,y);
	while(d[top[x]] > d[lca]){
		line tmp = {-a,a*dis[u]+b,id[top[x]],id[x]};
		insert(1,n,tmp);
		x = fa[top[x]];
	}
	while(d[top[y]] > d[lca]){
		line tmp = {a,a*(dis[u]-2ll*dis[lca])+b,id[top[y]],id[y]};
		insert(1,n,tmp);
		y = fa[top[y]];
	}
	if(x == lca){
		line tmp = {a,a*(dis[u]-2ll*dis[lca])+b,id[x],id[y]};
		insert(1,n,tmp);
	}	
	else{
		line tmp = {-a,a*dis[u]+b,id[y],id[x]};
		insert(1,n,tmp);
	}
}

ll getmin(int l,int r,int x,int y){
	int now = get_id(l,r);
	if(x <= l && r <= y) return sgt[now].mn;
	else{
		int mid = (l+r)>>1;
		ll ans = min(calc(sgt[now].t,dis[rk[max(l,x)]]),calc(sgt[now].t,dis[rk[min(y,r)]]));
		if(x <= mid) ans = min(ans,getmin(l,mid,x,y));
		if(mid < y) ans = min(ans,getmin(mid+1,r,x,y));
		return ans;
	}
}

ll query(int u,int v){
	ll ans = inf;
	while(top[u] != top[v]){
		if(d[top[u]] < d[top[v]]) swap(u,v);
		ans = min(ans,getmin(1,n,id[top[u]],id[u]));
		u = fa[top[u]];
	}
	if(d[u] < d[v]) ans = min(ans,getmin(1,n,id[u],id[v]));
	else ans = min(ans,getmin(1,n,id[v],id[u]));
	return ans;
}

int main()
{
	tot = 1;
	scanf("%d%d",&n,&m);
	rep(i,1,n-1){
		int u,v; ll w; scanf("%d%d%lld",&u,&v,&w);
		add(u,v,w); add(v,u,w);
	}
	tot = 0; dfs1(1); dfs2(1,1);
	build(1,n);
	rep(i,1,m){
		int op; scanf("%d",&op);
		if(op == 1){
			int s,t; ll a,b; scanf("%d%d%lld%lld",&s,&t,&a,&b);
			modify(s,t,a,b);
		}
		else{
			int s,t; scanf("%d%d",&s,&t);
			printf("%lld\n",query(s,t));
		}
	}
	return 0;
}