// ---
// 题意：\\
// \ 给出$m$条边，求出一条欧拉路，起点任意，终点任意，每条边只经过一次。要求给出的欧拉路字典序最小。$(1\leq m\leq 1024,1\leq n\leq 500)$ \\
// \\
// 思路：\\
// \ 先总结一下`有向图、无向图求欧拉路与欧拉回路的性质`。 \\
// \ 无向图：有且仅有两个点度数为奇数则有欧拉路，所有点度数均为偶数则有欧拉回路。\\
// \ 有向图：所有点入度=出度则有欧拉回路。有且仅有两个点入度不等于出度，且起点出度比入度大 $1$ ，终点入度比出度大 $1$ 则有欧拉路。\\
// \\
// 算法 —— Hierholzer (解决无向图、有向图、欧拉路、欧拉回路问题)\\
// \\
// \ 选一个点$x$为起点，存在边 <$x,y$>，则删去边<$x,y$>，若为无向图还需删除<$y,x$>。若无边可走，则将$x$加入结果栈。最后输出结构栈即可。\\
// \\
// \ 回到此题，要求找到字典序最小的欧拉路。因此选择一个编号最小的奇数点进行递归，递归过程中优先走字典序更小的点，可以用$multiset$维护。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <set>
#include <stack>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 500+100;
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;

multiset<int> st[N];
stack<int> stk;
int m,deg[N];

void dfs(int x){
	while(st[x].size()){
		int y = (*st[x].begin());
		st[x].erase(st[x].begin());
		st[y].erase(st[y].find(x));
		dfs(y);
	}
	stk.push(x);
}

int main()
{	
	rep(i,0,500) st[i].clear();
	while(stk.size()) stk.pop();
	scanf("%d",&m);
	rep(i,1,m){
		int xx,yy; scanf("%d%d",&xx,&yy);
		st[xx].insert(yy);
		st[yy].insert(xx);
		deg[xx]++, deg[yy]++;
	}
	int x = -1;
	rep(i,0,500) if(deg[i]%2){x = i; break;}
	if(x == -1) x = 1;
	dfs(x);
	while(stk.size()){
		printf("%d\n",stk.top());
		stk.pop();
	}
	return 0;
}