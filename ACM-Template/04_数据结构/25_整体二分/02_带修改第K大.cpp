// ---
// 题意:\\ 
// 带修改的区间第 $k$ 大数问题。$(1\leq n\leq 5*10^4,1\leq m\leq 10^4)$\\
// \\
// 思路:\\
// 带修改第 $k$ 大问题，如果要用主席树来解决的话，则需要再加上一层树状数组来维护修改信息，即用树套树解决该问题。\\
// \\
// 但是如果用整体二分来处理这个问题的话，难度则会瞬间骤降。其实此题与上题唯一的区别就是这题多了一个修改操作，而修改操作无非就是删除原来的数，加上新的数。\\
// \\
// 因此对于 $a[x]=y$ 的修改操作，我们将其拆成两部分，第一部分为删除 $a[x]$，然后令 $a[x]=y$，第二部分是加上 $a[x]$，具体细节可以参考下面的代码实现。\\
// ---
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
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

int n,m,ans[N],c[N],a[N];
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
		rep(i,L,R) if(q[i].k) ans[q[i].id] = l;
		return;
	}
	int cnt1 = 0, cnt2 = 0, mid = (l+r)>>1;
	rep(i,L,R){
		if(q[i].k){ //查询
			int tmp = ask(q[i].y)-ask(q[i].x-1);
			if(q[i].k <= tmp) q1[++cnt1] = q[i];
			else q[i].k -= tmp, q2[++cnt2] = q[i];
		}
		else{ //赋值
			if(q[i].x <= mid) update(q[i].id,q[i].y), q1[++cnt1] = q[i];
			else q2[++cnt2] = q[i];
		}
	}
	rep(i,1,cnt1) if(!q1[i].k) update(q1[i].id,-q1[i].y);
	rep(i,1,cnt1) q[L+i-1] = q1[i];
	rep(i,1,cnt2) q[L+cnt1+i-1] = q2[i];
	solve(l,mid,L,L+cnt1-1); solve(mid+1,r,L+cnt1,R);
}

int main()
{
	int _; scanf("%d",&_);
	while(_--){
		scanf("%d%d",&n,&m);
		int cnt = 0, tot = 0;
		memset(c,0,sizeof c);
		rep(i,1,n) {
			scanf("%d",&a[i]);
			q[++cnt] = {a[i],1,0,i};
		}
		rep(i,1,m){
			char op[5]; scanf("%s",op);
			int x,y,k;
			if(op[0] == 'Q'){
				scanf("%d%d%d",&x,&y,&k);
				q[++cnt] = {x,y,k,++tot};
			}
			else{
				scanf("%d%d",&x,&y);
				q[++cnt] = {a[x],-1,0,x};
				a[x] = y;
				q[++cnt] = {a[x],1,0,x};
			}
		}
		solve(-inf,inf,1,cnt);
		rep(i,1,tot) printf("%d\n",ans[i]);
	}
	return 0;
}