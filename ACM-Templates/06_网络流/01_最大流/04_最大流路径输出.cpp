// ---
// 题意：\\
// 给出$n$个起重机，每个起重机有两个属性，$W[i]$表示这个起重机的重量，$L[i]$表示这个起重机能够拉起的最大重量（可以拉重物也可以拉起重机）。现在有$m$栋楼以及$m$个重物，要求给出每栋楼起重机的分配方案，使得每栋楼最后留下的起重机可以拉起对应的重物。$(1\leq n\leq 100, 1\leq M\leq 100)$\\
// \\
// 思路：\\
// 比赛的时候以为所有的起重机都得用上...然后就想到了费用流...然后就是$wa\ wa\ wa$...其实这是一道比较简单的最大流问题，主要难点应该在路径输出。\\
// \\
// 我们先来考虑下如何建图。首先对于$n$个起重机进行拆点，拆点的目的是化边权为点权来限制流量。然后将源点$s$和所有$W[i]=0$的点的入点相连，流量为$1$。每个拆开的点的入点和出点连一条流量为$1$的边。对于点$x$的出点与点$y$的入点，$x$与$y$相连（流量为$1$），当且仅当$W[y]\leq L[x]$。\\
// \\
// 再将$m$个重物与汇点相连，流量为$1$。最后对于每个重物，将所有$L[i]$大于重物重量的点与重物相连，流量为$1$。然后直接跑最大流即可。如果最大流为$m$，则输出路径，如果小于m，则输出$impossible$。\\
// \\
// 最后考虑本题最关键的步骤，求出最大流之后如何输出路径。\\
// \\
// 对于$m$个点，通过反向边往回跑，仅当反向边流量不为$0$，而正向边流量为$0$时才走这条边，跑到源点即停止，进行$m$遍$dfs$即可。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue> 
#include <vector>
#define rep(i,a,b) for(int i = a;i <= b;i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
using namespace std;
const int inf = 1<<29,N = 1000,M = 1e5;  //处理1e4-1e5规模的网络

struct Edge{ 
	int to,next,v,f;
}e[M];
int n,m,s,t;  //顶点个数 边数 源点 汇点 
int head[N],tot,dis[N],L[N],W[N],T[N];
queue<int> q;
vector<int> ans[N];

void init()   //千万别忘了初始化！
{
	tot = 1; memset(head,0,sizeof head);  //点的编号是2~n，因为2^1 = 3, 3^1 = 2;  符合后续代码的操作 
}

void add(int x,int y,int v)
{
	e[++tot].to = y; e[tot].next = head[x]; e[tot].v = v; head[x] = tot; e[tot].f = 1;
	e[++tot].to = x; e[tot].next = head[y]; e[tot].v = 0; head[y] = tot; e[tot].f = -1;  //反向边与正向边的流量之和为v 
}

bool bfs()	//判断是否有增广路径
{
	memset(dis,0,sizeof dis);
	while(!q.empty()) q.pop();
	q.push(s); dis[s] = 1;
	while(!q.empty())
	{
		int x = q.front(); q.pop();
		for(int i = head[x];i;i = e[i].next)
		{
			if(e[i].v && !dis[e[i].to]){
				q.push(e[i].to);
				dis[e[i].to] = dis[x]+1;
				if(e[i].to == t) return 1;  //找到一条路就return 
			}
		}
	}
	return 0;
}

int dinic(int x,int flow) //dfs找增广路, 找到一条路, 就把增广的流量加到答案中
{	
	if(x == t) return flow;
	int rest = flow,k;  //rest为输入的流量 
	for(int i = head[x];i && rest; i = e[i].next)
	{
		if(e[i].v && dis[e[i].to] == dis[x]+1){
			k = dinic(e[i].to,min(rest,e[i].v));
			if(!k) dis[e[i].to] = 0;  //剪枝，去掉增广完毕的点 
			e[i].v -= k;
			e[i^1].v += k;  //反向边加上flow，相当于我们可以反悔从这条路流过 
			rest -= k; //k为能够被送出去的流量 
		}
	}
	return flow-rest;  //总共被送出去了多少流量 
}

int solve()
{
	int flow = 0,maxflow = 0;
	while(bfs())
		while((flow = dinic(s,inf))) maxflow += flow;	//while 循环内容 —— 判断是否还有增广路
	return maxflow;
}

void dfs(int x,int flag){ //路径输出
	if(1 < x && x <= 1+n) ans[flag].push_back(x-1);
	if(x == s) return;
	for(int i = head[x]; i; i = e[i].next)
		if(e[i^1].v == 0 && e[i].to <= 1+2*n && e[i].f == -1) dfs(e[i].to,flag);
}

int main()
{
	scanf("%d",&n); tot = 1;
	rep(i,1,n) scanf("%d%d",&W[i],&L[i]);
	scanf("%d",&m);
	rep(i,1,m) scanf("%d",&T[i]);
	s = 1, t = 2+2*n+m;
	rep(i,1,m) add(1+2*n+i,t,1);
	rep(i,1,n) 
		if(W[i] == 0) add(s,i+1,1);
	rep(i,1,n) add(i+1,1+n+i,1);
	rep(i,1,n)
		rep(j,1,n)
			if(i != j && L[i] >= W[j]) add(i+1+n,1+j,1);
	rep(i,1,m)
		rep(j,1,n)
			if(L[j] >= T[i]) add(1+n+j,1+2*n+i,1);
	int maxflow = solve();
	if(maxflow != m) printf("impossible\n");
	else{
		rep(i,1,m)
			dfs(1+2*n+i,i);
		rep(i,1,m){
			for(int j = ans[i].size()-1; j >= 0; j--){
				printf("%d",ans[i][j]);
				if(j == 0) printf("\n");
				else printf(" ");
			}
		}
	}
	return 0;
} 