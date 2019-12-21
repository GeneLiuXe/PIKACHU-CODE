// ---
// 题意:\\ 
// 寻找区间 $[l,r]$ 中所有与 $7$ 无关的数字的平方和。与 $7$ 有关需要符合下述三个条件之一：\\
// 1. 整数中某一位是 $7$\\
// 2. 整数的每一位加起来的和是 $7$ 的整数倍\\
// 3. 这个整数是 $7$ 的整数倍\\
// $(1\leq l\leq r\leq 10^{18})$\\
// \\
// 思路:\\ 
// 如果这题只是单纯地求个数，那就是一个普通数位 $dp$ 问题，但此题要求的是数字平方和，因此我们需要对每一个数位进行考虑。\\
// \\
// 当我们枚举 $pos$ 位时，先递归到 $pos-1$ 位，返回一个结构体 $B$，表示若 $pos-1$ 位前全为空时满足题意的 $cnt$、$sum1$、$sum2$，即个数、和、平方和。\\
// \\
// 设当前结构体为 $A$，因此 $A.cnt = A.cnt+B.cnt$，$A.sum1 = A.sum1+B.sum1+B.cnt*10^{pos-1}$，$A.sum2=A.sum2+B.sum2+10^{pos-1}*10^{pos-1}+2*10^{pos-1}*B.sum1$。\\
// \\
// 总结:\\ 
// 数位 $dp$ 的主要难点还是在于列出状态，然后采用递归的思想来思考如何根据第 $pos-1$ 位的答案推出 $pos$ 位的答案。\\
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
const ll mod = 1e9+7;
const db EPS = 1e-9;
using namespace std;

struct Node{
	ll cnt,sum1,sum2; //个数、和、平方和
}dp[25][10][10];
ll a[30];

ll pow_mod(ll a, ll b, ll p){
	ll base = a, ans = 1;
	while(b){
		if(b&1) ans = (ans*base)%p;
		base = (base*base)%p;
		b >>= 1;
	}
	return ans;
}

Node dfs(int pos, int pre1, int pre2, bool flag){ //pre1: 前面数位之和, pre2: 前面数位*贡献之和
	//flag = 1, 前面为答案继承而来
	if(pos == 0){
		if(pre1 == 0 || pre2 == 0) return {0,0,0};
		else return {1,0,0};
	}
	if(!flag && dp[pos][pre1][pre2].cnt != -1) return dp[pos][pre1][pre2];
	Node ans = {0,0,0}, tmp;
	int end = flag?a[pos]:9;
	rep(i,0,end){
		if(i == 7) continue;
		tmp = dfs(pos-1, (pre1+i)%7, (pre2*10ll+i)%7, flag && i == end);
		ans.cnt = (ans.cnt+tmp.cnt)%mod;
		ans.sum1 = (ans.sum1+tmp.sum1+(tmp.cnt*(ll)i)%mod*pow_mod(10,pos-1,mod)%mod)%mod;
		ans.sum2 = (ans.sum2+tmp.sum2+(2ll*tmp.sum1*(ll)i)%mod*pow_mod(10,pos-1,mod)%mod)%mod;
		ans.sum2 = (ans.sum2+(ll)i*(ll)i*pow_mod(10,pos-1,mod)%mod*pow_mod(10,pos-1,mod)%mod*tmp.cnt%mod)%mod;
	}
	if(!flag) dp[pos][pre1][pre2] = ans;
	return ans;
}

ll solve(ll n){
	int pos = 0;
	memset(a,0,sizeof a);
	while(n){
		a[++pos] = n%(10ll);
		n /= 10ll;
	}
	return dfs(pos,0,0,1).sum2;
}

int main()
{
	rep(i,0,20)
		rep(j,0,6)
			rep(k,0,6) dp[i][j][k].cnt = -1;
	int _; scanf("%d",&_);
	while(_--){
		ll L,R; scanf("%lld%lld",&L,&R); L--;
		printf("%lld\n",(solve(R)-solve(L)+mod)%mod);
	}
	return 0;
}