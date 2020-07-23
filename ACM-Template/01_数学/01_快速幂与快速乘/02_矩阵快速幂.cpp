#include <cstdio>
#include <iostream>
#include <cstring>
using namespace std; 
const int mod = 9973;
typedef long long ll;

int n,m,t;
//i、k、j只能作为临时变量 
struct Matrix{
	ll a[35][35];
	Matrix(){memset(a,0,sizeof(a));}
	
	Matrix operator * (const Matrix y)
	{
		Matrix ans;
		for(int i = 1;i <= n;i++)   //行 
			for(int j = 1;j <= n;j++)  //列 
				for(int k = 1;k <= n;k++)
					ans.a[i][j] = (ans.a[i][j]+a[i][k]*y.a[k][j])%mod;			
		return ans;
	}
	
	Matrix operator + (const Matrix y)
	{
		Matrix ans;
		for(int i = 1;i <= n;i++)
			for(int j = 1;j <= n;j++)
				ans.a[i][j] = (a[i][j]+y.a[i][j])%mod;
		return ans;
	}
};

Matrix q_pow(Matrix x,int k)
{
	Matrix ans;
	for(int i = 1;i <= n;i++)	ans.a[i][i] = 1;
	while(k)
	{
		if(k&1)
			ans = ans*x;
		x = x*x;
		k = k>>1;
	}
	return ans;
}

int main()
{
	cin>>t;
	while(t--)
	{
		scanf("%d%d",&n,&m);
		Matrix p;
		for(int i = 1;i <= n;i++)
			for(int j = 1;j <= n;j++)
				scanf("%lld",&p.a[i][j]);
		Matrix ans = q_pow(p,m);
		long long ans1 = 0;
		for(int i = 1;i <= n;i++)
			ans1 = (ans.a[i][i]+ans1)%mod;
		printf("%lld\n",ans1);
	}
	return 0;
}
