// ---
// 题意：$N$ 个人在玩剪刀石头布的游戏，其中有一个人是裁判，其余人随机被分到了三个阵营，即剪刀、石头、布。现在有 $M$ 轮游戏，$x <\ ,>\ ,= y$ 表示 $x$ 与 $y$ 之间的关系，其中裁判可以自由变换阵营。问是否可以根据这 $M$ 轮游戏，判断出谁是裁判，输出裁判是谁以及最早在哪一轮可以找到裁判。如果无法判断，则输出 $Can\ not\ determine$，如果游戏的情况不符合题意，则输出 $Impossible$ 。$(N\leq 500,M\leq 2000)$\\
// \\
// 思路：一开始做这题的时候，的确有些懵，只能想到如何发现有人变换了阵营，但是不知道如何找到这个人，并且确定这种情况是唯一的。\\
// \\
// 所以我们可以发现直接考虑整个问题会非常困难，再加上此题数据范围很小，直接做的话复杂度肯定很少，太对不起这个数据范围了。因此我们考虑 $N^2$ 做法，即枚举每个人为裁判。枚举 $x$ 为裁判时，如果 $x$ 恰好为裁判，则并查集合并时不会发生矛盾。我们统计有多少个人为裁判时，并查集合并会发生矛盾，记人数为 $cnt$，并且统计每个人为裁判时，发生矛盾的轮数 $ri$ 。\\
// \\
// 现在我们来考虑输出答案的所有情况。\\
// $1.$ 如果 $cnt = n-1$，则可以唯一确定裁判，并且最早发现该裁判的轮数为 $max(ri)$。\\
// $2.$ 如果 $cnt = n$，则输出 $impossible$。\\
// $3.$ 其余情况则为 $Can\ not\ determine$。\\
// \\
// 至于带权并查集的合并类似于食物链问题，是个模 $3$ 剩余系中的加减问题。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
typedef long long ll;
typedef double db;
const db EPS = 1e-9;
using namespace std;
const int N = 1000;
const int M  = 2000+100;
int n,m,fa[N],d[N],vis[N],error[N],cnt;

int find(int x){
	if(x == fa[x]) return x;
	int root = find(fa[x]);
	d[x] = (d[x]+d[fa[x]]+3)%3;
	return fa[x] = root;
}

struct Query{
	int x,y,cc;
}q[M];

int main()
{
	while(~scanf("%d%d",&n,&m))
	{
		memset(vis,0,sizeof vis);
		memset(error,0,sizeof error);
		cnt = 0;
		rep(i,1,m){
			int x,y; char cc;
			scanf("%d%c%d",&x,&cc,&y);
			q[i].x = x, q[i].y = y;
			if(cc == '=') q[i].cc = 0;
			else if(cc == '<') q[i].cc = 1;
			else q[i].cc = 2;
		}
		rep(i,0,n-1){
			rep(k,0,n) fa[k] = k, d[k] = 0;
			rep(j,1,m){
				int x = q[j].x, y = q[j].y, cc = q[j].cc;
				if(x == i || y == i) continue;
				int fx = find(x), fy = find(y);
				if(fx != fy){
					fa[fx] = fy;
					d[fx] = (d[y]-d[x]+cc+3)%3;
				}
				else{
					int ttp = (d[x]-d[y]+3)%3;
					if(ttp != cc){
						cnt++;		// 产生矛盾个数
						error[i] = j;
						vis[i] = 1;
						break;
					} 
				}
			}
		}
		// impossible
		// not determine
		// determine
		if(cnt == (n-1)) { //determine
			int ans = 0, hm;
			rep(i,0,n-1){
				ans = max(ans,error[i]);
				if(!vis[i]) hm = i;
			} 
			printf("Player %d can be determined to be the judge after %d lines\n",hm,ans);
		}
		else if(cnt == n){ // impossible
			printf("Impossible\n");
		}	
		else{  // not determine
			printf("Can not determine\n");
		}
	}
	return 0;
}
