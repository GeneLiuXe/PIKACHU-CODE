// ---
// 题意：给定一棵树, 敌人在1号节点, 每次询问给出k个节点, 每条边都有边权, 问删掉树中边使得1号节点不能到达k个节点中任意一个节点需要的最小代价\\
// \\
// 输入：第一行一个整数n，代表岛屿数量。\\
// 接下来n-1行，每行三个整数u,v,w，代表u号岛屿和v号岛屿由一条代价为c的桥梁直接相连，保证1<=u,v<=n且1<=c<=100000。\\
// 第n+1行，一个整数m，代表敌方机器能使用的次数。\\
// 接下来m行，每行一个整数ki，代表第i次后，有ki个岛屿资源丰富，接下来k个整数h1,h2,…hk，表示资源丰富岛屿的编号。每次输出最小代价\\
// \\
// 思路：\\
// 首先考虑最暴力的dp,设$f[x]$表示处理完以$x$为根的子树的最小花费\\
// \\
// 转移有两种情况\\
// 1.断开自己与父亲的联系，代价为从根到该节点的最小值\\
// 2.将子树内的节点全都处理掉的代价\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <queue>
#include <cmath>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 250000+100;
const int M = 500000+100;
const db EPS = 1e-9;
using namespace std;

int n,tot1,tot2,head1[N],head2[N],m,dfn[N],d[N],f[N][20],t,idx,idq[N],top,stk[N],vis[N];
//idq —— 存取有效节点, top —— stk栈顶, vis —— 表示这个节点是否是有效节点
ll me[N];
struct Node{
	int to,next; ll w;
}e1[M],e2[M];

void add1(int x,int y,ll cc){
	e1[++tot1].to = y, e1[tot1].next = head1[x], e1[tot1].w = cc, head1[x] = tot1;
}
void add2(int x,int y,ll cc){
	e2[++tot2].to = y, e2[tot2].next = head2[x], e2[tot2].w = cc, head2[x] = tot2;
}
bool cmp(int a,int b){return dfn[a] < dfn[b];}

void bfs() //LCA部分
{
	queue<int> q;
	while(q.size()) q.pop();
	q.push(1); d[1] = 1;	//把1当做树根
	while(q.size())
	{
		int x = q.front(); q.pop();
		for(int i = head1[x]; i ;i = e1[i].next){
			int y = e1[i].to;
			if(d[y]) continue;
			d[y] = d[x]+1;
			f[y][0] = x;  //y走2^0步到达x
			for(int j = 1; j <= t;j++)
				f[y][j] = f[f[y][j-1]][j-1];
			q.push(y);
		}
	}
}

int LCA(int x,int y) //LCA部分
{
	if(d[x] > d[y]) swap(x,y);
	for(int i = t; i >= 0; i--)
		if(d[f[y][i]] >= d[x]) y = f[y][i];  //往上追溯，直至y和x位于同一深度
	if(x == y) return x;  //如果已经找到了，就返回x
	for(int i = t; i >= 0; i--)
		if(f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];  //x和y同时往上走，一直到x和y恰好为lca的子节点
	return f[x][0];  //x和y共同的根节点就是lca 
}

void dfs(int x,int fa){
	dfn[x] = ++idx;
	for(int i = head1[x]; i; i = e1[i].next){
		int y = e1[i].to;
		if(y == fa) continue;
		if(x == 1) me[y] = e1[i].w;
		else me[y] = min(me[x],e1[i].w);
		dfs(y,x);
	}
}

void insert(int u){ //构成虚树
	if(top == 1) {stk[++top] = u; return;}
	int lca = LCA(u,stk[top]); //与栈顶比较
	if(lca == stk[top]) {stk[++top] = u; return;}
	while(top > 1 && dfn[lca] <= dfn[stk[top-1]]){
		add2(stk[top-1],stk[top],0); --top;
	}
	if(lca != stk[top]){
		add2(lca,stk[top],0); stk[top] = lca;
	}
	stk[++top] = u;
}

ll DP(int x){ //使当前节点与子树中非有效节点不连通的最小代价
	ll cost = 0;
	for(int i = head2[x]; i; i = e2[i].next){
		int y = e2[i].to;
		cost += min(me[y],DP(y));
	}
	head2[x] = 0;
	if(vis[x]){
		vis[x] = 0;
		return me[x];
	} 
	else return cost;
}

int main()
{
	scanf("%d",&n);
	t = (int)(log(n)/log(2))+1;
	rep(i,1,n-1){
		int xx,yy; ll cc;
		scanf("%d%d%lld",&xx,&yy,&cc);
		add1(xx,yy,cc); add1(yy,xx,cc);
	}
	bfs(); dfs(1,-1);
	scanf("%d",&m);
	rep(i,1,m){
		tot2 = 1;
		int sz; scanf("%d",&sz);
		rep(j,1,sz){
			scanf("%d",&idq[j]); vis[idq[j]] = 1;
		}
		sort(1+idq,1+idq+sz,cmp);
		top = 0; stk[++top] = 1; //加入树根
		rep(j,1,sz) insert(idq[j]); //构建虚树
		while(top > 1){
			add2(stk[top-1],stk[top],0); top--;
		}
		// LOG1("i",i);
		cout << DP(1) << endl;
	}
	return 0;
}