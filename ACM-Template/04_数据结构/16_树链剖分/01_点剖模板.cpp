// ---
// 一颗 $N$ 个节点的树，每个节点上都有初始权值。现在有四种操作：
// 操作 $1$ —— $1\ x\ y\ z$，表示将 $x$ 到 $y$ 节点最短路径上所有节点的值加z
// 操作 $2$ —— $2\ x\ y$，表示求 $x$ 到 $y$ 节点最短路径上所有节点值之和
// 操作 $3$ —— $3\ x\  z$，表示将以 $x$ 为根节点的子树内所有节点值加 $z$ 
// 操作 $4$ —— $4\ x$，表示求以 $x$ 为根节点的子树内所有节点值之和
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
typedef long long ll;
typedef double db;
#define int long long
const db EPS = 1e-9;
const int N = 1e5+100;
using namespace std;

struct Edge { int next,to;} e[2*N];
struct Node { int l,r,ls,rs,sum,lazy;} t[2*N];
int n,m,root,rt,mod,val[N],head[N],tot,fa[N],d[N],son[N],size[N],top[N],id[N],rk[N];
//top[x]: x节点所在链的顶端节点, id[x]: 节点dfs序, rk[x]: dfs序对应的节点
//val[x]: 每个点初始权值, fa[x]: 每个点父节点, d[x]: 节点深度, size[x]: 节点子树大小
//rt: 线段树根节点编号
void init(){
	memset(head,0,sizeof head);
	tot = 1, size[0] = 0;
}

void add(int x, int y){
	e[++tot].next = head[x], e[tot].to = y, head[x] = tot;
}

void dfs1(int x){	//求出每个点的子树大小、深度、重儿子
	size[x] = 1, d[x] = d[fa[x]]+1, son[x] = 0;
	for(int v,i = head[x]; i; i = e[i].next)
		if((v = e[i].to)!=fa[x]){
			fa[v] = x, dfs1(v), size[x] += size[v];
			if(size[son[x]] < size[v])
				son[x] = v;
		}
}

void dfs2(int x, int tp){	//求出每个节点的dfs序, dfs序对应的节点, 以及每个点所在链的顶端节点
	top[x] = tp, id[x] = ++tot, rk[tot] = x;
	if(son[x]) dfs2(son[x],tp);
	for(int v,i = head[x]; i; i = e[i].next)
		if((v = e[i].to)!=fa[x] && v!=son[x]) dfs2(v,v);
}

inline void pushup(int x){ //基础的线段树向上区间合并
	t[x].sum = (t[t[x].ls].sum+t[t[x].rs].sum)%mod;	 //此题需要将sum和对mod取模
}

void build(int l, int r, int x){ //基础建树，动态开点
	t[x].l = l, t[x].r = r, t[x].lazy = 0;
	if(l == r){
		t[x].sum = val[rk[l]]; return;
	}
	int mid = (l+r)>>1;
	t[x].ls = ++tot, t[x].rs = ++tot;
	build(l,mid,t[x].ls), build(mid+1,r,t[x].rs), pushup(x);
}

inline int len(int x) { return t[x].r-t[x].l+1; }

inline void pushdown(int x){ //基础的线段树标记下放
	if(t[x].lazy && t[x].l != t[x].r){
		int ls = t[x].ls, rs = t[x].rs, lz = t[x].lazy;
		(t[ls].lazy+=lz) %= mod, (t[rs].lazy+=lz) %= mod;
		(t[ls].sum+=lz*len(ls)) %= mod, (t[rs].sum+=lz*len(rs)) %= mod;
		t[x].lazy = 0;
	}
}

void update(int l, int r, int x, int c){ //基础的线段树更新
	if(t[x].l >= l && t[x].r <= r){
		(t[x].lazy += c) %= mod, (t[x].sum += len(x)*c) %= mod;
		return;
	}
	pushdown(x);
	int mid = (t[x].l+t[x].r)>>1;
	if(mid >= l) update(l,r,t[x].ls,c);
	if(mid < r) update(l,r,t[x].rs,c);
	pushup(x);
}

int query(int l, int r, int x){	 //基础的线段树查询
	if(t[x].l >= l && t[x].r <= r) return t[x].sum;
	pushdown(x);
	int mid = (t[x].l+t[x].r)>>1, tp = 0;
	if(mid >= l) tp += query(l,r,t[x].ls);
	if(mid < r) tp += query(l,r,t[x].rs);
	return tp%mod;
}

inline int sum(int x, int y){	//将区间分为多条链，对于每条链直接查询
	int ret = 0;
	while(top[x] != top[y]){ //让x与y到达同一条链
		if(d[top[x]] < d[top[y]]) swap(x,y); //找到更深的点
		(ret += query(id[top[x]],id[x],rt)) %= mod;
		x = fa[top[x]];
	}
	if(id[x] > id[y]) swap(x,y);
	return (ret+query(id[x],id[y],rt))%mod;
}

inline void updates(int x, int y, int c){	//区间加z, 将区间分为多条链
	while(top[x] != top[y]){
		if(d[top[x]] < d[top[y]]) swap(x,y);
		update(id[top[x]],id[x],rt,c);	//对于每条链直接修改
		x = fa[top[x]];
	}
	if(id[x] > id[y]) swap(x,y);
	update(id[x],id[y],rt,c);
}

signed main()
{
	scanf("%lld%lld%lld%lld",&n,&m,&root,&mod);
	rep(i,1,n) scanf("%lld",&val[i]);
	init();
	rep(i,1,n-1){
		int x,y; scanf("%lld%lld",&x,&y);
		add(x,y), add(y,x);
	}
	tot = 0, dfs1(root), dfs2(root, root);
	tot = 0, build(1,n,rt = ++tot);
	rep(i,1,m){
		int op,x,y,k; scanf("%lld",&op);
		if(op == 1){
			scanf("%lld%lld%lld",&x,&y,&k);
			updates(x,y,k);
		}
		else if(op == 2){
			scanf("%lld%lld",&x,&y);
			printf("%lld\n",sum(x,y));
		}
		else if(op == 3){
			scanf("%lld%lld",&x,&y);
			update(id[x],id[x]+size[x]-1,rt,y);
		}
		else{
			scanf("%lld",&x);
			printf("%lld\n",query(id[x],id[x]+size[x]-1,rt));
		}
	}
	return 0;
}
