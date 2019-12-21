// ---
// 题意：长度为 $n$ 的序列，每个点都有一个颜色，一共有 $C$ 个颜色，支持两种操作，第一种给出 $l\ r\ x$，将区间 $[l,r]$ 全部染成 $x$，第二种给出一个 $x$，询问 $x$ 颜色一共在序列中出现了多少次。\\
// \\
// 所有操作结束后，还要查询每种颜色出现的次数，给出最多出现的次数。$(1\leq n,C,m\leq 10^5)$ \\
// \\
// 思路: 非常明显的区间推平操作，应该使用珂朵莉树进行解决，即用 $set$ 维护每一块相同区域。\\
// \\
// 老司机树是可以的，但是并没有采用，还是使用了分块的方法，虽然 $T$ 的飞起...一开始的想法是给每个块记一个 $map$，标记每个块中各个数字出现的次数，如果整个块被赋值了，就 $map.clear()$。但是 $T$ 的不行，第 $63$ 个点怎么也过不去...\\
// \\
// 后来采用了完全的暴力分块，即每个块打个 $lazy$，如果 $lazy$ 不为 $0$，则整个块中所有点大小均为 $lazy$。如果 $lazy$ 为 $0$，则暴力修改、查询。那么如何分析这个做法的时间复杂度呢？\\
// \\
// 一共两种操作，修改操作每次最多涉及 $\sqrt n$ 个块，如果块被完全覆盖，$O(1)$ 打上标记，如果块只被部分覆盖，下放标记 $O(\sqrt n)$，修改点值 $O(\sqrt n)$，由于每次最多只有 $2$ 个块被部分覆盖，最多 $\sqrt n$ 个块被完全覆盖，因此修改操作总时间复杂度为 $O(\sqrt n)$。\\
// \\
// 而对于查询操作，如果该块被打上了标记，则查询复杂度为 $O(1)$，如果没有打上标记，则直接暴力查询，时间复杂度为 $O(\sqrt n)$。此题是修改与查询次数一致，而修改操作每次最多使得 $2$ 个块标记消失。因此可以发现，我们可以故意设置数据使得此种做法 $TLE$，即利用修改操作使得所有块的 $lazy$ 标记消失，然后每次查询颜色时就会遍历所有块，可以将时间复杂度卡成 $O(n^2)$。（不过此题数据较水，此种方法也可以过，而且很快，$390\ ms$ 即可通过...）\\
// \\
// 因此正确的做法是当块没有 $lazy$ 标记时，为了避免直接暴力询问，我们对于每个块记录一个 $unordered\_map$，查询复杂度为 $O(1)$。修改复杂度会变成 $O(\sqrt n*log(n))$，查询复杂度为 $O(\sqrt n)$，但是避免了被卡的可能。（$790\ ms$ 可以通过）\\
// \\
// 总结: 对于一些不太常见的数据结构问题，一定要尝试考虑分块，因为分块方法用途很多，而且算法核心是暴力。\\
// \\
// 而对于分块算法，一定要多多尝试，你可能觉得这样分块会被你故意构造的一组数据卡成 $O(n^2)$，但是出题人未必就考虑到了你的这种做法，因此一定要多尝试，不要只去尝试那些 $O(n*logn*\sqrt n)$ 的算法。当然如果能直接想到 $O(\sqrt n)$ 的算法则是最好的！\\
// ---
#include <bits/stdc++.h>
#define mem(a,b) memset(a,b,sizeof a);
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
typedef long long ll;
typedef double db;
const int N = 1e5+10;
const db EPS = 1e-9;
using namespace std;
 
void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}
 
int n,m,a[N],L[N],R[N],pos[N],cnt,sz,lazy[N],num[N];
unordered_map<int,int> mp[1010];
 
void change(int l,int r,int x){
	rep(i,pos[l],pos[r]){
		if(L[i] >= l && R[i] <= r){
			// mp[i].clear();
			// mp[i][x] = R[i]-L[i]+1;
			lazy[i] = x;
		}
		else{
			if(lazy[i]){
				rep(j,L[i],R[i]) a[j] = lazy[i];
				lazy[i] = 0;
			}
			rep(j,max(l,L[i]),min(R[i],r)){
				// mp[i][a[j]]--;
				// mp[i][x]++;
				a[j] = x;
			}
		}
	}
}
 
int ask(int x){
	int ans = 0;
	rep(i,1,cnt){
		if(lazy[i]){
			if(lazy[i] == x) ans += R[i]-L[i]+1;
		}
		else{
			rep(j,L[i],R[i])
				if(a[j] == x) ans++;
		}
	}
	return ans;
}
 
int main()
{
	int h; scanf("%d%d%d",&n,&h,&m);
	sz = sqrt(n);
	cnt = n/sz;
	if(n%sz != 0) cnt++;
	rep(i,1,cnt){
		L[i] = sz*(i-1)+1;
		R[i] = min(sz*i,n);
		mp[i][1] = R[i]-L[i]+1;
	}
	rep(i,1,n) pos[i] = ((i-1)/sz)+1;
	rep(i,1,n) a[i] = 1;
 
	while(m--){
		ll p,x,a,b;
		scanf("%lld%lld%lld%lld",&p,&x,&a,&b);
		ll s = ask(p);
		ll m1 = (a+s*s)%n;
		ll m2 = (a+(s+b)*(s+b))%n;
		change(min(m1,m2)+1,max(m1,m2)+1,x);
	}
	rep(i,1,cnt)
		if(lazy[i]){
			rep(j,L[i],R[i]) a[j] = lazy[i];
			lazy[i] = 0;
		}
	rep(i,1,n)
		num[a[i]]++;
	int maxx=0;
	rep(i,1,h)
		maxx = max(maxx,num[i]);
	printf("%d\n",maxx);
	return 0;	
}