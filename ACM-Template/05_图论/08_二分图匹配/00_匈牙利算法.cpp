#include <cstdio>
#include <iostream>
#include <cstring>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int MAXN = 5010;  //点数最大值 
const int MAXM = 50010; //边数最大值

struct Edge{
	int to,next; //next表示该边的上一条边的编号 
}e[MAXM];
int head[MAXN],tot,uN;  //uN表示一共有多少个点 
//head[u]表示以u为起点的边的编号 
int girl[MAXN],used[MAXN];
//girl表示每个点的匹配情况
//used表示改点有没有被标记过 

void init()
{
	tot = 0;
	memset(head,-1,sizeof head);	
} 

void addedge(int u,int v)
{
	tot++; //边的编号从1~n 
	e[tot].to = v; e[tot].next = head[u]; //next表示与这条边共点的上一条边 
	//如果next为-1，则表示该边无上一条边 
	head[u] = tot;  //head[u]表示以u为起点的第一条边的编号 
}

bool dfs(int u)
{
	for(int i = head[u]; i != -1;i = e[i].next){
		int v = e[i].to;
		if(!used[v])
		{
			used[v] = 1;
			if(!girl[v] || dfs(girl[v]))
			{
				girl[v] = u;
				return true;	
			} 
		}
	}
	return false;
}

int solve()
{
	int res = 0;
	memset(girl,0,sizeof girl);
	//点的编号 0 ~ uN-1
	for(int u = 0; u < uN;u++)
	{
		memset(used,0,sizeof used);
		if(dfs(u)) res++;	
	} 
	return res;
}

int main()
{
	//读入图
	printf("%d\n",solve()); 
	return 0;
}

