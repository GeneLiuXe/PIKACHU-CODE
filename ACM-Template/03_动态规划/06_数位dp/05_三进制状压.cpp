// ---
// 题意:\\ 
// 查找区间 $[l,r]$ 中，符合如下条件的数字的个数：\\
// 1. 所有出现过的偶数位，都出现了奇数次 \\
// 2. 所有出现过的奇数位，都出现了偶数次 \\
// $(1\leq l\leq r\leq 10^{19})$\\
// \\
// 思路:\\
// 这个问题主要的困难之处在于如何表示那些没有出现过的数字，如果用二进制状压的方式显然无法表示那些从未出现过的数字。\\
// \\
// 因此我们考虑使用三进制状压的方式，$0$ 表示没有出现过，$1$ 表示出现了奇数次，$2$ 表示出现了偶数次。\\
// \\
// 因此我们设置状态数组 $f[pos][S]$，表示还有 $pos$ 个位置没有填数，之前填了的数字继承下来的状态为 $S$，符合题干条件的数的个数。\\
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
const db EPS = 1e-9;
using namespace std;

ll f[21][60000],a[25],tp[25];

ll pow_mod(ll a,ll b){
	ll base = a, ans = 1;
	while(b){
		if(b&1) ans *= base;
		base *= base;
		b >>= 1;
	}
	return ans;
}

ll dfs(int pos,ll S,bool flag){ //S-各数位出现次数的状压
	//flag只是表示是否表示了该位的全部情况
	if(pos == 0){
		int jud = 1;
		memset(tp,0,sizeof tp); int tot = -1;
		while(S){
			tp[++tot] = S % 3;
			S /= 3;
		}
		rep(i,0,9){
			if(tp[i] == 0) continue;
			if(i%2){ //奇数
				if(tp[i] != 2) {jud = 0; break;}
			}
			else{ //偶数
				if(tp[i] != 1) {jud = 0; break;}
			}
		}
		if(jud) return 1;
		else return 0;
	}
	if(!flag && f[pos][S] != -1) return f[pos][S];
	int end = flag?a[pos]:9;
	ll ans = 0;
	rep(i,0,end){
		if(i == 0 && S == 0) ans += dfs(pos-1,S,flag && i == end); //不能把前面的前导0继承过来
		else{
			int tot = -1, p = 0; ll hp = S;
			while(hp){
				++tot;
				if(tot == i) {p = hp%3; break;}
				hp /= 3ll;
			}
			hp = S;
			if(p == 0 || p == 1) hp += pow_mod(3,i);
			else hp -= pow_mod(3,i);
			ans += dfs(pos-1,hp,flag && i == end);
		}
	}
	// LOG3("pos",pos,"S",S,"ans",ans);
	if(!flag) f[pos][S] = ans;
	return ans;
}

ll solve(ll n){
	if(n == 0) return 1;
	int pos = 0;
	memset(a,0,sizeof a);
	while(n){
		a[++pos] = n % 10;
		n /= 10;
	}
	return dfs(pos,0,1);
}

int main()
{
	ll base = pow_mod(3,10);
	// LOG1("base",base);
	rep(i,0,20)
		rep(j,0,base)
			f[i][j] = -1;
	int _; scanf("%d",&_);
	while(_--){
		ll L,R; scanf("%lld%lld",&L,&R); L--;
		printf("%lld\n",solve(R)-solve(L));
	}
	return 0;
}
/*
数位DP主要就是数位移动时，对答案贡献的求取
在数位移动时，要仔细考虑各个细节点，包括前导0等信息
*/