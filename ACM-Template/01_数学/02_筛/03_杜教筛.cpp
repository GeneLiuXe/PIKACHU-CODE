// ---
// 给定一个n $(n\leq 2^31-1)$, 求 $ans_1$ 和 $ans_2$ \\
// $ans_1=\sum\limits_{i=1}^{n}\varphi(i)$ \\ 
// $ans_2=\sum\limits_{i=1}^{n}\mu(i)$ \\
// ---
#include<bits/stdc++.h>
#include<tr1/unordered_map>
#define N 6000010
using namespace std;
template<typename T>inline void read(T &x)
{
    x=0;
    static int p;p=1;
    static char c;c=getchar();
    while(!isdigit(c)){if(c=='-')p=-1;c=getchar();}
    while(isdigit(c)) {x=(x<<1)+(x<<3)+(c-48);c=getchar();}
    x*=p;
}
bool vis[N];
int mu[N],sum1[N],phi[N];
long long sum2[N];
int cnt,prim[N];
tr1::unordered_map<long long,long long>w1;
tr1::unordered_map<int,int>w;
void get(int maxn)
{
    phi[1]=mu[1]=1;
    for(int i=2;i<=maxn;i++)
    {
        if(!vis[i])
        {
            prim[++cnt]=i;
            mu[i]=-1;phi[i]=i-1;
        }
        for(int j=1;j<=cnt&&prim[j]*i<=maxn;j++)
        {
            vis[i*prim[j]]=1;
            if(i%prim[j]==0)
            {
                phi[i*prim[j]]=phi[i]*prim[j];
                break;
            }
            else mu[i*prim[j]]=-mu[i],phi[i*prim[j]]=phi[i]*(prim[j]-1);
        }
    }
    for(int i=1;i<=maxn;i++)sum1[i]=sum1[i-1]+mu[i],sum2[i]=sum2[i-1]+phi[i];
}
int djsmu(int x)
{
    if(x<=6000000)return sum1[x];
    if(w[x])return w[x];
    int ans=1;
    for(int l=2,r;l>=0&&l<=x;l=r+1)
    {
        r=x/(x/l);
        ans-=(r-l+1)*djsmu(x/l);
    }
    return w[x]=ans;
}
long long djsphi(long long x)
{
    if(x<=6000000)return sum2[x];
    if(w1[x])return w1[x];
    long long ans=x*(x+1)/2;
    for(long long l=2,r;l<=x;l=r+1)
    {
        r=x/(x/l);
        ans-=(r-l+1)*djsphi(x/l);
    }
    return w1[x]=ans;
}
int main()
{
    int t,n;
    read(t);
    get(6000000);
    while(t--)
    {
        read(n);
        printf("%lld %d\n",djsphi(n),djsmu(n));
    }
    return 0;
}