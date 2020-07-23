// ---
// 描述: \\
// 维护一个W*W的矩阵，初始值均为S.每次操作可以增加某格子的权值,或询问某子矩阵的总权值.修改操作数M<=160000,询问数Q<=10000,W<=2000000.\\
// \\
// 输入：\\
// 第一行两个整数,S,W;其中S为矩阵初始值;W为矩阵大小\\
// \\
// 接下来每行为一下三种输入之一(不包含引号):\\
// \\
// "1 x y a", 你需要把(x,y)(第x行第y列)的格子权值增加a\\
// \\
// "2 x1 y1 x2 y2", 你需要求出以左下角为(x1,y1),右上角为(x2,y2)的矩阵内所有格子的权值和,并输出\\
// \\
// "3", 表示输入结束\\
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
const int N = 160000+4*1e4+100;
const int M = 1e4+100;
const db EPS = 1e-9;
using namespace std;

int T,n,ans[M],tot,num,t[2000010]; //tot-操作个数  num-查询个数
struct Node{
	int x,y,op,val,f; //x坐标 y坐标 op:1-修改 op:2-查询 val:答案贡献(1/-1) f:对应答案编号
	bool operator < (Node xx) const {
		if(x != xx.x) return x < xx.x;
		else return op < xx.op;
	}
}p[N];

inline int lowbit(int x) { return x&(-x); }
inline void update(int x, int val) { while(x <= n) t[x] += val, x += lowbit(x); }
inline int ask(int x){
	int tp = 0; while(x >= 1) tp += t[x], x -= lowbit(x);
	return tp;
}

void solve(int l, int r){
	if(l == r) return;
	int mid = (l+r)>>1;
	solve(l,mid); solve(mid+1,r);
	sort(p+l,p+mid+1); sort(p+mid+1,p+r+1);
	int i = l, j = mid+1;
	while(j <= r){
		if(p[j].op == 1){
			j++; continue;
		}
		while(p[i].x <= p[j].x && i <= mid){
			if(p[i].op == 1) update(p[i].y,p[i].val);
			i++;
		}
		ans[p[j].f] += p[j].val*ask(p[j].y);
		j++;
	}
	rep(kk,l,i-1){
		if(p[kk].op == 1) update(p[kk].y,-p[kk].val);
	} 
}

int main()
{
	scanf("%d%d",&T,&n);
	int op,xx,yy,aa,zz;
	while(scanf("%d",&op)){	
		if(op == 3) break;
		if(op == 1){
			scanf("%d%d%d",&xx,&yy,&aa); 
			p[++tot] = {xx,yy,1,aa,0};
		}
		else{
			scanf("%d%d%d%d",&xx,&yy,&aa,&zz);
			++num;
			p[++tot] = {aa,zz,2,1,num};
			p[++tot] = {xx-1,yy-1,2,1,num};
			p[++tot] = {xx-1,zz,2,-1,num};
			p[++tot] = {aa,yy-1,2,-1,num};
		}
	}
	solve(1,tot);
	rep(i,1,num) printf("%d\n",ans[i]);
	return 0;
}

/*
0 4
1 2 3 3
2 1 1 3 3
1 2 2 2
2 2 2 3 4
3
*/