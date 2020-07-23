#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
const int N = 20;
const double eps = 1e-7;
double coe[N];
int n;

double func(double x)
{
	double ans = 0;
	double base = 1;
	rep(i,0,n)
	{
		ans += base*coe[i];
		base *= x;
	}
	return ans;
}

void solve(double l, double r)
{
	while(l + eps < r)
	{
		double m1 = (2*l+r)/3, m2 = (2*r+l)/3;
		double r1 = func(m1), r2 = func(m2);
		if(r1 >= r2)
			r = m2;
		else 
			l = m1;
	}
	printf("%.5f\n",r);
}

int main()
{
	double l,r;
	scanf("%d",&n);
	scanf("%lf%lf",&l,&r);
	for(int i = n; i >= 0; i--)
		scanf("%lf",&coe[i]);
	solve(l,r);
	return 0;
}