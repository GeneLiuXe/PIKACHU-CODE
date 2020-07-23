// ---
// 题意:\\ 
// 给出四个数，$L、R、A、B$，表示 $x\in [L,R],y\in [0,A],z\in [0,B]$，求 $(x \wedge y)+(y\&z)+(z\wedge x)$ 的最大值。$(0\leq L\leq R\leq 10^{18},0\leq A,B\leq 10^{18})$\\
// \\
// 思路:\\ 
// 这个问题考察的是涉及三个数字的数位 $dp$，首先我们来思考一下如何表示状态。\\
// \\
// 此题是求取最大值，因此很明显需要从数位的角度入手进行考虑，所以状态的设置一定会包含各个数位的状态，我们另 $f[pos][p1][p2][p3][p4]$ 表示仅考虑最后 $pos$ 位，从前面转移来的状态为 $p1、p2、p3、p4$ 时答案的最大值。\\
// 1. $p1$ 为 $1$ 表示比 $L$ 大\\
// 2. $p2$ 为 $1$ 表示比 $R$ 小\\
// 3. $p3$ 为 $1$ 表示比 $A$ 小\\
// 4. $p4$ 为 $1$ 表示比 $B$ 小\\
// \\
// 然后在 $dfs$ 的时候枚举 $pos$ 位放的值，然后更新答案即可。代码中将每个数拆成了二进制进行计算，因为仅考虑 $0、1$ 可以简化问题。\\
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
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;
 
ll f[70][2][2][2][2],a1[70],a2[70],a3[70],a4[70];
 
ll pow_mod(ll a,ll b){
	ll base = a, ans = 1;
	while(b){
		if(b&1) ans *= base;
		base *= base;
		b >>= 1;
	}
	return ans;
}
 
ll dfs(int pos,bool f1,bool f2,bool f3,bool f4){ 
	//比L大, 比R小, 比A小, 比B小
	if(pos == 0) return 0;
	ll ans = f[pos][f1][f2][f3][f4];
	if(ans != -1) return ans;
 
	int b1 = 0, b2 = 1, d1 = 0, d2 = 1, e1 = 0, e2 = 1;
	if(!f1 && a1[pos] != 0) b1 = 1;
	if(!f2 && a2[pos] != 1) b2 = 0;
	if(!f3 && a3[pos] != 1) d2 = 0;
	if(!f4 && a4[pos] != 1) e2 = 0;
	bool k1,k2,k3,k4;
	rep(i,b1,b2)
		rep(j,d1,d2)
			rep(k,e1,e2){
				k1 = f1, k2 = f2, k3 = f3, k4 = f4;
				ll x = (i^j)+(j&k)+(k^i);
				x *= pow_mod(2,pos-1);
				// printf("********\n");
				// LOG3("i",i,"j",j,"k",k);
				// LOG2("pos",pos,"x",x);
				if(i > a1[pos]) k1 = 1;
				if(i < a2[pos]) k2 = 1;
				if(j < a3[pos]) k3 = 1;
				if(k < a4[pos]) k4 = 1;
				ans = max(ans,x+dfs(pos-1,k1,k2,k3,k4));
			}
	return (f[pos][f1][f2][f3][f4] = ans);
}
 
ll solve(ll L,ll R,ll A,ll B){
	memset(a1,0,sizeof a1);
	memset(a2,0,sizeof a2);
	memset(a3,0,sizeof a3);
	memset(a4,0,sizeof a4);
	int pos = 0;
	while(L || R || A || B){
		++pos;
		a1[pos] = L & 1;
		a2[pos] = R & 1;
		a3[pos] = A & 1;
		a4[pos] = B & 1;
		L /= 2; R /= 2; A /= 2; B /= 2; 
	}
	return dfs(pos,0,0,0,0);
}
 
int main()
{
	int _; scanf("%d",&_);
	while(_--){
		rep(i,0,65)
			rep(j,0,1)
				rep(k,0,1)
					rep(t1,0,1)
						rep(t2,0,1)
							f[i][j][k][t1][t2] = -1;
		ll L,R,A,B; scanf("%lld%lld%lld%lld",&L,&R,&A,&B);
		printf("%lld\n",solve(L,R,A,B));
	}
	return 0;
}