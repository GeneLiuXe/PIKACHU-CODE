// ---
// 一、适用问题: \\
// 整体二分，即对所有的查询进行一个整体的二分答案，需要数据结构题满足以下性质。\\
// 1. 询问的答案具有可二分性\\
// 2. 修改对判定答案的贡献相对独立，修改之间互不影响效果\\
// 3. 修改如果对判定答案有贡献，则贡献为一确定的与判定标准无关的值\\
// 4. 贡献满足交换律、结合律，具有可加性\\
// 5. 题目允许离线操作\\
// \\
// (来自《浅谈数据结构题的几个非经典解法》)
// \\
// 上面的性质看上去复杂，其实只要满足询问答案具有可二分性，且题目允许离线操作，就可以考虑一下是否可以利用整体二分算法进行求解。\\
// \\
// 二、整体二分算法解析 \\
// 上面的文字可能有些过于理论，我们现在用浅显一点的方式来理解这个算法。\\
// \\
// 假设你现在有 $q$ 次查询，查询区间第 $k$ 大的值。首先考虑如果只有 $1$ 个查询，是否可以直接二分解决。\\
// \\
// 显然是可以的，我们只需要定位到具体区间，数一下小于等于当前二分值的数个数是否大于等于 $k$ 即可。于是问题就变成了如何从单次二分演变到整体二分。\\
// \\
// 我们首先维护一个操作序列，即每个点的赋值和查询，共 $n+q$ 个操作。然后实现一个 $solve(l,r,L,R)$ 函数，表示当前的操作序列在 $[L,R]$ 范围内，而该操作序列中所有的查询操作的答案都在 $[l,r]$ 中。\\
// \\
// 于是我们二分一个值 $mid=(l+r)/2$，然后将 $[L,R]$ 中所有的赋值操作中数值小于等于 $mid$ 的数加入到对应位置，比如 $a[x]\leq mid$，则 $sum[x]=sum[x]+1$，对于所有的查询操作，判断其查询区间 $[x,y]$ 的值是否大于等于 $k$，如果是则将其递归到 $solve(l,mid)$ 中，否则递归到 $solve(mid+1,r)$ 中，具体内容看一下下面的例题就可以理解。\\
// \\
// 最后分析一下时间复杂度，最多分了 $logn$ 层，每一层的时间复杂度为 $O(nlogn)$，因此总时间复杂度为 $O(nlog^2n)$。\\
// \\
// 最后附上《浅谈数据结构题的几个非经典解法》中对该算法的理论概述。\\
// \\
// 询问的答案可二分且修改对判定标准的贡献相对独立，且贡献的值与判定标准无关。因此如果我们已经计算过某一些修改对询问的贡献，那么这个贡献永远不会改变，我们没有必要当判定标准改变时再次计算这部分修改的贡献，只要记录下当前的总贡献，再进一步二分时，直接加上新的贡献即可。\\
// \\
// 三、例题解析\\
// 题意: \\
// 无修改的区间第 $k$ 大数问题。$(1\leq n\leq 10^5,1\leq m\leq 5000)$\\
// \\
// 思路: \\
// 主席树模板题，但此处我们要用整体二分的方法来解决此题。\\
// \\
// 首先我们将所有赋值操作和查询操作都放到一个数组中，形成了此题的操作序列。然后就是代码中的核心关键点 $solve(l,r,L,R)$ 函数，该函数表示区间 $[L,R]$ 中的操作序列中的查询操作的答案一定在 $[l,r]$ 范围内。\\
// \\
// 因此问题就变成了如何将 $[L,R]$ 中的序列进行分组，再递归到 $solve(l,mid)$ 和 $solve(mid+1,r)$ 中。我们只需遍历 $[L,R]$ 中的所有操作，如果是赋值操作，则判断数值 $x$ 是否大于 $mid$，如果小于等于 $mid$，则将该操作丢到 $q_1$ 数组中，并在树状数组的 $x$ 位置加 $1$；否则将操作丢到 $q_2$ 数组中。\\
// \\
// 如果是查询操作，则在树状数组中查询区间 $[q[i].x,q[i].y]$ 的值 $tmp$，如果 $k\leq tmp$，则将该操作丢到 $q_1$ 中；否则将 $k=k-tmp$，然后丢到 $q_2$ 中。\\
// \\
// 可能说起来比较复杂，但是代码比较清晰，推荐直接对代码进行理解。\\
// ---
#include <iostream>
#include <algorithm>
#include <cstdio>
#define mem(a,b) memset(a,b,sizeof a);
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
typedef long long ll;
typedef double db;
const int N = 1e5+100;
const int inf = 1e9+10;
const db EPS = 1e-9;
using namespace std;

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

int n,m,ans[N],c[N];
struct Node{int x,y,k,id;}q[2*N],q1[2*N],q2[2*N];
inline int lowbit(int x) {return x&(~x+1);}
inline void update(int x,int v) {for(; x<=n; x+=lowbit(x)) c[x] += v;}
inline int ask(int x){
	int res = 0;
	while(x) res += c[x], x -= lowbit(x);
	return res;
}

void solve(int l,int r,int L,int R){
	if(l > r || L > R) return;
	if(l == r){
		rep(i,L,R) if(q[i].id) ans[q[i].id] = l;
		return;
	}
	int cnt1 = 0, cnt2 = 0, mid = (l+r)>>1;
	rep(i,L,R){
		if(q[i].id){ //查询
			int tmp = ask(q[i].y)-ask(q[i].x-1);
			if(q[i].k <= tmp) q1[++cnt1] = q[i];
			else q[i].k -= tmp, q2[++cnt2] = q[i];
		}
		else{ //赋值
			if(q[i].x <= mid) update(q[i].y,1), q1[++cnt1] = q[i];
			else q2[++cnt2] = q[i];
		}
	}
	rep(i,1,cnt1) if(!q1[i].id) update(q1[i].y,-1);
	rep(i,1,cnt1) q[L+i-1] = q1[i];
	rep(i,1,cnt2) q[L+cnt1+i-1] = q2[i];
	solve(l,mid,L,L+cnt1-1); solve(mid+1,r,L+cnt1,R);
}

int main()
{
	scanf("%d%d",&n,&m);
	rep(i,1,n) {scanf("%d",&q[i].x); q[i].id = 0; q[i].y = i;}
	rep(i,1,m) {scanf("%d%d%d",&q[i+n].x,&q[i+n].y,&q[i+n].k); q[i+n].id = i;}
	solve(-inf,inf,1,n+m);
	rep(i,1,m) printf("%d\n",ans[i]);
	return 0;
}