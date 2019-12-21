// ---
// 题意：\\
// 两种操作。第一种操作查询第 $T$ 天的最大收益，第二种操作给出一个设计方案，该方案第一天收益为$S$，之后每一天比上一天多收益$P$，注意设计方案收益不可叠加。$(1\leq T\leq 5*10^4)$\\
// \\
// 思路：\\
// 这是一道模板题，每次动态插入一条直线，单点查询最大值即可。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
typedef double db;
const int N = 5*1e4+100;
const int M = 1e5+100;
const db EPS = 1e-12;
using namespace std;

struct line{
	db k,b;
	int l,r,flag;
	line() {}
	line(db a1,db a2,int l1,int r1,int f1) : k(a1),b(a2),l(l1),r(r1),flag(f1) {}
}sgt[2*N];
int n;

inline int get_id(int l,int r) {return (l+r)|(l!=r);}
//计算某条线段在某一个横坐标的纵坐标值
inline db calc(line a,int pos) {return a.k*pos+a.b;}

void modify(int l,int r,line k){
	int now = get_id(l,r);
	if(k.l <= l && k.r >= r){
		if(!sgt[now].flag) sgt[now] = k, sgt[now].flag = 1; //原区间没有优势线段
		else if(calc(k,l)-calc(sgt[now],l) > EPS && calc(k,r)-calc(sgt[now],r) > EPS) sgt[now] = k; //原区间优势线段被覆盖
		else if(calc(k,l)-calc(sgt[now],l) > EPS || calc(k,r)-calc(sgt[now],r) > EPS){ //有一端比原线段大
			int mid = (l+r)>>1;
			//中点坐标处，新线段比原来优势线段更优
			if(calc(k,mid)-calc(sgt[now],mid) > EPS) swap(k,sgt[now]);
			if(calc(k,l)-calc(sgt[now],l) > EPS) modify(l,mid,k);
			else modify(mid+1,r,k);
		}
	}
	else{
		int mid = (l+r)>>1;
		if(k.l <= mid) modify(l,mid,k); //涉及了左区间
		if(mid < k.r) modify(mid+1,r,k); //涉及了右区间
	} 
}

db query(int l,int r,int x){
	int now = get_id(l,r);
	if(l == r) return calc(sgt[now],x);
	else{
		int mid = (l+r)>>1;
		db ans = calc(sgt[now],x);
		if(x <= mid) return max(ans,query(l,mid,x));
		else return max(ans,query(mid+1,r,x));
	}
}

int main()
{
	scanf("%d",&n);
	rep(i,1,n){
		char op[10]; scanf("%s",op);
		if(op[0] == 'P'){
			db k,b; scanf("%lf%lf",&b,&k);
			b = b-k;
			line tmp(k,b,1,5*1e4,1);
			modify(1,5*1e4,tmp);
		}
		else{
			int x; scanf("%d",&x);
			printf("%lld\n",(ll)floor(query(1,50000,x)/100));
		}
	}
	return 0;
}