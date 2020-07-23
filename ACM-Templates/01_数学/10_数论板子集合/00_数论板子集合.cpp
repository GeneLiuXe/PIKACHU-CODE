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

//gcd
ll gcd(ll a,ll b)
{
	return b == 0 ? a:gcd(b,a%b);
}

//快速幂
ll poww(ll a,ll b){
	ll base = a, ans = 1ll;
	while(b){
		if(b&1) ans = (ans*base)%mod;
		base = (base*base)%mod;
		b >>= 1;
	}
	return ans;
}

//线性筛
void primes(int x){
	tot = 0;
	memset(v,0,sizeof v);
	rep(i,2,x){
		if(!v[i]){
			v[i] = i, prime[++tot] = i;
		}
		rep(j,1,tot){
			if(prime[j] * i > x) break;
			v[i*prime[j]] = prime[j];
			if(i % prime[j] == 0) break;
		}
	}
}

//唯一分解
void getFactors(ll x){
	ll tmp = x; tc = 0;
	for(ll i = 1; prime[i]*prime[i] <= tmp; i++){
		if(tmp%prime[i] == 0){
			c[++tc][0] = prime[i]; c[tc][1] = 0;
			while(tmp%prime[i] == 0){
				tmp /= prime[i];
				c[tc][1]++;
			}
		}
	}
	if(tmp != 1){
		c[++tc][0] = tmp;
		c[tc][1] = 1;
	}
}

//费马小定理 a^(p-2) 为逆元
ll comp(ll base,ll x){ //求组合数, C(base,x)
	ll ans1 = 1, ans2 = 1;
	for(ll i = base; i >= base-x+1ll; i--) ans1 = (ans1*i)%mod;
	for(ll i = 1ll; i <= x; i++) ans2 = (ans2*i)%mod;
	ll tp = (ans1*poww(ans2,mod-2ll))%mod;
	return tp;
}

//递推求组合数
c[1][0] = c[1][1] = 1; //c[i][j] = C(i,j)
for(int i = 2; i <= 3000; i++){
	c[i][0] = 1;
	for(int j = 1; j <= i; j++)
		c[i][j] = (c[i-1][j]+c[i-1][j-1])%mod;
}