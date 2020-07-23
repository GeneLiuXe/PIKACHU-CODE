// ---
// 题意:\\ 
// 给出区间 $[l,r]$，查询多少对 $(a,b)$ 满足如下条件。$(0\leq l\leq r\leq 10^9)$\\
// 1. $a+b=a \ xor \ b$ \\
// 2. $l\leq a\leq r$ \\
// 3. $l\leq b\leq r$ \\
// \\
// 思路:\\ 
// $CF$ $div2$ 的最后一题，比赛的时候有 $1h$ 来考虑这个问题。当时主要在思考这个题想要考察的是什么内容，思考过数位 $dp$，但是没有做过 $pair$ 类型的数位 $dp$，于是就没有从这个角度继续往下深入思考。因此剩下的大部分时间都在思考是不是一道结合某些数据结构的思维题，说实话，感觉数据结构开始限制我的思维了，什么题都老从数据结构考虑，这样非常容易被治，必须要改！\\
// \\
// 继续回到该题，此题其实想要询问的就是 $a \& b=0$ 的 $pair$ 对数。因此我们处理出一个 $solve(x,y)$ 函数，表示 $a\in[0,x],b\in[0,y]$，符合条件的 $pair$ 对数，所以最终答案就是 $solve(r,r)-solve(l-1,r)-solve(r,l-1)+solve(l-1,l-1)$。\\
// \\
// 接下来就是如何计算 $solve(x,y)$ 函数，其实维护 $x$ 和 $y$ 各自的数组，两个 $flag$，然后套上最基本的 $dfs$ 板子，稍微改一下就可以过了。\\
// \\
// 总结:\\ 
// 此题其实应该是两个数同时数位 $dp$ 的裸题，套上了一个最基本的容斥。而具体的函数实现过程还是比较套路的，并不难思考。\\
// \\
// 做不出来的原因也主要是没有从数位 $dp$ 这个角度继续往下深挖，是自己思考的片面性错失了 $AC$。\\
// ---
#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
typedef double db;
const int N = 1e5+100;
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

ll f[40],a[40],b[40]; //左-右 

ll dfs(int pos,bool flag1,bool flag2){
	if(pos == 0) return 1;
	if(!flag1 && !flag2 && f[pos] != -1) return f[pos];
	ll ans = 0;
	int end1 = flag1?a[pos]:1;
	int end2 = flag2?b[pos]:1;
	if(!flag1 && !flag2){
		ans += 3ll*dfs(pos-1,0,0);
	}
	else if(!flag1){
		rep(i,0,end2){
			if(i == 0) ans += 2ll*dfs(pos-1,0,flag2 && i == end2);
			else ans += dfs(pos-1,0,flag2 && i == end2);
		}
	}
	else if(!flag2){
		rep(i,0,end1){
			if(i == 0) ans += 2ll*dfs(pos-1,flag1 && i == end1,0);
			else ans += dfs(pos-1,flag1 && i == end1,0);
		}
	}
	else{
		rep(i,0,end1){
			rep(j,0,end2){
				if(i != 1 || j != 1) ans += dfs(pos-1,flag1 && i == end1,flag2 && j == end2);
			}
		}
	}
	if(!flag1 && !flag2) f[pos] = ans;
	return ans;
}

ll solve(ll x,ll y){
	if(x == -1 || y == -1) return 0;
	int p1 = 0, p2 = 0;
	memset(a,0,sizeof a);
	memset(b,0,sizeof b);
	while(x){
		a[++p1] = x%2;
		x /= 2;
	}
	while(y){
		b[++p2] = y%2;
		y /= 2;
	}
	return dfs(max(p1,p2),1,1);
}

int main()
{
	int _; scanf("%d",&_);
	memset(f,-1,sizeof f);
	while(_--){
		ll L,R; scanf("%lld%lld",&L,&R);
		ll ans = solve(R,R)-solve(L-1,R)-solve(R,L-1)+solve(L-1,L-1);
		printf("%lld\n",ans);
	}
	return 0;
}