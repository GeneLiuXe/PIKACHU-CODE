// ---
// 题意：给出一个 $n$ 个点 $m$ 条边的有向图，每条边都有一个权重。一共有 $q$ 组询问，每次询问 $k$，表示询问图中第 $k$ 小的路径。$(1\leq n,m,q,k\leq 5*10^4)$\\
// \\
// 思路：$q$ 组询问，我们离线下来，只用求出最大的那个 $k$ 的答案即可。\\
// \\
// 然后一样的套路，定状态。一条路径的表示，无非是起点和终点，$(u,v)$，但是如果这样定状态的话，会发现不好更新，难道更新 $v$ 的所有出边，如果更新 $v$ 的所有出边很容易 $T$ ？\\
// \\
// 因此我们令 $u$ 为 $v$ 的前驱，状态定义为 $(u,v,id,w)$ 表示 $u$ 是 $v$ 的前驱，且 $v$ 是 $u$ 的第 $id$ 条出边，当前路径权重为 $w$。\\
// \\
// 然后每次更新 $u$ 的第 $id+1$ 条出边，或者更新 $v$ 的第一条出边即可，记得将每个点的出边排序。\\
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
const int N = 5*1e4+10;
using namespace std;

int n,m,qn,Q[N],maxn;
ll ans[N];
vector<pair<ll,int> > v[N];
struct Node{
	int u,v,id;
	ll w;
	Node() {}
	Node(int x,int y,int d,ll z):u(x),v(y),id(d),w(z) {}
	bool operator < (Node xx) const {
		return w > xx.w;
	}
};

priority_queue<Node> q;

void solve(){
	while(q.size()) q.pop();
	int ct = 0;
	rep(i,1,n) sort(v[i].begin(),v[i].end());
	rep(i,1,n){
		if(v[i].size())
			q.push({i,v[i][0].second,0,v[i][0].first});
	}
	while(q.size()){
		Node x = q.top(); q.pop();
		ct++;
		ans[ct] = x.w;
		if(ct == maxn) break;
		if(v[x.v].size()){
			q.push({x.v,v[x.v][0].second,0,x.w+(v[x.v][0].first)});
		}
		if((int)v[x.u].size() > (int)(x.id+1)){
			q.push({x.u,v[x.u][x.id+1].second,x.id+1,x.w-v[x.u][x.id].first+v[x.u][x.id+1].first});
		}	
	}
	rep(i,1,qn){
		printf("%lld\n",ans[Q[i]]);
	}
}

int main()
{
	int _; scanf("%d",&_);
	while(_--){
		scanf("%d%d%d",&n,&m,&qn);
		rep(i,0,n) v[i].clear();
		rep(i,1,m){
			int x,y; ll w; scanf("%d%d%lld",&x,&y,&w);
			v[x].push_back(make_pair(w,y));
		}
		maxn = 0;
		rep(i,1,qn){
			scanf("%d",&Q[i]);
			maxn = max(maxn,Q[i]);
		}
		solve();
	}
	return 0;
}