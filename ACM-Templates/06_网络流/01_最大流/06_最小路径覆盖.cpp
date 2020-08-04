// ---
// 题意：\\
// 给出一个 $n$ 个点，$m$ 条边的 $DAG$，现要在该图中找到最少的不相交路径，求出具体路径数。$(1\leq n\leq 150,1\leq m\leq 6000)$\\
// \\
// 思路：\\
// 最小路径覆盖问题，如果是第一次遇到该问题，可以先排除费用流的做法，因为总流量不固定，也正因为总流量不固定，我们采取最大流算法。\\
// \\
// 将一个点拆成两个点，若存在边 $(x,y)$，则将左边的 $x$ 连向右边的 $y$，最后答案为 $ans = n-maxflow$。接下来给出该算法的大致证明，在构建的图中，若选择了边 $(x,y)$，则意味着在原图中将这两点放在了一条路径中，相当于这两点所在的路径发生了合并，因此总路径数 $-1$，即最终 $ans=n-maxflow$。\\
// \\
// 此题还可以扩展到最少相交路径问题（POJ-2594）。而处理最少相交路径只需要将连边条件 $(x,y)$ 从原图中存在边 $(x,y)$ 改成原图中 $(x,y)$ 可达即可，即可以跨越一个点进行路径合并。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue> 
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int inf = 1<<29,N = 2000+10,M = 300500;  //处理1e4-1e5规模的网络

struct Edge{ 
	int to,next,v;
}e[M];
int n,m,s,t;  //顶点个数 边数 源点 汇点 
int head[N],tot,dis[N],mp[N][N];
queue<int> q;

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

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

int to[N],deg[N];

int main()
{
	scanf("%d%d",&n,&m);
	init();
	s = 2*n+1, t = 2*n+2;
	int ans = 0;
	rep(i,1,m){
		int x,y; scanf("%d%d",&x,&y);
		add(x,n+y,1);
	}
	rep(i,1,n) add(s,i,1), add(i+n,t,1);
	ans = solve();
	rep(i,2,tot){
		int x = e[i^1].to, y = e[i].to;
		if(x <= n && y <= 2*n && e[i].v == 0){
			to[x] = y-n;
			deg[y-n]++;
		}
	}
	rep(i,1,n){
		if(!deg[i]){
			int pos = i;
			while(pos){
				printf("%d",pos);
				pos = to[pos];
				if(pos == 0) printf("\n");
				else printf(" ");
			}
		}
	}
	printf("%d\n",n-ans);
	return 0;
}