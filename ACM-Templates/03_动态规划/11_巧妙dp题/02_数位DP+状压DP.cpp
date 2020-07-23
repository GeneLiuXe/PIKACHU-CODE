// ---
// 题意:\\
// 给出一个 $n$，现可以从 $1$ ~ $n$ 中任意选取若干个数字，要求所有数字不存在重复的数位。例如 {$1,2,3$} 和 {$2,11$} 是合法的，但是 {$1,2,10$} 和 {$2,5,12$} 是不合法的。\\
// \\
// 现给出一个 $n$，询问存在多少个集合满足上述要求，结果模 $1e9+7$。$(1\leq n\leq 10^9)$\\
// \\
// 思路:\\
// 首先，不难想到此题需要将 $0$ ～ $9$ 的数位状压起来，然后再进行数位 $dp$ 求解，最后再进行一个类似于容斥或背包的操作求出答案。\\
// \\
// 我们定义 $num[i]$ 表示 $1$ ~ $n$ 中有多少个数字，数字中包含的$0$ ～ $9$ 的状态恰好为 $i$。再定义 $ans[i]$ 表示 $1$ ~ $n$ 中有多少个不同的满足条件的集合，集合中数位的状态恰好为 $i$。\\
// \\
// 因此我们先求 $num[i]$，定义数位 $dp$ 的状态为 $dp[S][pos]$ 表示长度为 $pos$ 的数位，数位状态为 $S$ 的数的个数。\\
// \\
// 然后数位 $dp$ 经典操作 $dfs$ 求取 $num[i]$，中间用 $dp[S][pos]$ 进行记忆化搜索。枚举第 $pos$ 位的数位时，答案既可以由 $dp[S][pos-1]$ 更新而来，也可以由 $dp[S\ xor\ (1<<i)][pos-1]$ 更新而来。因为 $pos$ 位已经提供了 $i$ 这个数位，因此之后的数位可以提供也可以不提供，因此有两种情况。\\
// \\
// 所以我们可以只用这个二维的状态即可求解出 $num[i]$。比赛时数位 $dp$ 部分写了一个比较复杂的三维状态，最后卡时通过。\\
// \\
// 然后我们再来考虑如何求取 $ans[i]$。两种考虑方法，一种是利用的背包的想法，一种是利用组合的想法。\\
// \\
// 先讲背包的想法。我们已经求出了 $num[i]$ 表示 $1$ ~ $n$ 中数位状态为 $i$ 的数的个数。然后可以将 $num[i]$ 中的每一个 $i$ 看成一个物品，然后 $ans[i][j]$ 表示利用前 $i$ 个物品组成的状态为 $j$ 的方案总数。\\
// ---
ans[0][0] = 1;
for(nt i = 0; i < 1024; i++)
	for(int j = 0; j < 1024; j++){
		ans[i+1][j] += ans[i][j];
		if((j&i) == 0){
			ans[i+1][j|i] += ans[i][j] * num[i];
		}
	}
// ---
// 再讲组合容斥的想法。定义 $ans[i][j]$ 表示有多少个集合，组成了 $i$ 这个状态，一共含有 $j$ 个不同的数位状态。比如数位 $12$ 这个状态，既可以由 $1$ 和 $2$ 两个状态组成，也可以直接由 $12$ 这一个状态组成。然后我们就可以直接枚举子集进行计算。这里需要注意，枚举子集时，每次由一个单体和一个集合一起算贡献，而不是每次用两个集合算贡献，这样可以尽量避免重复。利用单体算贡献，每个单体被重复计算 $k$ 次，因此最后答案除 $k$。\\
// ---
rep(S, 1, endS){
	 rep(k, 2, 9){
    	 ll cnt = 0;
         for(int sub = S & (S - 1); sub; sub = (sub - 1) & S){
        	 int x = sub, y = S - sub;
        	 cnt = (cnt + DP[x] * ans[y][k - 1]) % mod;
         }
         cnt = cnt * inv[k] % mod;
         ans[S][k] = (ans[S][k] + cnt) % mod;
    }
 }
// ---
// 完整代码如下。\\
// ---
#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define mem(a,b) memset(a,b,sizeof a);
#define rep(i, a, b) for(int i = a; i <= b; i++)
#define LOG1(x1, x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1, x2, y1, y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1, x2, y1, y2, z1, z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const db EPS = 1e-9;
const ll mod = 1e9 + 7;
using namespace std;

ll dp[1 << 11][13], endS, ans[1 << 11][10], DP[1 << 11], inv[100];
int len, a[13];

void init() {
    mem(dp,-1);
    mem(ans,0);
    mem(DP,0);
}

ll dfs(int pos, int state, bool flag, int jud) { //jud表示有无数字
    if (pos == 0) {
        if(state == 0 && jud == 1) return 1;
        else return 0;
    }
    if (!flag && dp[state][pos] != -1 && jud == 1) return dp[state][pos];
    ll base = 0;
    int end = flag ? a[pos] : 9;
    rep(i, 0, end){
        if(jud == 0 && i == 0){ //无数字
            base = (base + dfs(pos - 1, state, flag && i == end, 0)) % mod;
        }
        else{
            if (((1 << i) & state) == 0) continue;
            base = (base + dfs(pos - 1, state, flag && i == end, 1)) % mod;
            base = (base + dfs(pos - 1, state^(1<<i), flag && i == end, 1)) % mod;
        }
    }
    if (!flag && jud == 1) dp[state][pos] = base;
    return base;
}

void solve(ll n) {
    len = 0;
    memset(a, 0, sizeof a);
    while (n) {
        a[++len] = n % (10ll);
        n /= 10ll;
    }
    rep(S, 1, endS) DP[S] = dfs(len,S,1,0);
}

ll pow_mod(ll a, ll b, ll m) {
    ll ans = 1;
    while (b) {
        if (b & 1)ans = ans * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return ans;
}

int main() {
    int _;
    scanf("%d", &_);
    for (int i = 1; i <= 100; ++i) {
        inv[i] = pow_mod(i, mod - 2, mod);
    }
    endS = (1 << 11) - 1;
    rep(Ca, 1, _) {
        init();
        ll n;
        scanf("%lld", &n);
        solve(n);
        rep(S, 1, endS) ans[S][1] = DP[S];
        rep(S, 1, endS){
            rep(k, 2, 9){
                ll cnt = 0;
                for(int sub = S & (S - 1); sub; sub = (sub - 1) & S){
                    int x = sub, y = S - sub;
                    cnt = (cnt + DP[x] * ans[y][k - 1]) % mod;
                }
                cnt = cnt * inv[k] % mod;
                ans[S][k] = (ans[S][k] + cnt) % mod;
            }
        }
        ll sum = 0;
        rep(S, 1, endS) rep(i, 1, 9) sum = (sum + ans[S][i]) % mod;
        printf("Case %d: %lld\n", Ca, sum);
    }
    return 0;
}