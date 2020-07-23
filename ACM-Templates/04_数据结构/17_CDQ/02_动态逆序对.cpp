// ---
// 描述：\\
// 对于序列A，它的逆序对数定义为满足i<j，且Ai>Aj的数对(i,j)的个数。给1到n的一个排列，按照某种顺序依次删\\
// 除m个元素，你的任务是在每次删除一个元素之前统计整个序列的逆序对数\\
// \\
// 输入：\\
// 输入第一行包含两个整数n和m，即初始元素的个数和删除的元素个数。\\
// 以下n行每行包含一个1到n之间的正整数，即初始排列。\\
// 以下m行每行一个正整数，依次为每次删除的元素。\\
// N<=100000 M<=50000\\
// \\
// 输出：\\
// 输出包含m行，依次为删除每个元素之前，逆序对的个数。\\
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
const int M = 5*1e4+100;
const db EPS = 1e-9;
using namespace std;

int n,m,tot,a[N],vis[N],b[N],mp[N];
ll t[N],ans[N];
struct Node{
	int x,y,f; //插在x处，val = y，贡献对象为f
	bool operator < (Node xx) const {
		return x < xx.x;
	}
}p[N];

inline int lowbit(int x) { return x&(-x); }
inline void update(int x, int cc) { while(x <= n) t[x] += cc, x += lowbit(x); }
inline ll ask(int x) { ll tp = 0; while(x) tp += t[x], x -= lowbit(x); return tp; }

bool cmp(Node xx, Node yy){
	return xx.x > yy.x;
}

void solve(int l, int r){
	if(l == r) return;
	int mid = (l+r)>>1;
	solve(l,mid); solve(mid+1,r);
	sort(p+l,p+mid+1); sort(p+mid+1,p+r+1);
	int i = l, j = mid+1;
	while(j <= r){
		while(p[i].x <= p[j].x && i <= mid){
			update(p[i].y,1); i++;
		}
		int tp = ask(n)-ask(p[j].y);
		ans[p[j].f] += tp; j++;
	}
	rep(kk,l,i-1) update(p[kk].y,-1);

	sort(p+l,p+mid+1,cmp); sort(p+mid+1,p+r+1,cmp);
	i = l, j = mid+1;
	while(j <= r){
		while(p[i].x >= p[j].x && i <= mid){
			update(p[i].y,1); i++;
		}
		int tp = ask(p[j].y);
		ans[p[j].f] += tp; j++;
	}
	rep(kk,l,i-1) update(p[kk].y,-1);
}

int main()
{
	scanf("%d%d",&n,&m);
	rep(i,1,n){
		scanf("%d",&a[i]);
		mp[a[i]] = i;
	} 
	rep(i,1,m){
		scanf("%d",&b[i]);
		vis[mp[b[i]]] = 1;
	}
	rep(i,1,n)
		if(!vis[i]) p[++tot] = {i,a[i],m};
	for(int i = m; i; i--){
		p[++tot] = {mp[b[i]],b[i],i};
	} 
	solve(1,tot);
	for(int i = m-1; i; i--) ans[i] += ans[i+1];
	rep(i,1,m) printf("%lld\n",ans[i]);
	return 0;
}