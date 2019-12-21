// ---
// 题意:\\ 
// 一个正整数被称为漂亮数，当且仅当其能够被其所有非零数位整除，现需要求 $[l,r]$ 中漂亮数的个数。$(1\leq l\leq r\leq 9*10^{18})$\\
// \\
// 思路:\\ 
// 整除所有非零数位，即整除非零数位的 $LCM$。因此我们需要在 $dfs$ 的过程中，记录出现数位的 $LCM$，以及记录当前数字的大小。\\
// \\
// 这里会出现一个问题，即当前数字非常大，直接记录不可行，因此我们需要将当前数字对 $2520$ 取模，即 $1～9$ 所有数的 $LCM$。思考到这一步，剩下的就是一些代码细节了，可以参考一下下述代码。\\
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

ll a[30],b[3000],tot,f[20][55][3000];

ll gcd(ll a,ll b){
	return b == 0 ? a:gcd(b,a%b);
}

int find(ll x){
	return lower_bound(b+1,b+1+tot,x)-b;
}

ll dfs(int pos,int lcm,ll m,bool flag){
	if(pos == 0){
		if(m%b[lcm] == 0) return 1;
		else return 0;
	}
	if(!flag && f[pos][lcm][m] != -1) return f[pos][lcm][m];
	int end = flag?a[pos]:9;
	ll ans = 0;
	rep(i,0,end){
		if(i == 0) ans += dfs(pos-1,lcm,(m*10ll+i)%(2520ll),flag && i == end);
		else{
			ll tp = gcd(b[lcm],i);
			ll hp = b[lcm]*i/tp;
			int xp = find(hp);
			ans += dfs(pos-1,xp,(m*10ll+i)%(2520ll),flag && i == end);
		}
	}
	if(!flag) f[pos][lcm][m] = ans;
	return ans;
}

ll solve(ll n){
	if(n == 0) return 1;
	int pos = 0;
	memset(a,0,sizeof a);
	while(n){
		a[++pos] = n % 10;
		n /= 10;
	}
	return dfs(pos,1,0,1);
}

int main()
{
	rep(i,1,((1<<9)-1)){
		ll ans = 1;
		rep(j,1,9){
			if(i&(1<<(j-1))){
				ll tp = gcd(ans,j);
				ans = ans*j/tp; 
			}
		}
		b[++tot] = ans;
	}
	sort(b+1,b+1+tot);
	tot = unique(b+1,b+1+tot)-b-1;
	rep(i,0,19)
		rep(j,0,50)
			rep(k,0,2900) f[i][j][k] = -1;
	int _; scanf("%d",&_);
	while(_--){
		ll L,R; scanf("%lld%lld",&L,&R); L--;
		printf("%lld\n",solve(R)-solve(L));
	}
	return 0;
}