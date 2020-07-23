#include <cstdio>
#include <algorithm>
#define rep(i,a,b) for(int i = a; i <= b; i++)
const int N = 1e5+100;
using namespace std;

int ls[2*N],rs[2*N],maxn[2*N],lazy[2*N],rt,sz,a[N],n; //rt: root, sz: 当前节点编号

void push_down(int now){
	if(ls[now] == 0) ls[now] = ++sz;
	if(rs[now] == 0) rs[now] = ++sz;
	maxn[ls[now]] += lazy[now], maxn[rs[now]] += lazy[now];
	lazy[ls[now]] += lazy[now], lazy[rs[now]] += lazy[now];
	lazy[now] = 0;
}
void update(int& now, int l, int r, int lx, int rx, int c){ //区间修改
	if(!now) now = ++sz;
	if(lx <= l && rx >= r){
		maxn[now] += c; lazy[now] += c;
		return;
	}
	if(lazy[now] != 0) push_down(now);
	int mid = (l+r)>>1;
	if(lx <= mid) update(ls[now],l,mid,lx,rx,c);
	if(rx > mid) update(rs[now],mid+1,r,lx,rx,c);
	maxn[now] = max(maxn[ls[now]],maxn[rs[now]]);
}
int query(int& now, int l, int r){	//查询最右边第一个值大于0的点
	if(!now) now = ++sz;
	if(maxn[now] <= 0) return -1;
	if(l == r) return a[l];
	if(lazy[now] != 0) push_down(now);
	int mid = (l+r)>>1;
	if(maxn[rs[now]] > 0) return query(rs[now],mid+1,r);
	else return query(ls[now],l,mid);
}

int main()
{
	scanf("%d",&n);
	rep(i,1,n){
		int pos,op; scanf("%d%d",&pos,&op);
		if(op == 1){ //push
			int xx; scanf("%d",&xx); a[pos] = xx;
			update(rt,1,n,1,pos,1);
		}
		else update(rt,1,n,1,pos,-1); //pop
		printf("%d\n",query(rt,1,n));
	}
	return 0;
}