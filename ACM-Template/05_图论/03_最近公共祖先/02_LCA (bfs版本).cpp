#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <queue>
#include <cmath>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int N = 50010;

int f[N][20],d[N],dis[N];  //f[x][k]:表示点x向根节点走2^k步到达的点
//d[x]:表示点x在图中的深度	dis[x]:表示根节点到点x的距离
struct Edge{
	int to,next,w;
}e[2*N];		//树的边为n-1，此处是因为无向边，所以正反各一条
int head[N],tot,t,n,m; //n个点，m次询问

void init()
{
	tot = 1;
	memset(head,0,sizeof head);
	memset(d,0,sizeof d);
}

void add(int x,int y,int z)
{
	e[++tot].to = y; e[tot].next = head[x]; e[tot].w = z; head[x] = tot;
}

void bfs()
{
	queue<int> q;
	while(q.size()) q.pop();
	q.push(1); d[1] = 1; dis[1] = 0;	//把1当做树根
	while(q.size())
	{
		int x = q.front(); q.pop();
		for(int i = head[x]; i ;i = e[i].next){
			int y = e[i].to;
			if(d[y]) continue;
			d[y] = d[x]+1;
			dis[y] = dis[x]+e[i].w;	//dist[y]:从1到y的距离
			f[y][0] = x;  //y走2^0步到达x
			for(int j = 1; j <= t;j++)
				f[y][j] = f[f[y][j-1]][j-1];
			q.push(y);
		}
	}
}

/*
	这里有一个非常容易错的点，因为每一个点的父节点初始化为0，所以如果你的点从0-n编号的话，一定要注意bfs的时候将0作为根节点
	否则如果用1作为根节点的话，你会发现1的父节点是0，那么程序就开始出错了！

	所以更好的方法是以后给点编号的时候，统一用1-n编号，不要用0编号，否则极易出错

	从1-n编号的话，可以从任意起点拉起一棵树，但是要注意题目中n的最小值
*/

int lca(int x,int y)
{
	if(d[x] > d[y]) swap(x,y);
	for(int i = t; i >= 0; i--)
		if(d[f[y][i]] >= d[x]) y = f[y][i];  //往上追溯，直至y和x位于同一深度
	if(x == y) return x;  //如果已经找到了，就返回x
	for(int i = t; i >= 0; i--)
		if(f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];  //x和y同时往上走，一直到x和y恰好为lca的子节点
	return f[x][0];  //x和y共同的根节点就是lca 
}

int main()
{
	int T;
	scanf("%d",&T);
	while(T--)
	{
		scanf("%d%d",&n,&m);
		t = (int)(log(n)/log(2))+1;
		init();
		rep(i,1,n-1){
			int x,y,z;
			scanf("%d%d%d",&x,&y,&z);
			add(x,y,z), add(y,x,z);
		}
		bfs();
		for(int i = 1;i <= m;i++)
		{
			int x,y;
			scanf("%d%d",&x,&y);
			printf("%d\n",dis[x]+dis[y]-2*dis[lca(x,y)]);
		}
	}
	return 0;
}