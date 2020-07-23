// ---
// 题意：\\
// 给一堆矩阵，求出所有矩阵拼起来，求出矩阵并起来的总周长。\\
// \\
// 思路：\\
// 其实与面积并差不多，就是求ans的时候，ans += abs(last-t[1].len)，每次插入一根线段，对答案的贡献值为使得t[1].len增加或减少的长度。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <cmath>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
const int N = 10000+100;
 
int n;
struct Line{
	int x,y1,y2;
	int flag;
}s1[N],s2[N];
 
bool cmp(Line a,Line b)
{
	return a.x < b.x;
}
 
struct Tree{
	int l,r,s;
	int ml,mr,len;
}t[N*4];
 
int y[4][N],ans;
int num1,num2;
 
void build(int p,int l,int r,int idx)
{
	t[p].l = l, t[p].r = r, t[p].s = 0, t[p].ml = y[idx][l], t[p].mr = y[idx][r];
	if(l == (r-1)) return;
	int mid = (l+r)>>1;
	build(p*2,l,mid,idx);
	build(p*2+1,mid,r,idx);
}
 
void calc(int p)
{
	if(t[p].s >= 1) t[p].len = t[p].mr-t[p].ml;
	else if(t[p].l == (t[p].r-1)) t[p].len = 0;
	else{
		t[p].len = t[p*2].len+t[p*2+1].len;
	}
}
 
void update(int p, Line tp)
{
	if(tp.y1 <= t[p].ml && t[p].mr <= tp.y2)
	{
		t[p].s += tp.flag;
		calc(p);
		return;
	}
	if(t[p*2].mr >= tp.y2) update(p*2,tp);
	else if(t[p*2+1].ml <= tp.y1) update(p*2+1,tp);
	else{
		update(p*2,tp);
		update(p*2+1,tp);
	}
	calc(p);
}
 
int main()
{
	while(~scanf("%d",&n))
	{
		num1 = 0, num2 = 0, ans = 0;
		rep(i,1,n)
		{
			int x1,y1,x2,y2;
			scanf("%d%d%d%d",&x1,&y1,&x2,&y2);
			s1[++num1].x = x1, s1[num1].y1 = y1, s1[num1].y2 = y2, y[1][num1] = y1, s1[num1].flag = 1;
			s1[++num1].x = x2, s1[num1].y1 = y1, s1[num1].y2 = y2, y[1][num1] = y2, s1[num1].flag = -1;
 
			s2[++num2].x = y1, s2[num2].y1 = x1, s2[num2].y2 = x2, y[2][num2] = x1, s2[num2].flag = 1;
			s2[++num2].x = y2, s2[num2].y1 = x1, s2[num2].y2 = x2, y[2][num2] = x2, s2[num2].flag = -1;			
		}
		sort(s1+1,s1+1+num1,cmp);
		sort(s2+1,s2+1+num2,cmp);
		sort(y[1]+1,y[1]+1+num1);
		sort(y[2]+1,y[2]+1+num2);
 
		int scr1 = unique(y[1]+1,y[1]+1+num1)-y[1]-1;
		int scr2 = unique(y[2]+1,y[2]+1+num2)-y[2]-1;
 
		build(1,1,scr1,1);
		rep(i,1,num1)
		{
			int last = t[1].len;
			update(1,s1[i]);
			ans += abs(last-t[1].len);
		}
 
		build(1,1,scr2,2);
		rep(i,1,num2)
		{
			int last = t[1].len;
			update(1,s2[i]);
			ans += abs(last-t[1].len);
		}
 
		printf("%d\n",ans);
	}
	return 0;
}