// ---
// 题意：\\
// 主人公小智一共会捕捉 $n$ 只宝可梦，宝可梦有两个属性，攻击值 $A$，防御值 $B$。每当捕捉到一只新的宝可梦 $T$，小智有两种方法判断这只宝可梦是否是无用的。\\
// 	1. 存在一只宝可梦 $X$，使得 $X.A > T.A$ 并且 $X.B > T.B$\\
// 	2. 存在两只宝可梦 $X,Y$，使得 $c*X.A+(1-c)*Y.A>T.A$ 并且 $c*X.B+(1-c)*Y.B>T.B$，$0\leq c\leq 1$\\
// 每当捕捉到一只新宝可梦，需要输出当前共有几只无用宝可梦。$(1\leq n\leq 10^5,0\leq A_i,B_i\leq 10^9)$\\
// \\
// 思路：\\
// 参数 $c$ 的约束其实为线段的参数方程，可以转换成动态维护上凸壳进行求解。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
using namespace std;

struct Node{
	ll x,y;
	Node operator - (Node p) const {return {x-p.x,y-p.y};}
	ll operator ^ (Node p) const {return x*p.y-y*p.x;}
	bool operator < (Node p) const {return (x == p.x ? y > p.y : x < p.x);}
};

struct Hull : public multiset<Node>{
	bool inside(iterator p){
		auto t2 = next(p);
		if(t2 == end()) return 0;
		if(p == begin()) return t2->x > p->x && t2->y > p->y;
		auto t1 = prev(p);
		if(((*t1-*p)^(*t2-*p)) < 0) return 1;
		else return 0;
	}
	void ins(Node p){
		auto t = insert(p);
		if(inside(t)) { erase(t); return; }
		while(t != begin() && inside(prev(t))) erase(prev(t));
		while(next(t) != end() && inside(next(t))) erase(next(t));
	}
};

int main()
{
	int n; scanf("%d",&n);
	Hull hull;
	rep(i,1,n){
		ll x,y; scanf("%lld%lld",&x,&y);
		hull.ins({x,y});
		printf("%d\n",i-(int)hull.size());
	}
	return 0;
}