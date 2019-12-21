// ---
// 树上带修莫队概述: \\
// 树上莫队问题仍然是通过分块进行解决，但是分块的序列发生了变化。这个序列需要满足，给出两点就能在序列上找出这两点之间的路径。\\
// \\
// 我们考虑常见的树上序列，$dfs$ 序，但是很明显 $dfs$ 序不满足这个条件，其中会有很多无效的节点。因此我们引出欧拉序来解决这个问题，欧拉序和 $dfs$ 序的区别是，$dfs$ 序只在遍历到这个节点时才会将这个节点加入序列，而欧拉序还会在回溯到这个节点时将节点加入序列。\\
// \\
// 因此在欧拉序中，每个点会有一个第一次到达的点和第二次到达的点，我们分别记为 $fir[i]$ 与 $las[i]$。对于树上两点 $x$、$y$（$fir[x]<fir[y]$），$u$ 为 $x$、$y$ 两点的 $lca$，若 $x=u$，则在 $[fir[x],fir[y]]$ 这段区间中，只有 $x$ 到 $y$ 路径上的点只出现一次。若 $x =\not u$，则在 $[las[x],fir[y]]$ 这段区间中只有 $x$ 到 $y$ 路径上的点只出现一次，而且不包含 $lca(x,y)$ 这个点。因此我们在树上莫队问题中，需要记录每个点出现的次数，第一次出现则加贡献，第二次出现则减贡献，且若 $x=\not u$，还需加上 $lca$ 的贡献。\\
// \\
// 解决完树上莫队的序列问题，就可以转化成普通莫队进行计算了。不带修改则块大小为 $\sqrt {2n}$，带修改则块大小为 $(2n)^{\frac{2}{3}}$，其中 $2n$ 为欧拉序长度。\\
// \\
// 题意: \\
// $n$ 个点的一棵树，每个点上都有一个糖果，糖果的种类不同，第 $i$ 类糖果的贡献为 $V[i]$，第 $j$ 次吃第 $i$ 类糖果对答案的贡献为 $V[i]*W[j]$。现有 $q$ 次操作，每次可以将第 $x$ 个点上的糖果类型改为 $y$，也可以查询从 $x$ 点到 $y$ 点的答案。$(1\leq n,q\leq 10^5)$ \\
// \\
// 思路: \\
// 莫队问题只需要关注加入节点和删除节点对答案的影响，因此只需要统计每一类糖果在路径中出现的次数即可完成节点增删时对答案的影响。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 1e5+100;
const int M = 1e6+100;
const db EPS = 1e-9;
using namespace std;

int n,m,k,V[N],W[N],head[N],tot,C[N],qnum,cnum,f[N][25],t,d[N],Euler[2*N],ncnt,fir[N],las[N],pos[2*N],L,R,T,flag[N],vis[N];
//pos-分块位置、fir-欧拉序第一次、las-欧拉序第二次、Euler-欧拉序数组、ncnt-欧拉序数组长度
//vis-这个树上节点出现了几次, flag-这个糖果种类
ll ans[N],now;
struct Edge{
	int to,next;
}e[2*N];
struct Query{
	int l,r,id,lca,t; //l、r、id-查询顺序、lca-两点lca、t-之前有几次修改
	bool operator < (Query xx) const {
		if(pos[l] != pos[xx.l]) return pos[l] < pos[xx.l];
		else if(pos[r] != pos[xx.r]) return pos[r] < pos[xx.r];
		else return t < xx.t;
	}
}q[N];
struct Change{
	int pos, val;
}ch[N];

void add(int x,int y){
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot;
}

void dfs(int u,int fa)
{
	Euler[++ncnt] = u; fir[u] = ncnt;
    d[u]=d[fa]+1; f[u][0]=fa;
    for(int i=1;(1<<i)<=d[u];i++)
        f[u][i]=f[f[u][i-1]][i-1];
    for(int i=head[u]; i; i=e[i].next){
        int v=e[i].to;
        if(v!=fa) dfs(v,u);
    }
    Euler[++ncnt] = u; las[u] = ncnt;
}    

int LCA(int x,int y)
{
	if(d[x] > d[y]) swap(x,y);
	for(int i = t; i >= 0; i--)
		if(d[f[y][i]] >= d[x]) y = f[y][i];  //往上追溯，直至y和x位于同一深度
	if(x == y) return x;  //如果已经找到了，就返回x
	for(int i = t; i >= 0; i--)
		if(f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];  //x和y同时往上走，一直到x和y恰好为lca的子节点
	return f[x][0];  //x和y共同的根节点就是lca 
}

void Add(int pos){
	flag[C[pos]]++;
	now += (ll)W[flag[C[pos]]]*(ll)V[C[pos]];
}

void Del(int pos){
	now -= (ll)W[flag[C[pos]]]*(ll)V[C[pos]];
	flag[C[pos]]--;
}

void add_del(int pos){ //增加和减少取决于这个点被遍历了几次
	vis[pos] ? Del(pos) : Add(pos);
	vis[pos] ^= 1;
}

void work(int x){
	if(vis[ch[x].pos]){ //修改点为有效点
		add_del(ch[x].pos); //减掉
		swap(C[ch[x].pos], ch[x].val);
		add_del(ch[x].pos); //加上
	}
	else swap(C[ch[x].pos], ch[x].val);
}

int main()
{
	scanf("%d%d%d",&n,&m,&k);
	rep(i,1,m) scanf("%d",&V[i]);
	rep(i,1,n) scanf("%d",&W[i]);
	rep(i,1,n-1){
		int xx,yy; scanf("%d%d",&xx,&yy);
		add(xx,yy); add(yy,xx);
	}
	rep(i,1,n) scanf("%d",&C[i]);
	t = (int)(log(n)/log(2))+1;
	dfs(1,0);
	int sz = pow(ncnt,2.0/3.0);
	for(int i = 0; i <= ncnt; i++) pos[i] = i/sz;
	rep(i,1,k){
		int op,x,y; scanf("%d%d%d",&op,&x,&y);
		if(op){
			int lca = LCA(x,y);
			q[++qnum].t = cnum; q[qnum].id = qnum;
			if(fir[x] > fir[y]) swap(x,y);
			if(x == lca) q[qnum].l = fir[x], q[qnum].r = fir[y], q[qnum].lca = 0;
			else q[qnum].l = las[x], q[qnum].r = fir[y], q[qnum].lca = lca;
		}
		else ch[++cnum] = {x,y};
	}
	sort(q+1,q+1+qnum);
	L = 1, R = 0, T = 0;
	rep(i,1,qnum){
		while(L < q[i].l){
			add_del(Euler[L]); L++;
		}
		while(L > q[i].l){
			L--; add_del(Euler[L]);
		}
		while(R < q[i].r){
			R++; add_del(Euler[R]);
		}
		while(R > q[i].r){
			add_del(Euler[R]); R--;
		}
		while(T < q[i].t){
			++T; work(T);
		}
		while(T > q[i].t){
			work(T); --T;
		}
		if(q[i].lca) add_del(q[i].lca); //lca不在欧拉序列区间中
		ans[q[i].id] = now;
		if(q[i].lca) add_del(q[i].lca); //恢复这个区间的状态
	}
	rep(i,1,qnum) printf("%lld\n",ans[i]);
	return 0;
}