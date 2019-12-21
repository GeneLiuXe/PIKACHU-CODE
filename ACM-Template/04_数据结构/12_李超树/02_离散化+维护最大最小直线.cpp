// ---
// 题意：\\
// 现有 $n$ 个机器人，第 $i$ 个机器人初始在 $a_i$ 位置。一共有两个操作。第一种操作为 $T_i\  \text{command}\  k_i \ x_i$，表示 $T_i$ 时间，第 $k_i$ 个机器人以 $x_i$ 的速度开始移动，$x_i$ 可以为负值。第二种操作为 $T_i \ \text{query}$，表示查询 $T_i$ 时，离原点最远的机器人的坐标。$(0\leq T_i\leq 10^9)$\\
// \\
// 思路：\\
// 对于每一个机器人来说，其速度变化都是一个分段函数，只需要先离线，将时间离散化，处理出每一个机器人各速度下的分段直线。然后在第一种操作时，动态插入一条直线，第二种操作时，单点查询最大值与最小值即可完成此题。\\
// \\
// 本题主要特点就是对时间离散化，然后在离散化后的时间上插入直线。每一个节点需要同时维护最大值和最小值直线。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
const int N = 1e5+100;
const int M = 6*1e5+100;
using namespace std;

int n,m,tot,rp[N];
ll a[N],b[M];
struct Query{
	ll t,x; //x是速度
	int k,op; //k是第k个机器人
}q[M];
struct NP{
	ll t,x,pos,t2;
	NP() {}
	NP(ll _a,ll _b,ll _c,ll _d) : t(_a),x(_b),pos(_c),t2(_d) {}
};
vector<NP> hp[N];
struct line{
	ll k,b;
	int l,r;
	line() {}
	line(ll _a, ll _b, int _c, int _d) : k(_a), b(_b), l(_c), r(_d) {}
};
struct Node{
	line minn,maxn;
	int flag;
	Node() {flag = 0;}
}sgt[2*M];

inline int get_id(int l,int r) {return (l+r)|(l!=r);}
//计算某线段在某一个横坐标的纵坐标值
inline ll calc(line a,ll pos) {return a.k*b[pos]+a.b;}
//离散化
int find(ll x) {return lower_bound(b+1,b+1+tot,x)-b;}

void modify(int l,int r,line k){
	int now = get_id(l,r);
	if(k.l <= l && k.r >= r){
		if(!sgt[now].flag) sgt[now].minn = sgt[now].maxn = k, sgt[now].flag = 1;
		else{
			line tp = k;
			if(calc(k,l) >= calc(sgt[now].maxn,l) && calc(k,r) >= calc(sgt[now].maxn,r)) sgt[now].maxn = k;
			else if(calc(k,l) > calc(sgt[now].maxn,l) || calc(k,r) > calc(sgt[now].maxn,r)){
				int mid = (l+r)>>1;
				//让非优势线段向下更新
				if(calc(k,mid) > calc(sgt[now].maxn,mid)) swap(k,sgt[now].maxn);
				//只需要判断两条直线左右端点大小
				if(calc(k,l) > calc(sgt[now].maxn,l)) modify(l,mid,k);
				else modify(mid+1,r,k);
			}

			k = tp;
			if(calc(k,l) <= calc(sgt[now].minn,l) && calc(k,r) <= calc(sgt[now].minn,r)) sgt[now].minn = k;
			else if(calc(k,l) < calc(sgt[now].minn,l) || calc(k,r) < calc(sgt[now].minn,r)){
				int mid = (l+r)>>1;
				//让非优势线段向下更新
				if(calc(k,mid) < calc(sgt[now].minn,mid)) swap(k,sgt[now].minn);
				//只需要判断两条直线左右端点大小
				if(calc(k,l) < calc(sgt[now].minn,l)) modify(l,mid,k);
				else modify(mid+1,r,k);
			}			
		}	
	}
	else{
		int mid = (l+r)>>1;
		if(k.l <= mid) modify(l,mid,k);
		if(k.r > mid) modify(mid+1,r,k);
	}
}

ll Query(int l,int r,int x){
	int now = get_id(l,r);
	ll ans = max(calc(sgt[now].maxn,x),-calc(sgt[now].minn,x));
	if(l == r) return ans;
	else{
		int mid = (l+r)>>1;
		if(x <= mid) return max(ans,Query(l,mid,x));
		else return max(ans,Query(mid+1,r,x));
		return ans;
	}
}

int main()
{
	scanf("%d%d",&n,&m);
	rep(i,1,n) scanf("%lld",&a[i]);
	rep(i,1,n) hp[i].push_back({1,0,a[i],0});
	b[++tot] = 0;
	rep(i,1,m){
		ll t; scanf("%lld",&t); b[++tot] = t;
		char op[10]; scanf("%s",op+1);
		if(op[1] == 'c'){
			int k; ll x; scanf("%d%lld",&k,&x);
			q[i].t = t; q[i].x = x; q[i].k = k; q[i].op = 0;
		}
		else q[i].op = 1, q[i].t = t;
	}
	tot = unique(b+1,b+1+tot)-b-1;
	rep(i,1,m){
		if(q[i].op == 0){
			q[i].t = find(q[i].t);
			int len = hp[q[i].k].size();
			ll pos = hp[q[i].k][len-1].x*b[q[i].t]+hp[q[i].k][len-1].pos;
			ll now = -b[q[i].t]*q[i].x+pos;
			hp[q[i].k][len-1].t2 = q[i].t;
			hp[q[i].k].push_back({q[i].t,q[i].x,now,0});
		}
		else q[i].t = find(q[i].t);
	}
	rep(i,1,n){
		int len = hp[i].size();
		hp[i][len-1].t2 = tot;
	}
	rep(i,1,n){
		line tmp; tmp.l = hp[i][rp[i]].t;
		tmp.r = hp[i][rp[i]].t2;
		tmp.k = hp[i][rp[i]].x;
		tmp.b = hp[i][rp[i]].pos;
		rp[i]++;
		modify(1,tot,tmp);
	}
	rep(i,1,m){
		if(q[i].op == 0){
			int xp = q[i].k;
			line tmp; tmp.l = hp[xp][rp[xp]].t;
			tmp.r = hp[xp][rp[xp]].t2;
			tmp.k = hp[xp][rp[xp]].x;
			tmp.b = hp[xp][rp[xp]].pos;
			rp[xp]++;
			modify(1,tot,tmp);
		}	
		else{
			ll ans = Query(1,tot,q[i].t);
			printf("%lld\n",ans);
		}
	}
	return 0;
}