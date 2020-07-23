#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <queue>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int M = 1e5+100;
const int N = 2000;
const int inf = 0x3f3f3f3f;

int n,m,s,t,k;
struct Edge{
	int to,next,w;
}e[M],ef[M];
int head[N],headf[N],tot,totf;
int dis[N],book[N],dist[N];
struct Po{
	int id;	//到达哪一个点
	int fw; //到该点已经走过的距离
	int w; //到该点已经走过的距离+该点距离终点的距离
}tmp;

bool operator < (Po x,Po y)	//优先队列只能重载 < 
{
	return x.w>y.w;	//如果放在结构体内重载，需要加const
}
void init()
{
	tot = 1; totf = 1;
	memset(head,0,sizeof head);
	memset(headf,0,sizeof headf);
}

void add(int x,int y,int w)
{
	e[++tot].to = y; e[tot].next = head[x]; head[x] = tot; e[tot].w = w;
}

void addf(int x,int y,int w)
{
	ef[++totf].to = y; ef[totf].next = headf[x]; headf[x] = totf; ef[totf].w = w;
}

void spfa(int x)
{
	queue<int> q;
	memset(book,0,sizeof book);
	rep(i,1,n) dis[i] = inf;
	while(!q.empty()) q.pop();
	q.push(x);
	book[x] = 1;
	dis[x] = 0;
	while(!q.empty())
	{
		int p = q.front();
		for(int i = headf[p]; i ; i = ef[i].next)
		{
			if(dis[ef[i].to] > dis[p]+ef[i].w)
			{
				dis[ef[i].to] = dis[p]+ef[i].w;
				if(book[ef[i].to] == 0)
				{
					q.push(ef[i].to);
					book[ef[i].to] = 1;
				}
			}
		}
		book[p] = 0;
		q.pop();
	}
}

int bfs()
{
	if(dis[s] == inf) return -1;	//如果不加这句话，就是wa，确保连通性
	int val = 0;
	priority_queue<Po> q;
	while(!q.empty()) q.pop();
	tmp.fw = 0;	//走到id这个点目前所需要的距离
	tmp.id = s;
	tmp.w = tmp.fw+dis[tmp.id];
	q.push(tmp);
	if(t==s)k++;	//这里也是一个大坑点
	while(!q.empty())
	{
		Po p = q.top();
		q.pop();	//优先队列，必须在push之前就pop，不然队列顶的那个点会被调到底下去
		int id = p.id;	//p所在的点
		if(id == t) val++;
		if(val == k) return p.fw;	//求第k短路
		for(int i = head[id]; i ; i = e[i].next)	//加入新边
		{
			tmp.fw = p.fw+e[i].w;
			tmp.id = e[i].to;
			tmp.w = tmp.fw+dis[tmp.id];	//此处为估价函数
			q.push(tmp);
		}
	}
	if(val!=k){return -1;}
}

int main()
{
	while(~scanf("%d%d",&n,&m))
	{
		init();
		rep(i,1,m)
		{
			int x,y,z;
			scanf("%d%d%d",&x,&y,&z);
			add(x,y,z);
			addf(y,x,z);
		}
		scanf("%d%d%d",&s,&t,&k);
		spfa(t);
		printf("%d\n",bfs());
	}
	return 0;
}