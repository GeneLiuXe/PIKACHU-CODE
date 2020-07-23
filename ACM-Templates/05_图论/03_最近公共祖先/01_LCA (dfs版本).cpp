#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <queue>
#include <cmath>
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int N = 5*1e5+100;

int f[N][25],d[N],dis[N];  //f[x][k]:表示点x向根节点走2^k步到达的点
//d[x]:表示点x在图中的深度	dis[x]:表示根节点到点x的距离
struct Edge{
	int to,next;
}e[2*N];	//树的边为n-1，此处是因为无向边，所以正反各一条
int head[N],tot,t,n,m,s; //n个点，m次询问

void init()
{
	tot = 1;
	memset(head,0,sizeof head);
	memset(d,0,sizeof d);
}

void add(int x,int y)
{
	e[++tot].to = y; e[tot].next = head[x]; head[x] = tot;
}

void dfs(int u,int fa)
{
    d[u]=d[fa]+1; f[u][0]=fa;
    for(int i=1;(1<<i)<=d[u];i++)
        f[u][i]=f[f[u][i-1]][i-1];
    for(int i=head[u]; i; i=e[i].next){
        int v=e[i].to;
        if(v!=fa) dfs(v,u);
    }
}    

int lca(int x,int y)
{
	if(d[x] > d[y]) swap(x,y);
	for(int i = t; i >= 0; i--)
		if(d[f[y][i]] >= d[x]) y = f[y][i];  //往上追溯，直至y和x位于同一深度
	if(x == y) return x;  //如果已经找到了，就返回x
	for(int i = t; i >= 0; i--)
		if(f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];  //x和y同时往上走，一直到x和y恰好为lca的子节点
	return f[x][0];  //x和y共同的根节点就是lca 
}

int main()
{
	scanf("%d%d%d",&n,&m,&s);
	t = (int)(log(n)/log(2))+1;
	init();
	rep(i,1,n-1){
		int x,y;
		scanf("%d%d",&x,&y);
		add(x,y), add(y,x);
	}
	dfs(s,0);
	for(int i = 1;i <= m;i++)
	{
		int x,y;
		scanf("%d%d",&x,&y);
		printf("%d\n",lca(x,y));
	}
	return 0;
}