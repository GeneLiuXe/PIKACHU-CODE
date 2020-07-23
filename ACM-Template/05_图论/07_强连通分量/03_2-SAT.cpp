// ---
// 题意：\\
// \quad 有一个小镇上只有一个牧师。这个小镇上有一个传说，在九月一日结婚的人会受到爱神的保佑，但是要牧师举办一个仪式。这个仪式要么在婚礼刚刚开始的时候举行，要么举行完婚礼正好结束。 \\
// \quad 现在已知有n场婚礼，告诉你每一场的开始和结束时间，以及举行仪式所需要的时间。问牧师能否参加所有的婚礼，如果能则输出一种方案。\\
// ---
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int N = 1e5+10, M = 1e7;

struct Edge{
	int to,next;
}e[M];
int head[N],dfn[N],low[N];	//dfn:dfs的序号	low:该点能够到达的最小的dfn值的点
int stack[N],vis[N],c[N];	//stack:维护通过点x到达的所有点  vis[x]:标记点x是否在堆栈中  c[x]:记录点x所在的强连通分量的编号
vector<int> scc[N];	//scc[i]:记录第i个强连通分量中的所有节点
int n,m,tot,num,top,cnt;	//tot记录边的序号，num记录dfs序，top记录stack堆栈的顶点，cnt记录图中强连通分量个数
int s[N],t[N],d[N];

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

void add(int x,int y)
{
	e[++tot].to = y; e[tot].next = head[x]; head[x] = tot;
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

int overlap(int a1,int b1,int a2,int b2)
{
	if(a1>=a2&&a1<b2 || b1>a2&&b1<=b2 || a1<=a2&&b1>=b2) return 1;
	return 0;
}

void solve()
{
	rep(i,1,n){
		if(c[i] == c[i+n]){
			printf("NO\n");
			return;
		}
	}
	printf("YES\n");
	int val[N];
	memset(val,0,sizeof val);
	rep(i,1,n){
		if(c[i] < c[i+n])	//Tarjan之后的图就已经进行了拓扑排序
			val[i] = 0;		//如果！a -> a，则a为真
		else val[i] = 1;
	}
	rep(i,1,n){
		if(val[i] == 0){
			printf("%02d:%02d %02d:%02d\n",s[i]/60,s[i]%60,(s[i]+d[i])/60,(s[i]+d[i])%60);
		}
		else 
			printf("%02d:%02d %02d:%02d\n",(t[i]-d[i])/60,(t[i]-d[i])%60,t[i]/60,t[i]%60);
	}
}

int main()
{
	while(~scanf("%d",&n))
	{
		init();
		rep(i,1,n){
			int x1,y1,x2,y2;
			scanf("%d:%d %d:%d %d",&x1,&y1,&x2,&y2,&d[i]);
			s[i] = x1*60+y1;
			t[i] = x2*60+y2;
		}
		//每个点都有两个状态，一个为真，一个为假，因此共2*n个状态
		rep(i,1,n){
			rep(j,i+1,n){
				if(overlap(s[i],s[i]+d[i],s[j],s[j]+d[j]))
					add(i,j+n),add(j,i+n);
				if(overlap(s[i],s[i]+d[i],t[j]-d[j],t[j]))
					add(i,j),add(j+n,i+n);
				if(overlap(t[i]-d[i],t[i],s[j],s[j]+d[j]))
					add(i+n,j+n),add(j,i);
				if(overlap(t[i]-d[i],t[i],t[j]-d[j],t[j]))
					add(i+n,j),add(j+n,i);
			}
		}
		rep(i,1,2*n)
			if(!dfn[i]) tarjan(i);
		solve();
	}
	return 0;
}
