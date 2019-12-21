// ---
// 题意：\\
// $N*N$ 的矩阵中有 $K$ 个小行星，现在每行每列都有一个武器，可以消除此行或者此列中的所有小行星，问最少需要多少个武器可以将矩阵中所有小行星消除。\\
// \\
// 思路：\\
// 这是一个典型的最小点覆盖问题，可以用二分图匹配算法或者最大流算法进行解决 。我们主要来讨论网络流的做法。\\
// \\
// 先来回顾一下最大流 $Dinic$ 的基础性质，先在残量网络上 $BFS$ 求出所有节点的层次，构造了一个分层图。然后在分层图上 $DFS$ 寻找增广路，在回溯时实时更新剩余容量。当在残量网络中 $S$ 不能到达 $T$ 时，算法结束。\\
// \\
// 因此最大流这个算法有个很重要的性质，即==最大流 $=$ 最小割 $=$  最大匹配 $=$ 最小点覆盖==，基本覆盖了大部分最大流应用的题目。\\
// \\
// 现在我们再回过头来看这题如何建图，这题就是用最少的点覆盖所有的边，典型的最小点覆盖题目。因此我们将其转化为最大匹配，即每个点只能选择一次，最多选择多少条边。因此对于一个在 $(i,j)$ 位置的行星，将左边第 $i$ 个点和右边第 $j$ 个点相连，图中所有边流量均设为 $1$，跑最大流即可。\\
// \\
// 最后再来一个不严谨的 "最大匹配 $=$ 最小点覆盖" 的证明。即当图中选的边已经达到了最大匹配之后，一定不存在一条未选的边，边的两个端点同时都没有被选。并且由于每个点只能被选择一次，因此每条边都代表只选了一个点，而这些点的集合覆盖了所有的边。不太严谨的证明，只是有利于记忆。详细证明可以网上继续查阅题解。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue> 
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int inf = 1<<29,N = 1000+10,M = 300500;  //处理1e4-1e5规模的网络

struct Edge{ 
	int to,next,v;
}e[M];
int n,m,s,t,k;  //顶点个数 边数 源点 汇点 
int head[N],tot,dis[N],mp[N][N];
queue<int> q;

void init()   //千万别忘了初始化！
{
	tot = 1; memset(head,0,sizeof head);  //点的编号是2~n，因为2^1 = 3, 3^1 = 2;  符合后续代码的操作 
}

void add(int x,int y,int v)
{
	e[++tot].to = y; e[tot].next = head[x]; e[tot].v = v; head[x] = tot;
	e[++tot].to = x; e[tot].next = head[y]; e[tot].v = 0; head[y] = tot;  //反向边与正向边的流量之和为v 
}

bool bfs()
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

int dinic(int x,int flow) //找增广路 
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
		while((flow = dinic(s,inf))) maxflow += flow;	
	return maxflow;
}

int main()
{
	while(~scanf("%d%d",&n,&k))
	{
		init();
		s = 1, t = 1+n+n+1;
		rep(i,1,k){
			int xx,yy; scanf("%d%d",&xx,&yy);
			add(xx+1,yy+1+n,1);
		}
		rep(i,1,n) add(s,i+1,1), add(1+n+i,t,1);
		printf("%d\n",solve());
	}
	return 0;
}