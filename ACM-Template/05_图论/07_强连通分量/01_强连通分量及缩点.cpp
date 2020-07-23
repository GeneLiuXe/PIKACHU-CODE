// ---
// 求有向图中的强连通分量\\
// 将SCC中的强连通分量进行缩点\\
// \\
// 求解算法：\\
// \quad 1.当节点x第一次被访问时，把x入栈，初始化low[x] = dfn[x]\\
// \quad 2.扫描从x出发的每条边(x,y)\\
// \quad \quad(1)若y没被访问过，则说明(x,y)是树枝边，递归访问y，从y回溯之后，令low[x] = min(low[x],low[y]).\\
// \quad \quad(2)若y被访问过并且y在栈中，即y被访问过，并且y不属于之前的强连通分量，则令low[x] = min(low[x],dfn[y]).\\
// \quad 3.从x回溯之前，判断是否有low[x] = dfn[x]。若成立，则不断从栈中弹出节点，直至x出栈.\\
// ---
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int N = 1e5+10, M = 1e6+10;

struct Edge{
	int to,next;
}e[M],ec[M];
int head[N],dfn[N],low[N];	//dfn:dfs的序号	low:该点能够到达的最小的dfn值的点
int stack[N],vis[N],c[N];	//stack:维护通过点x到达的所有点  vis[x]:标记点x是否在堆栈中  c[x]:记录点x所在的强连通分量的编号
vector<int> scc[N];	//scc[i]:记录第i个强连通分量中的所有节点
int n,m,tot,num,top,cnt;	//tot记录边的序号，num记录dfs序，top记录stack堆栈的顶点，cnt记录图中强连通分量个数
int tc,headc[N];

void init()
{
	tot = 1; num = 0;
	top = 0; cnt = 0;
	memset(head,0,sizeof head);
	memset(c,0,sizeof c);
	memset(vis,0,sizeof vis);
	memset(dfn,0,sizeof dfn);
	memset(low,0,sizeof low);
	rep(i,1,n)
		scc[i].clear();
}

void initc()
{
	tc = 1;
	memset(headc,0,sizeof headc);
}

void add(int x,int y)
{
	e[++tot].to = y; e[tot].next = head[x]; head[x] = tot;
}

void addc(int x,int y)
{
	ec[++tc].to = y; ec[tc].next = headc[x]; headc[x] = tc;
}

void tarjan(int x)
{
	dfn[x] = low[x] = ++num;
	stack[++top] = x, vis[x] = 1;	//标记点x在栈中
	for(int i = head[x]; i ; i = e[i].next)
	{
		int y = e[i].to;
		if(!dfn[y]){	//如果点y没有被访问过
			tarjan(y);	//从y点一直递归下去
			low[x] = min(low[x],low[y]);
		}
		else if(vis[y]) low[x] = min(low[x],dfn[y]);
		//在求双连通分量时，此处为dfn[y]，即y点被访问过，则可用y的dfn值来更新x的low值
		//但是在强连通分量中，图是有向图，因此vis[y]==0的点，要么没被访问过，要么已经出栈了，即已经属于另一块强连通分量中
		//若y点已经属于另一块强连通分量，由于是有向图，因此y无法与x构成环，所以不能用y点dfn值来更新x的low值
	}
	if(dfn[x] == low[x])	//low[x] == dfn[x]成立，表明栈中从x到栈顶的所有节点构成一个强连通分量
	{						//这些节点无法与其他节点一起构成环
		cnt++; int y;
		do{
			y = stack[top--],vis[y] = 0;  //节点出栈
			c[y] = cnt, scc[cnt].push_back(y);	//将这些节点存入新的强连通分量中
		}while(x != y);
	}
}

int main()
{
	while(~scanf("%d%d",&n,&m))
	{
		init();
		rep(i,1,m){
			int x,y;
			scanf("%d%d",&x,&y);
			add(x,y);
		}
		rep(i,1,n)
			if(!dfn[i]) tarjan(i);
		//将每个SCC缩成一个点，对于原图中的每条有向边
		//如果c[x]!=c[y]，则在编号为c[x]和编号为c[y]的SCC之间连边
		//最后结果将是一张有向无环图
		initc();
		rep(x,1,n){
			for(int i = head[x]; i ;i = e[i].next){
				int y = e[i].to;
				if(c[x] == c[y]) continue;
				addc(c[x],c[y]);
			}
		}
	}
	return 0;
}