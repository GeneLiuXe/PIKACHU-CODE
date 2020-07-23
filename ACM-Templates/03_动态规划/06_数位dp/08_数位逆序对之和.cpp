// ---
// 题意:\\ 
// 给出区间 $[l,r]$，求所有数字中数位逆序对之和。$(1\leq l\leq r\leq 10^8)$\\
// \\ 
// 思路:\\ 
// 这是一个涉及到组合计数的数位 $dp$ 问题，初次看到肯定觉得很棘手，但是我们可以将这个问题不断地进行分解。\\
// \\
// 我们先考虑 $pos$ 个位置，每个位置可以填 $[0,9]$ 时，所有可能的数的数位逆序对之和。这个问题不难处理，直接从 $pos$ 中取两个位置组成逆序队，其它位置任意取，答案为 $ans1(pos)=C(pos,2)*45*10^{pos-2}$。\\
// \\
// 然后再考虑如果 $pos$ 个位置，第一个位置不能为 $0$ 时的答案。我们先计算第一个位置和之后位置的贡献，再计算后面 $pos-1$ 位置产生的贡献。因此 $ans2(pos)=36*C(pos-1,1)*10^{pos-2}+9*ans1(pos-1)$。\\
// \\
// 处理完这两个子问题之后，我们直接按照数位 $dp$ 的套路 $dfs$ 求解即可。如果不是很清楚，可以查看代码进行进一步了解。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
using namespace std;

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

ll a[30],POS;

//快速幂
ll calc(ll a,ll b){
	if(b < 0) return 0;
	ll ans = 1, base = a;
	while(b){
		if(b&1) ans *= base;
		base *= base;
		b >>= 1;
	}
	return ans;
}

//数字长度为pos且第一个数字不为0
ll calc1(ll pos){
	ll ans = 0;
	ans += 36ll*(pos-1ll)*calc(10,pos-2); //第一个数不为0，后续数字与其组成的贡献
	ans += 9ll*(pos-2ll)*(pos-1ll)*45ll*calc(10,pos-3)/2ll; //除第一个数字之外的贡献
	return ans;
}

//数字长度为pos且第一个数字可以为0
ll calc2(ll pos){
	return pos*(pos-1ll)*45ll*calc(10,pos-2)/2ll;
}

ll dfs(int pos,int *base,bool flag,ll hp){
	ll ans = 0;
	if(pos == 0){
		return hp;
	}
	if(!flag){
		ans = calc2(pos);
		rep(i,0,8) ans += (ll)base[i]*(ll)pos*(ll)(9-i)*calc(10,pos-1);
		ans += hp*calc(10,pos);
		return ans;
	}
	int end = a[pos];
	ll tmp = 0;
	rep(i,0,end){
		if(POS == pos && i == 0) continue; //保证起始位不为0
		if(i > 0) tmp += (ll)base[i-1];
		base[i]++;
		ans += dfs(pos-1,base,flag && i == end,hp+tmp);
		base[i]--;
	}
	return ans;
}

ll solve(ll n){
	if(n <= 9) return 0;
	POS = 0; memset(a,0,sizeof a);
	while(n){
		a[++POS] = n%10ll;
		n /= 10ll;
	}
	int base[11];
	rep(i,0,9) base[i] = 0;
	ll ans = dfs(POS,base,1,0);
	rep(i,2,POS-1) ans += calc1(i);
	return ans;
}	

int main(){
	int _; scanf("%d",&_);
	rep(Ca,1,_){
		ll x,y; scanf("%lld%lld",&x,&y);
		x--;
		printf("Case %d: %lld\n",Ca,solve(y)-solve(x));
	}
}