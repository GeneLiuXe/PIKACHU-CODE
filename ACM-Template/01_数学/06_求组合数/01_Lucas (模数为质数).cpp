#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
typedef long long ll;

//求ax+by = gcd(a,b)的特解
ll exgcd(ll a,ll b,ll &x,ll &y)
{
	if(b == 0) {x = 1,y = 0; return a;}  //此处a为gcd(a,b)
	ll d = exgcd(b,a%b,x,y);
	ll z = x; x = y; y = z-y*(a/b);
	return d;
}
// ax+by = c的通解可以表示为 x = (c/d)x1+k*(b/a) 【d为gcd(a,b)】
//						    y = (c/d)y1-k*(a/d)	【k为正整数】

//调用mod_reverse就是求逆元，求a在%p意义下的逆元
ll mod_reverse(ll a,ll p)   
{
	ll x,y;      //求a的逆元，a*ai 与 1 % mi同余，则a*ai-mi*k = 1；a与mi已知，求ai
	ll d = exgcd(a,p,x,y);
	if(d == 1) return (x%p+p)%p;  //保证逆元为正
	else return -1;
}

ll fact(ll n, ll p){//n的阶乘 % p 
	ll ret = 1;
	for (int i = 1; i <= n ; i ++) ret = ret * i % p ;
	return ret ;
}

ll comb(ll n, ll m, ll p){//C(n, m) % p，将n和m限制在1-p之内
	if (m < 0 || m > n) return 0;
	//调用逆元和求解阶乘
	return fact(n, p) * mod_reverse(fact(m, p), p) % p * mod_reverse(fact(n-m, p), p) % p;
}

ll Lucas(ll n, ll m, ll p)  //求组合数C(n,m)%p
{
	return m ? Lucas(n/p, m/p, p) * comb(n%p, m%p, p) % p : 1;
}


int main()
{
/*注意事项：
	C(n,m)%p用Lucas定理求解，p必须为质数，并且p < 1e5，这个限制主要是针对如果p大于1e5，那就算n和m在1-p之内，如果n和m大于1e5，也难以求解
	本模板还可以进一步进行加速，那就是预处理每个数的阶乘
*/
	ll n,m,p;
	int T;
	scanf("%d",&T);
	while(T--)
	{
		scanf("%lld%lld%lld",&n,&m,&p);
		printf("%lld\n",Lucas(n+m,m,p));
	}
	return 0;
}