// ---
// 题意：\\
// 给出 $n$ 个点，先从中任意选取三个点，使得这三个点为一个锐角三角形，问一共有多少个锐角三角形。$(3\leq n\leq 2000)$\\
// \\
// 思路：\\
// 正难则反，很明显正向思考这题难度非常高，因此考虑逆向思考，即能够构造出的直角和钝角三角形个数。\\
// \\
// 我们枚举一个点，然后按这个点对其它所有点进行极角排序，然后进行双指针枚举，枚举一个刚好变成直角的点，再枚举一个刚好不是钝角的点，然后直接计算答案即可，具体内容可以看代码。\\
// ---
#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 2000+100;
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;

inline int sign(db a) {return a < -EPS ? -1 : a > EPS;}

struct Point{
	ll x,y;
	Point() {}
	Point(ll _x, ll _y) : x(_x), y(_y) {}
	Point operator+(Point p) {return {x+p.x, y+p.y};}
	Point operator-(Point p) {return {x-p.x, y-p.y};}
	ll dot(Point p) {return x*p.x+y*p.y;} //点积
	ll det(Point p) {return x*p.y-y*p.x;} //叉积
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0);}
}P[N],Q[2*N];

db rad(Point p1, Point p2){
	return atan2l(p1.det(p2), p1.dot(p2));
}

bool cmp1(Point a, Point b){ //极角排序
	if(a.quad() != b.quad()) return a.quad() < b.quad();
	else return sign(a.det(b)) > 0;
}

int n;

int main()
{
	int _; scanf("%d",&_);
	while(_--){
		ll ans = 0;
		scanf("%d",&n);
		rep(i,1,n){
			scanf("%lld%lld",&P[i].x,&P[i].y);
		}
		int l = 0, r = 0;
		rep(i,1,n){
			int tot = 0;
			rep(j,1,n){
				if(j == i) continue;
				Q[++tot].x = P[j].x-P[i].x, Q[tot].y = P[j].y-P[i].y;
			}
			sort(Q+1,Q+1+tot,cmp1);
			// rep(j,tot+1,2*tot) Q[j].x = Q[j-tot].x, Q[j].y = Q[j-tot].y;
			int l = 1, r = 1;
			rep(j,1,tot){
				while(l <= tot && Q[j].det(Q[l]) >= 0 && Q[j].dot(Q[l]) > 0) l++;
				while(r <= tot && (Q[j].det(Q[r]) >= 0 || Q[j].dot(Q[r]) <= 0)) r++;
				ans += r-l;
			}
		}
		ans = ((n*(n-1ll)*(n-2ll))/6ll)-ans;
		printf("%lld\n",ans);
	}
	return 0;
}