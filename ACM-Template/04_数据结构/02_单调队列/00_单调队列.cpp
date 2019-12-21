// ---
// 题意：\\
// \\
// 给定一串01串，m次询问，每次询问给你一个数k。k为对于这个01串所能进行的最多次操作，每次操作可以将该串中任意一个位置的数移到任意一个其他的位置。\\
// \\
// 每次询问之后，输出在这个操作数之内，所能达到的最长的连续0的长度。\\
// ---
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int N = 1e6+1000;

char s[N];
int m,n,a[N],sum[N],q[N],cnt;

void solve(int k)
{
	int ans = 0;
	q[1] = 0;
	int l = 1, r = 1;
	rep(i,1,n)
	{	
		while(l <= r && a[q[r]] >= a[i]) r--;	//维护单调队列，先对右端点进行更新
		q[++r] = i;
		while(l <= r && sum[i]-sum[q[l]] > k) l++;	//再对左端点进行更新
		ans = max(ans,a[q[r]]-a[q[l]]+k);
	}
	printf("%d\n",min(ans,cnt));
}

int main()
{
	while(~scanf("%s",s))
	{
		cnt = 0;
		scanf("%d",&m);
		int len = strlen(s);
		n = len;
		sum[0] = 0;a[0]=0;
		rep(i,1,len)
		{
			if(s[i-1] == '0'){
				sum[i] = sum[i-1];
				cnt++;
			} 
			else sum[i] = sum[i-1]+1;
			a[i] = i-2*sum[i];

		}
		rep(i,1,m)
		{	
			int x;
			scanf("%d",&x);
			solve(x);
		}
	}
	return 0;
}