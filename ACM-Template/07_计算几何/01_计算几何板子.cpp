#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>
#include <cmath>
#include <algorithm>
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x) cout << "x: " << x << endl;
#define LOG2(x,y) cout << "x: " << x << ", y: " << y << endl;
#define pi acos(-1.0)
#define cross(p1,p2,p3) ((p2.x-p1.x)*(p3.y-p1.y)-(p3.x-p1.x)*(p2.y-p1.y)) //向量(p1,p2)与(p1,p3)叉乘
#define crossOp(p1,p2,p3) sign(cross(p1,p2,p3))	//判断正负，顺时针为负, 为0则代表三点共线
using namespace std;

//实数比较
typedef double db;
const db EPS = 1e-9;
inline int sign(db a) {return a < -EPS ? -1 : a > EPS; } //返回-1表示a < 0, 1表示a > 0, 0表示a = 0
inline int cmp(db a, db b) {return sign(a-b); } //返回-1表示a < b, 1表示a > b，0表示 a==b

//点类
struct Point {
	db x,y;
	Point() {}
	Point(db _x, db _y) : x(_x), y(_y) {}
	Point operator+(Point p) { return {x+p.x, y+p.y}; }
	Point operator-(Point p) { return {x-p.x, y-p.y}; }
	Point operator*(db d) { return {x*d, y*d}; }
	Point operator/(db d) { return {x/d, y/d}; }
	Point rotleft() { return Point(-y,x); } //逆时针旋转90度
	Point rotright() { return Point(y,-x); } //顺时针旋转90度
	db dot(Point p) { return x*p.x+y*p.y; }	//点积
	db det(Point p) { return x*p.y-y*p.x; } //叉积
	Point rot(db an) { return {x*cos(an)-y*sin(an),x*sin(an)+y*cos(an)}; }	//旋转
	db abs() { return sqrt(abs2()); }
	db abs2() { return x*x+y*y; }
	db disTo(Point p) { return (*this-p).abs(); }
	//此时在x负半轴上的点, 排序结果是最小的。如果去掉sign(x)>=0, 则排序结果是最大的
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); } //判断该点是否在x轴上方或x轴上

	bool operator<(Point p) const {
		int c = cmp(x, p.x);
		if (c) return c == -1;	//先判断x大小
		return cmp(y, p.y) == -1;	//再判断y大小
	}
	bool operator==(Point p) const {
		return cmp(x, p.x) == 0 && cmp(y, p.y) == 0;
	}
	bool operator!=(Point p) const{
		return (cmp(x, p.x) || cmp(y,p.y));
	}
};

db area(vector<Point> ps){	//凸包面积
	db ret = 0; rep(i,0,ps.size()-1) ret += ps[i].det(ps[(i+1)%ps.size()]);
	return ret/2;
}

db perimeter(vector<Point> ps){	//凸包周长
	db ret = 0; rep(i,0,ps.size()-1) ret += ps[i].disTo(ps[(i+1)%ps.size()]);
	return ret;
}

db dot(Point A, Point B, Point C){	//三点点积
	return (B-A).dot(C-A);
}

db rad(Point p1, Point p2){	//返回两个向量的夹角，范围为-2*pi~2*pi，顺时针转为负数，逆时针转为正数
	return atan2l(p1.det(p2),p1.dot(p2));
	//取绝对值就是旋转角度，否则会有正负区别，返回幅度制
}

/*
坐标系变换 —— 选中点集中的两点，两点构成的向量为x轴
	Point p1 = {AA[2].x-AA[1].x,AA[2].y-AA[1].y}; //选定两点作为向量
	rep(i,1,n){
		Point p2 = {AA[i].x-AA[1].x,AA[i].y-AA[1].y};
		db ang = rad(p1,p2); 		//返回两个向量角度
		db len = p2.abs();
		Point tmp = {len*cos(ang),len*sin(ang)};	//得到新坐标系下的点坐标
		mp[tmp] = 1;  //可以用map存变更坐标轴之后的点
	}
*/

//求凸包
vector<Point> convexHull(vector<Point> ps) {
	int n = ps.size(); if(n <= 1) return ps;
	sort(ps.begin(),ps.end());
	vector<Point> qs(n*2); int k = 0;
	for(int i = 0; i < n; qs[k++] = ps[i++])
		while(k > 1 && crossOp(qs[k-2],qs[k-1],ps[i]) <= 0) --k;	//把 <= 改成 <, 即可将凸包边上的点也包括在凸包中, 不稳定凸包问题 
	for(int i = n-2, t = k; i >= 0; qs[k++] = ps[i--])
		while(k > t && crossOp(qs[k-2],qs[k-1],ps[i]) <= 0) --k;
	qs.resize(k-1);
	return qs;
}

//最小矩形覆盖
db minRectangleCover(vector<Point> ps){
	//凸包点集顺序按逆时针
	int n = ps.size();	
	if(n < 3) return 0.0;
	ps.push_back(ps[0]);
	db ans = -1;
	int r = 1, p = 1, q;
	for(int i = 0; i < n; i++){
		//求出离边 ps[i]-ps[i+1] 最远的点 r
		while(sign(cross(ps[i],ps[i+1],ps[r+1])-cross(ps[i],ps[i+1],ps[r])) >= 0) //叉积最大即为到点r到ps[i+1]-ps[i]这条边的距离最大
			r = (r+1)%n;
		//卡出 ps[i]-ps[i+1] 方向上正向 n 最远的点 p
		while(sign(dot(ps[i],ps[i+1],ps[p+1])-dot(ps[i],ps[i+1],ps[p])) >= 0)	//正向最远即为点积最大
			p = (p+1)%n;
		if(i == 0) q = p;
		//卡出 ps[i]-ps[i+1]方向上负向最远的点 q
		while(sign(dot(ps[i],ps[i+1],ps[q+1]) - dot(ps[i],ps[i+1],ps[q])) <= 0)	//负向最大即为点积最小
			q = (q+1)%n;
		db d = ps[i].disTo(ps[i+1]);	//线段长度
		d = d*d;
		//叉积求出高，点积求出底边
		db tmp = cross(ps[i],ps[i+1],ps[r]) * 	
			(dot(ps[i],ps[i+1],ps[p])-dot(ps[i],ps[i+1],ps[q]))/d;
		if(ans < 0 || ans > tmp) ans = tmp;
	}
	return ans;
}

