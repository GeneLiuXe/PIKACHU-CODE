// ---
// 描述：\\
// 有n朵花,每朵花有三个属性:花形(s)、颜色(c)、气味(m),用三个整数表示。\\
// 现在要对每朵花评级,一朵花的级别是它拥有的美丽能超过的花的数量。\\
// 定义一朵花A比另一朵花B要美丽,当且仅Sa>=Sb,Ca>=Cb,Ma>=Mb。\\
// 显然,两朵花可能有同样的属性。需要统计出评出每个等级的花的数量。\\
// \\
// 输入：\\
// 第一行为N,K (1 <= N <= 100,000, 1 <= K <= 200,000 ), 分别表示花的数量和最大属性值。\\
// 以下N行，每行三个整数si, ci, mi (1 <= si, ci, mi <= K)，表示第i朵花的属性\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
typedef long long ll;
typedef double db;
const int N = 2*1e5+100;
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;

int n,k,ans[N],tot,t[N];
struct Node{
	int a,b,c,s,ans;
	bool operator == (Node xx) const {
		return ((a == xx.a) && (b == xx.b) && (c == xx.c));
	}
	bool operator < (Node xx) const {
		if(b != xx.b) return b < xx.b;
		else return c < xx.c;
	}
}p[N];

bool cmp(Node x,Node y){
	if(x.a != y.a) return x.a < y.a;
	else if(x.b != y.b) return x.b < y.b;
	else return x.c < y.c;
}

inline int lowbit(int x) { return x&(-x); }

inline void update(int x, int c){
	for(int i = x; i <= k; i += lowbit(i)) t[i] += c;
}

inline int ask(int x){
	int tp = 0;
	for(int i = x; i; i -= lowbit(i)) tp += t[i];
	return tp;
}

void CDQ(int l, int r)
{
	if(l == r) return;
	int mid = (l+r)>>1;
	CDQ(l,mid); CDQ(mid+1,r);
	sort(p+l,p+mid+1); sort(p+mid+1,p+r+1); //将第二维排序
	int i = l, j = mid+1;
	while(j <= r){
		while(i <= mid && p[i].b <= p[j].b){
			update(p[i].c,p[i].s);
			i++;
		}
		p[j].ans += ask(p[j].c);
		j++;
	}
	rep(kk,l,i-1) update(p[kk].c,-p[kk].s);	//消除之前的影响，此处是 i, 不是 mid
}

int main()
{
	scanf("%d%d",&n,&k);
	rep(i,1,n) scanf("%d%d%d",&p[i].a,&p[i].b,&p[i].c);
	//离散化
	sort(p+1,p+1+n,cmp);
	int cnt = 0;
	rep(i,1,n){
		cnt++;
		if(p[i]==p[i+1]) continue;
		p[++tot] = p[i], p[tot].s = cnt, cnt = 0, p[tot].ans = 0;
	}
	CDQ(1,tot);
	rep(i,1,tot)
		ans[p[i].ans+p[i].s-1] += p[i].s;
	rep(i,0,n-1) printf("%d\n",ans[i]);
	return 0;
}