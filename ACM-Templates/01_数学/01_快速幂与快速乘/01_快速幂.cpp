//快速幂
#include <cstdio>
#include <iostream>
typedef long long LL;
using namespace std;

LL poww(LL a,LL b)
{
	LL base = a,ans = 1;
	while(b!=0)
	{
		if(b&1)
			ans *= base;
		base *= base;
		b >>= 1;
	}
	return ans;
}

LL poww(LL a,LL b,LL p)
{
	LL base = a%p,ans = 1;
	while(b!=0)
	{
		if(b&1)
			ans = (ans*base)%p;
		base = (base*base)%p;
		b >>= 1;
	}
	return (ans%p);
}

int main()
{
	LL h = poww(11,11);
	cout<<h<<endl;
	return 0;	
} 