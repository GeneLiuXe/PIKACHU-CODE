//拓展中国剩余定理 —— m1,m2,m3之间不互质
#include<iostream>
#include<cstdio>
#include<climits>
#include<cstring>
#include<algorithm>
using namespace std;
#define LL long long
const int maxn=1e5+5;
int n;
LL exgcd(LL a,LL b,LL &x,LL &y){
    if(!b){x=1,y=0;return a;}
    LL re=exgcd(b,a%b,x,y),tmp=x;
    x=y,y=tmp-(a/b)*y;
    return re;
}
LL m[maxn],a[maxn];
LL work(){
    LL M=m[1],A=a[1],t,d,x,y;int i;
    for(i=2;i<=n;i++){
        d=exgcd(M,m[i],x,y);//解方程
        if((a[i]-A)%d)return -1;//无解
        x*=(a[i]-A)/d,t=m[i]/d,x=(x%t+t)%t;//求x
        A=M*x+A,M=M/d*m[i],A%=M;//日常膜一膜（划掉）模一模，防止爆
    }
    A=(A%M+M)%M;
    return A;
}
int main()
{
    int i,j;
    while(scanf("%d",&n)!=EOF){
        for(i=1;i<=n;i++)scanf("%lld%lld",&m[i],&a[i]);
        printf("%lld\n",work());
    }
    return 0;
}