// ---
// 出现负环的条件有两个：\\
// 1. 最短路边数 $\geq$ n \\
// 2. 入队次数 $\geq$ n\\
// $\quad$ spfa 类似于 bfs 的过程 \\
// $\quad$ 第 i 轮入队意味着确定了最短路边数为 i 的最短路长度 \\
// $\quad$ 如果一个点入队了 n 次，则意味着最短路边数至少为 n，即出现了环 \\
// ---
#include <bits/stdc++.h>
#define mem(a,b) memset(a,b,sizeof a);
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
typedef long long ll;
typedef double db;
const int inf = 1e8;
const int N = 1e3+10;
const db EPS = 1e-9;
using namespace std;

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

int n,m,Q,a[N],vis[N],dis[N],cnt[N];
vector<pair<int,int> > G[N];
queue<int> q;

void init(){
	rep(i,1,n) G[i].clear();
	rep(i,1,n) vis[i] = cnt[i] = 0, dis[i] = inf;
}

// 找到被负环影响的所有点，并标记
void dfs(int x){
	cnt[x] = inf;
	for(auto item:G[x])
		if(cnt[item.first] != inf) dfs(item.first);
}

void spfa(int s){
	while(q.size()) q.pop();
	dis[s] = 0; vis[s] = 1;
	q.push(s);
	while(q.size()){
		int x = q.front(); q.pop();
		vis[x] = 0;
		if(cnt[x] >= n) continue;
		for(auto item:G[x]){
			int y = item.first, w = item.second;
			if(cnt[y] == inf) continue;
			if(dis[y] > dis[x] + w){
				dis[y] = dis[x] + w;
				// cnt[x] 记录最短路边数
				cnt[y] = cnt[x] + 1;
				if(cnt[y] >= n){
					if(cnt[y] == n) dfs(y);
					continue;
				}
				if(!vis[y]) q.push(y), vis[y] = 1;
			}
		}
	}
}	

int main()
{
	int _; scanf("%d",&_);
	rep(Ca,1,_){
		scanf("%d",&n);
		init();
		rep(i,1,n) scanf("%d",&a[i]);
		scanf("%d",&m);
		rep(i,1,m){
			int x,y; scanf("%d%d",&x,&y);
			int v = (a[y]-a[x])*(a[y]-a[x])*(a[y]-a[x]);
			G[x].push_back(make_pair(y,(a[y]-a[x])*(a[y]-a[x])*(a[y]-a[x])));
		}
		spfa(1);
		scanf("%d",&Q);
		printf("Case %d:\n", Ca);
		rep(i,1,Q){
			int p; scanf("%d",&p);
			if(dis[p] == inf || cnt[p] == inf || dis[p] < 3) printf("?\n");
			else printf("%d\n", dis[p]);
		}
	}
	return 0;
}