// ---
// 莫队概述: \\
// \\
// 莫队本质上就是用分块去优化暴力的离线算法，将总复杂度降到 $O(n\sqrt n)$ 的位置。说白了，就是分块+暴力。\\
// \\
// 我们先讲`暴力的部分`。比如一个长度为 $n$ 的序列，$m$ 次查询，每次查询询问区间 $[l,r]$ 之间的众数。对于这个问题，暴力求的话就是直接用桶记录每个数出现的次数，然后遍历区间 $[l,r]$，直接统计答案即可。这个暴力过程和莫队暴力过程没有任何区别，然后问题就变成了如何用分块来优化这个暴力呢？\\
// \\
// 在`分块的部分`，该算法将整个序列按照 $\sqrt n$ 大小进行分块，共分成 $\sqrt n$ 块，然后对于所有的询问，先按照左端点所在的块编号进行排序，如果块编号相同，再按照右端点排序。询问排序完之后，就直接暴力求解即可。代码的话看一下下面习题就可以掌握了。\\
// \\
// 最后就是`时间复杂度`的问题了。如何证明这个算法的时间复杂度呢？我们对每一个块分开进行考虑，假设有 $b_i$ 次操作在第 $i$ 个块中，则在这个块中，右端点一定递增，因此右端点最多移动 $n$ 次，而左端点每次最多移动 $\sqrt n$，一共最多移动 $b_i*\sqrt n$ 次，每次端点移动的时间复杂度为 $O(1)$，因此移动的总次数为 $\sum\limits_{i=1}^{\sqrt n}(b_i*\sqrt n+n)=m*\sqrt n+n*\sqrt n$，因此总复杂度为 $O(n\sqrt n+m\sqrt n)$。\\
// \\
// 题意: \\
// $n$ 双颜色不同袜子，$m$ 次询问，每次询问给出 $[L,R]$ 区间，询问在 $[L,R]$ 区间中随机抽出两双颜色相同的袜子的概率，输出最简分数形式 $(A/B)$。$(1\leq n,m\leq 50000)$\\
// \\
// 思路: \\
// 普通莫队算法的复杂度是 $O(N\sqrt N)$，实现关键点就在于能否在区间左右端点移动时，$O(1)$ 的更新答案。\\
// \\
// 我们观察这道题目，可以发现区间 $[L,R]$ 取出两双颜色相同袜子的概率 = $\frac{\frac{1}{2}*\sum\limits \\
// _{i=L}^{R}num[i]}{C(R-L+1,2)}$，$num[i]$ 表示在区间 $[L,R]$ 中有多少双与 $i$ 颜色相同的袜子，乘以 $\frac{1}{2}$ 的原因在于每一对颜色相同的袜子被计算了两遍。\\
// \\
// 分析到这里，就可以发现这是一道普通莫队的裸题，我们添加与删除时只需加上或减去当前与该点颜色相同的袜子数，这样同时可以避免重复计算。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
const int N = 2*1e5+100;
using namespace std;

int a[N],pos[N],n,m,L,R;
ll ans[N][2],flag[N],Ans;
struct Node{
	int l,r,id;
	bool operator < (Node xx) const{
		if(pos[l] == pos[xx.l]) return r < xx.r;
		else return pos[l] < pos[xx.l];
	}
}Q[N];

ll gcd(ll a,ll b) {return b == 0 ? a:gcd(b,a%b);}

void add(int x){
	Ans += flag[a[x]];
	flag[a[x]]++;
}

void del(int x){
	flag[a[x]]--;
	Ans -= flag[a[x]];
}

int main()
{
	L = 1, R = 0;
	scanf("%d%d",&n,&m);
	int sz = sqrt(n);
	rep(i,1,n){
		scanf("%d",&a[i]);
		pos[i] = i/sz;
	}
	rep(i,1,m){
		scanf("%d%d",&Q[i].l,&Q[i].r);
		Q[i].id = i;
	}
	sort(Q+1,Q+1+m);
	rep(i,1,m){
		while(L < Q[i].l) del(L),L++;

		while(L > Q[i].l) L--, add(L);
		
		while(R < Q[i].r) R++, add(R);
		
		while(R > Q[i].r) del(R), R--;

		ll len = Q[i].r-Q[i].l+1;
		ll tp = len*(len-1ll)/(ll)2;
		ll g = gcd(Ans,tp);
		ans[Q[i].id][0] = Ans/g;
		ans[Q[i].id][1] = tp/g;
	}
	rep(i,1,m) printf("%lld/%lld\n",ans[i][0],ans[i][1]);
	return 0;
}