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


// ---
// 题意：\\
// 每个点的度数都是偶数，每次删边的两个顶点读书不能同时是奇数，问最多能删多少边 \\
// \\
// 思路：\\
// 对于每个连通图， 删除的边肯定就是 $m - 1$ 了， 所以就是把欧拉图跑出来，然后顺序删边，只留下最后一条边就行了。 \\
// 这个题目的欧拉图，栈里面记录的是边，每个边只走一次，dfs 的时候也优化了一下，\\
// ---

#include<bits/stdc++.h>
using namespace std;
const int N = 5e5+100;
typedef pair<int,int>P;
P f[N];
vector<int>g;
vector<P>e[N];
bool used[N], vis[N];
int n,m,ans[N],cnt,now[N];

void dfs(int u){
    used[u] = 1;
    while(now[u] < (int)e[u].size()){
        auto it = e[u][now[u]];
        int v = it.first, id = it.second;
        now[u]++;   // 这个记录的是每个顶点走到什么地方了。 
        if (vis[id]) continue;
        vis[id] = 1;   // 把边置反。
        dfs(v);
        ans[++cnt] = id;  // 把边放到栈里面。 
    }
} 
int cal(int x, int y){  // 这个是算下一个顶点。 因为栈里面存的是边。 
    return f[y].first ^ f[y].second ^ x;
}
int main(){
    int x,y;
    scanf("%d%d",&n,&m);
    for (int i = 1; i <= m; ++i){
        scanf("%d%d",&x,&y);
        e[x].push_back(P(y, i));
        e[y].push_back(P(x, i));
        f[i] = P(x,y);
    }
    for (int i = 1; i <= n; ++i){
        if (!e[i].size()) continue;
        if(!used[i]){
            cnt = 0;
            dfs(i);
            int now = i, nxt = cal(i, ans[1]);
            g.push_back(ans[1]);
            for (int j = 2; j < cnt; ++j){  // 最后一条边不能删，
                if (cal(nxt, ans[j]) == now){  // 如果回到原点，要特判，边只能走一次，但是点可以走多次。
                    g.push_back(ans[j+1]);   // 这个时候 当前点 和 原点 的度数都是奇数，不能删，所以删 j+1， 然后回来删 j 。
                    g.push_back(ans[j]);
                    nxt = cal(now, ans[j+1]);
                    j++;
                } else {
                    g.push_back(ans[j]);
                    nxt = cal(nxt, ans[j]);
                }
            }
        }
    }
    int sz = g.size();
    printf("%d\n",sz);
    for (int i = 0; i < sz; ++i)
        printf("%d ", g[i]);
    return 0;
}