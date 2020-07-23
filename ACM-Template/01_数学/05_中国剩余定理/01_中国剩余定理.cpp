//中国剩余定理
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;

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

//调用mod_reverse就是求逆元，求a在%n意义下的逆元
ll mod_reverse(ll a,ll n)   
{
	ll x,y;      //求a的逆元，a*ai 与 1 % mi同余，则a*ai-mi*k = 1；a与mi已知，求ai
	ll d = exgcd(a,n,x,y);
	if(d == 1) return (x%n+n)%n;  //保证逆元为正
	else return -1;
}

//求ans%m1=a1,ans%m2=a2,...中的ans，其中m1,m2,m3...互质
int CRT(int a[],int m[],int cn){  
    int M = 1;  
    int ans = 0;  
    for(int i=1; i<=cn; i++)  
        M *= m[i];   		//M为所有的除数相乘
    for(int i=1; i<=cn; i++){  
        int Mi = M / m[i];  
        int x = mod_reverse(Mi, m[i]);  //求Mi在%m[i]意义下的逆元
        ans = (ans + Mi * x * a[i]) % M; //最后求出的结果在1-M之间  
        //注意此处的Mi*x*a[i]如果有爆long long的风险，则需要调用快速乘来进行乘法运算
    }  
    if(ans < 0) ans += M;  //ans是特解，ans的通解 = ans+K*M（K为整数）
    return ans;  
}  