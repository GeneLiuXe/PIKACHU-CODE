// ---
// 题意：给出一个$n*m$的矩阵，$q$次操作，每次操作在矩阵中指定两个不重叠且不接触的小矩阵，将两个矩阵中的对应元素互换。在$q$次操作之后，输出最后的结果矩阵。$(2\leq n,m\leq 10^3,1\leq q\leq 10^4)$\\
// \\
// 思路：此题是子矩阵交换，我们可以先考虑一维状态下的子序列交换，区间$[a,b]$与$[c,d]$交换，如下图所示。每个节点存储右指针，只需将节点[a-1]与[c-1]以及[b]与[d]交换右指针即可。此处需注意，如果两个区间重叠或相互接触，则会发生错误，可以自行手动模拟一下。\\
// \\
// 知道了一维情况下的操作，我们可以考虑二维情况下如何操作。首先每个节点需要维护向右与向下的指针，然后我们来考虑下图情况，两个子矩形相互交换元素。我们可以发现决定这个矩形具体所在位置只是A1、A3区域的向下指针与A2、A4区域的向右指针。\\
// \\
// 因此我们只需将A1、A3与B1、B3的向下指针进行交换，A2、A4与B2、B4的向右指针进行交换即可完成题目要求。注意如果两个矩形有可能相交或接触，则此算法不可行。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 2*1e6+100;
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;

int n,m,q;
struct Node{
	int r,d,v;
}t[N];

int Pos(int x,int y){
	x++, y++;
	return (x-1)*(m+1)+y;
}

int main()
{
	scanf("%d%d%d",&n,&m,&q);
	rep(i,1,n)
		rep(j,1,m) scanf("%d",&t[Pos(i,j)].v);
	//一共n+1行，m+1列，从(0,0)开始编号
	rep(i,0,n)
		rep(j,0,m){
			t[Pos(i,j)].r = Pos(i,j+1);
			t[Pos(i,j)].d = Pos(i+1,j);
		}
	rep(i,1,q){
		int a,b,c,d,h,w;
		scanf("%d%d%d%d%d%d",&a,&b,&c,&d,&h,&w);
		int x = 1,y = 1,u,v;
		rep(j,1,a-1) x = t[x].d;
		rep(j,1,b-1) x = t[x].r;
		rep(j,1,c-1) y = t[y].d;
		rep(j,1,d-1) y = t[y].r;
		u = x, v = y; //先往下再往右，左下周长部分
		rep(j,1,h){
			u = t[u].d, v = t[v].d;
			swap(t[u].r,t[v].r);
		}
		rep(j,1,w){
			u = t[u].r, v = t[v].r;
			swap(t[u].d,t[v].d);
		}
		u = x, v = y; //先往右再往下，右上周长部分
		rep(j,1,w){
			u = t[u].r, v = t[v].r;
			swap(t[u].d,t[v].d);
		}
		rep(j,1,h){
			u = t[u].d, v = t[v].d;
			swap(t[u].r,t[v].r);
		}
	}
	int x = 1;
	x = t[x].d, x = t[x].r;
	rep(i,1,n){
		int y = x;
		rep(j,1,m){
			printf("%d",t[y].v);
			y = t[y].r;
			if(j == m) printf("\n");
			else printf(" ");
		}
		x = t[x].d;
	}
	return 0;
}