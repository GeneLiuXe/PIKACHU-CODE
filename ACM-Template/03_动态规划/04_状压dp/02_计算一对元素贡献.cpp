// ---
// 题意：\\
// 有一个 $n$ 个数字的序列，数字范围在 $1$ ～ $20$ 之间，现在要对这些数字进行重新排列，使得相同数字出现在一个连续区间，如 $113322$、$22222233444$ 都是符合题意的序列。重新排列这个序列的操作只有交换两个相邻的数字，现问最少需要交换多少次可以使这个序列变成符合题意的序列。$(2\leq n\leq 4*10^5)$\\
// \\
// 思路：\\
// 由于所有数字都在 $1$ ～ $20$ 之间，因此应该对于有多少个数字进行考虑，而不是对 $n$ 进行考虑。\\
// \\
// 如果直接考虑数字的移动会将问题复杂化，因为考虑的内容会非常多。因此我们直接构建一个最终序列，然后求原序列转化到这个最终序列的贡献。\\
// \\
// 先说如何求，再说如何构建。因为如果不会求的话，也不知道构建的入手点在哪。对于求贡献，此处主要对于一对元素进行考虑。\\
// \\
// 对一对元素进行考虑，即仅考虑 $1$、$2$ 时，序列为 $1$ $1$ $2$ $1$ $1$ $2$ $1$ $2$ $2$，我们令 $val[i][j]$ 表示元素 $i$ 在元素 $j$ 之前对答案的影响，因此只需要考虑对每个 $1$ 来说前面有多少个 $2$，即 $val[1][2]=1+1+2=4$，$val[2][1]=2+4+5+5=16$。\\
// \\
// 知道如何求贡献之后，我们来考虑如何解决这个问题。令 $dp[i][S]$ 表示当前放置第 $i$ 个元素，前面放的元素状压的状态为 $S$，因此我们枚举第 $i$ 个元素为 $j$，$dp[i][S]=min(dp[i-1][S \wedge (1$<<$j)])$。计算贡献时可以使用一对元素的考虑方法，需要进行预处理。\\
// 总结: \\
// 这其实是一道计算贡献的状压 $dp$ 问题，主要难点应该在于从考虑过程转变为考虑结果，然后再思考如何根据初始序列和结果序列直接得到答案。再之后就是直接通过状压再加上一定的预处理通过此题。\\
// \\
// 考虑结果序列的贡献中，一对元素的考虑方式比较巧妙，类似于逆序对的感觉，主要抓住的特点在于一个元素在序列中的位置本质上是由它前面有多少个元素决定的，因此想到了一对元素的思考方式来解决问题。\\
// ---
#include <bits/stdc++.h>
#define mem(a,b) memset(a,b,sizeof a);
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
typedef long long ll;
typedef double db;
const int N = 4e5+100;
const ll inf = 1e15;
const db EPS = 1e-9;
using namespace std;

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

int n,a[N],b[N],tot,num[25],cnt[1<<20];
ll dp[22][1<<20],val[25][25];

int main()
{
	rep(S,0,(1<<20)-1){
		int hp = S, c1 = 0;
		while(hp){
			if(hp&1) c1++;
			hp /= 2;
		}
		cnt[S] = c1;
	}
	scanf("%d",&n);
	rep(i,1,n){
		scanf("%d",&a[i]);
		b[++tot] = a[i];
	}
	sort(b+1,b+1+tot);
	tot = unique(b+1,b+1+tot)-b-1;
	rep(i,1,n){
		num[a[i]]++;
		int pos = lower_bound(b+1,b+1+tot,a[i])-b;
		rep(j,1,tot)
			if(b[j] != a[i]) val[pos][j] += num[b[j]];
	}
	rep(i,0,tot)
		rep(S,0,(1<<tot)-1) dp[i][S] = inf;
	dp[0][0] = 0;
	rep(i,1,tot){
		rep(j,1,tot){ //枚举放的东西
			rep(S,0,(1<<tot)-1){ //前面的状态
				if(S&(1<<(j-1)) && cnt[S] == i){
					ll base = dp[i-1][S^(1<<(j-1))];
					rep(k,1,tot)
						if(S&(1<<(k-1))){
							base += val[k][j];
						}
					dp[i][S] = min(dp[i][S],base);
				}
			}
		}
	}
	printf("%lld\n",dp[tot][(1<<tot)-1]);
	return 0;
}