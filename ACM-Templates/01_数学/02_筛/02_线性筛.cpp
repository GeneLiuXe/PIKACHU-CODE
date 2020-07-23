#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
const int N = 1e7+1000;

int v[N],prime[N],tot;	//v[i]：i的最小质因子

void primes(int x)	//线性筛
{
	memset(v,0,sizeof v);
	tot = 0;
	rep(i,2,x){
		if(!v[i]) v[i] = i, prime[++tot] = i;
		rep(j,1,tot)
			if(prime[j] > v[i] || prime[j] > x/i) break;
			else v[i*prime[j]] = prime[j];
	}
}

int main()
{
	int n = 1e5;
	primes(n);
	rep(i,1,tot) printf("%d\n",prime[i]);
	return 0;
}