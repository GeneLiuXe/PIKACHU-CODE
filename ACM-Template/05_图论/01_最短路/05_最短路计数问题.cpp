// ---
// 题意: 一个高度为 $h$ 的电梯，初始位置在第一层。电梯有四个按钮。\\
// 1. 向上移动 $a$ 格 \\
// 2. 向上移动 $b$ 格 \\
// 3. 向上移动 $c$ 格 \\
// 4. 返回第一层 \\
// 问 $1～h$ 层中有多少层是可达的。$(1\leq h\leq 10^{18},1\leq a,b,c\leq 100000)$\\
// \\
// 思路: 观察数据范围可以发现，$a、b、c$ 数据范围比较小，可以从此处着手。\\
// 不难发现，如果 $2$ 可以达到，则 $2+a$、$2+2*a$、$2+3*a$... 均可到达。即我们以 $a$ 作为基底，如果 $x\ (x<a)$ 可以到达，则 $y\ (y\%a=x)$ 也可到达。\\
// 因此问题转化为对于 $x(x<a)$ 来说，最小的可达的 $y$ 是多少？$(y\%a=x)$ 既然问题变成了求最小，那么不难想到使用最短路来解决这个问题。\\
// 我们枚举 $x\ (0\leq x<a)$，令 $x$ 与 $(x+b)\%a$ 和 $(x+c)\%a$ 连边，边权分别为 $b$、$c$。然后起点为 $1$，即可求出到达所有 $x$ 时的最小值。\\
// 求取答案时就可以枚举 $x$，然后求 $dis[x]～h$ 中有多少个 $mod\ a=x$ 的值即可。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
typedef double db;
const int N = 1e5+100;
const int M = 1e6+100;
const ll inf = 1e15;
const db EPS = 1e-9;
using namespace std;
 
ll h,a,b,c,dis[N];
int head[N],tot,vis[N];
struct Node{
	int to,next;
	ll w;
}e[M];
 
void init() {tot = 1;}
 
void add(int x, int y, ll w){
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot, e[tot].w = w;
}
 
void dijkstra(int s){
	priority_queue<pair<ll,int> > q;
	while(q.size()) q.pop();
	rep(i,0,a-1) dis[i] = inf, vis[i] = 0;
	dis[s%a] = 1;
	q.push(make_pair(-dis[s%a],s%a)); 
	while(q.size()){
		int x = q.top().second; q.pop();
		if(vis[x]) continue;
		vis[x] = 1;
		for(int i = head[x]; i; i = e[i].next){
			int y = e[i].to;
			if(dis[y] > dis[x] + e[i].w){
				dis[y] = dis[x] + e[i].w;
				q.push(make_pair(-dis[y],y));
			}
		}
	}
}
 
int main()
{
	scanf("%lld%lld%lld%lld",&h,&a,&b,&c);
	init();
	rep(i,0,a-1){
		add(i,(i+b)%a,b);
		add(i,(i+c)%a,c);
	}
	dijkstra(1);
	ll ans = 0;
	rep(i,0,a-1){
		if(dis[i] == inf || dis[i] > h) continue;
		ans += 1ll + (h-dis[i])/a;
	}
	printf("%lld\n",ans);
	return 0;
}