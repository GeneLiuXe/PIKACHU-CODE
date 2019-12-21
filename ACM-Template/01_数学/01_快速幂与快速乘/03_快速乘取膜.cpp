// ---
// 方法一：\\
//    a b p 范围都在1e18\\
//    求a*b的结果\\
//    为了不让结果爆ll，则将a进行二进制分解，然后递推*b即可\\
// \\
// 方法二：\\
//    用long double存数, long double的有效数字有18-19位\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
typedef long long ll;

ll mul(ll a,ll b,ll p)
{
    a %= p, b %= p;
    long long c = (long double)a*b/p;
    long long ans = a*b-c*p;
    //此处会溢出，但是由于二者相差的部分不会太大，所以前面的数字都是一样的
    //因此溢出不但不影响答案，还是符合我们的要求的
    if(ans < 0) ans+=p;
    else if(ans >= p) ans-=p;
    return ans;
}

int main()
{
    ll a,b,p;
    while(~scanf("%lld%lld%lld",&a,&b,&p))
    {
        printf("%lld\n",mul(a,b,p));
    }
    return 0;
}