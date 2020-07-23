#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int N = 1.5*1e7+1000;

int b[N],c[N][2]; //c[i][0] —— 表示第 i 个唯一分解数
//c[i][1] —— 表示第 i 个唯一分解数的指数
int prime[N],tot;
int n;

//唯一分解
void getFactors(ll x)
{
	ll tmp = x;
	for(ll i = 1; prime[i]*prime[i] <= tmp; i++)
	{
		if(tmp%prime[i] == 0)
		{
			c[++tot][0] = prime[i]; 
			while(tmp%prime[i] == 0){
				tmp /= prime[i];
				c[tot][1]++;
			}
		}
	}
	if(tmp != 1){
		c[++tot][0] = tmp;
		c[tot][1] = 1;
	}
}

int main()
{
	scanf("%d",&n);
	tot = 0;
	
	//埃氏筛
	for(int i = 2; i <= 1e5; i++)
	{
		if(b[i] == 0)
			prime[++tot] = i;
		else continue;
		int base = i;
		while(base <= 1e5){
			base += i;
			b[base] = 1;
		}
	}
	rep(i,1,n)
	{
		int x;
		scanf("%d",&x);
		getFactors(x);
	}
	return 0;
}