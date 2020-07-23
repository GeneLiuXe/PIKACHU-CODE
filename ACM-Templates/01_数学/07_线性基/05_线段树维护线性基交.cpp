// ---
// 引理：若 $V_1$、$V_2$ 是线性空间，$B_1$、$B_2$ 分别是他们的一组基，令 $W=B_2 \cap V_1$，若 $B_1 \cup (B_2 \setminus W)$ 线性无关，则 $W$ 是 $V_1 \cap V_2$ 的一组基。\\
// \\
// 证明:\\
// 采用反证法，假设 $\exists \ v\in V_1\cap V_2$，且 $v$ 不能由 $W$ 线性组合表示。由于 $v$ 一定可以由 $W$ 和 $B_2\setminus W$ 线性组合表示，即 $v=xor(B_1)=xor(W)$ $\wedge$ $xor(B_2\setminus W)$，转换可得 $xor(B_2\setminus W)=xor(B_1)$ $\wedge$ $xor(W)$，因此 $B_2\setminus W$ 与 $B_1$ 线性相关，与题设条件不符，因此假设不成立，原定理正确。\\
// \\
// 求解:\\
// 因此问题就变成了如何求取 $W$。令 $tmp=B_1\cup (B_2\setminus W)$，$tmp$ 初始等于 $B_1$，我们在构建 $tmp$ 的过程中求取 $W$，因此我们不断取出 $B_2$ 的基底加入 $tmp$。假设当前取出的元素为 $x$，若 $x$ 不能被 $tmp$ 表示，则直接在 $tmp$ 中加入 $x$。否则 $x$ 一定能被 $tmp$ 线性组合而成，设 $x=xor(B_1)$ $\wedge$ $xor(tmp\setminus B_1),cur=xor(B_1)=x$ $\wedge$ $xor(tmp\setminus B_1)$，即 $cur$ 既可以由 $B_1$ 表示也可以由 $B_2$ 表示，因此直接将 $cur$ 加入 $W$ 即可。\\
// 枚举 $x$ 时，按照从低到高的顺序进行枚举，保证 $x$ 不能仅由 $tmp\setminus B_1$ 表示。\\
// \\
// 问题描述：给出 $n$ 个线性基，每次询问给出 $l$、$r$、$x$，询问区间 $[l,r]$ 的线性基交能否线性组合出 $x$。\\
// \\
// 问题解决：直接线段树维护区间线性基交即可。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
typedef long long ll;
const int N = 50000+100;
using namespace std;

int n,m;
struct Linear_Basis{
	ll p[50];
	Linear_Basis() {memset(p,0,sizeof p);}
	~Linear_Basis() {}
	bool ins(ll x,int jud){
		for(int i = 33; i >= 0; i--){
			if(x&(1ll<<i)){
				if(!p[i]) {
					if(jud) p[i] = x;
					break;
				}
				x ^= p[i];
			}
		}
		return x > 0ll;
	}
}t[2*N],base[N];
inline int get_id(int l,int r) {return (l+r)|(l!=r);}

Linear_Basis merge(Linear_Basis a, Linear_Basis b){
	Linear_Basis A = a, tmp = a, ans; //tmp不断构建A+(B\ans)
	ll cur,d;
	rep(i,0,33) //从低到高，使得不存在一个基底可以仅由(tmp\A)表示
		if(b.p[i]){ //b中有这个基底
			cur = 0, d = b.p[i];
			per(j,i,0)
				if((d>>j)&1){
					if(tmp.p[j]){
						d ^= tmp.p[j], cur ^= A.p[j];
						if(d) continue;
						ans.p[i] = cur; //cur的第i位不为0
					}
					else tmp.p[j] = d, A.p[j] = cur;
					//如果不能被表示, A的赋值是为了让高位中含有j这位的基底下放到A中j的位置
					break;
				}
		}
	return ans;
}

void build(int l,int r){
	int now = get_id(l,r);
	if(l == r) {t[now] = base[l]; return;}
	int mid = (l+r)>>1;
	build(l,mid); build(mid+1,r);
	t[now] = merge(t[get_id(l,mid)],t[get_id(mid+1,r)]);
}

bool query(int l,int r,int tl,int tr,ll x){
	int now = get_id(l,r);
	if(tl <= l && r <= tr) return !t[now].ins(x,0);
	int mid = (l+r)>>1;
	if(tr <= mid) return query(l,mid,tl,tr,x);
	else if(tl > mid) return query(mid+1,r,tl,tr,x);
	else return query(l,mid,tl,tr,x) && query(mid+1,r,tl,tr,x);
}

int main()
{
	scanf("%d%d",&n,&m);
	rep(i,1,n){
		int sz; scanf("%d",&sz);
		rep(j,1,sz){
			ll tp; scanf("%lld",&tp);
			base[i].ins(tp,1);
		}
	}
	build(1,n);
	rep(i,1,m){
		int l,r; ll x; scanf("%d%d%lld",&l,&r,&x);
		if(query(1,n,l,r,x)) printf("YES\n");
		else printf("NO\n");
	}
	return 0;
}