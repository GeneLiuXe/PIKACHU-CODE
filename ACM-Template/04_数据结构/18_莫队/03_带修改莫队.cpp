// ---
// 带修改莫队概述: \\
// 带修改的莫队仍然是利用分块对查询和修改排序，尽可能地减少运行时间。\\
// \\
// 假设我们按照 $k$ 大小进行分块，则一共有 $\frac{n}{k}$ 个块，然后对于每个操作，一共有三个参数，分别是 $l$、$r$、$id$，表示区间左右端点和操作时间，我们先按照左端点的块号进行排序，再按照右端点的块号进行排序，最后按照操作时间进行排序。\\
// \\
// 莫队暴力时也需要维护三个值，L、R、T 表示当前控制的左右区间以及操作时间。`对于每个查询，需要将 $L$、$R$、$T$ 移动到指定位置再进行计算，因此可以将带修改莫队理解为三维莫队。\\
// \\
// 接下来估算复杂度，假设 $m$ 次查询中，一共有 $a$ 次查询，$b$ 次修改。因此当确定左右端点块号时，即查询即按照时间排序时，$T$ 最多移动 $b$ 次，因此 $T$ 的移动一共有 $\frac{n}{k}*\frac{n}{k}*b$ 次。而每次查询，区间左右端点最多移动 $2*k$ 次，因此 $l$、$r$ 最多移动 $a*2*k$ 次，因此总时间复杂度为 $O(b*\frac{n^2}{k^2}+2*a*k)$。我们可以求导求这个函数的最小值，可以发现最后的答案会在 $k=n^{\frac{2}{3}}$ 处取到最优解，因此整个算法的复杂度也就达到了 $O(n^{\frac{5}{3}})$ 处。\\
// \\
// 题意:\\
// 长度为 $n$ 的初始序列，共有 $m$ 次操作，操作 $1$ 给出一个 $l$、$r$，令 $c_i$ 为 $i$ 在 $[l,r]$ 中出现的次数，询问 $Mex(c_0,c_1,...,c_{10^9})$。操作 $2$ 则将 $a_p$ 改成 $x$。$(1\leq n,m\leq 10^5)$ \\
// \\
// 思路: \\
// 这个问题唯一的操作难点在于 $mex$ 函数的求取，其实我们可以像求取 $SG$ 函数的 $mex$ 一样，直接暴力求取即可。然后其余部分就是常规的带修改莫队的操作了。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 2*1e5+100;
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;

int n,qq,a[N],b[N],tot,Qnum,Cnum,pos[N],ans[N],L,R,T,flag[N],vis[N];
struct Query{
	int l,r,id,t;
	bool operator < (Query xx) const {
		if(pos[l] != pos[xx.l]) return pos[l] < pos[xx.l];
		else if(pos[r] != pos[xx.r]) return pos[r] < pos[xx.r];
		else return t < xx.t;
	}
}q[M];
struct Change{
	int pos,val;
}C[M];

int find(int x){
	return lower_bound(b+1,b+1+tot,x)-b;
}

void add(int x){
	if(flag[a[x]]!=0) vis[flag[a[x]]]--;
	flag[a[x]]++; vis[flag[a[x]]]++;
}

void del(int x){
	vis[flag[a[x]]]--; flag[a[x]]--;
	if(flag[a[x]] != 0) vis[flag[a[x]]]++;
}

void Work(int x,int i){
	if(C[x].pos >= q[i].l && C[x].pos <= q[i].r){
		vis[flag[a[C[x].pos]]]--; flag[a[C[x].pos]]--;
		if(flag[a[C[x].pos]] != 0) vis[flag[a[C[x].pos]]]++;
		if(flag[C[x].val] != 0) vis[flag[C[x].val]]--;
		flag[C[x].val]++; vis[flag[C[x].val]]++;
	}
	swap(a[C[x].pos],C[x].val);
}

int solve(){
	rep(i,0,n)
		if(!vis[i]) return i;
}

int main()
{
	scanf("%d%d",&n,&qq);
	rep(i,1,n){
		scanf("%d",&a[i]);
		b[++tot] = a[i];
	}
	rep(i,1,qq){
		int op,l,r; scanf("%d%d%d",&op,&l,&r);
		if(op == 1) Qnum++, q[Qnum] = {l,r,Qnum,Cnum};
		else C[++Cnum] = {l,r}, b[++tot] = r;
	}
	sort(b+1,b+1+tot);
	tot = unique(b+1,b+1+tot)-b-1;
	int sz = pow(n,0.66666666666666);
	rep(i,1,n) pos[i] = i/sz;
	sort(q+1,q+1+Qnum);
	L = 1, R = 0, T = 0;
	vis[0] = 1;
	rep(i,1,n) a[i] = find(a[i]);
	rep(i,1,Cnum) C[i].val = find(C[i].val);
	rep(i,1,Qnum){
		while(L < q[i].l) del(L++); 
		while(L > q[i].l) add(--L);
		while(R < q[i].r) add(++R);
		while(R > q[i].r) del(R--);
		while(T < q[i].t) Work(++T,i);
		while(T > q[i].t) Work(T--,i);
		ans[q[i].id] = solve();
	}
	rep(i,1,Qnum) printf("%d\n",ans[i]);
	return 0;
}