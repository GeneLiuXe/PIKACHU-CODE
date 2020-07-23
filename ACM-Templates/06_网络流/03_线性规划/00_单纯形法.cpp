// ---
// 题意：\\
// 从 N 个团队中挑人，其中第 i 个团队中的人能力值为 c[i]。现有 m 条约束，每条约束为 [l, r, c]，表示从区间 [l, r] 的团队中选的总人数要小于等于 c。求在 m 条约束下，选出的人的能力值之和的最大值。（$1\leq N\leq 200,1\leq M\leq 4000,0\leq c[i]\leq 10^6$）\\
// \\
// 思路：\\
// 这是一个典型的线形规划问题，因此直接用单纯形算法求解即可。\\
// ---
#include <bits/stdc++.h>
using namespace std;

namespace simplex{
	using T = long double;
	const int N = 210, M = 4010;
	const T eps = 1e-7;
	int n, m;
	int Left[M], Down[M];
	//Ax<=b, max c^T x
    //result : v,xi=sol[i],1 based
	T a[M][N], b[M], c[N], v, sol[N];
	bool eq(T a, T b) {return fabs(a-b) < eps;}
	bool ls(T a, T b) {return a<b&&!eq(a,b);}
	void init(int p,int q)
    {
        n=p, m=q, v=0;
        for(int i = 1; i <= m; i++){
            for(int j = 1; j <= n; j++)
                a[i][j]=0;
        }
        for (int i=1;i<=m;i++) b[i]=0;
        for (int i=1;i<=n;i++) c[i]=sol[i]=0;
    }
	void input(){
		scanf("%d%d",&n,&m);
		for(int i = 1; i <= n; i++){
			int tp; scanf("%d",&tp);
			c[i] = (long double)tp;
		}
		for(int i = 1; i <= m; i++){
			int l,r,c; scanf("%d%d%d",&l,&r,&c);
			for(int j = l; j <= r; j++) a[i][j] = 1;
			b[i] = (long double)c;
		}
	}
	void pivot(int x, int y){
		swap(Left[x], Down[y]);
		T k = a[x][y]; a[x][y] = 1;
		vector<int> nz;
		for(int i = 1; i <= n; i++){
			a[x][i] /= k;
			if(!eq(a[x][i],0)) nz.push_back(i);
		}
		b[x] /= k;
		for(int i = 1; i <= m; i++){
			if(i==x||eq(a[i][y],0)) continue;
			k = a[i][y]; a[i][y] = 0;
			b[i] -= k*b[x];
			for(int j:nz) a[i][j] -= k*a[x][j];
		}
		if(eq(c[y],0)) return;
		k = c[y]; c[y] = 0;
		v += k*b[x];
		for(int i:nz) c[i] -= k*a[x][i];
	}
 	//0:find solution,1:no feasible solution,2:unbounded
	int solve(){
		for(int i = 1; i <= n; i++) Down[i] = i;
		for(int i = 1; i <= m; i++) Left[i] = n+i;
		while(1){
			int x = 0, y = 0;
			for(int i = 1; i <= m; i++)
				if(ls(b[i],0)&&(x==0||b[i]<b[x])) x = i;
			if(x == 0) break;
			for(int i = 1; i <= n; i++)
				if(ls(a[x][i],0)&&(y==0||a[x][i]<a[x][y])) y = i;
			if(y == 0) return 1;
			pivot(x,y);
		}
		while(1){
			int x = 0, y = 0;
			for(int i = 1; i <= n; i++)
				if(ls(0,c[i])&&(!y||c[i]>c[y])) y = i;
			if(y == 0) break;
			for(int i = 1; i <= m; i++)
				if(ls(0,a[i][y]) && (!x || b[i]/a[i][y]<b[x]/a[x][y])) x = i;
			if(x == 0) return 2;
			pivot(x,y);
		}
		for(int i = 1; i <= m; i++) if(Left[i] <= n) sol[Left[i]] = b[i];
		return 0;
	}
}

int main()
{
	simplex::input();
	simplex::solve();
	printf("%lld\n", (long long)(simplex::v+0.5));
	return 0;
}