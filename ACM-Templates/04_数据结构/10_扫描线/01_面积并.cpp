// ---
// 只要矩形一条线一条线扫描的时候，右边的线没有被读进来，则右边的线一直在扫描面积\\
// \\
// 举个简单的例子，本题中的扫描线只是一个消除矩阵面积重复计算部分的一个方法\\
// ---
#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;
const int SIZE = 300+10;
struct Line{
	double x,y1,y2;
	int flag;
}line[SIZE];

bool cmp(Line a,Line b)
{          
	return a.x<b.x;
}
struct tree{
	int l,r;
	double ml,mr;
	int s;   //s始终 >= 0
	double len;  
}t[SIZE*4];
int n;
double y[SIZE];
void build(int p,int l,int r)
{
	t[p].l = l; t[p].r = r; t[p].ml = y[l]; t[p].mr = y[r];
	t[p].s = 0;
	t[p].len = 0;
	if(t[p].l+1 == t[p].r) return;
	int mid = (l+r)>>1;
	build(p*2,l,mid);
	build(p*2+1,mid,r);
}

void callen(int p)
{
	if(t[p].s > 0)
		t[p].len = t[p].mr-t[p].ml;
	else if(t[p].l == (t[p].r-1)) //所以s == 0的点，最终长度会被赋成0
		t[p].len = 0;
	else 
		t[p].len = t[p*2].len+t[p*2+1].len;    　　 
}

void change(int p,Line tmp)
{
	if(t[p].ml == tmp.y1 && t[p].mr == tmp.y2)
	{
		t[p].s += tmp.flag;
		callen(p);
		return;
	}
	if(tmp.y2 <= t[p*2].mr) change(p*2,tmp);
	else if(tmp.y1 >= t[p*2+1].ml) change(p*2+1,tmp);
	else
	{
		Line tp = tmp;
		tp.y2 = t[p*2].mr;
		change(p*2,tp);
		tp = tmp;
		tp.y1 = t[p*2+1].ml;
		change(p*2+1,tp);
	}
	callen(p);
}

int main()
{
	int cnt = 1;
	while(scanf("%d",&n) && n!=0)
	{
		int num = 1;
		double x1,x2,y1,y2;
		for(int i = 0;i < n;i++) 
		{
			scanf("%lf%lf%lf%lf",&x1,&y1,&x2,&y2);
			line[num].x = x1; line[num].y1 = y1; line[num].y2 = y2;
			line[num].flag = 1;  
			//保存的是线，flag == 1 表示该线在左边
			y[num++] = y1; //将所有的y都读入数组中，进行离散化
			line[num].x = x2; line[num].y1 = y1; line[num].y2 = y2;
			line[num].flag = -1;
			//flag == -1 表示该线在右边
			y[num++] = y2; 	
		} 
		sort(line+1,line+num,cmp); //按照横坐标进行排序
		//对纵坐标进行离散化
		sort(y+1,y+num);  
		int cm = unique(y+1,y+num)-y-1;	
		//在y轴上建立线段树
		build(1,1,cm); 
		change(1,line[1]); 
		double ans = 0;
		for(int i = 2;i < num;i++)
		{
			ans += t[1].len*(line[i].x-line[i-1].x); 
			change(1,line[i]);
		}
		printf("Test case #%d\n",cnt++);
		printf("Total explored area: %.2f\n\n",ans);
	}
	return 0; 
} 