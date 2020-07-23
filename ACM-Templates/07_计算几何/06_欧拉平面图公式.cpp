// ---
// 题意：\\
// 给出 $n$ 条线段，求平面中一共有多少个闭合区域。保证不存在三条线段相交与一点，并且线段都是规范相交，即不存在相交点在线段端点。$(1\leq n\leq 1000)$\\
// \\
// 思路：\\
// 此题是一个公式题，因此直接上公式。\\
// \\
// 基本术语：\\
// 平面图：不存在边交叉的二维平面上的图 \\
// 连通平面图：平面图上的图形相互连通，即可以仅依靠图中的边从任意一个节点到达另外一个节点\\
// \\
// 欧拉平面图公式：\\
// 1. 基本公式（针对连通平面图）\\
// · $V-E+F=2$，其中 $V$ 为点数，$E$ 为边数，$F$ 为面数（即闭合区域）\\
// · 此处的 $F$ 包括最外层的面 \\
// · 此公式可用数学归纳法证明，证明过程简单易懂，读者可自行查阅 \\
// 2. 广义公式（针对平面图）\\
// · $V-E+F=1+C$，其中 $C$ 为平面图中的连通块个数\\
// · 此公式可根据基本公式推出，推导过程较为简单，读者可自行思考\\
// \\
// 本题做法：\\
// 由于本题仅给出了线段，并不满足平面图的限制，因此我们可以将线段的交点当作一个新的点，将原来的图形转化成平面图。\\
// 因此在转化后的图中，点数变为 $(2*n+inter)$，边数变为 $(n+2*inter)$，其中 $inter$ 为直线交点个数，$comp$ 为连通块个数，即最终答案如下：（本题忽略最外层的点）\\
// $$ \\
// F=C+E-V=comp+(n+2*inter)-(2*n+inter)=comp+inter-n \\
// $$ \\
// ---
#include <bits/stdc++.h>
#define mem(a,b) memset(a,b,sizeof a);
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
typedef long long ll;
const int N = 1e4+100;
using namespace std;

int n, inter, vis[N], comp;
vector<int> G[N];

inline int sgn(ll x) { return x >= 0 ? x > 0 : -1;}

struct Point{
	ll x,y;
	Point() {}
	Point(ll _x, ll _y) : x(_x), y(_y) {}
	Point operator-(Point p) { return {x-p.x, y-p.y}; }
	ll det(Point p) { return x*p.y-y*p.x; } //叉积
	ll dot(Point p) { return x*p.x+y*p.y; }	//点积
}P1[N],P2[N];

struct Line{
	Point s,e;
	Line() {}
	Line(Point _s, Point _e){
		s = _s;
		e = _e;
	}
	int segcrosseg(Line v){
		int d1 = sgn((e-s).det(v.s-s));
		int d2 = sgn((e-s).det(v.e-s));
		int d3 = sgn((v.e-v.s).det(s-v.s));
		int d4 = sgn((v.e-v.s).det(e-v.s));
		if(((d1^d2) == -2) && (d3^d4) == -2) return 2;
		return (d1==0 && sgn((v.s-s).dot(v.s-e)) <= 0) ||
			   (d2==0 && sgn((v.e-s).dot(v.e-e)) <= 0) ||
			   (d3==0 && sgn((s-v.s).dot(s-v.e)) <= 0) ||
			   (d4==0 && sgn((e-v.s).dot(e-v.e)) <= 0);
	}
};

void dfs(int x){
	if(vis[x]) return;
	vis[x] = 1;
	for(auto y:G[x]) dfs(y);
}

int main()
{
	scanf("%d",&n);
	rep(i,1,n) scanf("%lld%lld%lld%lld",&P1[i].x,&P1[i].y,&P2[i].x,&P2[i].y);
	rep(i,1,n){
		Line l1 = {P1[i], P2[i]};
		rep(j,i+1,n){
			Line l2 = {P1[j], P2[j]};
			if(l1.segcrosseg(l2) != 0){
				G[i].push_back(j);
				G[j].push_back(i);
				inter++;
			}
		}
	}
	rep(i,1,n)
		if(!vis[i]){
			comp++;
			dfs(i);
		}
	printf("%d\n", inter+comp-n);
	return 0;
}