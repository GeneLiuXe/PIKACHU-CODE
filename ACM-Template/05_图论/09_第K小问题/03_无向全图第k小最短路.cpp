// ---
// 题意：$n$ 个点，$m$ 条边的有权无向图，询问图中第 $k$ 大的最短路，注意路径 $(u,v)$ 被计算，仅当这条路径是 $(u,v)$ 之间的最短路，保证有解。$(2\leq n,m\leq 2*10^5,1\leq k\leq 400)$\\
// \\
// 思路：无向图中的问题，主要难点在于如何才能避免算重。\\
// \\
// 我们注意到此题是两点之间的最短路，而两点之间只会有一条最短路，路径端点相同且长度相同算一条。因此我们将路径两点存入 $map$，当且仅当路径两点第一次在一条路径中出现时，才更新答案。\\
// \\
// 因此我们令 $u$ 为 $v$ 的前驱，状态定义为 $(u,v,id,w)$ 表示 $u$ 是 $v$ 的前驱，且 $v$ 是 $u$ 的第 $id$ 条出边，当前路径权重为 $w$。\\
// \\
// 因此状态就是路径的起点和终点以及路径的权重，每次选择终点的出边进行状态更新即可。\\
// ---
#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 2*1e5+100;
const int M = 4*1e5+100;
const db EPS = 1e-9;
using namespace std;
 
int n,m,k;
vector<pair<ll,int> > v[N];
struct Node{
	ll w; int u,v;
	Node(ll a,int b,int c) : w(a), u(b), v(c) {}
	bool operator < (Node xx) const {
		return w > xx.w;
	}
};
map<pair<int,int>,int> mp;
vector<Node> temp;
priority_queue<Node> q;
 
void dijsktra(){
	int cnt = 0;
	mp.clear();
	while(q.size()) q.pop();
	rep(i,1,n) q.push({0,i,i});
	while(q.size()){
		Node tp = q.top(); q.pop();
		if(mp.find(make_pair(tp.u,tp.v)) != mp.end()) continue;
		if(tp.u != tp.v && mp.find(make_pair(tp.u,tp.v)) == mp.end() && mp.find(make_pair(tp.v,tp.u)) == mp.end()){
			cnt++;
			if(cnt == k){
				printf("%lld\n",tp.w);
				return;
			}
		}
		mp[make_pair(tp.u,tp.v)] = 1;
		int rt = 0;
		for(pair<ll,int> &thp:v[tp.v]){
			rt++;
			if(rt+cnt > k) break;
			int y = thp.second;
			if(mp.find(make_pair(tp.u,y)) != mp.end()) continue;
			q.push({tp.w+thp.first,tp.u,y});
		}
	}
}
 
int main()
{
	scanf("%d%d%d",&n,&m,&k);
	rep(i,1,m){
		int x,y; ll w; scanf("%d%d%lld",&x,&y,&w);
		temp.push_back({w,y,x});
	}
	sort(temp.begin(),temp.end());
	int siz = temp.size();
	for(int i = siz-1; i >= max(siz-1-k,0); i--){
		int x = temp[i].u, y = temp[i].v, w = temp[i].w;
		v[x].push_back(make_pair(w,y));
		v[y].push_back(make_pair(w,x));
	}
	rep(i,1,n) sort(v[i].begin(),v[i].end());
	dijsktra();
	return 0;
}