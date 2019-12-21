//spfa判断负环 
#include <cstdio>
#include <iostream>
#include <queue>
#include <cstring>
using namespace std;
const int maxn = 105;
const int inf = 0x3fffffff;

bool p[maxn][maxn];
bool f[maxn][maxn];
int n, pow[maxn], book[maxn], cnt[maxn], dis[maxn];

void Floyd() //判连通
{
	for(int k = 1;k <= n;k++)
		for(int i = 1;i <= n;i++)
			for(int j = 1;j <= n;j++)
				f[i][j] = f[i][j] || (f[i][k] && f[k][j]);
}

bool spfa(int x)
{
	memset(book,0,sizeof(book));
	memset(cnt,0,sizeof(cnt));
	memset(dis,0,sizeof(dis));
	queue<int>q;
	q.push(x);
	book[x] = 1;
	dis[x] = 100;
	cnt[x]++;
	while(!q.empty())
	{
		x = q.front();
		q.pop();
		if(cnt[x] > n) continue;   //如果一个点进入队列次数>n，就不能用这个点去松弛，否则会TLE 
		for(int i = 1;i <= n;i++)
		{
			if(p[x][i] && dis[i] < pow[x]+dis[x] && book[i] == 0 && (pow[x]+dis[x]) > 0)
			{
				q.push(i);
				dis[i] = pow[x]+dis[x];
				cnt[i]++;
				book[i] = 1;
				if(cnt[i] >= n) dis[i] = inf;	//判断负环 
			}	
		}
		book[x] = 0;	
	}	
	return dis[n] > 0;
}

int main()
{
	while(~scanf("%d",&n) && n!=-1)
	{
		memset(p,0,sizeof(p));
		memset(f,0,sizeof(f));
		for(int i = 1;i <= n;i++)
		{
			int k;
			scanf("%d%d",&pow[i],&k);
			for(int j = 1;j <= k;j++)
			{
				int tmp;
				scanf("%d",&tmp);
				p[i][tmp] = true;
				f[i][tmp] = true;
			}
		}
		Floyd();
		if(spfa(1) && f[1][n])
			printf("winnable\n");
		else
			printf("hopeless\n");
	}
	return 0;
}
