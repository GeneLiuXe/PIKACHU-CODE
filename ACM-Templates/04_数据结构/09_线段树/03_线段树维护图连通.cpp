// ---
// 题意：$n$ 个点，$m$ 条边，每条边有一个区间 $[l_i,r_i]$，表示只有体重在这个范围内的人才能通过这条边，问有多少种不同的体重可以从起点到达终点。$(1\leq n,m\leq 10^5,1\leq l_i,r_i\leq 10^9)$\\
// \\
// 思路：这应该是一类经典题目，即按照每条边的权重建线段树，这次是题目给出的上下界，下次可能是到达时间等。\\
// \\
// 然后每个节点维护该权重下有哪些边是连通的。因此每个节点均维护一个 $vector$，存储当前权重下哪些边连通。然后线段树分治的时候，将当前权重涉及到的边全部用并查集连起来。\\
// \\
// 每到达线段树的一个节点，就将节点的边连通，然后判断当前 $1$ 是否和 $n$ 可达，如果可达，则返回。否则继续二分区间，向下继续分治。没做过这类题的话，可能不太好理解，建议直接看代码，代码还是很好理解的。\\
// \\
// 这里还涉及到一个问题，就是每条边的权重是一个区间，如果我们直接按照闭区间往线段树中添加，是会出错的。具体出错原因可以手跑一下下面这个数据，大致来说就是权重若为闭合区间，在线段树分治的过程中，会丢失很多相邻区间之间的答案。\\
// \\
// 因此离散化时，需要将右端点 $+1$，插入一个区间时，需要将右端点离散化后的值再 $-1$。分治时，查询到一段可行区间后，对答案的贡献应该是 $b[r+1]-b[l]$，这样就不会忽略掉 $[b[r],b[r+1])$ 之间的一段了。而开区间的话，会忽略掉这一段。\\
// ---
#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const db EPS = 1e-9;
const int N = 2*1e5+100;
using namespace std;

int n,m,tot,sz[N],fa[N];
struct Node{
	int u,v;
	ll l,r;
}e[N];
ll b[N],ans;
vector<int> t[2*N];

int findx(ll x) {return lower_bound(b+1,b+1+tot,x)-b;}
int find(int x) {return x==fa[x]?x:find(fa[x]);}
inline int get_id(int l,int r) {return (l+r)|(l!=r);}

void update(int l,int r,int ls,int rs,int id){
	int now = get_id(l,r);
	if(ls <= l && r <= rs){
		t[now].push_back(id);
		return;
	}
	int mid = (l+r)>>1;
	if(ls <= mid) update(l,mid,ls,rs,id);
	if(rs > mid) update(mid+1,r,ls,rs,id);
}

void dfs(int l,int r){
	int now = get_id(l,r);
	int len = t[now].size();
	vector<int> back; back.clear();
	rep(i,0,len-1){
		int u = find(e[t[now][i]].u), v = find(e[t[now][i]].v);
		if(u != v){
			if(sz[u] > sz[v]) swap(u,v);
			fa[u] = v; sz[v] += sz[u];
			back.push_back(u); back.push_back(v);
		}
	}
	if(find(1) == find(n)) ans += b[r+1]-b[l];
	else if(l != r){
		int mid = (l+r)>>1;
		dfs(l,mid); dfs(mid+1,r);
	}
	len = back.size();
	rep(i,0,len-1){
		fa[back[i]] = back[i], sz[back[i]] = 1;
	}
	back.clear();
}

int main()
{
	scanf("%d%d",&n,&m);
	rep(i,1,n) fa[i] = i, sz[i] = 1;
	rep(i,1,m){
		scanf("%d%d%lld%lld",&e[i].u,&e[i].v,&e[i].l,&e[i].r);
		e[i].r++;
		b[++tot] = e[i].l; b[++tot] = e[i].r;
	}
	sort(b+1,b+1+tot);
	tot = unique(b+1,b+1+tot)-b-1;
	rep(i,1,m) update(1,tot,findx(e[i].l),findx(e[i].r)-1,i);
	dfs(1,tot);
	printf("%lld\n",ans);
	return 0; 
}