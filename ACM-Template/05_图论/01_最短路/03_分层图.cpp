// ---
// 题意：\\
// \quad 给一张有向图，给一个k，可以将图中k条边的边权变为0，求从1->n的最短路径\\
// \\
// 解法：\\
// \quad 由于 k<=10，由此考虑分层图做法 \\
// \quad 给每个点建10个分身，dis[i][j]表示从城市1到达城市i，一共改了j条边的最短路径 \\
// \quad 其余操作和正常最短路求法一致，更新的时候有两种更新方法。假设队列弹出的节点是dis[i][j]，y为i可达到的点 \\
// \quad 则用dis[i][j]去更新dis[y][j]和dis[y][j+1]，相当于扩点操作，其余操作和dij模板一致 \\
// \quad 如此操作即可求出答案 \\
// ---
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <queue>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int N = 1e5+100;
const int M = 5*1e5+10;
typedef long long ll;
const ll inf = 1e15;

struct Edge{
	int to,next,w;
}e[M];
int n,m,k,head[N],tot;
ll dis[N][15];
int vis[N][15];

void init()
{
	tot = 1;
	rep(i,0,n) head[i] = 0;
}

void add(int x,int y,int z)
{
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot, e[tot].w = z;
}

struct Point{
	ll ans;
	int u,cnt;
}thp;

bool operator < (Point a,Point b)
{
	return a.ans > b.ans;
}

priority_queue<Point> q;

void dijstra(int s)
{
	while(q.size()) q.pop();
	thp.ans = 0, thp.u = s, thp.cnt = 0;
	q.push(thp);
	rep(i,1,n)
		rep(j,0,k) vis[i][j] = 0;
	rep(i,1,n) 
		rep(j,0,k) dis[i][j] = inf;
	dis[s][0] = 0;
	while(q.size())
	{
		thp = q.top();
		q.pop();
		int u = thp.u;
		int x = thp.cnt;
		if(vis[u][x]) continue;
		vis[u][x] = 1;
		for(int i = head[u]; i ; i = e[i].next)
		{
			int y = e[i].to;
			if(!vis[y][x]){
				if(dis[y][x] > dis[u][x]+e[i].w){
					dis[y][x] = dis[u][x] + e[i].w;
					thp.u = y, thp.ans = dis[y][x], thp.cnt = x;
					q.push(thp);
				}
			}
			if(!vis[y][x+1] && (x < k)){
				if(dis[y][x+1] > dis[u][x]){
					dis[y][x+1] = dis[u][x];
					thp.u = y, thp.ans = dis[y][x+1], thp.cnt = x+1;
					q.push(thp);
				}
			}
		} 	
	}
}

int main()
{
	int T;
	scanf("%d",&T);
	while(T--)
	{
		init();
		scanf("%d%d%d",&n,&m,&k);
		rep(i,1,m)
		{
			int x,y,z;
			scanf("%d%d%d",&x,&y,&z);
			add(x,y,z);
		}
		dijstra(1);
		printf("%lld\n",dis[n][k]);
	}	
	return 0;
}