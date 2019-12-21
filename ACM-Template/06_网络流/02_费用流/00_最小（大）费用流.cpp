#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <queue>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int N = 1100, M = 1e5+100;

struct Edge{
	int to,next,cap,cost; //cap为该边的容量，cost为该边的花费
}e[M];
int head[N],tot,s,t; //tot记录边数，s为源点，t为汇点
int dis[N],incf[N],pre[N],vis[N]; //dis为到每一个点的最短距离，incf为每一个点流入的流量
//pre为每一个点的最短距离是由哪一条边更新而来，vis标记该点有没有被访问过
int n,m,maxflow,ans;

void init()
{
	tot = 1; //从2开始“成对存储”，2和3是一对，4和5是一对
	memset(head,0,sizeof head);
}

void add(int x,int y,int z,int c) //z为容量，c为花费
{
	//正向边，初始容量为z，单位费用为c
	e[++tot].to = y; e[tot].next = head[x]; head[x] = tot; e[tot].cap = z; e[tot].cost = c;
	//反向边，初始容量为0，单位费用为-c，与正向边“成对存储”
	e[++tot].to = x; e[tot].next = head[y]; head[y] = tot; e[tot].cap = 0; e[tot].cost = -c;
}

bool spfa()
{
	//用最短路找增广路
	queue<int> q;
	//建议不要用memset赋值，太容易出错了，当dis设置为long long时
	//tmp还是一个int，就特别容易出错
	fill(dis,dis+N,1e8);
//	memset(dis,0x3f,sizeof dis); //memset是按字节赋值，一个字节8位，一个16进制位表示4个二进制位，因此0x3f为一个字节
	//此处dis赋值为inf，寻找最短路（最小费用流）
	//如果dis赋值为0xcf，则为寻找最长路（最大费用流）
	//0xcf为11001111，因此按位赋值之后为负数
	memset(vis,0,sizeof vis); //每个点都没被访问过
	q.push(s); dis[s] = 0; vis[s] = 1;
	incf[s] = 1<<30; //incf为每一个点流入的流量
	while(q.size())
	{
		int x = q.front(); vis[x] = 0; q.pop();
		for(int i = head[x]; i ; i = e[i].next) //通过与x相连的各边遍历与x相连的点
		{
			if(!e[i].cap) continue; //该边剩余流量为0，不在残量网络中，无法通过
			int y = e[i].to;
			if(dis[y] > dis[x] + e[i].cost)  //如果此处为 < ，则为最长路算法【最大费用流】
			{
				dis[y] = dis[x]+e[i].cost;
				incf[y] = min(incf[x],e[i].cap);
				pre[y] = i; //记录y点的最短路是由哪一条边更新而来的
				if(!vis[y]) vis[y] = 1, q.push(y);
			}
		}
	}
//	int tmp = 0x3f3f3f3f;
	if(dis[t] == 1e8) return false; //汇点不可达，已求出最大流
	return true; 
}

void update()
{
	int x = t;
	while(x!=s)
	{
		int i = pre[x];
		e[i].cap -= incf[t];
		e[i^1].cap += incf[t];
		x = e[i^1].to; //相反边的终点，即为x的上一个点
	}
	maxflow += incf[t];
	ans += dis[t]*incf[t];  //dis[t]为这条最短路上的总花费，incf为这条最短路上的流
}

void solve()
{
	ans = 0; maxflow = 0;
	while(spfa()) update();
	printf("%d\n",ans);
}

int main()
{
	while(~scanf("%d%d",&n,&m))
	{
		//建图过程
		s = 1; t = 2+n;
		init();
		add(s,2,2,0);
		add(n+1,t,2,0);
		rep(i,1,m){
			int a,b,c;
			scanf("%d%d%d",&a,&b,&c);
			add(a+1,b+1,1,c); add(b+1,a+1,1,c);
		}
		solve();
	}
	return 0;
}

/*
常见错误：
	1.没有init();
	2.spfa()函数部分dis是long long型的，用memset赋值，再用int型的tmp比较...
	3.建议spfa()函数部分的dis用fill来进行赋值【不易出错】
*/