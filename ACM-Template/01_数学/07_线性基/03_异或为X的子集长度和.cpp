// ---
//题意: 给出一个集合$A$，求出 $A$ 中所有异或和为 $0$ 的子集的长度和。$(1\leq n\leq 2*10^6,0\leq a_i\leq 10^{18})$\\
// \\
// 思路: 首先线性基中并没有可以直接求出长度和的方法，因此我们需要考虑其他方法进行求解。\\
// \\
// 既然无法从整体上进行求解，从单个异或和为 $0$ 的子集入手也不现实，因此考虑最小单元 $a_i$。即求出每一个 $a_i$ 对答案的贡献。此处思考的入手点在于将题目要求解的值不断细分，从整体到子集再到子集中的一个元素。\\
// \\
// 对于每一个 $a_i$ 求贡献，问题就简化了很多。首先对集合 $A$ 求线性基，则有 $cnt$ 个元素成功插入，$n-cnt$ 个元素未插入，则该 $n-cnt$ 个元素对答案的贡献为 $(n-cnt)*2^{n-cnt}$。\\
// \\
// 再对于线性基中的 $cnt$ 个元素求贡献。每次取出 $cnt$ 中的一个元素 $a_x$，将剩下的 $n-1$ 个元素插入线性基中，新形成的线性基元素个数为 $T$。若新形成的线性基中无法插入 $a_x$，则 $a_x$ 对答案对贡献为 $2^{n-1-T}$。至此本题结束。\\
// \\
// 总结: 本题中还需涉及从线性基中删除一个元素和合并线性基。合并线性基不用多说，删除一个元素则是记录每一个元素插入线性基的位置，从最后一个元素往回不断删除，每次将插入位置置$0$即可。\\
// \\
// 本题还可以进行修改，修改为求 $A$ 中所有异或和为 $X$ 的子集长度和。其余地方都无需改动，只需要将 "判断新形成的线性基中是否可以插入 $a_x$" 修改为 "判断新形成的线性基中是否可以插入 $a_x\ xor\ \text{X}$" 即可。 \\
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
const int M = 1e5+100;
const ll mod = 1e9+7;
const db EPS = 1e-9;
using namespace std;

int n;
ll a[N],ans;
vector<pair<ll,int> > v;

struct Linear_Basis{
	ll p[65]; int cnt;
	Linear_Basis() {memset(p,0,sizeof p); cnt = 0;}
	~Linear_Basis() {}
	void init() {memset(p,0,sizeof p); cnt = 0;}
	int ins(ll x){
		int pos = -1;
		for(int i = 62; i >= 0; i--)
			if(x&(1ll<<i)){
				if(!p[i]) {p[i] = x; pos = i; cnt++; break;}
				else x ^= p[i];
			}
		return pos;
	}
	Linear_Basis& merge(Linear_Basis x){
		for(int i = 62; i >= 0; i--)
			if(x.p[i]) ins(x.p[i]);
		return *this;
	}
}LB1,LB2;

ll poww(ll a,ll b){
	ll base = a, tp = 1;
	while(b){
		if(b&1) tp = (tp*base)%mod;
		base = (base*base)%mod;
		b >>= 1;
	}
	return (tp%mod);
}

int main()
{
	while(~scanf("%d",&n))
	{
		ans = 0; LB1.init(); LB2.init(); v.clear();
		rep(i,1,n) scanf("%lld",&a[i]);
		rep(i,1,n){
			int flag = LB1.ins(a[i]);
			if(flag != -1) v.push_back(make_pair(a[i],flag));
			else LB2.ins(a[i]);
		}
		int SIZE = v.size();
		// LOG1("SIZE",SIZE);
		ll tp = 1;
		if(n-SIZE-1 >= 1) tp = poww(2ll,n-SIZE-1);
		ans = (ans+((ll)n-(ll)SIZE)*tp%mod)%mod;
		for(int i = SIZE-1; i >= 0; i--){
			LB1.p[v[i].second] = 0; LB1.cnt--;
			Linear_Basis LB3 = LB1;
			LB3 = LB3.merge(LB2);
			tp = n-1-LB3.cnt;
			int flag = LB3.ins(v[i].first);
			if(flag == -1){
				ll hp = poww(2ll,tp);
				ans = (ans+hp)%mod;
			}
			LB2.ins(v[i].first);
		}
		printf("%lld\n",ans);
	}
	return 0;
}