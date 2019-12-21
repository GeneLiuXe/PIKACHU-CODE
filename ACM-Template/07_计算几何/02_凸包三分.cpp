// ---
// 题意：\\
// 给出一个 $N$ 个点的凸包，$Q$ 次询问，每次给出一个 $idx$ 和 $k$，表示现有一根长度无限的垂直 $(P[idx],P[idx+1])$ 向量且距离 $P[idx]$ 为 $k$ 的垂直线。先问这根垂直线倒落时凸包上第一个被碰到的点，如果第一个被碰到的是直线，则输出两个点。$(1\leq n\leq 10^5,1\leq Q\leq 10^5)$\\
// \\
// 思路：\\
// 稍微画图模拟一下就可以发现，最终倒在哪个点上取决于凸包上的点与这个点连线，连线之后与 $(P[idx],P[idx+1])$ 之间的夹角大小，夹角最大的点即为第一个碰到的点。若有两个相同大的点，即碰到的是直线。\\
// \\
// 这个夹角是先增后减的，因此不能进行二分，而是三分，而且是整数三分。而整数三分的精髓就是先三分到大致区间，然后在确定的小区间内暴力求解。\\
// \\
// 但是直接判断角度不好处理，我们转化成向量的叉乘进行计算，判断大小是变大还是变小即可。\\
// \\
// 总结：\\
// 1. 计算几何中判断大小的问题，尽量转化成叉积或者点积进行计算。\\
// \\
// 2. 整数三分是先三分到一个小范围，然后在小范围内直接暴力。\\
// ---
#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 1e5+100;
const int M = 1e5+100;
const db EPS = 1e-9;
#define cross(p1,p2,p3) ((p2.x-p1.x)*(p3.y-p1.y)-(p3.x-p1.x)*(p2.y-p1.y))
#define crossOp(p1,p2,p3) sign((cross(p1,p2,p3)))
using namespace std;

inline int sign(db a) {return a < -EPS ? -1 : a > EPS; }
inline int cmp(db a,db b) {return sign(a-b);}

struct Point{
	db x,y;
	Point() {}
	Point(db _x, db _y) : x(_x), y(_y) {}
	Point operator-(Point p) {return {x-p.x,y-p.y};}
	db dot(Point p) {return x*p.x+y*p.y;}
	db det(Point p) { return x*p.y-y*p.x; } //叉积
	db abs() { return sqrt(abs2()); }
	db abs2() { return x*x+y*y; }
	db disTo(Point p) { return (*this-p).abs(); }
}P[N];

int T,Q;

int calc(Point p2,Point p3,Point p4){
	Point t1 = p3-p2, t2 = p4-p2;
	return sign(t2.det(t1));
}

int main()
{
	scanf("%d%d",&T,&Q);
	rep(i,0,T-1) scanf("%lf%lf",&P[i].x,&P[i].y);
	int n = T;
	rep(i,1,Q){
		int pos; db x; scanf("%d%lf",&pos,&x);
		db len = P[pos].disTo(P[(pos+1)%n]);
		db tx = ((db)(P[(pos+1)%n].x-P[pos].x)*(db)x/len)+P[pos].x;
		db ty = ((db)(P[(pos+1)%n].y-P[pos].y)*(db)x/len)+P[pos].y;
		int l = 1, r = T-1, lmid, rmid;
		//三分先确定一个小范围
		while(r-l >= 15){
			lmid = l+(r-l)/3;
			rmid = r-(r-l)/3;
			if(calc({tx,ty},P[(pos+lmid)%n],P[(pos+rmid)%n]) == 1){
				l = lmid;
			}
			else r = rmid;
		}
		int indx1 = 1, indx2 = 0;
		rep(k,l,r){
			int jud = calc({tx,ty},P[(pos+k)%n],P[(pos+indx1)%n]);
			if(jud == -1) indx1 = k, indx2 = 0;
			else if(jud == 0) indx2 = k;
		}
		if(indx2 == 0) printf("%d\n",(indx1+pos)%n);
		else printf("%d %d\n",(indx1+pos)%n,(indx2+pos)%n);
	}
	return 0;
}
