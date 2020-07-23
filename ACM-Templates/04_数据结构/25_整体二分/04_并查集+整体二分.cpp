// ---
// 题意:\\ 
// 一个 $n$ 个点，$m$ 条边的图，第 $i$ 条边连接 $a_i$ 和 $b_i$，保证图是连通的。\\
// \\
// 现在有 $q$ 次询问，每次询问给出一个三元组 $x\ y\ z$，表示询问从 $x\ y$ 两个点出发，一共扩展 $z$ 个不同的点（包括起始点），求所经过的边中最大编号的最小值。$(3\leq n\leq 10^5,1\leq q\leq 10^5)$\\
// \\
// 思路:\\
// 此题较之上面三题，没有那么套路，因此我们先从只有一个询问开始找思路。\\
// \\
// 首先考虑能不能把图变成树，因为图上问题往往都很复杂，而变成树上问题后我们的可操作空间会大很多。继续思考不难发现，如果我们按边的编号为权值构建一棵最小生成树，每次询问的答案也一定会落在最小生成树上的边上。\\
// \\
// 转到树上问题之后，我们考虑能不能二分答案然后 $check$，如果只有一个询问的话，显然是可以的。只需要维护一个可加边可删边的按秩合并的并查集即可。\\
// \\
// 既然单个查询可以二分，那一定可以用整体二分的方法对所有查询进行二分。我们在二分值为 $mid$ 时，将所有编号小于等于 $mid$ 的边连接起来，然后再递归到 $[mid+1,r]$ 区间。右区间递归结束后，再撤销二分值为 $mid$ 时连接的边，然后递归 $[l,mid]$。\\
// \\
// 如果连接操作是 $fa[x]=y,sz[y]=sz[y]+sz[x]$，那么撤销操作的时候，如果只有 $fa[x]=x,sz[y]=sz[y]-sz[x]$ 是不够的，需要从 $y$ 开始不断向上访问，对于访问到的每一个节点都减去 $sz[x]$，如此才能保证撤销操作的正确性。\\
// ---
#include <bits/stdc++.h>
#define mem(a,b) memset(a,b,sizeof a);
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
typedef long long ll;
typedef double db;
const db EPS = 1e-9;
const int N = 1e6+100;
using namespace std;

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

int n,m,Q,fa[N],sz[N],ans[N];
struct Node {int x,y,k,id,h1,h2;} q[N],q1[N],q2[N];

int find(int x) {return x == fa[x] ? x : find(fa[x]);}
int calc(int x,int y){
	int fx = find(x), fy = find(y);
	return fx == fy ? sz[fx] : (sz[fx] + sz[fy]);
}
pair<int,int> merge(int x,int y){
	int fx = find(x), fy = find(y);
	if(fx == fy) return make_pair(-1,-1);
	if(sz[fx] < sz[fy]){
		fa[fx] = fy, sz[fy] += sz[fx];
		return make_pair(fx,fy);
	}
	else{
		fa[fy] = fx, sz[fx] += sz[fy];
		return make_pair(fy,fx);
	}
}
void Delete(int x,int y) {
	fa[x] = x;
	while(y){
		sz[y] -= sz[x];
		if(y == fa[y]) break;
		y = fa[y];
	}
}

void solve(int l,int r,int L,int R){
	if(l > r || L > R) return;
	if(l == r){
		rep(i,L,R) if(q[i].k) ans[q[i].id] = l;
		return;
	}
	int mid = (l+r)>>1, cnt1 = 0, cnt2 = 0;
	// logs(mid,L,R);
	rep(i,L,R){
		if(q[i].k){ //查询
			int tmp = calc(q[i].x,q[i].y);
			if(q[i].k <= tmp) q1[++cnt1] = q[i];
			else q2[++cnt2] = q[i];
		}
		else{
			if(q[i].id <= mid){
				pair<int,int> tmp = merge(q[i].x,q[i].y);
				q1[++cnt1] = q[i];
				q1[cnt1].h1 = tmp.first; q1[cnt1].h2 = tmp.second;
			}
			else q2[++cnt2] = q[i];
		}
	}
	rep(i,1,cnt1) q[L+i-1] = q1[i];
	rep(i,1,cnt2) q[L+cnt1+i-1] = q2[i];
	solve(mid+1,r,L+cnt1,R);
	rep(i,1,cnt1) if(!q[L+i-1].k) Delete(q[L+i-1].h1,q[L+i-1].h2);
	solve(l,mid,L,L+cnt1-1);
}

int main()
{
	int cnt = 0;
	scanf("%d%d",&n,&m);
	rep(i,1,n) fa[i] = i, sz[i] = 1;
	rep(i,1,m){
		int x,y; scanf("%d%d",&x,&y);
		if(merge(x,y).first == -1) continue;
		q[++cnt] = {x,y,0,i,0,0};
		// logs(i);
	}
	scanf("%d",&Q);
	rep(i,1,Q){
		int x,y,z; scanf("%d%d%d",&x,&y,&z);
		q[++cnt] = {x,y,z,i,0,0};
	}
	rep(i,1,n) fa[i] = i, sz[i] = 1;
	solve(0,m+1,1,cnt);
	rep(i,1,Q) printf("%d\n",ans[i]);
	return 0;
}