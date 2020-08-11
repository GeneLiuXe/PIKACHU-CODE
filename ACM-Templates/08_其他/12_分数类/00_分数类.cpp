#include <bits/stdc++.h>
using namespace std;

struct Fraction {
	typedef long long R;
    R a, b; // a/b

    Fraction() {a = 0, b = 1;}
    Fraction(R x, R y) : a(x), b(y) {}
    R gcd(R x, R y) {return y == 0 ? x : gcd(y, x%y);}

    Fraction(const Fraction &y) { // 拷贝构造函数
    	a = y.a, b = y.b;
    	if(a < 0 && b < 0) a = abs(a), b = abs(b);
    	else if(b < 0) a = -a, b = abs(b);
    }

    Fraction adjust(const Fraction &y) {
    	R g = max(gcd(abs(y.a), abs(y.b)), (R)1);
    	R v1 = y.a/g, v2 = y.b/g;
    	if(v1 < 0 && v2 < 0) v1 = abs(v1), v2 = abs(v2);
    	else if(v2 < 0) v1 = -v1, v2 = abs(v2);
    	return {v1, v2};
    }

    Fraction Abs() {
    	return {abs(a), abs(b)};
    }

    Fraction operator + (const Fraction y) {
        R v1 = a*y.b+b*y.a;
        R v2 = b*y.b;
        return adjust({v1, v2});
    }

    Fraction operator - (const Fraction y) {
        R v1 = a*y.b-b*y.a;
        R v2 = b*y.b;
        return adjust({v1, v2});
    }

    Fraction operator * (const Fraction y) {
    	/* 防止乘爆, 确保不会乘爆则可注释掉 */
    	Fraction t1 = {a, b}, t2 = y;
        R g1 = max(gcd(abs(t2.a),abs(t1.b)), (R)1);
        t2.a /= g1, t1.b /= g1;

        R g2 = max(gcd(abs(t1.a), abs(t2.b)), (R)1);
        t1.a /= g2, t2.b /= g2;
        /* ------- */

        R v1 = t1.a*t2.a;
        R v2 = t1.b*t2.b;

        return adjust({v1, v2});
    }

    Fraction operator / (const Fraction y) {
    	/* 防止乘爆, 确保不会乘爆则可注释掉 */
    	Fraction t1 = {a, b}, t2 = y;
        R g1 = max(gcd(abs(t1.a),abs(t2.a)), (R)1);
        t1.a /= g1, t2.a /= g1;

        R g2 = max(gcd(abs(t1.b), abs(t2.b)), (R)1);
        t1.b /= g2, t2.b /= g2;
        /* ------- */

        R v1 = t1.a*t2.b;
        R v2 = t1.b*t2.a;

        return adjust({v1, v2});
    }

    bool operator > (Fraction y) const {
        Fraction t(a,b);
        t = t-y;
        return t.a > 0 && t.b > 0;
    }
};