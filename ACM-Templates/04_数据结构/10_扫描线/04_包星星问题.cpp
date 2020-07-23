// ---
// 题意：\\
// 给出一大堆星星的坐标，给出每个星星的亮度。然后给出一个矩形，要求用这个矩形包住的星星的亮度最大。注意：如果星星在矩形边界上，则不计算这个星星的亮度。\\
// \\
// 思路：\\
// 我们来思考一下，一个矩形的位置是不是由这个矩形右上角这个点所决定的，所以我们可以把考虑矩形的位置改为考虑右上角这个点所在的位置。\\
// \\
// 然后我们可以发现，对于一颗星星，(x,y)，只要右上角这个点在（x+0.1，y+0.1）～（x+w-0.1，y+h-0.1）这个范围内，即可包住这颗星星。此处取0.1的原因是星星不能在矩形边界上。\\
// \\
// 因此一个星星就可以确定一个矩形，那么本题就变成了给出一大堆矩形，每个矩形都有一个权值，问其中哪一个区域矩形值之和最大。\\
// \\
// 因此我们可以将每个矩形的左右边界抽离出来，然后就变成了区间覆盖问题。\\
// \\
// 询问在线段树维护下的这根扫描线上亮度最大的值是多少，所以线段树上只需要维护一个最大值，再加上一个lazy标记，然后边插入边，边更新ans，就可以通过此题。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
const int N = 100000;
 
struct Line{
	double x,y1,y2;
	int flag;
}line[N];
 
bool cmp(Line a,Line b)
{
	return a.x < b.x;
}
 
int n,w,h,num,ans;
double y[N];
 
struct Tree{
	int l,r,lazy;
	double ml,mr;
	int maxn;
}t[N*4];
 
void build(int p,int l,int r)
{
	t[p].l = l, t[p].r = r, t[p].ml = y[l], t[p].mr = y[r], t[p].maxn = 0, t[p].lazy = 0;
	if(l == r) return;
	int mid = (l+r)>>1;
	build(p*2,l,mid);
	build(p*2+1,mid+1,r);
}
 
void pushup(int p)
{
	if(t[p].lazy != 0)
	{
		t[p*2].lazy += t[p].lazy;
		t[p*2+1].lazy += t[p].lazy;
		t[p*2].maxn += t[p].lazy;
		t[p*2+1].maxn += t[p].lazy;
		t[p].lazy = 0;
	}
}
 
void change(int p, Line a)
{
//	cout << a.y1 << " " << a.y2 << endl;
	if(a.y1 <= t[p].ml && t[p].mr <= a.y2)
	{
	//	t[p].s += a.flag;
		t[p].lazy += a.flag;
		t[p].maxn += a.flag;
		return;
	}
	pushup(p);
	if(t[p*2].mr >= a.y2) change(p*2,a);
	else if(t[p*2+1].ml <= a.y1) change(p*2+1,a);
	else{
		change(p*2,a);
		change(p*2+1,a);
	}
	t[p].maxn = max(t[p*2].maxn,t[p*2+1].maxn);
}	
 
int main()
{
	while(~scanf("%d%d%d",&n,&w,&h))
	{
		ans = 0, num = 0;
		rep(i,1,n)
		{
			double x1,y1,z1;
			scanf("%lf%lf%lf",&x1,&y1,&z1);
			line[++num].x = x1+0.1, line[num].y1 = y1+0.1, line[num].y2 = y1+h-0.1, y[num] = y1+0.1, line[num].flag = z1;
			line[++num].x = x1+w-0.1, line[num].y1 = y1+0.1, line[num].y2 = y1+h-0.1, y[num] = y1+h-0.1, line[num].flag = -z1;	
		}
		sort(line+1,line+1+num,cmp);
		sort(y+1,y+1+num);
		int scr = unique(y+1,y+1+num)-y-1;
		build(1,1,scr);
 
		rep(i,1,num)
		{
			change(1,line[i]);
			ans = max(ans,t[1].maxn);
		}
		printf("%d\n",ans);
	}
	return 0;
}