// ---
// 题意: \\
// 长度为 $n$ 的序列，每个数的大小为 $a_i$。一共 $m$ 次查询，每次给出一个区间 $l$、$r$，询问区间 $[l,r]$ 中数的 $mex$，其中一个区间的 $mex$ 指该区间内最小没有出现过的自然数。$(1\leq n,q,\leq 2*10^5, 0\leq a_i\leq 10^9)$\\
// \\
// 思路: \\
// 由于是求 $mex$，而数字总数为 $2e5$，因此不需要对数字进行离散化。然后我们来分析这个问题的关键点，即增删节点的特性。\\
// \\
// 不难发现，对于这个问题来说，删除节点可以 $O(1)$ 的更新答案，但是增加节点后答案的变化难以确定，因此考虑采用删除节点形式的回滚莫队来解决这个问题。\\
// \\
// 删除节点的回滚莫队，就是区间长度不断缩小的情况。因此我们需要对每个查询的左端点所在块编号进行升序，对每个查询的右端点进行降序，这样可以保证右端点是不断递减的。\\
// \\
// 然后对于左右端点在同一个块中的情况，我们依然是暴力求取答案。而对于不在同一块中的情况，我们需要每次查询结束后都将左端点移动到查询左端点所在的块的左边界上，这样才能保证区间长度在不断缩小。\\
// \\
// 除了上述这些回滚莫队的共性点之外，我们还需要关注一些特性点。对于这个问题，我们需要在最开始将左右边界分别设置为 $1$ 和 $n$，这样的目的是保证区间长度是不断递减的。然后求取答案时，我们需要维护两部分答案，一部分是区间 $[l,r]$ 中完全包含左端点所在块的部分的答案，另一部分即为当前查询的结果。\\
// \\
// 保存第一部分答案的目的在于增加节点是不能 $O(1)$ 维护答案的，因此左端点递增之后答案就会变化而且不能恢复，所以如果不保存第一部分的答案是不能直接继承到下一个查询的，具体细节看代码就能够理解。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
const int N = 2e5+100;
using namespace std;

int n,m,a[N],sz,pos[N],xl[N],xr[N],cnt[N],ans[N],_cnt[N],lastblock,L,R;
struct Node{
	int l,r,id;
	bool operator < (Node xx) const {
		if(pos[l] == pos[xx.l]) return r > xx.r;
		else return pos[l] < pos[xx.l];
	}
}q[N];

void init(){
	scanf("%d%d",&n,&m);
	rep(i,1,n) scanf("%d",&a[i]);
	rep(i,1,m) scanf("%d%d",&q[i].l,&q[i].r), q[i].id = i;
	rep(i,1,n)
		if(a[i] > 2e5) a[i] = 2e5+1;
	int sz = sqrt(n);
	rep(i,1,n){
		pos[i] = i/sz; //点i所在块
		xl[pos[i]] = xl[pos[i]] == 0 ? i : xl[pos[i]]; //pos[i]块的左端点
		xr[pos[i]] = xr[pos[i]] < i ? i : xr[pos[i]]; //pos[i]块的右端点
	}
	sort(q+1,q+1+m);
}

inline void add(int x){
	cnt[a[x]]++;
}

inline void del(int x,int& hp){
	cnt[a[x]]--;
	if(cnt[a[x]] == 0 && a[x] < hp) hp = a[x];
}

void solve(){
	L = 1, R = n, lastblock = -1;
	rep(i,1,n) cnt[a[i]]++;
	int minn = 0;
	while(cnt[minn]) minn++;
	int base_min = minn;
	rep(i,1,m){
		if(pos[q[i].l] == pos[q[i].r]){
			rep(j,q[i].l,q[i].r) _cnt[a[j]]++;
			int now = 0;
			while(_cnt[now]) now++;
			rep(j,q[i].l,q[i].r) _cnt[a[j]]--;
			ans[q[i].id] = now;
		}
		else{
			if(lastblock != pos[q[i].l]){
				//每一次进入新的块时，右端点都是直接到n的，因此区间只有左端点在递增，可以不断O(1)维护答案
				while(R < n) R++, add(R);
				while(L < xl[pos[q[i].l]]) del(L,base_min), L++;
				minn = base_min; lastblock = pos[q[i].l];
			}
			//minn为包含左端点整个块的答案，用于继承到后续查询
			while(R > q[i].r) del(R,minn), R--;
			//temp为查询的答案
			int temp = minn;
			while(L < q[i].l) del(L,temp), L++;
			while(L > xl[pos[q[i].l]]) L--, add(L);
			ans[q[i].id] = temp;
		}
	}
}

int main()
{
	init();
	solve();
	rep(i,1,m) printf("%d\n",ans[i]);
	return 0;
}