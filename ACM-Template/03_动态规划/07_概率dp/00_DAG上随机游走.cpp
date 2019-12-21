// ---
// 题意:\\
// 给出一个有向无环图，从 $1$ 号点向 $n$ 号点前进。每次等概率地转移到后继节点，也有可能留在当前节点。定义每一天的花费是当前走过的天数，即第 $i$ 天的花费是 $i$，求从起点走到终点的总花费。$(2\leq n\leq 10^5,1\leq m\leq 2*10^5)$\\
// \\
// 思路:\\
// 令 $day[i]$ 表示从第 $i$ 个点到达第 $n$ 个点的期望天数，$cost[i]$ 表示从第 $i$ 个点到达第 $n$ 个点的期望花费，$deg[i]$ 表示第 $i$ 个点的出度。\\
// \\
// $day[i] = \sum\limits_{j\in deg[i]}\frac{day[j]}{deg[i]+1}\ +\ \frac{day[i]}{deg[i]+1} \ +\ 1$ \\
// \\
// $day[i] = \sum\limits_{j\in deg[i]}\frac{day[j]}{deg[i]}\ +\ \frac{deg[i]+1}{deg[i]}$ \\
// \\
// 然后再来求取 $cost[i]$。\\
// \\
// $cost[i] = \sum\limits_{j\in deg[i]}\frac{cost[j]}{deg[i]+1}\ +\ \frac{cost[i]}{deg[i]+1} \ +\ day[i]$ \\
// \\
// $cost[i] = \sum\limits_{j\in deg[i]}\frac{cost[j]}{deg[i]}\ +\ \frac{deg[i]*(deg[i]+1)}{deg[i]}$ \\
// \\
// 为什么求 $cost[i]$ 的最后要加上一个 $day[i]$，因为当前之后还有 $day[i]-1$ 天，每天的期望要加上一个 $1$，即当前走的这一步。因此 $day[i]-1+1=day[i]$。\\
// \\
// 接下来就是把边反向跑拓扑序转移即可。\\
// \\
// 总结:\\
// 概率 $dp$ 题目，主要难点在于考虑清所有的情况，精准地列出式子。\\
// \\
// 有时最终的式子也有可能由多个式子组成，需要求出多个变量，需要对问题进行一定的拆分。\\
// ---
#include <bits/stdc++.h>
#define mem(a,b) memset(a,b,sizeof a);
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
typedef long long ll;
typedef double db;
const int N = 4e5+100;
const int M = 5e5+100;
const db EPS = 1e-9;
using namespace std;

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

int n,m,tot,head[N],deg[N],base[N];
db day[N],cost[N];
struct Node{
	int to,next;
}e[M];

void add(int x,int y){
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot;
}

void init(){
	tot = 1;
	rep(i,0,n){
		head[i] = deg[i] = 0;
		day[i] = cost[i] = 0;
	}
}

void solve(){
	queue<int> q;
	while(q.size()) q.pop();
	rep(i,1,n) base[i] = deg[i];
	rep(i,1,n)
		if(!deg[i]) q.push(i);
	while(q.size()){
		int x = q.front(); q.pop();
		if(base[x] != 0){
			day[x] += (db)(base[x]+1)/(db)base[x];
			cost[x] += ((db)day[x]*(db)(base[x]+1))/(db)base[x];
		}
		for(int i = head[x]; i; i = e[i].next){
			int y = e[i].to;
			deg[y]--;
			day[y] += (db)day[x]/(db)base[y];
			cost[y] += (db)cost[x]/(db)base[y];
			if(!deg[y]) q.push(y);
		}
	}
	printf("%.2f\n",cost[1]);
}

int main()
{
	int _; scanf("%d",&_);
	while(_--){
		scanf("%d%d",&n,&m);
		init();
		rep(i,1,m){
			int u,v; scanf("%d%d",&u,&v);
			deg[u]++; add(v,u);
		}
		solve();
	}
	return 0;
}