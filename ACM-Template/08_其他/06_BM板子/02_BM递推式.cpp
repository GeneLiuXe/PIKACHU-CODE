#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f
#define rep(i,a,b) for(int i=a;i<=b;i++)
#define per(i,a,b) for(int i=a;i>=b;i--)
#define ll long long
#define VI vector<ll>
const int MAXN = 105;
const int MOD = 1e9+7;
int a[5]={2,3,7};
int b[MAXN];
const int N = 50010;///多项式系数最大值
int64_t res[N],c[N],md[N],COEF[N]/**COEF是多项式系数*/,Mod=10000007;
vector<ll> Md;

inline static int64_t gcdEx(int64_t a, int64_t b, int64_t&x, int64_t& y)
{
    if(!b) {x=1;y=0;return a;}
    int64_t d = gcdEx(b,a%b,y,x);
    y -= (a/b)*x;
    return d;
}

static int64_t Inv(int64_t a, int64_t Mod) {
    int64_t x, y;
    return gcdEx(a, Mod, x, y)==1?(x%Mod+Mod)%Mod:-1;
};

VI BM(VI s) {///BM算法求模数是质数的递推式子的通项公式,可以单独用
        VI C(1,1),B(1,1);
        int L(0),m(1),b(1);
        for(size_t n(0);n<s.size();++n) {
            int64_t d(0);
            for(int i(0);i<=L;++i) d=(d+C[i]*s[n-i])%Mod;
            if (!d) ++m;
            else {
                VI T(C);
                int64_t c(Mod-d*Inv(b,Mod)%Mod);
                while (C.size()<B.size()+m) C.push_back(0);
                for (size_t i(0);i<B.size();++i)
                    C[i+m]=(C[i+m]+c*B[i])%Mod;
                if (2*L<=(int)n) {L=n+1-L; B=T; b=d; m=1;}
                else ++m ;
            }
        }
        ///下边这样写能够输出递推式的系数.
        printf("F[n] = ") ;
        for(size_t i(0);i<C.size();++i) {
            COEF[i+1] = min((int64_t)C[i],Mod-C[i]) ;
            if(i>0) {
                if(i != 1) printf(" + ") ;
                printf("%lld*F[n-%d]",COEF[i+1],i) ;
                putchar(i+1==C.size()?'\n':' ') ;
            }
        }
        return C;
    }
int main()
{
    VI v = {0,1,2,4,11,25} ;
    BM(v);
    return 0;
}