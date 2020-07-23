// ---
// 题意：\\
// 长度为 n 的数组，m 次查询，每次查询给出 (l, r, x)，询问区间 [l, r] 中与 x 互质的数的个数，强制在线。（$1\leq n,m,a_i,x\leq 10^5$）\\
// \\
// 思路：\\
// 推出莫比乌斯方程，去除 mu[i] = 0 的数进行剪枝。\\
// ---
#include <bits/stdc++.h>
const int N = 5e5+10;
using namespace std;

int vis[N], primes, prime[N], mu[N];
int a[N], n, m, maxx;
vector<int> fac[N],pos[N];

void Init()
{
	primes = 0;
	mu[1] = 1;
	for(int i = 2; i < N; i++){
		if(!vis[i]){
			prime[primes++] = i;
			mu[i] = -1;
		}
		for(int j = 0;j < primes && i*prime[j] < N; j++){
			vis[i*prime[j]] = 1;
			if(i%prime[j]) mu[i*prime[j]] = -mu[i];
			else{
				mu[i*prime[j]] = 0;
				break;
			}
		}
	}

	for(int i = 1; i < N; i++){
		for(int j = i; j < N; j += i){
			if(mu[i] != 0) fac[j].push_back(i);//有贡献的因子 
		}
	}
}

int F(int num,int posss)
{
	int l = 0,r = pos[num].size()-1,ret = -1;
	//二分最近位置
	while(l <= r){
		int mid = (l+r) / 2;
		if(pos[num][mid] <= posss){
			ret = mid;
			l = mid+1;
		}
		else{
			r = mid-1;
		}
	}
	return ret;
}

int query(int l, int r, int x){
	int ans = 0;
	for(int j = 0; j < fac[x].size(); j++) {
		int v = fac[x][j];
		int tmp1 = F(v,r);
		int tmp2 = F(v,l-1);
		ans += mu[v]*(tmp1-tmp2);
	}
	return ans;
}

int main()
{
	Init();
	int _; scanf("%d", &_);
	while(_--){
		maxx = 0;
		scanf("%d%d",&n,&m);
		for(int i = 1;i <= n;i ++){
			scanf("%d",&a[i]);
			maxx = max(maxx,a[i]);
			for(int j = 0; j < fac[a[i]].size(); j++){
				int v = fac[a[i]][j];//放倍数位置
				pos[v].push_back(i);
			}
		}
		int ans = 0, l, r, x;
		for(int i = 1; i <= m; i++){
			scanf("%d%d%d", &l, &r, &x);
			l ^= ans, r ^= ans, x ^= ans;
			ans = query(l, r, x);
			printf("%d\n", ans);
		}
		for(int i = 1; i <= maxx; i++) pos[i].clear();
	}
	return 0;
}