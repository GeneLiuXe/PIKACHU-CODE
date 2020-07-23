// ---
// 题意：\\
// 给出了一个 $Y*X$ 的地图，在地图的四个边缘有门，用 $'D'$ 表示，$'X'$ 表示障碍物即不能走，$'.'$表示这个位置初始有一个人，现在地图中的所有人要逃出门外，每个 $'.'$ 这个点可以站好多人，但是出门的时候只能一个一个出，每个人移动一格的时间为 $1$，问最少需要多少时间，所有人可以撤出场地。如果不能撤出，输出 $impossible$ 。\\
// \\
// 思路：\\
// 因为是最少的时间，所以一开始想到的是最小割和最小费用最大流，然后发现均无法解决这个问题，因为难以解决每个时间每个门只能被经过一次这个问题。\\
// \\
// 后来发现我们可以==对每一个时刻的门分别建点==，然后使这个门的容量为 $1$，因此就可以满足每个时刻每个门只能出去一个人这个条件。\\
// \\
// 但是如果对门的所有时刻都建点的话，那么无疑跑出来的结果是不对的，即我们现在只能验证当前每一个时刻上限的情况下，所有人能不能出来。因此发现这个题的答案，即所需要的时间是单调的，因此可以进行二分答案。二分所有人出门的时间，然后用最大流建图来验证\\
// \\
// 当二分的时间为 $x$ 时，则对每个门都建 $x$ 个点，然后每个人到达这个门所需的时间如果为 $y$ 的话，则将这个人与这个门所有 $>=y$ 的时刻连边。至此即可完成本题。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <map>
#include <queue> 
#define rep(i,a,b) for(int i = a;i <= b;i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
using namespace std;
const int inf = 1<<29,N = 1e5+100,M = 5*1e6+100;  //处理1e4-1e5规模的网络

struct Edge{ 
	int to,next,v;
}e[M];
int n,m,s,t;  //顶点个数 边数 源点 汇点 
int head[N],tot,dis[N];
queue<int> q;
char mp[20][20];
int numdoor,mandoor[200][200],numman,hp[200],vis[300];
int dir[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

void bbfs(int x)
{
	// printf("x:%d\n",x);
	queue<pair<int,int> > qp;
	while(qp.size()) qp.pop();
	memset(vis,0,sizeof vis);
	vis[x] = 1;
	qp.push(make_pair(x,0));
	while(qp.size()){
		int xx = qp.front().first, dist = qp.front().second;
		int yy = xx%m;
		qp.pop();
		xx = xx/m+1;
		rep(i,0,3){
			int xxx = xx+dir[i][0], yyy = yy+dir[i][1];
			int tp = (xxx-1)*m+yyy;
			if(xxx < 1 || xxx > n || yyy < 1 || yyy > m || mp[xxx][yyy] == 'X') continue;
			if(vis[tp]) continue;
			if(mp[xxx][yyy] == 'D'){
				// printf("oops!\n");
				// LOG2("x",x,"tp",tp);
				// LOG1("dist",dist);
				mandoor[hp[x]][hp[tp]] = dist+1;
				continue;
			}
			vis[tp] = 1;
			qp.push(make_pair(tp,dist+1));
		}
	}
}

void init_solve()
{
	memset(mandoor,0,sizeof mandoor);
	numman = numdoor = 0;
	memset(hp,0,sizeof hp);
	rep(i,1,n)
		rep(j,1,m)
			if(mp[i][j] == '.') hp[(i-1)*m+j] = ++numman;
			else if(mp[i][j] == 'D') hp[(i-1)*m+j] = ++numdoor;
	rep(i,1,n)
		rep(j,1,m)
			if(mp[i][j] == '.'){
				// printf("i:%d,j:%d\n",i,j);
				bbfs((i-1)*m+j);
			} 
}

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
		while(flow = dinic(s,inf)) maxflow += flow;
	return maxflow;
}

void mainsolve()
{
	int l = 1, r = 200, ans = -1;
	while(l <= r){
		int mid = (l+r)>>1;
		init();
		s = 1, t = 1+numman+mid*numdoor+1;
		rep(i,1,numman) add(s,i+1,1);
		rep(i,1,numdoor)
			rep(j,1,mid) add(1+numman+(i-1)*mid+j,t,1);
		rep(i,1,numman)
			rep(j,1,numdoor)
				if(mandoor[i][j] <= mid && mandoor[i][j])
					rep(k,mandoor[i][j],mid) add(1+i,1+numman+(j-1)*mid+k,1);
		if(solve() == numman) r = mid-1, ans = mid;
		else l = mid+1;
	}
	if(ans == -1) printf("impossible\n");
	else printf("%d\n",ans);
}

int main()
{
	int _; scanf("%d",&_);
	while(_--)
	{
		scanf("%d%d",&n,&m);
		rep(i,1,n) scanf("%s",mp[i]+1);
		init_solve();
		mainsolve();
	}
	return 0;
} 