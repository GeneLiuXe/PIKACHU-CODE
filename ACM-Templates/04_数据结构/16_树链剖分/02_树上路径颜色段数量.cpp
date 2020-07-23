// ---
// 题意：\\
// 一颗 $n$ 个节点的树，两个操作：\\
// $①$ 将 $a->b$ 路径上的点都染成颜色 $c$\\
// $②$ 查询 $a->b$ 路径上的颜色段数量\\
// \\
// 思路：\\
// 很明显是一个树剖问题，树剖的基础实现就不多说了，我们来考虑一下线段树需要维护什么。\\
// \\
// 首先求的是路径上不同颜色段数量，因此肯定需要维护一个 $cnt$，表示路径上不同颜色段的数量。然后我们来看如何实现区间合并，主要观察的就是左区间的右端点和右区间的左端点是否一样，如果一样，合并的 $cnt$ 需要减 $1$。但是如何去快速查询左右节点颜色呢，因此可以想到再维护区间左右节点颜色即可完成本题。\\
// \\
// 树剖成多个树链时需要注意链条交界处的颜色是否一致。\\
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
struct Node { int l,r,ls,rs,cnt,lazy,lc,rc;} t[2*N]; //cnt: 不同颜色个数, lc: 左端颜色, rc: 右端颜色
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
	t[x].lc = t[t[x].ls].lc, t[x].rc = t[t[x].rs].rc, t[x].cnt = t[t[x].ls].cnt + t[t[x].rs].cnt;
	if(t[t[x].ls].rc == t[t[x].rs].lc) t[x].cnt--;
}

void build(int l, int r, int x){ //基础建树，动态开点
	t[x].l = l, t[x].r = r, t[x].lazy = -1;
	if(l == r){
		t[x].lc = t[x].rc = val[rk[l]], t[x].cnt = 1; return;
	}
	int mid = (l+r)>>1;
	t[x].ls = ++tot, t[x].rs = ++tot;
	build(l,mid,t[x].ls), build(mid+1,r,t[x].rs), pushup(x);
}

inline void pushdown(int x){ //基础的线段树标记下放
	if(t[x].lazy!=-1 && t[x].l != t[x].r){
		int ls = t[x].ls, rs = t[x].rs, lz = t[x].lazy;
		t[ls].lazy = lz, t[rs].lazy = lz;
		t[ls].lc = t[ls].rc = lz, t[rs].lc = t[rs].rc = lz;
		t[ls].cnt = t[rs].cnt = 1;
		t[x].lazy = -1;
	}
}

void update(int l, int r, int x, int c){ //基础的线段树更新
	if(t[x].l >= l && t[x].r <= r){
		t[x].lazy = c, t[x].cnt = 1, t[x].lc = t[x].rc = c;
		return;
	}
	pushdown(x);
	int mid = (t[x].l+t[x].r)>>1;
	if(mid >= l) update(l,r,t[x].ls,c);
	if(mid < r) update(l,r,t[x].rs,c);
	pushup(x);
}

int query(int l, int r, int x){	 //基础的线段树查询
	// LOG2("l",l,"r",r);
	// LOG2("t[x].l",t[x].l,"t[x].r",t[x].r);
	if(t[x].l >= l && t[x].r <= r) return t[x].cnt;
	pushdown(x);
	int mid = (t[x].l+t[x].r)>>1, tp = 0;
	if(mid >= l && mid >= r) tp += query(l,r,t[x].ls);
	else if(mid < l && mid < r) tp += query(l,r,t[x].rs);
	else if(mid >= l && mid < r){
		tp += query(l,r,t[x].ls);
		tp += query(l,r,t[x].rs);
		if(t[t[x].ls].rc == t[t[x].rs].lc) tp--;
	}
	return tp;
}

int query_color(int pos, int x){  //查询单点颜色
	if(t[x].l == pos) return t[x].lc;
	if(t[x].r == pos) return t[x].rc;
	if(pos >= t[x].l && pos <= t[x].r && t[x].cnt == 1) return t[x].lc;
	int mid = (t[x].l+t[x].r)>>1;
	if(pos <= mid) return query_color(pos,t[x].ls);
	else return query_color(pos,t[x].rs);
}

inline int sum(int x, int y){	//将区间分为多条链，对于每条链直接查询
	int ret = 0;
	while(top[x] != top[y]){ //让x与y到达同一条链
		if(d[top[x]] < d[top[y]]) swap(x,y); //找到更深的点
		ret += query(id[top[x]],id[x],rt);
		// LOG1("ret",ret);
		if(query_color(id[top[x]],rt) == query_color(id[fa[top[x]]],rt)) ret--;
		x = fa[top[x]];
	}
	if(id[x] > id[y]) swap(x,y);
	return (ret+query(id[x],id[y],rt));
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
	scanf("%lld%lld",&n,&m);
	rep(i,1,n) scanf("%lld",&val[i]);
	init();
	rep(i,1,n-1){
		int x,y; scanf("%lld%lld",&x,&y);
		add(x,y), add(y,x);
	}
	scanf("%lld",&m);
	root = 1;
	tot = 0, dfs1(root), dfs2(root, root);
	tot = 0, build(1,n,rt = ++tot);
	rep(i,1,m){
		char op[10]; scanf("%s",op);
		int x,y,z;
		if(op[0] == 'Q'){
			scanf("%lld%lld",&x,&y);
			printf("%lld\n",sum(x,y));
		}
		else{
			scanf("%lld%lld%lld",&x,&y,&z);
			updates(x,y,z);
		}
	}
	return 0;
}