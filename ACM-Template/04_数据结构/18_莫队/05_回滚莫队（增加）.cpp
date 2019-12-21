// ---
// 回滚莫队概述: \\
// 回滚莫队的关键点在于只能增加或删除节点，我们以求取 $max$ 为例。求取 $max$ 时，增加节点时可以顺便更新答案，但是删除节点时就非常不好维护，因此我们需要设计一个只需要增加节点的莫队算法。\\
// 1. 首先还是老套路，按照 $\sqrt n$ 进行分块，并确定每一块的左右边界，分别为 $xl[i]、xr[i]$。\\
// 2. 然后我们按照左端点所在块编号为第一关键字，右端点大小为第二关键字，对所有查询进行排序。\\
// 3. 接下来对于所有左右端点在同一块中的查询，我们直接暴力求取答案，复杂度为 $O(\sqrt n)$。\\
// 4. 对于左端点所在块相同的查询，其右端点不断递增，因此右端点最多移动 $O(n)$，总共 $\sqrt n$ 个块，右端点复杂度为 $O(n\sqrt n)$。\\
// 5. 接下来考虑左端点的移动，我们对于所有左端点所在块相同的查询，每一个查询结束之后都要把左端点移动到 $xr[i]+1$ 的位置，即左端点所在块的右端点 $+1$ 的位置，这样可以保证每次查询都是不断增加节点的，因此不会影响最终答案。每个查询，左端点最多移动距离为 $O(\sqrt n)$，因此左端点移动的复杂度为 $O(m\sqrt n)$。所以综合左右端点的移动，该算法的复杂度为 $O(m\sqrt n+n\sqrt n)$，考虑到 $m$ 的范围通常与 $n$ 一致，因此最终复杂度为 $O(n\sqrt n)$。\\
// \\
// 上述过程就是回滚莫队的求取过程，习题中分别给出了增加节点和减少节点的回滚莫队算法，其它具体实现细节可以查看代码。\\
// \\
// 题意: \\
// 长度为 $n$ 的序列，每个数的大小为 $x_i$。一共 $q$ 次查询，每次给出一个区间 $l$、$r$，询问区间 $[l,r]$ 中每个数贡献的最大值，一个数的贡献为 $x_i*cnt[x_i]$，即数大小 $*$ 该数出现次数。$(1\leq n,q,\leq 10^5, 1\leq x_i\leq 10^9)$ \\
// \\
// 思路: \\
// 首先把序列离散化，然后用一个桶记录每一个数字出现的次数。\\
// \\
// 接下来就是回滚莫队的基本操作了，求出每块的左右端点，然后对查询排序。每次查询时判断左右端点是否在同一个快内，如果在就暴力求，如果不在就增加节点扩充区间。每个查询结束后，要将左端点再移动到该块的右边界 $+1$ 位置，具体的实现细节见代码。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
const int N = 2e5+100;
using namespace std;

int n,m,sz,pos[N],a[N],b[N],tot,val[N],xl[N],xr[N],cnt[N],L,R,_cnt[N],lastblock;
//Maxn - 左右端点控制的最大值，temp - 临时最大值
//cnt - 左右端点移动时计数，_cnt - 左右端点同块时的计数
ll ans[N],Maxn,temp;
struct Node{
	int l,r,id;
	bool operator < (Node xx) const {
		if(pos[l] == pos[xx.l]) return r < xx.r;
		else return pos[l] < pos[xx.l];
	}
}q[N];

void init(){
	scanf("%d%d",&n,&m); sz = sqrt(n);
	rep(i,1,n) {scanf("%d",&a[i]); b[++tot] = a[i];}
	rep(i,1,m) {scanf("%d%d",&q[i].l,&q[i].r); q[i].id = i;}
	sort(b+1,b+1+tot); tot = unique(b+1,b+1+tot)-b-1;
	rep(i,1,n) val[i] = lower_bound(b+1,b+1+tot,a[i])-b;
	rep(i,1,n){
		pos[i] = i/sz;
		xl[pos[i]] = (xl[pos[i]] == 0 || xl[pos[i]] > i) ? i : xl[pos[i]];
		xr[pos[i]] = (xr[pos[i]] < i) ? i : xr[pos[i]];
	}
	sort(q+1,q+1+m);
}	

inline ll add(int x){
	return (++cnt[val[x]])*(ll)b[val[x]];
}

inline void del(int x) {cnt[val[x]]--;}

void solve(){
	L = 1, R = 0, lastblock = -1;
	rep(i,1,m){
		if(pos[q[i].l] == pos[q[i].r]){
			ll temp = 0;
			rep(j,q[i].l,q[i].r) temp = max(temp,(++_cnt[val[j]])*(ll)b[val[j]]);
			rep(j,q[i].l,q[i].r) _cnt[val[j]]--;
			ans[q[i].id] = temp;
		}
		else{
			if(lastblock != pos[q[i].l]){
				while(L < xr[pos[q[i].l]]+1) del(L), L++;
				while(R > L-1) del(R), R--;
				Maxn = 0; lastblock = pos[q[i].l];
			}
			//Maxn为右半部分的最大值，不包含左端点所在块的情况
			while(R < q[i].r) R++, Maxn = max(Maxn,add(R));
			temp = Maxn;
			//temp从Maxn继承而来，表示整个区间的最大值
			while(L > q[i].l) L--, temp = max(temp,add(L));
			while(L < xr[pos[q[i].l]]+1) del(L), L++;
			ans[q[i].id] = temp;
		}
	}
}

int main()
{
	init();
	solve();
	rep(i,1,m) printf("%lld\n",ans[i]);
	return 0;
}