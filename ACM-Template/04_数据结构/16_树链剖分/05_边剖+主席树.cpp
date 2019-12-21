// ---
// 题意：\\
// 给定一棵$n$个节点的树，每条边都有一个权值，$m$次查询，每次询问树上两点路径上边权小于$k$的边有多少条？$(1\leq n,m\leq 10^5)$\\
// \\
// 思路：\\
// 比较裸的题目，可以离线操作，然后将询问值从小到大进行排序，然后每次单点修改，将比当前询问小的边加入树中。\\
// \\
// 此处需要注意是边权树剖，因此将每条边的权值压到深度更深的节点上，然后树剖路径查询时，最后两点处于同一条链时，将深度最浅的点改为该点的儿子即可。\\
// \\
// 比赛时没有考虑到将询问值排序，然后依次修改边权，因此采用了树上主席树的做法。先将所有的边都插入树中，然后直接查询区间中小于k的节点有多少个即可。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
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

struct Edge { int next,to,w;} e[2*N];
struct Node { int l,r,ls,rs,sum;} t[20*N];
int n,m,rt,mod,val[N],head[N],tot,fa[N],d[N],son[N],size[N],top[N],id[N],rk[N],root[N],rot;
//top[x]: x节点所在链的顶端节点, id[x]: 节点dfs序, rk[x]: dfs序对应的节点
//val[x]: 每个点初始权值, fa[x]: 每个点父节点, d[x]: 节点深度, size[x]: 节点子树大小
//rt: 线段树根节点编号
void init(){
	memset(head,0,sizeof head);
	tot = 1, size[0] = 0;
}

void add(int x, int y, int z){
	e[++tot].next = head[x], e[tot].to = y, head[x] = tot, e[tot].w = z;
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
	t[x].sum = t[t[x].ls].sum+t[t[x].rs].sum;
}

int build(int l,int r) //主席树建树部分
{
	int p = ++tot; // 新建一个节点，编号为p，代表当前区间[l,r]
	t[p].l = l, t[p].r = r, t[p].sum = 0;
	if(l == r) return p;
	int mid = (l+r)>>1;
	t[p].ls = build(l,mid);
	t[p].rs = build(mid+1,r);
	return p;
}

inline int len(int x) { return t[x].r-t[x].l+1; }

int ask(int lp,int rp,int k){ //主席树查询[lp,rp]比k小的有多少个
	if(t[lp].l == t[lp].r) return (t[rp].sum-t[lp].sum);
	int mid = (t[lp].l+t[lp].r)>>1;
	int tp = 0;
	if(mid < k){
		tp += (t[t[rp].ls].sum-t[t[lp].ls].sum);
		tp += ask(t[lp].rs,t[rp].rs,k);
	} 
	else tp += ask(t[lp].ls,t[rp].ls,k);
	return tp;
}

inline int solve(int x,int y,int k){
	int ret = 0;
	while(top[x] != top[y]){ //让x与y到达同一条链
		if(d[top[x]] < d[top[y]]) swap(x,y); //找到更深的点
		int xx = id[top[x]]-1, yy = id[x];
		int hp = ask(root[xx],root[yy],k);
		ret += hp;
		x = fa[top[x]];
	}
	if(x == y) return ret;
	if(id[x] > id[y]) swap(x,y);
	int xx = id[son[x]]-1, yy = id[y]; //边权树链剖分, 最后两点同链之后，取最高点的儿子来计算
	//-1是因为主席树要获取-1时候的副本进行sum减运算
	int hp = ask(root[xx],root[yy],k);
	return (ret+hp);
}

int insert(int now,int pos,int k) //主席树插入一个新的值
{
	int p = ++tot;
	t[p] = t[now]; //建立副本
	if(t[p].l == t[p].r){
		t[p].sum += k; //在副本上修改
		return p;
	}
	int mid = (t[p].l+t[p].r)>>1;
	if(pos <= mid) t[p].ls = insert(t[p].ls,pos,k); //保留右儿子部分，把左儿子更新
	else t[p].rs = insert(t[p].rs,pos,k);
	t[p].sum = t[t[p].ls].sum + t[t[p].rs].sum;
	return p;
}

int ed[2*N][3];
int b[2*N],tb;

signed main()
{
	scanf("%d%d",&n,&m);
	init();
	rep(i,1,n-1){
		int x,y,z; scanf("%d %d %d",&x,&y,&z);
		add(x,y,z), add(y,x,z);
		ed[i][0] = x, ed[i][1] = y, ed[i][2] = z;
	}
	rot = 1;
	tot = 0, dfs1(rot), dfs2(rot, rot);
	rep(i,1,n-1){
		if(d[ed[i][0]] > d[ed[i][1]]) swap(ed[i][0],ed[i][1]);
		val[ed[i][1]] = ed[i][2];
		b[++tb] = ed[i][2];
	}	
	sort(b+1,b+1+tb);
	tb = unique(b+1,b+1+tb)-b-1;
	tot = 0;
	root[0] = build(1,tb);
	rep(i,1,n){
		int x = lower_bound(b+1,b+1+tb,val[rk[i]])-b;
		root[i] = insert(root[i-1],x,1);
	}
	rep(i,1,m){
		int u,v,k; scanf("%d%d%d",&u,&v,&k);
		int pos = upper_bound(b+1,b+1+tb,k)-b;
		pos--;
		if(pos == 0) printf("0\n");
		else printf("%d\n",solve(u,v,pos));
	}
	return 0;
}