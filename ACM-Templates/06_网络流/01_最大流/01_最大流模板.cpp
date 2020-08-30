// ---
// 用 vector 实现的最大流，速度较快 \\
// ---
#include <bits/stdc++.h>
using namespace std;

typedef long long R;
const R inf = numeric_limits<R>::max();

struct flow {
	static const int N = 1e4+10;	
	R s, t, mx, d[N], cur[N], q[N];
	struct node {
		int u, v;
		R f;
		node(short u, short v, R f) : u(u), v(v), f(f) {}
	};
	vector<node> e;
	vector<int> v[N];
	
	void init(int mxx) { // 传入最大点数
		mx = mxx, e.clear();
		for(int i = 0; i <= mx; i++) v[i].clear();
	}

	inline void add(int x, int y, R flow) {
		e.emplace_back(x, y, flow);
		e.emplace_back(y, x, 0);
		v[x].emplace_back(e.size()-2);
		v[y].emplace_back(e.size()-1);
	}

	inline bool bfs(int l = 1, int r = 0) {
		fill(d, d + 1 + mx, -1);
		q[++r] = s, d[s] = 1;
		while(l <= r) {
			int x = q[l++];
			for(int i:v[x])
				if(d[e[i].v] == -1 && e[i].f > 0)
					d[e[i].v] = d[x] + 1, q[++r] = e[i].v;
		}
		return d[t] != -1;
	}

	inline R dfs(int x, R rf) {
		if(x == t || !rf) return rf;
		R sf = 0, c;
		for(R &i = cur[x]; i < v[x].size(); i++) {
			node &u = e[v[x][i]];
			if(d[u.v] == d[x] + 1 && (c = dfs(u.v, min(rf, u.f))))
				u.f -= c, e[v[x][i] ^ 1].f += c, rf -= c, sf += c;
			if(!rf) break;
		}
		return sf;
	}

	R dinic(int S, int T) {
		s = S, t = T;
		R res = 0;
		while(bfs()) {
			fill(cur, cur + mx + 1, 0);
			res += dfs(s, inf);
		}
		return res;
	}
} e;

int main()
{
	int n,m,s,t;
	scanf("%d%d%d%d",&n,&m,&s,&t);
	e.init(n);
	for(int u, v, i = 1; i <= m; i++) {
		R w; scanf("%d%d%lld",&u,&v,&w);
		e.add(u,v,w);
	}
	printf("%lld\n", e.dinic(s,t));
	return 0;
}