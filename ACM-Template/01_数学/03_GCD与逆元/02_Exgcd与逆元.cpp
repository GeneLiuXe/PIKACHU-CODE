#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
typedef long long ll;
const ll mod = 9973; 

//求ax+by = gcd(a,b)的特解
ll exgcd(ll a,ll b,ll &x,ll &y)
{
	if(b == 0) {x = 1,y = 0; return a;}  //此处a为gcd(a,b)
	ll d = exgcd(b,a%b,x,y);
	ll z = x; x = y; y = z-y*(a/b);
	return d;
}
// ax+by = c的通解可以表示为 x = (c/d)x1+k*(b/d) 【d为gcd(a,b)】
//						    y = (c/d)y1-k*(a/d)	【k为正整数】

//调用mod_reverse就是求逆元，求a在%n意义下的逆元
ll mod_reverse(ll a,ll n)   
{
	ll x,y;      //求a的逆元，a*ai 与 1 % mi同余，则a*ai-mi*k = 1；a与mi已知，求ai
	ll d = exgcd(a,n,x,y);
	if(d == 1) return (x%n+n)%n;  //保证逆元为正
	else return -1;
}

// ---
// 费马小定理求逆元\\
// 1. 模数$p$为素数\\
//		$a^{-1}=a^{p-2}$ \\
// 2. 模数$p$不为素数\\
//		$a^{-1}=a^{\phi(p)-1}$\\
// ---

int main()
{
	ll x;
	while(~scanf("%lld",&x))
	{
		printf("%lld\n",mod_reverse(x,mod));
	}
	return 0;
}



