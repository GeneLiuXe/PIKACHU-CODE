//最短路 —— spfa
#include <cstdio>
#include <iostream>
#include <queue>
#include <algorithm>
#include <cstring>
using namespace std;

const int maxn = 1000+10;
const int maxm = 4000+10;
const int inf = 0x3fffffff;

struct edge{
	int u,v,w;
}e[maxm];
int book[maxn],first[maxn],next[maxm];	
//book用来标记		first[i]指以节点i为出发点的边的编号 
int dis[maxn];
int n,m;
queue<int>q;		//用以保存节点的队列 

//x为要求从哪一个点出发 
void spfa(int x)
{
	memset(book,0,sizeof(book));
	for(int i = 1;i <= n;i++) dis[i] = inf;   //距离赋成无穷大 
	while(!q.empty()) q.pop();
	q.push(x);   
	book[x] = 1;  
	dis[x] = 0;
	while(!q.empty())
	{
		int k = first[q.front()];
		while(k!=-1)   //遍历图 
		{
			if(dis[e[k].v] > dis[e[k].u]+e[k].w)   //用队列中的边去松弛两端的点 
			{
				dis[e[k].v] = dis[e[k].u]+e[k].w;  //松弛成功 
				if(book[e[k].v] == 0)   //把松弛成功的点加入队列 
				{
					q.push(e[k].v);
					book[e[k].v] = 1;   //标记这个点在队列中了 
				}
			}
			k = next[k];  //遍历这个点的下一条边 
		}
		book[q.front()] = 0;   //将这个点出队列 
		q.pop();
	}
}

int main()
{
	cin>>m>>n;
	memset(first,-1,sizeof(first));
	for(int i = 1;i <= m;i++)
	{
		scanf("%d%d%d",&e[i].u,&e[i].v,&e[i].w);
		//双向边的保存 
		e[m+i].u = e[i].v;
		e[m+i].v = e[i].u;
		e[m+i].w = e[i].w;
		next[i] = first[e[i].u];   //表示e[i].u这个点当前这条边的上一条边 
		first[e[i].u] = i;         //表示e[i].u这个点现在的边  
		next[m+i] = first[e[i+m].u];
		first[e[i+m].u] = i+m;
	}
	spfa(1);
	cout<<dis[n]<<endl;
	return 0;
}
