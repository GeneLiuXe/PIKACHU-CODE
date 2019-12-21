#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <queue>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 5*1e6+100;
const int M = 5*1e6+100;
const ll inf = 5*1e16;
const db EPS = 1e-9;
using namespace std;

struct Edge{
	int to,next;
	ll w;
}e[M];
int head[N],tot,n,vis[N];
ll c[N],dis[N];

void add(int x,int y,ll w){
	e[++tot].to = y, e[tot].next = head[x], e[tot].w = w, head[x] = tot;
}

priority_queue< pair<ll,int> > q;

ll dijkstra(int s)
{
	while(q.size()) q.pop();
	memset(vis,0,sizeof vis);
	rep(i,0,2*n+2) dis[i] = inf;
	dis[s] = 0;
	q.push(make_pair(0ll,s));
	while(q.size())
	{
		int x = q.top().second;
		q.pop();
		if(vis[x]) continue;
		vis[x] = 1;	//只有从队列中弹出时才是更新好了这个点，此时才可以更新vis，不能在加入的时候就更新vis
		for(int i = head[x]; i; i = e[i].next)
		{
			int y = e[i].to;
			ll z = e[i].w;
			if(dis[y] > (ll)(dis[x] + z)){
				dis[y] = dis[x]+z;
				q.push(make_pair(-dis[y],y));
			}
		}
	}
	if(dis[n*2+2] == inf) return -1;
	return dis[2*n+2];
}

int main()
{
	__; cin >> n; tot = 1;
	rep(i,1,n) cin >> c[i];
	string b1,b2;
	int r1,r2;
	rep(i,1,n){
		string s1,s2;
		cin >> s1; s2 = s1;
		reverse(s2.begin(),s2.end());
		if(i == 1){
			b1 = s1, b2 = s2;
			r1 = i, r2 = i+n;
			add(2*n+1,r1,0);
			add(2*n+1,r2,c[1]);
		}
		else{
			if(b1 <= s1) add(r1,i,0ll);
			if(b1 <= s2) add(r1,i+n,c[i]);
			if(b2 <= s1) add(r2,i,0ll);
			if(b2 <= s2) add(r2,i+n,c[i]);
			b1 = s1, b2 = s2;
			r1 = i, r2 = i+n;	
		}
	}
	add(n,2*n+2,0);
	add(2*n,2*n+2,0);
	ll x1 = dijkstra(2*n+1);
	cout << x1 << endl;
	return 0;
}