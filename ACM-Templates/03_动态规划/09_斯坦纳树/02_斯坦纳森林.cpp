// ---
// 题意：$n$ 个点，$m$ 条边，前 $k$ 个点为居住处，后 $k$ 个点为避难处。现在 $k$ 个居民需要从居住处逃避到避难处，问最少的连边的花费。注意只要满足居民有避难处可待即可，因此是个森林。$(1\leq n\leq 50, 0\leq m\leq 1000,1\leq k\leq 5,2*k\leq n)$\\
// \\
// 思路：这其实是个斯坦纳森林模板题。首先正常的求出斯坦纳树的结果，即 $dp[i][state]$ 表示第 $i$ 个点为树根，关键点连通状态为 $state$ 的最小花费。\\
// \\
// 然后再设置一个新的函数 $f[1<<K]$，$f[state]$ 表示关键节点连通状态为 $state$ 时的最小代价。由于答案可以是森林，因此枚举 $state$ 的子状态进行转移即可。\\
// \\
// $dp[i][state] = min(dp[i][state],dp[i][sub]+dp[i][state-sub])$。这里需要注意要检验状态 $state$ 是否合法，$state$ 状态合法当且仅当该状态中居住处点个数与避难处点个数相同。\\
// ---
#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const db EPS = 1e-9;
const int N = 50+5;
const int M = 2*1000+100;
const int inf = 1e8;
using namespace std;

int n,m,k,head[N],dp[N][1<<10],f[1<<10],endS,st[N],tot;
bool vis[N][1<<10];
struct Node{
	int to,next,w;
}e[M];
queue<int> q;

void add(int x,int y,int w){
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot, e[tot].w = w;
}

void init(){
	tot = 1;
	rep(i,0,n) head[i] = 0;
	endS = (1<<(2*k))-1;
	rep(i,0,n) memset(vis[i],0,sizeof vis[i]);
	rep(i,0,n) st[i] = 0;
	rep(i,0,n)
		rep(S,0,endS) dp[i][S] = inf, vis[i][S] = 0;
	rep(i,1,k) st[i] = 1<<(i-1);
	rep(i,n-k+1,n) st[i] = 1<<(i-(n-k)+k-1);
	rep(i,1,n)
		if(st[i]) dp[i][st[i]] = 0;
	rep(S,0,endS) f[S] = inf;
}

void SPFA(int state){
	while(q.size()){
		int x = q.front(); q.pop(); vis[x][state] = 0;
		for(int i = head[x]; i; i = e[i].next){
			int y = e[i].to;
			// LOG2("x",x,"y",y);
			if(y == x) continue;
			if(dp[y][st[y]|state] > dp[x][state]+e[i].w){
				dp[y][st[y]|state] = dp[x][state]+e[i].w;
				if((st[y]|state) != state || vis[y][state]) continue;
				q.push(y); vis[y][state] = 1;
			}
		}
	}
}

void steinerTree(){
	rep(S,1,endS){
		rep(i,1,n){
			//i在S中
			if(st[i] && (st[i]|S) != S) continue;
			//枚举子集
			for(int sub = (S-1)&S; sub; sub = (sub-1)&S){
				int x = st[i]|sub, y = st[i]|(S-sub);
				// LOG2("x",x,"y",y);
				if(dp[i][x] != inf && dp[i][y] != inf)
					dp[i][S] = min(dp[i][S],dp[i][x]+dp[i][y]);
			}
			if(dp[i][S] != inf){
				q.push(i), vis[i][S] = 1;
				// LOG3("i",i,"S",S,"dp[i][S]",dp[i][S])
			}
		}
		SPFA(S);
	}
}

int check(int state){
	int cnt = 0;
	rep(i,0,2*k-1){
		if(state&(1<<i))
			cnt += i<k?1:-1;
	}
	return cnt == 0 ? 1:0;
}

void solve(){
	rep(S,1,endS)
		if(check(S)){
			// LOG1("S",S);
			rep(i,1,n)
				f[S] = min(f[S],dp[i][S]);
		}
	rep(S,1,endS)
		for(int sub = S&(S-1); sub; sub = (sub-1)&S){
			int x = sub, y = S-sub;
			if(check(x) && check(y))
				f[S] = min(f[S],f[x]+f[y]);
		}
	if(f[endS] == inf) printf("No solution\n");
	else printf("%d\n",f[endS]);
}

int main()
{
	int _; scanf("%d",&_);
	while(_--){
		scanf("%d%d%d",&n,&m,&k);
		init();
		rep(i,1,m){
			int x,y,w; scanf("%d%d%d",&x,&y,&w);
			add(x,y,w); add(y,x,w);
		}
		steinerTree();
		solve();
	}
	return 0;
}