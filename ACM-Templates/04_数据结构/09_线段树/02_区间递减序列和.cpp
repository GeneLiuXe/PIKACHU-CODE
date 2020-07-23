// ---
// 题意：与楼房重建题意类似，但是求的是递减序列，而且询问的是区间$[l,r]$的递减序列和。\\
// \\
// 思路：只需将维护内容的$maxn$改为$minn$即可，然后解决一下区间查询的问题。继续使用刚才的$calc(now,tp)$函数，计算节点$now$在最小值为$tp$下的贡献。依然是左右区间二分，如果左区间最小值小于$tp$，则直接计算右区间贡献，然后递归左区间。如果左区间最小值大于$tp$，则直接递归右区间。\\
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
const int N = 1e5+100;
const int M = 3*1e5+100;
const ll inf = 1e13;
const db EPS = 1e-9;
using namespace std;

int n,q,rt,sz,ls[M],rs[M];
ll a[N],sum[M],minn[M],ans;

ll calc(int &now,int l,int r,ll tp){
	if(!now) now = ++sz;
	if(l == r) return (minn[now]<tp?minn[now]:0ll);
	int mid = (l+r)>>1;
	if(minn[ls[now]] < tp) return (sum[now]-sum[ls[now]]+calc(ls[now],l,mid,tp));
	else return calc(rs[now],mid+1,r,tp);
}

void update(int &now,int l,int r,int pos,ll w){
	if(!now) now = ++sz;
	if(l == r){
		sum[now] = minn[now] = w;
		return;
	}
	int mid = (l+r)>>1;
	if(pos <= mid) update(ls[now],l,mid,pos,w);
	else update(rs[now],mid+1,r,pos,w);
	minn[now] = min(minn[rs[now]],minn[ls[now]]);
	sum[now] = sum[ls[now]]+calc(rs[now],mid+1,r,minn[ls[now]]);
}

ll query(int &now,int l,int r,int pos1,int pos2,ll w){
	if(!now) now = ++sz;
	if(pos1 <= l && pos2 >= r){
		ans += calc(now,l,r,w);
		return minn[now];
	}
	int mid = (l+r)>>1;
	if(pos1 <= mid) w = min(w,query(ls[now],l,mid,pos1,pos2,w));
	if(pos2 > mid) w = min(w,query(rs[now],mid+1,r,pos1,pos2,w));
	return w; 
}

int main()
{
	int _; scanf("%d",&_);
	while(_--){
		rt = sz = 0;
		memset(ls,0,sizeof ls);
		memset(rs,0,sizeof rs);
		scanf("%d%d",&n,&q);
		rep(i,1,n){
			scanf("%lld",&a[i]);
			update(rt,1,n,i,a[i]);
		} 
		rep(i,1,q){
			int l,r,p,c; scanf("%d%d%d%d",&l,&r,&p,&c);
			ans = 0, query(rt,1,n,l,r,inf);
			printf("%lld\n",ans);
			if(p != 0 || c != 0) update(rt,1,n,p,c);
		}
	}
	return 0;
}
