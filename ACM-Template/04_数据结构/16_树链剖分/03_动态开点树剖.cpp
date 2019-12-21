// ---
// 题意：\\
// 现在有 $n$ 个城市，构成了一颗树。每个城市都有自己信仰的宗教，以及城市评级。现在一共有四种操作：\\
// $①$ 某个城市改信 $c$ 教\\
// $②$ 某个城市的评级调整为 $w$\\
// $③$ $x->y$ 路径上所有与 $x$ 信仰相同的城市的评级之和\\
// $④$ $x->y$ 路径上所有与 $x$ 信仰相同的城市的评级最大值\\
// \\
// $(N,Q \leq 10^5，C \leq 10^5)$\\
// \\
// 思路：\\
// 需要维护的操作只是单点修改和区间最值与最大值，维护的操作都不难。主要困难的地方在于最值和最大值都只在信仰相同的城市之间统计，因此我们需要对每个信仰都建一颗线段树。\\
// \\
// 但是由于空间的限制，对每个宗教都建一颗完整的线段树是不可能的，因此我们需要动态开点的操作。采用动态开点的原因是本题初始最多只有 $10^5$ 个点，操作最多也只有 $10^5$ 次，因此有效的点最多只有 $2*10^5$ 个，所以我们只需要维护这些有效点即可。\\
// \\
// 我们再来仔细讲一下动态开点的原理。如下图所示，现在只有点 $A$ 是有效点，因此我们只需给 $root->A$ 路径上的点分配空间，不需要给其他的点分配空间，因此就达到了简化空间的目的。所以动态开一个点的空间费用最多是 $logn$，我们只需要给没颗树记一个根节点，以及每个节点对应的左右儿子编号即可。因此我们也不需要之前建树的 $build$ 函数了，用 $update$ 函数动态插入每个点即可。\\
// \\
// 回到这道题来，解决的思路就很简单了。给每个宗教建一颗线段树，维护每颗线段树的根节点编号，然后对于每个线段树进行查询和修改即可，一个点从信仰 $a$ 变为信仰 $b$，只需在 $a$ 线段树中将这个点赋为 $0$，然后在 $b$ 线段树将这个点再赋值即可。\\
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
const db EPS = 1e-9;
using namespace std;
const int N = 2*1e5+1000;

int ls[20*N], rs[20*N], val[20*N], maxn[20*N], sz, rt[N];
int n,m,head[N],tot,son[N],size[N],d[N],id[N],rk[N],fa[N],top[N],ans1,ans2;
int re[N],lev[N];
struct Edge{
	int to,next;
}e[N];

void init() { tot = 1, memset(head,0,sizeof head); }
void add(int x, int y) { e[++tot].to = y, e[tot].next = head[x], head[x] = tot; } 
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
void update(int& now, int l, int r, int x, int c){	//单点修改
	if(!now) now = ++sz;
	if(l == r){
		val[now] = c, maxn[now] = c;
		return;
	} 
	int mid = (l+r)>>1;
	if(x <= mid) update(ls[now],l,mid,x,c);
	if(x > mid) update(rs[now],mid+1,r,x,c);
	val[now] = val[ls[now]]+val[rs[now]];
	maxn[now] = max(maxn[ls[now]],maxn[rs[now]]);
}
void query(int now, int l, int r, int xx, int yy){
	if(l >= xx && r <= yy){
		ans1 += val[now], ans2 = max(ans2,maxn[now]);
		return;
	}
	int mid = (l+r)>>1;
	if(xx <= mid) query(ls[now],l,mid,xx,yy);
	if(yy > mid) query(rs[now],mid+1,r,xx,yy);
}
inline void updates(int x, int y){	//区间加z, 将区间分为多条链
	int tp = re[x];
	while(top[x] != top[y]){
		if(d[top[x]] < d[top[y]]) swap(x,y);
		query(rt[tp],1,n,id[top[x]],id[x]);	//对于每条链直接修改
		x = fa[top[x]];
	}
	if(id[x] > id[y]) swap(x,y);
	query(rt[tp],1,n,id[x],id[y]);
}
int main()
{
	scanf("%d%d",&n,&m);
	rep(i,1,n) scanf("%d%d",&lev[i],&re[i]);
	init();
	rep(i,1,n-1){
		int xx,yy; scanf("%d%d",&xx,&yy);
		add(xx,yy), add(yy,xx);
	}
	tot = sz = 0, dfs1(1), dfs2(1,1);
	rep(i,1,n) update(rt[re[i]],1,n,id[i],lev[i]);
	rep(i,1,m){
		char s[10]; int x,y;
		scanf("%s",s);
		if(s[1] == 'C'){
			scanf("%d%d",&x,&y);
			update(rt[re[x]],1,n,id[x],0);
			update(rt[y],1,n,id[x],lev[x]);
			re[x] = y;
		}
		else if(s[1] == 'W'){
			scanf("%d%d",&x,&y);
			update(rt[re[x]],1,n,id[x],y);
			lev[x] = y;
		}
		else{
			scanf("%d%d",&x,&y);
			ans1 = ans2 = 0;
			updates(x,y);
			if(s[1] == 'S') printf("%d\n",ans1);
			else printf("%d\n",ans2);
		}
	}
	return 0;
}