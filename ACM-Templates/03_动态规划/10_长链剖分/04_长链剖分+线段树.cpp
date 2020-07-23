// ---
// 题意：\\
// $n$ 个点的一棵树，树上每条边都有一个权值。现要在树中找到一条长度在 $[L,R]$ 范围内的一条路径，使得 $AvgValue=\frac{\sum\limits_{e\in S}v(e)}{|S|}$ 最大，即道路平均价值最大。$(1\leq n\leq 10^5,1\leq L\leq R\leq n-1,v_i\leq 10^6)$\\
// \\
// 思路：\\
// 看完题干中的道路平均价值 $AvgValue=\frac{\sum\limits_{e\in S}v(e)}{|S|}$ 最大，应该就能反应过来这是一道 $01$ 分数规划问题。而 $01$ 分数规划问题，解法比较固定，即二分答案，然后令 $Sum=\sum\limits_{e\in S}(v(e)-ans)$，其中 $ans$ 为你二分的答案。因此将图中所有边的边权减去 $ans$，然后在图中找一条长度在 $[L,R]$ 范围内的路径，使得 $Sum$ 最大。若 $Sum\geq 0$，则二分右半部分，否则二分左半部分。\\
// \\
// 因此我们的问题就变成了在树中找一条长度在 $[L,R]$ 范围内的路径，使得路径权值最大。\\
// \\
// 涉及到了路径长度，因此我们考虑树形 $dp$，定义 $dp[u][j]$ 表示从节点 $u$ 向其子树出发，路径长度为 $j$ 时的最大路径权值和。
// $$
// dp[u][j+1]=max(dp[u][j+1],dp[v][j]+e[i].w)
// $$
// \\
// $$
// ans=max(ans,dp[v][j]+dp[u][x]+e[i].w)
// $$
// \\
// $$
// L\leq1+j+x\leq R
// $$
// \\
// 从上述的 $dp$ 方程中，我们可以发现我们需要在 $dp[u]$ 的一个范围内寻找最大值，由此想到了利用线段树来寻找最值。\\
// \\
// 因此我们在原先的长链剖分数组上维护一个线段树，每个节点保存该节点到根的距离，由此 $dp[u][x]=dis[u][x]-dis[u]$，就可以避免继承重儿子信息时的区间修改等操作。如若定义 $dp[u][x]$ 为 $u$ 到深度为 $x$ 的节点的距离，则在继承重儿子信息时，无可避免地需要用到区间修改。而定义 $dp[u][x]$ 为根到 $u$ 深度为 $x$ 的节点的距离的话，即可避免修改问题。\\
// ---
#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 1e5+100;
const db EPS = 1e-5;
const db inf = 1e14;
using namespace std;

int n,L,R,head[N],tot,d[N],son[N];
db *f[N],tmp[N],*id = tmp, ans, maxn[2*N], dis[N];
struct Node{
	int to,next;
	ll wt;
	db w;
}e[2*N];
inline int get_id(int l,int r) {return (l+r)|(l!=r);}

void add(int x,int y,ll w){
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot, e[tot].wt = w;
}

void build(int l,int r){
	maxn[get_id(l,r)] = -inf;
	if(l == r) return;
	int mid = (l+r)>>1;
	build(l,mid); build(mid+1,r);
}

void insert(int l,int r,int pos,db v){
	int now = get_id(l,r);
	if(l == r) {maxn[now] = max(maxn[now],v); return;}
	int mid = (l+r)>>1;
	if(pos <= mid) insert(l,mid,pos,v);
	else insert(mid+1,r,pos,v);
	maxn[now] = max(maxn[get_id(l,mid)],maxn[get_id(mid+1,r)]);
}

db query(int l,int r,int ls,int rs){
	if(rs < ls) return -inf;
	int now = get_id(l,r);
	if(ls <= l && r <= rs) return maxn[now];
	int mid = (l+r)>>1;
	db tmp = -inf;
	if(ls <= mid) tmp = max(tmp,query(l,mid,ls,rs));
	if(rs > mid) tmp = max(tmp,query(mid+1,r,ls,rs));
	return tmp;
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
	int pos = f[x]-tmp;
	f[x][0] = dis[x];
	insert(1,n,pos+1,f[x][0]);
	for(int i = head[x]; i; i = e[i].next){
		if(son[x] == e[i].to){
			f[son[x]] = f[x]+1;
			dis[son[x]] = dis[x]+e[i].w;
			dfs2(son[x],x);
		}
	}
	for(int i = head[x]; i; i = e[i].next){
		int y = e[i].to;
		if(y == fa || y == son[x]) continue;
		f[y] = id; id += d[y];
		dis[y] = dis[x]+e[i].w;
		dfs2(y,x);
		rep(j,0,d[y]-1){ //找最大值
			if(j+1 <= R){
				db base = query(1,n,max(1,pos+L-j),min(pos+d[x],pos+R-j))-2.0*dis[x];
				ans = max(ans,base+f[y][j]);
			}
		}
		rep(j,0,d[y]-1){ //更新最值
			if(f[y][j] > f[x][j+1]){
				f[x][j+1] = f[y][j];
				insert(1,n,pos+j+2,f[x][j+1]);
			}
		}
	}
	db base = query(1,n,pos+L+1,min(pos+d[x],pos+R+1))-dis[x];
	ans = max(ans,base);
}

int check(db base){
	ans = -inf;
	rep(i,2,tot) e[i].w = (db)e[i].wt-base;
	build(1,n);
	rep(i,0,n) tmp[i] = 0, dis[i] = 0;
	id = tmp; f[1] = id; id += d[1];
	dfs2(1,0);
	if(ans-0 >= EPS) return 1;
	else return 0;
}

int main()
{
	scanf("%d",&n); tot = 1;
	scanf("%d%d",&L,&R);
	rep(i,1,n-1){
		int x,y; ll w; scanf("%d%d%lld",&x,&y,&w);
		add(x,y,w); add(y,x,w);
	}
	dfs1(1,0);
	db l = 1, r = 1000000;
	while(r-l >= EPS){
		db mid = (l+r)/2.0;
		if(check(mid)) l = mid;
		else r = mid;
	}
	printf("%.3f\n",r);
	return 0;
}