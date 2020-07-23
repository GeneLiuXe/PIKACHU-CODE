// ---
// 题意：\\
// 一共 $N$ 个商店，$1-N$ 依次编号。购买一件纪念品的花费为纪念品的花费加上税费。初始没有纪念品，税费为0。现有三种操作。\\
// 1 u v a b: 新增纪念品, 编号为x的商店，新增 b+(x-u)*a 的商品，$u\leq x\leq v$\\
// 2 u v a b: 税费调整操作, 表示编号为x的商店，税费增加 b+(x-u)*a，$u\leq x\leq v$\\
// 3 i: 询问操作，询问顾客在编号为i的商店，购买一件纪念品最高的花费是多少。\\
// \\
// 思路：\\
// 第一种操作就是李超树基础操作。第二种操作可以将 $b$ 和 $a$ 分开维护，对于 $b$ 的维护只需打 $lazy$ 区间更新即可，对于 $a$ 的维护，我们可以这样思考，对于编号为 $x$ 的商店，税费增加 $b+(x-u)*a=(b-u*a)+x*a$，因此我们将 $b-u*a$ 统一维护，再单独维护 $a$。最后每个点的税费即为 $x*sum(a)+sum(b)$。\\
// \\
// 此题还需注意将物品离散化，维护线段的同时维护两个$sum$，两个$lazy$，每次对税费和物品分开求最大值，累加到答案即可。当然维护$sum$的过程也可以用树状数组完成。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
const int N = 6*1e5+100;
const int M = 1e5+100;
const ll inf = -5*1e18;
using namespace std;

int n,m;
struct line{
	ll k,b;
	int l,r;
};
struct Node{
	line t;
	int flag;
	Node() {flag = 0;}
}sgt[2*N];
int b[N],tot;
ll sum1[2*N],sum2[2*N],lazy1[2*N],lazy2[2*N];
struct query{
	int op; int u,v,a,b;
}q[N];
inline int get_id(int l,int r) {return (l+r)|(l!=r);}
//计算坐标
inline ll calc(line t,ll pos) {return (t.k*b[pos]+t.b);}

void modify(int l,int r,line k){
	int now = get_id(l,r);
	if(k.l <= l && r <= k.r){
		if(!sgt[now].flag) sgt[now].flag = 1, sgt[now].t = k;
		else if(calc(k,l) >= calc(sgt[now].t,l) && calc(k,r) >= calc(sgt[now].t,r)) sgt[now].t = k;
		else if(calc(k,l) > calc(sgt[now].t,l) || calc(k,r) > calc(sgt[now].t,r)){
			int mid = (l+r)>>1;
			if(calc(k,mid) > calc(sgt[now].t,mid)) swap(k,sgt[now].t);
			if(calc(k,l) > calc(sgt[now].t,l)) modify(l,mid,k);
			else modify(mid+1,r,k);
		}
	}
	else{
		int mid = (l+r)>>1;
		if(k.l <= mid) modify(l,mid,k);
		if(k.r > mid) modify(mid+1,r,k);
	}
}

void push_down(int l,int r){
	int now = get_id(l,r), mid = (l+r)>>1, ls = get_id(l,mid), rs = get_id(mid+1,r);
	if(lazy1[now]){
		sum1[ls] += lazy1[now]; sum1[rs] += lazy1[now];
		lazy1[ls] += lazy1[now]; lazy1[rs] += lazy1[now];
		lazy1[now] = 0;
	}
	if(lazy2[now]){
		sum2[ls] += lazy2[now]; sum2[rs] += lazy2[now];
		lazy2[ls] += lazy2[now]; lazy2[rs] += lazy2[now];
		lazy2[now] = 0;		
	}
}

void update(int l,int r,int L,int R,ll a,ll b){
	int now = get_id(l,r);
	if(L <= l && r <= R){
		sum1[now] += a;
		sum2[now] += b;
		lazy1[now] += a;
		lazy2[now] += b;
		return;
	}
	push_down(l,r);
	int mid = (l+r)>>1;
	if(L <= mid) update(l,mid,L,R,a,b);
	if(R > mid) update(mid+1,r,L,R,a,b);
	sum1[now] = sum1[get_id(l,mid)]+sum1[get_id(mid+1,r)];
	sum2[now] = sum2[get_id(l,mid)]+sum2[get_id(mid+1,r)];
}

ll query1(int l,int r,int pos){
	int now = get_id(l,r);
	if(l == r){
		if((sgt[now].t).l <= pos && (sgt[now].t).r >= pos) return calc(sgt[now].t,pos);
		else return inf;
	}
	int mid = (l+r)>>1;
	ll ans = inf;
	if((sgt[now].t).l <= pos && (sgt[now].t).r >= pos) ans = calc(sgt[now].t,pos);
	if(pos <= mid) return max(query1(l,mid,pos),ans);
	else return max(query1(mid+1,r,pos),ans);
}

pair<ll,ll> query2(int l,int r,int pos){
	int now = get_id(l,r);
	if(l == r) return make_pair(sum1[now],sum2[now]);
	int mid = (l+r)>>1;
	push_down(l,r);
	if(pos <= mid) return query2(l,mid,pos);
	else return query2(mid+1,r,pos);
}

int main()
{
	scanf("%d%d",&n,&m);
	rep(i,1,m){
		scanf("%d",&q[i].op);
		if(q[i].op != 3){
			scanf("%d%d%d%d",&q[i].u,&q[i].v,&q[i].a,&q[i].b);
			b[++tot] = q[i].u; b[++tot] = q[i].v;
		}
		else{
			scanf("%d",&q[i].u); 
			b[++tot] = q[i].u;
		}
	}
	sort(b+1,b+1+tot);
	tot = unique(b+1,b+1+tot)-b-1;
	rep(i,1,m){
		if(q[i].op != 3){
			q[i].u = lower_bound(b+1,b+1+tot,q[i].u)-b;
			q[i].v = lower_bound(b+1,b+1+tot,q[i].v)-b;
		}
		else q[i].u = lower_bound(b+1,b+1+tot,q[i].u)-b;
	}
	rep(i,1,m){
		if(q[i].op == 1){
			line tmp; tmp.l = q[i].u, tmp.r = q[i].v;
			tmp.k = q[i].a, tmp.b = (ll)q[i].b-(ll)q[i].a*(ll)b[q[i].u];
			modify(1,tot,tmp);
		}
		else if(q[i].op == 2){
			update(1,tot,q[i].u,q[i].v,q[i].a,(ll)q[i].b-(ll)q[i].a*(ll)b[q[i].u]);
		}
		else{
			ll ans1 = query1(1,tot,q[i].u);
			pair<ll,ll> ans2 = query2(1,tot,q[i].u);
			if(ans1 == inf) printf("NA\n");
			else{
				ll ans = ans1+ans2.second+ans2.first*(ll)b[q[i].u];
				printf("%lld\n",ans);
			}
		}
	}
	return 0;
}