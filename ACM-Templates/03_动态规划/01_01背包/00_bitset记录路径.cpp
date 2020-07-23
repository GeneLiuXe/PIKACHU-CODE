// ---
// 题意：一个比赛有 n 个裁判，每个裁判有一个权值 ai，并且给出通过或不通过。有一个判断值 p，若所有给出通过的裁判的权值和大于等于 p，那么整体就算通过。给出第二组裁判，权值为 bi，判断值为 q，判断所有 $2^n$ 种判断下这两组给出的结果是否完全相同，如果不相同，给出一种不相同的情况。\\
// \\
// 思路：比赛的时候完全没思路，一个劲的贪心...赛后才知道这题其实是个dp，dp[i][j]表示前 i 个人，序列a中选的人的值到达 j 的所有情况中，序列b中对应人的权值之和的最大值。因此当dp[i][j] >= q，而 j < p时，即是错误的情况。然后再判断一遍b序列即可。此处还有一个地方，记错误方案时如果是用pre数组不断回溯的话，会T。正确做法是对于每个dp[j]记一个bitset的状态，检查出错误后，直接输出即可。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <bitset>
#include <algorithm>
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
typedef long long ll;
typedef double db;
const db EPS = 1e-9;
using namespace std;
const int N = 200;
const int M = 1e6+1000;
 
int n,a[N],b[N],dp[M],am,bm,tp[M],ans[N];
//dp[i]表示a组中，价值为i的所有组合中，b组最大价值的一组
bitset<110> base1[M],base2[M];
 
void solve()
{
	rep(i,1,n) ans[i] = 0;
	rep(i,1,n){
		for(int j = am-1; j >= a[i]; j--){
			if(j == a[i]){
				if(dp[j] < b[i]) dp[j] = b[i], base1[j].reset(),base1[j].set(i);
			}
			if(dp[j] < dp[j-a[i]]+b[i] && dp[j-a[i]]!=0){
				dp[j] = dp[j-a[i]]+b[i];
				base1[j] = base1[j-a[i]];
				base1[j].set(i);
			}
			if(dp[j] >= bm){
				printf("NO\n");
				rep(i,1,n) printf("%d",(int)base1[j][i]);
				printf("\n");
				return;
			} 
		}
	}
	rep(i,1,n){
		for(int j = bm-1; j >= 1; j--){
			if(j == b[i]){
				if(tp[j] < a[i]) tp[j] = a[i], base2[j].reset(),base2[j].set(i);
			} 
			if(j < b[i]) continue;
			if(tp[j] < tp[j-b[i]]+a[i] && tp[j-b[i]]!=0){
				tp[j] = tp[j-b[i]]+a[i];
				base2[j] = base2[j-b[i]];
				base2[j].set(i);
			}
			if(tp[j] >= am){
				printf("NO\n");
				rep(i,1,n) printf("%d",(int)base2[j][i]);
				return;
			} 
		}
	}
	printf("YES\n");
}
 
int main()
{
	scanf("%d",&n);
	scanf("%d",&am);
	rep(i,1,n) scanf("%d",&a[i]);
	scanf("%d",&bm);
	rep(i,1,n) scanf("%d",&b[i]);
	solve();
	return 0;
}