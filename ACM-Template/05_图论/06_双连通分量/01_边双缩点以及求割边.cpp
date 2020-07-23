// ---
// 求边双连通分量、割边（e-DCC）\\
// \quad 将每个e-DCC看成一个节点，进行缩点\\
// \quad 求边双连通分量 —— 即该分量中不存在割边\\
// \\ 
// 边双连通图的条件 —— 当且仅当任意一条边都包含在至少一个简单环中\\
// 求边双连通分量的原理：\\
// \quad 先求出无向图中所有的桥，把桥都删除后，无向图会分成若干个连通块，每一个连通块就是一个“边双连通分量”\\
// ---
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int N = 1e5+10, M = 5*1e5+10;

struct Edge{	
	int to,next;
}e[2*M],ec[2*M]; //双向边，ec为缩点之后的图【森林/树】
int head[N],dfn[N],low[N],headc[N];  //dfn:记录该点的dfs序		low:记录该点所能达到的最小dfn值
//hc:缩点之后记录每一个点所连接的第一条边的边序
bool bridge[2*M];	//记录该边是否为割边
int tot,n,m,num,dcc,tc; //tot记录边序，num记录dfs序，dcc记录有多少个边双连通分量
//tc:缩点之后记录边序
int c[N];  //c[x]表示节点x所属的“边双连通分量”的编号

void init()
{
	num  = 0; tot = 1; dcc = 0;
	rep(i,0,n) head[i] = dfn[i] = low[i] = c[i] = 0;
	rep(i,0,2*m+2) bridge[i] = 0;
}

void initc()
{
	tc = 1;	//缩点之后记录边序
	// memset(headc,0,sizeof headc); //缩点之后记录每一个点所连接的第一条边的边序
	rep(i,0,dcc) headc[i] = 0;
}

void add(int x,int y)
{
	e[++tot].to = y; e[tot].next = head[x]; head[x] = tot; 
}

void addc(int x,int y)	//对缩点之后的图进行加边操作
{
	ec[++tc].to = y; ec[tc].next = headc[x]; headc[x] = tc;
}

void tarjan(int x,int in_edge)
{
	dfn[x] = low[x] = ++num;
	for(int i = head[x]; i ; i = e[i].next)
	{
		int y = e[i].to;
		if(!dfn[y]){
			tarjan(y,i);  //传入边(x,y)的序号
			low[x] = min(low[x],low[y]);
			if(low[y] > dfn[x])  //该边连接(x,y)，y点无法连接x点上面的点，因此该边是割边
				bridge[i] = bridge[i^1] = true;
		}
		else if(i != (in_edge^1)) //y点所连接的边 不能是(x,y)边的反向边
			low[x] = min(low[x],dfn[y]);
	}
}

void dfs(int x)	//用于将图划分为多个边双连通分量
{
	c[x] = dcc;
	for(int i = head[x]; i ; i = e[i].next)
	{
		int y = e[i].to;
		if(c[y] || bridge[i]) continue;  //如果点y已经属于别的强连通分量，或者边i是割边，则continue
		dfs(y);
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
			add(x,y); add(y,x);
		}
		rep(i,1,n)
			if(!dfn[i]) tarjan(i,0); //将边序号传进去
		rep(i,1,n)
			if(!c[i]){ //如果i点未被标记过
				++dcc;
				dfs(i);
			}
		initc(); //对缩点之后的图进行初始化	这里很重要！不要忘记！！！
		for(int i = 2; i <= tot; i += 2){ //遍历所有边 2-tot
			int x = e[i^1].to, y = e[i].to; //记录该边连接的两个端点
			if(c[x] == c[y]) continue; //如果连接的两个点属于同一连通分量，则continue
			//没必要用mp记录,不会重复，可以用ct记录有多少条边
			addc(c[x],c[y]);  //用连通的分量的编号来代表整个连通分量，以此来进行缩点
			addc(c[y],c[x]);
		}
		printf("缩点之后的森林，点数%d，边数%d(可能有重边)\n",dcc,tc/2);
	/*	printf("There are %d e-DCCs.\n",dcc);
		rep(i,1,n){
			printf("%d belongs to DCC %d.\n",i,c[i]); //输出每个点属于的e-DCC
		}*/
	/*	for(int i = 2; i <= tot; i+=2)
			if(bridge[i])
				printf("%d %d\n",e[i^1].to,e[i].to);  //该割边：(x,y)*/
	}
	return 0;
}