bool cmp1(Point a, Point b){		//按照角度排序，第四象限-第三象限-第二象限-第一象限
	if(a.quad() != b.quad()) return a.quad() < b.quad();
	else return sign(a.det(b)) > 0;
}

bool cmp2(Point a, Point b){	//第二种极角排序方式, 用角度直接排
	return a.ang < b.ang;	//tp[j].ang = atan2(am[j].y-am[i].y,am[j].x-am[i].x), 返回幅度制，范围是-pi~pi
}

struct Line {
	Point s,e;
	Line() {}
	Line(Point _s, Point _e) : s(_s), e(_e) {}
	bool operator == (Line v) { return (s == v.s) && (e == v.e); }
	// 根据一个点和倾斜角 angle 确定直线，0 <= angle < pi
	Line(Point p, double angle) {
		s = p;
		if(sign(angle-pi/2) == 0) e = (s+Point(0,1));
		else e = (s+Point(1,tan(angle)));
	}
	// ax+by+c = 0
	Line(db a, db b, db c){
		if(!sign(a)) s = Point(0,-c/b), e = Point(1,-c/b);
		else if(!sign(b)) s = Point(-c/a,0), e = Point(-c/a,1);
		else s = Point(0,-c/b), e = Point(1,(-c-a)/b);
	}
	void adjust() { if(e<s) swap(s,e); }
	db length() { return s.disTo(e); }	//求线段长度
	db angle() {	//返回直线倾斜角 0 <= angle < pi, 弧度制
		db k = atan2(e.y-s.y,e.x-s.x);
		if(sign(k) < 0) k += pi;
		if(sign(k-pi) == 0) k -= pi;
		return k;
	}
	Point crosspoint(Line v){ //求两直线交点
		db a1 = (v.e-v.s).det(s-v.s);
		db a2 = (v.e-v.s).det(e-v.s);
		return Point((s.x*a2-e.x*a1)/(a2-a1),(s.y*a2-e.y*a1)/(a2-a1));
	}
	// 点和直线关系，1在上方，2在下方，3在直线上
	int relation(Point p) {
		int c = sign((p-s).det(e-s));
		if(c < 0) return 1;
		else if(c > 0) return 2;
		else return 3;
	}
	// 点在线段上的判断
	bool pointonseg(Point p) { return sign((p-s).det(e-s)) == 0 && sign((p-s).dot(p-e)) <= 0; }
	// 两向量平行 (对应直线平行或重合)
	bool parallel(Line v) { return sign((e-s).det(v.e-v.s)) == 0; }
	// 线段比较，用于map存直线
	bool operator<(Line l) const {
		if(s != l.s) return s < l.s;
		else return e < l.e;
	}
	//两线段相交判断
	//2 规范相交
	//1 非规范相交
	//0 不相交
	int segcrossseg(Line v){
		int d1 = sign((e-s).det(v.s-s));
		int d2 = sign((e-s).det(v.e-s));
		int d3 = sign((v.e-v.s).det(s-v.s));
		int d4 = sign((v.e-v.s).det(e-v.s));
		if((d1^d2) == -2 && (d3^d4) == -2) return 2;
		return (d1 == 0 && sign((v.s-s).dot(v.s-e)) <= 0) ||
			(d2 == 0 && sign((v.e-s).dot(v.e-e)) <= 0) ||
			(d3 == 0 && sign((s-v.s).dot(s-v.e)) <= 0) ||
			(d4 == 0 && sign((e-v.s).dot(e-v.e)) <= 0);
	}
	//点在线段上的判断
	bool pointonseg(Point p){
		return sign((p-s).det(e-s)) == 0 && sign((p-s).dot(p-e)) <= 0;
	}
};

struct circle{
	Point p; //圆心
	db r; //半径
	circle() {}
	circle(Point _p, db _r){
		p = _p;
		r = _r;
	}
	circle(Point a,Point b,Point c){
		Line u = Line((a+b)/2,((a+b)/2)+((b-a).rotleft()));
		Line v = Line((b+c)/2,((b+c)/2)+((c-b).rotleft()));
		p = u.crosspoint(v);
		r = p.disTo(a);
	}
};

int main()
{
	Point p1(1,3), p2(2,3), p3(1.5,1), p4(1.5,5);
	Line l1(p1,p2);	l1.adjust();
	printf("%f\n",l1.angle());

	return 0;
}


/*
例题1：[hihocoder 1879]
	有n个点, 求锐角三角形的个数/总面积（2018 北京icpc ）
	n <= 2000, 可能三点共线 
解法1：
	(n,3)-直角-钝角，O((n^2)logn)，因为锐角三角形会出现重复，因此计算直角和钝角，直角和钝角不会重复


例题2：
	点旋转，用复数乘法来操作


例题3：
	n个点，查看是否有三点共线
	枚举一个点，将其它点进行极角排序，查看是否有三点共线 O((n^2)logn)
	如果是整数点，还可以进行哈希
	(x1,y1) (x2,y2)
	d1 = gcd(x1,y1)
	d2 = gcd(x2,y2)
	hash (x1/d1, y1/d1), (x2/d2,y2/d2)
*/
