// ---
// 题意：\\
//       给定一个区间，求这个区间第k小的数，支持单点修改。\\
// \\
// 思路：\\
//       动态主席树裸题。\\
// \\
//       我们先来回顾一下静态主席树的做法，对于数组中每一个位置都维护一棵权值线段树，该权值线段树保存的是区间 [1,x] 的信息。因此我想要求区间 [l,r] 之间第k大的时候，只需要将root[r]-root[l-1]就是维护区间 [l,r] 信息的权值线段树，因此就可以快速直接求出这个区间中第k大的元素是多少。\\
// \\
//       现在我们来看看单点修改的操作。\\
// \\
//       如果我现在要将a[pos]修改为x，那么最暴力的做法就是对于root[pos]～root[n]中的每一颗权值线段树都进行修改，即将a[pos]这个点的值减1，将x这个点的值+1。\\
// \\
//       最暴力的做法显然是无法通过此题的，因此我们可以想到有没有一种logn的方法，可以只修改logn个节点，就可以对于每一个线段树记录修改信息，于是我们想到了树状数组。\\
// \\
//       我们来回忆一下树状数组，每一个节点记录区间 [x-lowbit(x)+1, x] 的所有信息，因此当需要求[1,x]内维护的信息的时候，只需要从节点x出发，每次进行 x-=lowbit(x) 的操作，即可求出[1,x]内维护的所有信息。\\
// \\
//       每次对x节点进行修改的时候，只需要不断进行x+=lowbit(x)的操作，就可以访问到所有存储x节点信息的节点，因此实现了logn的查询。\\
// \\
//       因此本题也维护一个树状数组。\\
// \\
//       节点x维护的是区间 [x-lowbit(x)+1, x] 的权值线段树，因此当我需要访问 [l, r] 区间信息的时候，只需要将root[r]-root[l-1]+getsum(r)-getsum(l-1)这里面维护的便是区间 [l, r] 的权值线段树。\\
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
const int N = 6*1e4+100;
const int M = 32*N+100;
const db EPS = 1e-9;
using namespace std;
 
int rt[N],root[N],ls[M],rs[M],sz,n,m,tot,h1,h2,L[N],R[N],sum[M],a[N],b[N];
struct Node{
	int op,xx,yy,k; //保存修改信息
}t[N];
 
int find(ll x) {return (lower_bound(b+1,b+1+tot,x)-b);}
int lowbit(int x) {return x&(-x);}
 
void update(int pre,int &now,int l,int r,int pos,int c){
	if(!now) now = ++sz;
	sum[now] = sum[pre]+c;
	if(l == r) return;
	int mid = (l+r)>>1;
	if(pos <= mid){rs[now] = rs[pre]; update(ls[pre],ls[now],l,mid,pos,c);}
	else{ls[now] = ls[pre]; update(rs[pre],rs[now],mid+1,r,pos,c);}
}
 
int query(int pre,int now,int l,int r,int k){
	int sum1 = 0, sum2 = 0;
	if(l == r) return l;
	rep(i,1,h1) sum1 += sum[ls[L[i]]]; //左边树状数组节点累加值
	rep(i,1,h2) sum2 += sum[ls[R[i]]]; //右边树状数组节点累加值
	int temp = sum[ls[now]]-sum[ls[pre]]+sum2-sum1;
	int mid = (l+r)>>1;
	if(temp >= k){
		rep(i,1,h1) L[i] = ls[L[i]];
		rep(i,1,h2) R[i] = ls[R[i]];
		return query(ls[pre],ls[now],l,mid,k);
	}
	else{
		rep(i,1,h1) L[i] = rs[L[i]];
		rep(i,1,h2) R[i] = rs[R[i]];
		return query(rs[pre],rs[now],mid+1,r,k-temp);	
	}
}
 
void init(){
	scanf("%d%d",&n,&m);
	rep(i,1,n){
		scanf("%d",&a[i]); b[++tot] = a[i];
	}
	rep(i,1,m){
		char op[10]; scanf("%s",op);
		if(op[0] == 'Q'){
			t[i].op = 1; scanf("%d%d%d",&t[i].xx,&t[i].yy,&t[i].k);
		}
		else{
			t[i].op = 2; scanf("%d%d",&t[i].xx,&t[i].yy);
			b[++tot] = t[i].yy;
		}
	}
	sort(b+1,b+1+tot);
	tot = unique(b+1,b+1+tot)-b-1;
	rep(i,1,n)
		update(rt[i-1],rt[i],1,tot,find(a[i]),1);
}
 
void solve(){
	rep(i,1,m){
		if(t[i].op == 1){ //查询
			h1 = h2 = 0;
			for(int j = t[i].xx-1; j; j -= lowbit(j)) L[++h1] = root[j]; //记录树状数组要计算的节点
			for(int j = t[i].yy; j; j -= lowbit(j)) R[++h2] = root[j];
			int pos = query(rt[t[i].xx-1],rt[t[i].yy],1,tot,t[i].k);
			printf("%d\n",b[pos]);
		}
		else{ //修改
			int pos1 = find(a[t[i].xx]), pos2 = find(t[i].yy);
			for(int j = t[i].xx; j <= n; j += lowbit(j)) update(root[j],root[j],1,tot,pos1,-1);
			for(int j = t[i].xx; j <= n; j += lowbit(j)) update(root[j],root[j],1,tot,pos2,1);			
			a[t[i].xx] = t[i].yy;
		}
	}
}
 
int main()
{
	int _; scanf("%d",&_);
	while(_--){
		init();
		solve();
		rep(i,0,sz) ls[i] = rs[i] = sum[i] = 0;
		rep(i,0,n) root[i] = rt[i] = 0;
		sz = 0, tot = 0;
	}
	return 0;
}