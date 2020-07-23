// ---
// 题意：给出 $n$ 个数字，一共 $q$ 次查询，每次询问一个 $l$、$r$，查询区间 $[l,r]$ 中有多少个不同的 $gcd$，其中一个子区间代表一个 $gcd$。$(1\leq n,q\leq 10^5,1\leq a_i\leq 10^6)$\\
// \\
// 思路：区间查询不同 $gcd$ 的个数，这类题像一类套路问题，主要要抓住 $gcd$ 的几个性质。\\
// \\
// 1. 固定右端点，移动左端点，$gcd$ 的值从 $a_r$ 不断变化，要么不变，要么至少除 $2$（因为 $gcd$ 最小值为 $2$）。因此固定右端点之后，只会存在至多 $log$ 个不同的 $gcd$，我们对于相同的 $gcd$ 仅保留最靠右的位置。\\
// 2. 因此对于每个右端点，我们记录一个 $vector$，存储对于这个右端点的所有不同的 $gcd$ 值。我们可以根据 $vector[i-1]$ 来更新 $vector[i]$。\\
// \\
// 处理完上述操作之后，我们得到了 $n*logn$ 个 $(l,r,gcd)$ 三元组，然后我们将所有查询按照右端点排序。\\
// \\
// 记录一个 $pos$，不断右移到查询的右端点位置，每次移动时将 $vector[pos]$ 内的信息存储到树状数组中，即对于每个三元组 $(l,pos,gcd)$，如果该 $gcd$ 未出现过，则在树状数组的 $l$ 位置 $+1$，并设置 $vis[gcd]=l$。如果该 $gcd$ 出现过，则比较 $l$ 是否比 $vis[gcd]$ 更大，如果更大，则修改 $gcd$ 在树状数组中的位置。上述操作即不断维护 $gcd$ 最靠右的出现位置。然后对于每个查询，直接在树状数组中区间查询即可。\\
// \\
// 总结: 此题最关键的在于发现区间 $gcd$ 不断除 $2$ 的性质，然后将查询离线利用树状数组不断维护每个 $gcd$ 最后出现的位置即可完成。\\
// ---
#include <bits/stdc++.h>
#define mem(a,b) memset(a,b,sizeof a);
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
typedef long long ll;
typedef double db;
const int N = 1e6+100;
const db EPS = 1e-9;
using namespace std;

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

int n,q,a[N],vis[N];
ll ans[N],c[N];
struct Node{
	int l,r,id;
	bool operator < (Node xx) const {
		return r < xx.r;
	}
}Q[N];
vector<pair<int,int> > base[N];

inline int lowbit(int x) {return x&(~x+1);}
inline void update(int x,ll v) {for(;x<=n;x+=lowbit(x)) c[x] += v;}
inline ll ask(int x){
	ll tp = 0;
	while(x) tp += c[x], x -= lowbit(x);
	return tp;
}

int gcd(int a,int b){
	return b == 0 ? a:gcd(b,a%b);
}

int main()
{
	while(~scanf("%d%d",&n,&q)){
		rep(i,1,n) scanf("%d",&a[i]);
		rep(i,1,n) base[i].clear();
		rep(i,0,n) c[i] = 0;
		//更新每个点的vector
		rep(i,1,n){
			base[i].push_back(make_pair(i,a[i])); vis[a[i]] = 1;
			for(auto &v:base[i-1]){
				int tp = gcd(v.second,a[i]);
				if(!vis[tp]){
					base[i].push_back(make_pair(v.first,tp));
					vis[tp] = 1;
				}
			}
			for(auto &v:base[i])
				vis[v.second] = 0;
		}
		rep(i,1,q) scanf("%d%d",&Q[i].l,&Q[i].r), Q[i].id = i;
		sort(Q+1,Q+1+q);
		int pos = 0;
		rep(i,1,q){
			while(pos <= Q[i].r){
				for(auto &v:base[pos]){
					int hp = v.first;
					if(hp > vis[v.second]){
						if(vis[v.second]) update(vis[v.second],-1);
						vis[v.second] = hp;
						update(vis[v.second],1);
					}
				}
				pos++;
			}
			ans[Q[i].id] = ask(Q[i].r)-ask(Q[i].l-1);
		}
		rep(i,1,q) printf("%lld\n",ans[i]);
		memset(vis,0,sizeof vis);
	}
	return 0;
}