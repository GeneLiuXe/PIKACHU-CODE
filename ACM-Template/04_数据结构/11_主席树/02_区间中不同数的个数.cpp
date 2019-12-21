// ---
// 题意：\\
// 长度为$n$的序列，$q$次询问，每次给出$l$、$r$，返回序列$[l,r]$中不同数的个数。$(1\leq n\leq 3*10^4,1\leq q\leq 2*10^5)$\\
// \\
// 思路：\\
// 与之前主席树的权值线段树思路不同，此题的思路是建立$n$颗线段树，第$i$颗线段树存储区间$[1,i]$的信息。其中每个节点维护$sum$，表示节点对应区间中数的个数，因此每棵线段树中只保留每个数最后出现的位置。\\
// \\
// 举个例子，序列为$5\ 5\ 5\ 5\ 5$，则第$1$颗线段树中只有第一个位置$sum$为$1$，然后第二颗线段树从第一颗线段树继承过来，由于$5$这个数字之前出现过，因此在第二颗线段树中令第一个位置的$sum$为$0$，令第二个位置的$sum$为$1$，来保存每个数字最后出现的位置。\\
// \\
// 因此查询区间$[l,r]$时，就在第$r$颗线段树中查询区间$[l,n]$中数的个数即可。\\
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
const int N = 3*1e4+100;
const int M = 1e6+100;
const db EPS = 1e-9;
using namespace std;

int n,a[N],q;
int ls[40*N],rs[40*N],sum[40*N],root[N],sz;
int pos[M]; //记录每个数的位置

void build(int &now,int l,int r){
	if(!now) now = ++sz;
	if(l == r){
		sum[now] = 0; return;
	}
	build(ls[now],l,(l+r)>>1);
	build(rs[now],((l+r)>>1)+1,r);
	sum[now] = sum[ls[now]]+sum[rs[now]];
}

void update(int x,int &now,int l,int r,int p1,int ct){
	if(!now) now = ++sz;
	if(l == r){
		sum[now] = sum[x]+ct; return;
	}
	int mid = (l+r)>>1;
	if(p1 <= mid){
		if(!rs[now]) rs[now] = rs[x]; //继承前一个备份的节点
		if(ls[now] <= now) ls[now] = 0; //因为现在要修改ls[now], 因此要重新开空间
		//如果ls[now] <= now, 表示这个节点是之前继承的, 因此要赋为0, 重新开空间
		update(ls[x],ls[now],l,mid,p1,ct);
	}
	else{
		if(!ls[now]) ls[now] = ls[x];
		if(rs[now] <= now) rs[now] = 0;
		update(rs[x],rs[now],mid+1,r,p1,ct);
	}
	sum[now] = sum[ls[now]]+sum[rs[now]];
}

int ask(int &now,int l,int r,int p1){
	if(!now) now = ++sz;
	if(l >= p1) return sum[now];
	else if(r < p1) return 0;
	int mid = (l+r)>>1, ans = 0;
	if(mid >= p1) ans += ask(ls[now],l,mid,p1)+sum[rs[now]];
	else ans += ask(rs[now],mid+1,r,p1);
	return ans;
}

int main()
{
	scanf("%d",&n);
	rep(i,1,n) scanf("%d",&a[i]);
	build(root[0],1,n);
	rep(i,1,n){
		if(pos[a[i]]){
			update(root[i-1],root[i],1,n,pos[a[i]],-1);
			update(root[i-1],root[i],1,n,i,1);
			pos[a[i]] = i;
		}
		else update(root[i-1],root[i],1,n,i,1), pos[a[i]] = i;
	}
	scanf("%d",&q);
	rep(i,1,q){
		int xx,yy; scanf("%d%d",&xx,&yy);
		printf("%d\n",ask(root[yy],1,n,xx));
	}
	return 0;
}
