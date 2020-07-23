// ---
// 题意: 给出一个 $n$ 个点，$m$ 条边的无向图。定义一个三元组 $(u,v,s)$，表示 $u$ 到 $v$ 的一条路径异或和为 $s$，该三元组对答案的贡献为 $s$。求出所有三元组的贡献和。$(1\leq n\leq 10^5,0\leq m\leq 2*10^5)$ \\
// \\
// 思路: \\
// 直接求答案显然不现实，而由于这是一道异或问题，因此考虑对每一个数位求贡献。首先对连通分量求一个 $dfs$ 树，即求出从根节点到每一个点的异或和，因此两点路径异或和就等于两个点的异或和再异或。$dfs$ 时再求出连通分量中所有环的异或和，将异或和插入线性基中。然后依次统计每一个数位对答案的贡献。\\
// \\
// 对于数位 $i$ 来说，该数位统计到答案中只有两种情况。第一种情况，两点异或和在数位 $i$ 处为 $1$，则求出线性基数集合中所有第 $i$ 位为 $0$ 的数个数。\\
// \\
// 第二种情况，两点异或和在数位 $i$ 处为 $0$，则求出线性基数集合中所有第 $i$ 位为 $1$ 的数个数。\\
// \\
// 将两种情况分别统计累加到答案中，即可完成此题。此处需要注意，判断第 $i$ 位为 $1$ 的数能否由线性基组成，只需要将线性基中的 $p[i]$ 依次取或得到 $ALL$，若 $ALL$ 中第 $i$ 位为 $1$，则可以构造，否则不可。\\
// ---
#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 1e5+100;
const int M = 4*1e5+100;
const ll mod = 1e9+7;
const db EPS = 1e-9;
using namespace std;

int n,m,head[N],tot,vis[N];
ll rt[65],dis[N];
vector<ll> ve;
struct Node{
	int to,next;
	ll w;
}e[M];
ll ans = 0;

void add(int x,int y,ll w){
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot, e[tot].w = w;
}

struct Linear_Basis{
	ll p[65],cnt;
	Linear_Basis() {memset(p,0,sizeof p); cnt = 0;}
	~Linear_Basis() {}
	void init(){
		memset(p,0,sizeof p);
		cnt = 0;
	}
	bool ins(ll x){
		// LOG1("x",x);
		for(int i = 62; i >= 0; i--){
			if(x&(1ll<<i)){
				if(!p[i]) {p[i]=x; cnt++; break;}
				x ^= p[i];
			}
		}
		return x > 0;
	}
	void rebuild(){
		cnt = 0;
		for(int i = 62; i >= 0; i--)
			for(int j = i-1; j >= 0; j--)
				if(p[i]&(1ll<<j)) p[i]^=p[j];
		rep(i,0,62)
			if(p[i]) cnt++;
	}
}LB;

void dfs(int x,ll tp){	
	if(vis[x]) return;
	vis[x] = 1; ve.push_back(tp); dis[x] = tp;
	for(int i = head[x]; i; i = e[i].next){
		int y = e[i].to;
		if(vis[y]){
			ll xp = tp^e[i].w^dis[y];
			// LOG2("y",y,"xp",xp);
			LB.ins(xp);
			continue;
		}
		dfs(y,tp^e[i].w);
	}
}

int main()
{
	scanf("%d%d",&n,&m); tot = 1;
	rep(i,1,m){
		int a,b; ll c; scanf("%d%d%lld",&a,&b,&c);
		add(a,b,c); add(b,a,c);
	}
	rep(i,1,n)
		if(!vis[i]){
			LB.init(); ve.clear();
			dfs(i,0);
			ll thp = (ll)ve.size();
			// LOG1("thp",thp);
			memset(rt,0,sizeof rt);
			for(auto &v:ve){
				int tp = 0; ll hp = v;	
				while(hp){
					if(hp&1) rt[tp]++;
					tp++; hp /= 2ll;
				}
			}
			LB.rebuild();
			ll Total = 1;
			rep(H,1,LB.cnt) Total = (Total*2ll)%mod;
			ll ALL = 0;
			rep(H,0,62) ALL |= LB.p[H];
			// LOG1("cnt",LB.cnt);
			rep(k,0,62){
				ll hp1 = rt[k], hp2 = thp-rt[k]; //有和没有
				ll tp = 0;
				if(ALL&(1ll<<k)){
					tp = 1;
					rep(H,1,LB.cnt-1) tp = (tp*2ll)%mod;
				}
				// LOG3("Total",Total,"k",k,"tp",tp);
				//01、10
				ll now1 = (hp1*(hp1-1ll)/2ll+hp2*(hp2-1ll)/2ll)%mod, now2 = (hp1*hp2)%mod;
				ans = (ans+(1ll<<k)%mod*now1%mod*tp%mod)%mod;
				ans = (ans+(1ll<<k)%mod*now2%mod*(Total-tp)%mod)%mod;
			}
		}
	printf("%lld\n",ans);
	return 0;
}