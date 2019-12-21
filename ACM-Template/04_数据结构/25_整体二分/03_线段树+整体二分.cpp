// ---
// 题意:\\
// $n$ 个位置，$m$ 个操作。操作有两种，$1\ a\ b\ c$ 表示在第 $a$ 个位置到第 $b$ 个位置，每个位置加入一个数 $c$；$2\ a\ b\ c$ 表示询问从第 $a$ 个位置到第 $b$ 个位置，第 $c$ 大的数是多少。$(1\leq n,m\leq 5*10^4)$\\
// \\
// 思路:\\
// 其实和上面第二个问题没有太大的差别，只不过上一个问题是单点修改，而这题变成了区间修改。因此我们用线段树维护一下整体二分即可解决。\\
// ---
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#define mem(a,b) memset(a,b,sizeof a);
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
typedef long long ll;
typedef double db;
const int N = 1e5+100;
const int inf = 1e9+10;
const db EPS = 1e-9;
using namespace std;

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

int n,m;
ll ans[N],sum[2*N],lazy[2*N];
struct Node{ll x,y,k,id;}q[2*N],q1[2*N],q2[2*N];

inline int get_id(int l,int r) {return (l+r)|(l!=r);}
inline void pushDown(int l,int r){
	int mid = (l+r)>>1, now = get_id(l,r), ls = get_id(l,mid), rs = get_id(mid+1,r);
	sum[ls] += lazy[now]*(ll)(mid-l+1); sum[rs] += lazy[now]*(ll)(r-mid);
	lazy[ls] += lazy[now]; lazy[rs] += lazy[now];
	lazy[now] = 0;
}
inline void update(int l,int r,int L,int R,int v){
	int now = get_id(l,r);
	if(L <= l && r <= R){
		sum[now] += (ll)v*(ll)(r-l+1);
		lazy[now] += v;
		return;
	}
	if(lazy[now]) pushDown(l,r);
	int mid = (l+r)>>1;
	if(L <= mid) update(l,mid,L,R,v);
	if(R > mid) update(mid+1,r,L,R,v);
	sum[now] = sum[get_id(l,mid)]+sum[get_id(mid+1,r)];
}
inline ll query(int l,int r,int L,int R){
	int now = get_id(l,r);
	if(L <= l && r <= R) return sum[now];
	if(lazy[now]) pushDown(l,r);
	int mid = (l+r)>>1;
	ll thp = 0;
	if(L <= mid) thp += query(l,mid,L,R);
	if(R > mid) thp += query(mid+1,r,L,R);
	return thp; 
}

void solve(int l,int r,int L,int R){
	if(l > r || L > R) return;
	if(l == r){
		rep(i,L,R) if(q[i].id) ans[q[i].id] = l;
		return;
	}
	int cnt1 = 0, cnt2 = 0, mid = (l+r)>>1;
	rep(i,L,R){
		if(q[i].id){ //查询
			ll tmp = query(1,n,q[i].x,q[i].y);
			if(q[i].k <= tmp) q2[++cnt2] = q[i];
			else q[i].k -= tmp, q1[++cnt1] = q[i];
		}
		else{ //赋值
			//由于右区间的起始点为mid+1, 因此此处为 >= mid+1
			if(q[i].k >= mid+1) update(1,n,q[i].x,q[i].y,1), q2[++cnt2] = q[i];
			else q1[++cnt1] = q[i];
		}
	}
	rep(i,1,cnt2) if(!q2[i].id) update(1,n,q2[i].x,q2[i].y,-1);
	rep(i,1,cnt1) q[L+i-1] = q1[i];
	rep(i,1,cnt2) q[L+cnt1+i-1] = q2[i];
	solve(l,mid,L,L+cnt1-1); solve(mid+1,r,L+cnt1,R);
}

int main()
{
	scanf("%d%d",&n,&m);
	int tot = 0;
	rep(i,1,m){
		ll op,x,y,k;
		scanf("%lld%lld%lld%lld",&op,&x,&y,&k);
		if(op == 1) q[i] = {x,y,k,0};
		else q[i] = {x,y,k,++tot};
	}
	solve(-inf,inf,1,m);
	rep(i,1,tot) printf("%lld\n",ans[i]);
	return 0;
}