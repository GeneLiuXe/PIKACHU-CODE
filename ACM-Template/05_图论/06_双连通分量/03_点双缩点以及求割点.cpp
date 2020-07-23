// ---
// v-DCC的缩点\\
// \quad 由于一个割点可能属于多个v-DCC，因此缩点之后，图中包含p+t个节点，p个割点，t个v-DCC，将每个v-DCC和每个割点都作为新图中的节点\\
// \quad 并将每个割点与包含它的所有v-DCC之间连边\\
// \\
// \\	
// 此处需要注意：\\
// \quad 缩点之后，所有的点双连通分量变成一个点，该点只与割点相连，不会与其他点双连通分量相连\\
// \\
// \\
// 无向图中求割点\\
// x是割点当且仅当搜索树上存在x的一个子节点y，满足\\
// \quad dfn[x] <= low[y]\\
// \quad 即y点无法访问x点之上的点\\
// \\
// 如果x不是根节点，则只需要一个点y即可\\
// 如果x是根节点，则需要两个点y1、y2存在，x才能是割点，此时如果去掉x，则y1与y2不连通\\
// \\
// dfn:记录该点的dfs序	 \quad low:记录该点所能达到的最小dfn值\\
// \\
// \\
// 无向图中求点双连通分量\\
// 操作方法：\\
// \quad 1.当一个节点第一次被访问时，把该节点入栈。\\
// \quad 2.当割点判定法则中的条件dfn[x] <= low[y]成立时，无论x是否为根，都要：\\
// \quad\quad (1)从栈顶不断弹出节点，直至节点y被弹出。\\
// \quad\quad (2)刚才弹出的所有节点与节点x一起构成一个v-DCC\\
// \\
// \\
// \\
// 割点：\\
// \quad 若x为割点，则从图中删去节点x及所有与x相关联的边之后，G分裂成了两个或两个以上不相连的子图\\
// \\
// 点双连通分量(v-DCC) —— 分量中无割点\\
// \quad 注意:点双连通分量与“删除割点后图中剩余的连通块”是不一样的概念\\
// \quad\quad 桥不属于任何e-DCC，但是割点可能属于多个v-DCC\\
// \\	
// 无向连通图是点双连通图所需要满足的条件：【其中一个】\\
// \quad1.图的顶点数不超过2.\\
// \quad2.图中任意两点都同时包含在至少一个简单环中。其中“简单环”指的是不自交的环，也就是我们通常画出的环\\
// ---
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int N = 1e5+10, M = 5*1e5+10;

struct Edge{
	int to,next;
}e[M*2],ec[M*2];  //双向边
int head[N],dfn[N],low[N],stack[N];  //dfn:记录该点的dfs序		low:记录该点所能达到的最小dfn值
//stack:手动模拟堆栈，先进先出
int n,m,tot,num,root,top,cnt; //top:用来记录stack的堆顶位置	cnt:记录一共有多少个点双连通分量
bool cut[N];  //记录该点是否为割点
vector<int> dcc[N];	//dcc[i]保存编号为i的v-DCC中的所有节点
int new_id[2*N];  //用于记录缩点之后的新图的各个点的编号
int headc[2*N],tc; //tc:记录新图的边的序号		headc:记录新图中每个点的第一条边

void init()
{
	tot = 1;  //记录第一条边
	num = 0;  //记录dfs序
	cnt = 0;  //记录点双连通分量个数
	top = 0;  //初始化堆栈
	memset(head,0,sizeof head);
	memset(dfn,0,sizeof dfn);
	memset(low,0,sizeof low);
	memset(cut,0,sizeof cut);
	rep(i,1,n){  //n个点
		dcc[i].clear();
	}
}

void initc()
{
	//cnt记录一共有多少个点双连通分量
	num = cnt;	//割点的编号从cnt+1开始，1-cnt的点为点双连通分量的编号
	tc = 1;
	memset(headc,0,sizeof headc);
}

void add(int x,int y)
{
	e[++tot].to = y; e[tot].next = head[x]; head[x] = tot;
}

void addc(int x,int y)
{
	ec[++tc].to = y; e[tot].next = headc[x]; headc[x] = tc;
}

void tarjan(int x)
{
	dfn[x] = low[x] = ++num;
	stack[++top] = x;	//当一个节点第一次被访问时，把该节点入栈
	if(x == root && head[x] == 0){	//孤立点-无边连接
		dcc[++cnt].push_back(x);	//孤立点自己就是一个点双连通分量
		return;
	}
	int flag = 0;	//记录存在几个子节点，使得low[y]>=dfn[x]，用于判断割点
	for(int i = head[x]; i ; i = e[i].next)
	{
		int y = e[i].to;
		if(!dfn[y]){
			tarjan(y);
			low[x] = min(low[x],low[y]);
			if(low[y] >= dfn[x]){
				flag++;
				if(x != root || flag > 1) cut[x] = true;	//x为割点
				cnt++;	//出现了一个新的点双连通分量
				int z;
				do{
					z = stack[top--];	//x、y以及y之后的所有点构成一个点双连通分量
					dcc[cnt].push_back(z);
				}while(z != y);
				dcc[cnt].push_back(x);
			}
		}
		else low[x] = min(low[x],dfn[y]);
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
			if(x == y) continue; //判断自环
			add(x,y); add(y,x);
		}
		rep(i,1,n)
			if(!dfn[i]) root = i, tarjan(i);  //因为原图可能不连通，因此需要对于每个点进行dfs
		//以下是缩点操作
		initc();
		rep(i,1,n)
			if(cut[i]) new_id[i] = ++num;	//cut:记录该点是否为割点，记录割点i在新图中的编号为num+1
		//建新图，从每个v-DCC到它包含的所有割点连边
		rep(i,1,cnt) //cnt表示点双连通分量个数
			for(int j = 0; j < dcc[i].size(); j++)
			{
				int x = dcc[i][j];
				if(cut[x]){	//如果x为割点，则将x与其所属的点强连通分量相连
					addc(i,new_id[x]);
					addc(new_id[x],i);
				}
			//	else c[x] = i;	此处可有可无，此处的作用为c[x]表示x所在的强连通分量的编号
			}
		printf("缩点之后的森林，点数为%d，边数为%d\n",num,tc/2);	//num为新图中点数
		printf("编号 1~%d 的为原图的v-DCC，编号 > %d 的为原图割点\n",cnt,cnt);	//cnt为点双连通分量个数
		for(int i = 2;i < tc;i+=2)
			printf("%d %d\n",ec[i^1].to,e[i].to);	//输出新图中的所有无向边

	/*	rep(i,1,cnt){
			printf("e-DCC #%d:",i);
			for(int j = 0; j < dcc[i].size(); j++)
				printf(" %d",dcc[i][j]);  //将第i个点双连通分量中的点全部输出
			puts("");
		} */		

	/*	rep(i,1,n)
			if(cut[i]) printf("%d ",i);	//cut[i]标记i节点是否为割点
		puts("are cut-vertexes"); */
	}
	return 0;
}