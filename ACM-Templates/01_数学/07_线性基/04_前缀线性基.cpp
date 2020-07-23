// ---
// 题意: 给定 $n$ 个数字，$q$ 个询问，每次询问对应一个 $l$、$r$，询问区间 $[l,r]$ 中所有子集异或和的最大值。$(1\leq n\leq 5*10^5,1\leq q\leq 5*10^5)$ \\
// \\
// 思路: 处理前缀线性基。$p[i][j]$ 表示前 $i$ 个数的第 $j$ 位的线性基，$pos[i][j]$ 表示前 $i$ 个数的第 $j$ 位的线性基由哪一位数字得到，保留最靠右的位置。\\
// 每次插入一个数字，如果可以更新 $pos[i][j]$ 则更新，并将 $p[i][j]$ 替换下来，用于后续的更新。查询 $[l,r]$ 最值只需将 $p[r]$ 取出，所有 $pos[r][j]\geq l$ 均为有效基。\\
// \\
// 总结: 前缀线性基，维护区间线性基。\\
// ---
#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 5*1e5+100;
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;

int n,q,p[N][27],pos[N][27],a[N];

int main()
{
	scanf("%d",&n);
	rep(i,1,n) scanf("%d",&a[i]);
	rep(i,1,n){
		rep(k,0,24) p[i][k] = p[i-1][k], pos[i][k] = pos[i-1][k];
		int ti = i;
		for(int k = 24; k >= 0; k--){
			if(a[i]&(1<<k)){
				if(!p[i][k]) {p[i][k] = a[i]; pos[i][k] = ti; break;}
				if(pos[i][k] < ti) swap(p[i][k],a[i]), swap(pos[i][k],ti);
				a[i] ^= p[i][k];
			}
		}
	}
	scanf("%d",&q);
	while(q--){
		int l,r; scanf("%d%d",&l,&r);
		int ans = 0;
		for(int k = 24; k >= 0; k--){
			if(pos[r][k] >= l && ans < (ans^p[r][k]))
				ans = ans^p[r][k]; 
		}
		printf("%d\n",ans);
	}
	return 0;
}