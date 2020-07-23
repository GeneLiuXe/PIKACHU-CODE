// ---
// 题意：\\
// $n$ 个点的树，每个顶点都有一个值，需要进行三种操作：\\
// $①$ 将树根修改为 $x$\\
// $②$ 将 $x->y$ 路径上所有点的值修改为 $v$\\
// $③$ 询问在当前树根状态下，以节点 $x$ 为根的子树中的最小值\\
// 需要注意，树根修改了之后，节点 $x$ 对应的子树也就变化了。\\
// \\
// 思路:\\
// 我们来分类讨论，枚举一下情况。我们需要求节点 $x$ 在当前树根下的子树中的最小值。\\
// $①$ 如果当前树根就是 $x$，那很明显答案就是整棵树的节点最小值。\\
// $②$ 如果当前树根在 $x$ 的子树中，比如 $A1$，那么我们可以发现答案就是去除 $A1$ 在 $x$ 中的这一段子树部分，剩余部分中找最小值即可。\\
// $③$ 如果当前树根在 $x$ 子树之外，比如 $A2$，那么答案就是在 $x$ 原来对应的子树中找一个最小值即可。\\
// \\
// 所以这题就变成了对于 $x$ 与当前树根求一个 $LCA$ 的问题，然后只有在第二种情况中才需要进行特殊处理，即在 $x$ 的子树找到对应 $A1$ 的那一个儿子即可。当然在树剖中，利用 $top$ 数组就可以完成 $LCA$ 能够完成的功能，见如下代码。\\
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
struct Node { int l,r,ls,rs,lazy,minn;} t[2*N];
int n,m,root,rt,mod,val[N],head[N],tot,fa[N],d[N],son[N],size[N],top[N],id[N],rk[N];
//top[x]: x节点所在链的顶端节点, id[x]: 节点dfs序, rk[x]: dfs序对应的节点
//val[x]: 每个点初始权值, fa[x]: 每个点父节点, d[x]: 节点深度, size[x]: 节点子树大小
//rt: 线段树根节点编号
void init() { memset(head,0,sizeof head); tot = 1, size[0] = 0; }
void add(int x, int y) { e[++tot].next = head[x], e[tot].to = y, head[x] = tot; }
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
inline void pushup(int x) { t[x].minn = min(t[t[x].ls].minn,t[t[x].rs].minn); }
void build(int l, int r, int x){ //基础建树，动态开点
	t[x].l = l, t[x].r = r, t[x].lazy = 0;
	if(l == r){
		t[x].minn = val[rk[l]]; return;
	}
	int mid = (l+r)>>1;
	t[x].ls = ++tot, t[x].rs = ++tot;
	build(l,mid,t[x].ls), build(mid+1,r,t[x].rs), pushup(x);
}
inline int len(int x) { return t[x].r-t[x].l+1; }
inline void pushdown(int x){ //基础的线段树标记下放
	if(t[x].lazy && t[x].l != t[x].r){
		int ls = t[x].ls, rs = t[x].rs, lz = t[x].lazy;
		(t[ls].lazy=lz), (t[rs].lazy=lz);
		t[ls].minn = lz, t[rs].minn = lz;
		t[x].lazy = 0;
	}
}
void update(int l, int r, int x, int c){ //基础的线段树更新
	if(t[x].l >= l && t[x].r <= r){
		t[x].lazy = c, t[x].minn = c;
		return;
	}
	pushdown(x);
	int mid = (t[x].l+t[x].r)>>1;
	if(mid >= l) update(l,r,t[x].ls,c);
	if(mid < r) update(l,r,t[x].rs,c);
	pushup(x);
}
int query(int l, int r, int x){	 //基础的线段树查询
	if(t[x].l >= l && t[x].r <= r) return t[x].minn;
	pushdown(x);
	int mid = (t[x].l+t[x].r)>>1, tp = 1e15;
	if(mid >= l) tp = min(tp,query(l,r,t[x].ls));
	if(mid < r) tp = min(tp,query(l,r,t[x].rs));
	return tp;
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
int find(int x, int y){	// top数组求LCA
	int base = x;
	while(top[x] != top[y]){
		if(fa[top[y]] == base) return top[y]; //返回子树中存在当前树根的子儿子
		if(d[top[x]] < d[top[y]]) y = fa[top[y]];
		else x = fa[top[x]];
	}
	if(d[x] > d[y]) x = y;
	if(x == base) return son[x]; //返回子树中存在当前树根的子儿子
	else return 0; //当前树根在x子树之外
}
signed main()
{
	scanf("%lld%lld",&n,&m);
	init();
	rep(i,1,n-1){
		int x,y; scanf("%lld%lld",&x,&y);
		add(x,y), add(y,x);
	}
	rep(i,1,n) scanf("%lld",&val[i]);
	scanf("%lld",&root);
	tot = 0, dfs1(root), dfs2(root, root);
	tot = 0, build(1,n,rt = ++tot);
	rep(i,1,m){
		int op,x,y,z;
		scanf("%lld",&op);
		if(op == 1){
			scanf("%lld",&x);
			root = x;
		}
		else if(op == 2){
			scanf("%lld%lld%lld",&x,&y,&z);
			updates(x,y,z);
		}
		else{
			scanf("%lld",&x); int thp;
			if(root == x){
				printf("%lld\n",query(1,n,rt));
			}
			else if((thp = find(x,root))){
				printf("%lld\n",min(query(1,id[thp]-1,rt),query(id[thp]+size[thp],n,rt)));
			}
			else printf("%lld\n",query(id[x],id[x]+size[x]-1,rt));
		}
	}
	return 0;
}