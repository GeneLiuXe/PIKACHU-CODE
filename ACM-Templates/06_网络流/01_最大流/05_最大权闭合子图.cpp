// ---
// 一、算法介绍\\
// 有一个有向图，每一个点都有一个权值（可以为正或负或0），选择一个权值和最大的子图，使得每个点的后继都在子图里面，这个子图就叫最大权闭合子图。 \\
// \\ 
// 能选的子图有Ø,{4},{3,4},{2,4},{1,2,3,4},它们的权值分别为0,-1,5,-6,4. \\
// 所以最大权闭合子图为{3,4}，权值为5.\\
// \\
// · 解法\\
// 这个问题可以转化为最小割问题，用网络流解决。 \\
// 从源点s向每个正权点连一条容量为权值的边，每个负权点向汇点t连一条容量为权值的绝对值的边，有向图原来的边容量全部为无限大。 \\
// \\ 
// 求它的最小割，割掉后，与源点s连通的点构成最大权闭合子图，权值为（正权值之和-最小割）。\\
// \\
// · 如何理解\\
// 割掉一条边的含义\\
// 由于原图的边都是无穷大，那么割边一定是与源点s或汇点t相连的。\\
// \\
// 割掉s与i的边，表示不选择i点作为子图的点； \\
// 割掉i与t的边，表示选择i点为子图的点。\\
// \\
// 如果s与i有边，表示i存在子图中； \\
// 如果i与t有边，表示i不存在于子图中。\\
// \\
// · 合法性\\
// 只有s与t不连通时，才能得到闭合子图。\\
// \\
// 如果s与t连通，则存在点i,j，使得s到i有边，i到j连通，j到t有边，所以j一定是i的后继，但选择了i，没有选择j，不是闭合子图。\\
// \\
// 如果s与t不连通，选择了正权点i，一定选择了i后继中的所有负权点。设j是i的后继中的正权点，则割掉s到j的边是没有意义的，最小割不会割掉它，则j一点被选中，所以i的所有后继都被选中，符合闭合图的定义。\\
// \\
// · 最优性\\
// 最小割=(不选的正权之和+要选的负权绝对值之和） \\
// 最大权闭合子图=（正权之和-不选的正权之和-要选的负权绝对值之和）=正权值和-最小割 \\
// 因为正权值和，是定值，而最小割保证值最小，所以最大权闭合子图一定最优。\\
// \\
// 二、例题\\
// 题意: 一共有 $m$ 个实验，$n$ 个仪器，每个实验完成之后能得到 $p_i$ 的报酬，每个仪器购买需要 $v_i$ 的花费，每个实验需要依托某几个对应的器材才能完成。现要求给出购买仪器以及做实验的方案，使得净收益最大，给出具体方案与结果。$(1\leq n,m\leq 50)$\\
// \\
// 思路: 以往普通费用流的问题，其最终的总流量都是固定的，因此可以依据总流量固定这一特点，对流量赋予费用来求解。\\
// \\
// 而此题的难点恰好在于此，涉及了费用，但最终总流量不固定，如果总流量不固定是很难用费用流算法来建图的，因此我们来考虑最大流。\\
// \\
// 而最大流通常解决的都是总流量问题，或者有时二分一下答案用最大流来判断是否合法，基本不涉及费用的问题，由此可以判断此题非之前遇到的普通最大流问题，是根据一种特殊的建图方式用最大流来求解出了费用问题。\\
// \\
// 其实这是一道最大权闭合子图问题，应用最大流最小割定理，通过求解最大流来获得最小割。建图方式如下：\\
// 1. 最大权闭合子图问题，针对的是在一个有向图中，每个点都有权值，现要在这个有向图中找到一个闭合子图，即子图中不存在出边不在子图中的点，使该子图中点权值和最大。\\
// 2. 具体建图方式则为，有向图中的边流量赋为无穷，源点连向权值为正的点，流量为其权值；权值为负的点连向汇点，流量为其权值绝对值。最终的答案为（正权值之和-最小割）\\
// \\
// 证明该定理的话，可以通过最小割只能断源点的连边或者汇点的连边来证明，即放弃正权点，或者接纳负权点。最大流结束后，仍与源点相连的点极为最大权闭合子图中的点。\\
// \\
// 如上建图即可跑出答案，最后的方案输出即 $bfs$ 一遍找到仍与源点相连的点，此处可以依据最大流最后 $bfs$ 的 $dis$ 数组来判断。\\
// \\
// 总结: 通过此题我们学会了一个用最大流算法解决总流量不固定时的最大收益问题。\\
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

vector<int> v1,v2;
string S;

int main()
{
	scanf("%d%d",&m,&n); getchar();
	init();
	s = n+m+1, t = n+m+2;
	int ans = 0;
	rep(i,1,m){
		getline(cin,S);
		int len = S.length(), cnt = 0, base = 0;
		rep(j,0,len-1){
			if(S[j] >= '0' && S[j] <= '9'){
				if(base == 0) cnt++;
				base = base*10+S[j]-'0';
			}
			else{
				if(cnt == 1) add(s,i,base), ans += base;
				else add(i,m+base,inf);
				base = 0;
			}
		}
		if(cnt == 1) add(s,i,base);
		else add(i,m+base,inf);
	}
	rep(i,1,n){
		int v; scanf("%d",&v);
		add(m+i,t,v);
	}
	ans -= solve();
	rep(i,1,m+n)
		if(dis[i] != 0){
			if(i <= m) v1.push_back(i);
			else v2.push_back(i-m);
		}
	int sz1 = v1.size(), sz2 = v2.size();
	rep(i,0,sz1-1) printf("%d%c",v1[i]," \n"[i==sz1-1]);
	rep(i,0,sz2-1) printf("%d%c",v2[i]," \n"[i==sz2-1]);
	printf("%d\n",ans);
	return 0;
}