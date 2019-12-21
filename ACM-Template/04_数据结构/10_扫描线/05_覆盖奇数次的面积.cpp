// ---
// 题意：\\
// 给出n个矩形，求被覆盖区域为奇数次的总面积。\\
// \\
// 思路：\\
// 扫描线有很多种写法，可以打lazy更新到底，也可以不打lazy，只是单纯对目标边进行更新，然后再区间合并上去。\\
// \\
// 本题问的是被覆盖区域为奇数次的总面积。因此线段树每个节点记录被覆盖的次数，被覆盖奇数次的长度，被覆盖偶数次的长度。每次加入一条边，只对被覆盖的那个最大的区间，覆盖次数+1，对于该区间下面的区间不再进行更新，之后也不会更新。\\
// \\
// 因此假如第一次加入的线段是(1,4)，第二次加入的线段是(1,2)，因此第一次(1,4)区间覆盖次数变为1，第二次(1,2)区间覆盖次数变为1。然后区间合并的时候，len1表示覆盖奇数次的长度，len2表示覆盖偶数次的长度，(1,2)区间len1 = 1，len2 = 0. (1,4)区间覆盖次数为奇数，因此len1 = (1,2)与(3,4)区间被覆盖偶数次的长度，因为偶+奇 = 奇，而len2 = (1,2)与(3,4)区间被覆盖奇数次的长度。因此可以发现，虽然在加入直线的时候，没有将更新次数一次性更新到底，但是在区间合并的时候，会将之前覆盖的长度一并算入。\\
// \\
// 由于每次插入直线的时候，最后都会合并到整根扫描线上，因此询问整根扫描线的奇偶长度得到的答案是正确的。但是如果询问某一个区间被覆盖奇数次的长度则会得到错误答案，因为这个区间被覆盖的次数还取决于这个区间之上的区间。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
const int N = 2*1e5+1000;
typedef long long ll;
 
struct Line{
	ll x,y1,y2;
	int flag;
}line[N];
int n, num;
ll y[N], ans;
struct Tree{
	int l,r,s;
	ll ml,mr,len1,len2; //记录覆盖偶数次的长度 和 覆盖奇数次的长度
}t[N*4];
 
bool cmp(Line a,Line b)
{
	return a.x < b.x;
}
 
void build(int p,int l,int r)
{
	t[p].l = l, t[p].r = r, t[p].ml = y[l], t[p].mr = y[r], t[p].s = 0, t[p].len1 = y[r]-y[l], t[p].len2 = 0;
//	cout << t[p].ml << " " << t[p].mr << endl;
	if(l == (r-1)) return;
	int mid = (l+r)>>1;
	build(p*2,l,mid);
	build(p*2+1,mid,r);
}
 
void calc(int p)
{
	//len1：覆盖偶数次，len2：覆盖奇数次
	if(t[p].s % 2){	//覆盖奇数次
		if(t[p].r == (t[p].l+1)) t[p].len2 = t[p].mr-t[p].ml, t[p].len1 = 0;
		else{
			t[p].len1 = t[p*2].len2+t[p*2+1].len2;
			t[p].len2 = t[p*2].len1+t[p*2+1].len1;
			// t[p].len2 = t[p].mr-t[p].ml-t[p*2].len2-t[p*2+1].len2;
		}
	}
	else if(t[p].s % 2 == 0){	//覆盖偶数次
		if(t[p].r == (t[p].l+1)) t[p].len1 = t[p].mr-t[p].ml, t[p].len2 = 0;
		else{
			// t[p].len1 = t[p].mr-t[p].ml-t[p*2].len2-t[p*2+1].len2;
			t[p].len1 = t[p*2].len1+t[p*2+1].len1;
			t[p].len2 = t[p*2].len2+t[p*2+1].len2;
		}
	}
}
 
void change(int p, Line tp)
{
	if(tp.y1 <= t[p].ml && tp.y2 >= t[p].mr)
	{
		t[p].s += tp.flag;
		calc(p);
		return;
	}
	if(t[p*2].mr >= tp.y2) change(p*2,tp);
	else if(t[p*2+1].ml <= tp.y1) change(p*2+1,tp);
	else{
		change(p*2,tp);
		change(p*2+1,tp);
	} 
	calc(p);
}
 
int main()
{
	ans = 0, num = 0;
	scanf("%d",&n);
	rep(i,1,n){
		ll x1,x2,y1,y2;
		scanf("%lld%lld%lld%lld",&x1,&y1,&x2,&y2);
		line[++num].x = x1, line[num].y1 = y1, line[num].y2 = y2, line[num].flag = 1;
		y[num] = y1;
		line[++num].x = x2, line[num].y1 = y1, line[num].y2 = y2, line[num].flag = -1;			
		y[num] = y2;
	}
	sort(line+1,line+1+num,cmp);
	sort(y+1,y+1+num);
	int sc = unique(y+1,y+num+1)-y-1;	
	build(1,1,sc);
	change(1,line[1]);
	rep(i,2,num)
	{
		// printf("%f\n",t[1].len2);
		ans += t[1].len2*(line[i].x-line[i-1].x);
		change(1,line[i]);
	}
	cout << ans << endl;
	return 0;
}