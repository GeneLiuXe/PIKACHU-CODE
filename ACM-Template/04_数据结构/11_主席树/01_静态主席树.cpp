// ---
// 新建多个权值线段树副本，记录只考虑1-i个数时，每个数出现在各个区间的个数是多少，类似于建多棵权值线段树
// 然后第i棵线段树，参考第i-1棵线段树，优化空间
// 空间为4*n*log(n)
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
const int N = 1e5+100;

struct Tree{
	int lc,rc; //左右节点 t数组 编号
	int l,r; //节点的左右端点
	int sum; 
}t[N*20];
int n,m,a[N],num,b[N],root[N],tot;

int build(int l,int r)
{
	int p = ++tot; // 新建一个节点，编号为p，代表当前区间[l,r]
	t[p].l = l, t[p].r = r, t[p].sum = 0;
	if(l == r) return p;
	int mid = (l+r)>>1;
	t[p].lc = build(l,mid);
	t[p].rc = build(mid+1,r);
	return p;
}

int insert(int now,int pos,int k)
{
	int p = ++tot;
	t[p] = t[now]; //建立副本
	if(t[p].l == t[p].r){
		t[p].sum += k; //在副本上修改
		return p;
	}
	int mid = (t[p].l+t[p].r)>>1;
	if(pos <= mid) t[p].lc = insert(t[p].lc,pos,k); //保留右儿子部分，把左儿子更新
	else t[p].rc = insert(t[p].rc,pos,k);
	t[p].sum = t[t[p].lc].sum + t[t[p].rc].sum;
	return p;
}

int ask(int lp,int rp,int k) //lp和rp所代表的区间是相同的，他们只不过是在不同状态下的副本
{
	if(t[lp].l == t[lp].r) return t[lp].l; //找到答案
	int cnt = t[t[rp].lc].sum-t[t[lp].lc].sum; // 值在[l,mid]中的数有多少个
	if(cnt >= k) return ask(t[lp].lc,t[rp].lc,k);
	else return ask(t[lp].rc,t[rp].rc,k-cnt);
}

int main()
{
	num = tot = 0;
	scanf("%d%d",&n,&m);
	rep(i,1,n){
		scanf("%d",&a[i]);
		b[++num] = a[i];
	}
	sort(b+1,b+1+num); //离散化
	num = unique(b+1,b+1+num)-b-1;
	root[0] = build(1,num); //root[0]这颗树是一棵空树，关于离散化后的值域建树
	rep(i,1,n)
	{
		int x = lower_bound(b+1,b+1+num,a[i])-b; //离散化后的值
		root[i] = insert(root[i-1],x,1); //值为x的数增加1个
	}
	rep(i,1,m)
	{
		int x,y,z;
		scanf("%d%d%d",&x,&y,&z);
		int ans = ask(root[x-1],root[y],z);
		printf("%d\n",b[ans]); //从离散化后的值变回原值
	}
	return 0;
	//root[i]:表示只考虑1-i这些数时候建树的情况，这颗树树根的编号
}