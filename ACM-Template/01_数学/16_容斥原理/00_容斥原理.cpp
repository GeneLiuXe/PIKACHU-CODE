// ---
// 题意：给定一个长度为 $n$ 的数组，$a[1],a[2],...,a[n]$。维护一个集合，一开始集合为空。一共有 $q$ 次操作，每次操作给定一个下标 $pos$，如果 $a[pos]$ 已经在集合中，则将 $a[pos]$ 从集合中删除，否则将 $a[pos]$ 加入集合。注意，集合允许有重复的数字。$(1\leq n,q\leq 2*10^5,1\leq a[i]\leq 5*10^5,1\leq pos\leq n)$ \\
// \\
// 问每次操作完之后，集合中互质的数字有多少对。\\
// \\
// 思路：此题算是一道比较经典的容斥题，也可以用莫比乌斯函数来求解，但二者本质是一样的。\\
// \\
// 首先比较容易想到的是，往集合中加一个数 $a[pos]$，则答案加上集合中与 $a[pos]$ 互质数的个数；从集合中删除一个数 $a[pos]$，则答案减去集合中与 $a[pos]$ 互质数的个数，这种做法的复杂度是 $O(nq)$。\\
// \\
// 想要进一步优化复杂度，则需要降低求集合中与 $a[pos]$ 互质数个数的复杂度。我们将求互质转为求不互质，即：\\
// $$ 与 \ a[pos]\ 互质数的个数 = 集合大小-与 \ a[pos]\ 不互质数的个数 $$ \\
// 又因为如果 $x$ 与 $y$ 不互质，则必存在至少一个质数 $c$，满足 $x \% c==0$ 且 $y\%c==0$，因此我们假设 $a[pos]$ 的质因子有 $p_1*p_2$，$C[p_1]$ 表示集合中有多少个数有 $p_1$ 这个质因子，则可以根据容斥原理得到：\\
// $$ 与\ a[pos]\ 不互质的个数 = C[p_1]+C[p_2]-C[p_1*p_2] $$ \\
// 所有我们可以先求出每个数的质因子集合，然后用 $dfs$ 枚举质因子子集，每当得到一个子集，则将子集中的数字全部乘起来，同时更新 $C$ 数字与容斥答案，具体细节见代码。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
const int N = 5e5+1000;
using namespace std;

int n,q,a[N],vis[N],flag[N],C[N];
ll ans, tmp, cot;

vector<int> prime[N];

void init() {
	scanf("%d%d",&n,&q);
	rep(i,1,n) scanf("%d",&a[i]);
	for(int i = 2; i <= 5e5; i++){
		if(vis[i]) continue;
		for(int j = i; j <= 5e5; j += i) {
			vis[j] = 1;
			prime[j].push_back(i);
		}
	}
}

void dfs(int now, int num, int cnt, int mul, int op) {
	if(num == prime[now].size()) return;
	if(op == 1) {
		// 加入 now
		tmp += cnt*C[mul*prime[now][num]];
		C[mul*prime[now][num]]++;
	}
	else {
		// 减去 now
		C[mul*prime[now][num]]--;
		tmp += cnt*C[mul*prime[now][num]];
	}
	dfs(now, num + 1, cnt, mul, op);
	dfs(now, num + 1, cnt*(-1), mul*prime[now][num], op);
}

int main()
{
	init();
	while(q--) {
		int pos; scanf("%d",&pos);
		flag[pos] ^= 1;
		tmp = 0;
		if(flag[pos]) {
			dfs(a[pos], 0, 1, 1, 1);
			ans += cot - tmp;
			cot++;
		}
		else {
			dfs(a[pos], 0, 1, 1, -1), cot--;
			ans -= cot - tmp;
		}
		printf("%lld\n", ans);
	}
	return 0;
}