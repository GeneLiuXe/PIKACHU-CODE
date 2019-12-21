// ---
// 题意：\\
// 给定一个 $n$ 个点的凸包，再给出 $m$ 个光照点，每个光照点的照射范围为 $360$ 度，问最少选取几个光照点可以照亮整个凸包，要求输出方案，保证不会出现一个光照点位于凸包的延长线上，共 $200$ 组数据。$(1\leq n,m\leq 1000)$\\
// \\
// 思路：\\
// 其实这题思路比较明显，就是先求出每个光照点所能照射到的一段连续范围，然后问题就变成了给定一个长度为 $n$ 的环形区域，以及 $m$ 段区间，要求选取最少的区间覆盖整个区域。\\
// \\
// 首先求每个光照点对应的一段连续区域，比赛时的思路是极角排序，然后选择最两边的边。这样的复杂度是 $O(n^2logn)$，再加上 $200$ 组数据，成功 $TLE$...其实仔细思考一下可以发现并不需要进行极角排序，利用叉积即可解决该问题。因为照射范围的两个端点一定是光照点与凸包点连线形成的直线中最两边的两个点。\\
// \\
// 因此其它所有端点与光照点连线形成的直线都是顺时针或逆时针才能到达两个端点，即照射范围的两个端点是顺时针旋转的两个边界点，因此直接用叉积判断即可。\\
// \\
// 求出每个光照点范围之后，就只需要处理区间覆盖问题了。环形区域，我们只需要枚举起始点，$O(n^2)$ 求解即可。我们记录 $add[i]$ 数组表示所有覆盖第 $i$ 条的光照点中所能覆盖的最远距离，$id[i]$ 记录光照点编号，然后直接贪心即可解决该问题。\\
// ---
#include <bits/stdc++.h>
#define mem(a,b) memset(a,b,sizeof a);
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
typedef long long ll;
typedef double db;
#define pi acos(-1.0)
const int N = 1e3+100;
const db EPS = 1e-9;
using namespace std;

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}
inline int sign(db a) {return a < -EPS ? -1 : a > EPS;}
inline int cmp(db a,db b) {return sign(a-b);}

struct Point{
	ll x,y;
	Point operator-(Point p) {return {x-p.x,y-p.y};}
	ll dot(Point p) {return x*p.x+y*p.y;}
	ll det(Point p) {return x*p.y-y*p.x;} //叉积
}P[N],H[N];

int n,m,L[N],R[N],add[N],id[N],ans;
vector<int> base;

void init(){
	scanf("%d%d",&n,&m);
	rep(i,0,n-1) scanf("%lld%lld",&P[i].x,&P[i].y);
	rep(i,0,m-1) scanf("%lld%lld",&H[i].x,&H[i].y);
	rep(i,0,n-1) add[i] = id[i] = 0;
	rep(i,0,m-1){
		L[i] = 0, R[i] = 0;
		rep(j,1,n-1){
			//利用叉积求切线
			if((P[L[i]]-H[i]).det(P[j]-H[i]) > 0) L[i] = j;
			if((P[R[i]]-H[i]).det(P[j]-H[i]) < 0) R[i] = j;
		}
		int len = (R[i]-L[i]+n)%n;
		rep(j,0,len-1){
			int now = (L[i]+j)%n;
			int tlen = (R[i]-now+n)%n;
			if(tlen > add[now]) add[now] = tlen, id[now] = i;
		}
	}
}

void solve(){
	ans = 1e5; base.clear();
	rep(i,0,n-1){
		int pos = i, num = 0, left = n;
		vector<int> hp; hp.clear();
		while(left > 0){
			if(add[pos] == 0) {num = 1e5; break;}
			left -= add[pos];
			hp.push_back(id[pos]);
			pos = (pos+add[pos])%n;
			num++;
		}
		if(num < ans){
			ans = num;
			base = hp;
		}
	}
	if(ans == 1e5) printf("-1\n");
	else{
		printf("%d\n",ans);
		rep(i,0,ans-1) printf("%d%c",base[i]+1," \n"[i==ans-1]);
	}
}

int main()
{
	int _; scanf("%d",&_);
	while(_--){
		init();
		solve();
	}
	return 0;
}