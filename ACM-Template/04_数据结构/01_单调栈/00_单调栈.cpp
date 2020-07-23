// ---
// 给出一个序列，长度为 $n$。定义区间价值为区间和*区间最小值，求出这个序列中的最大区间价值。$(n\leq 10^5,0\leq ai\leq 10^6)$\\
// \\
// 回忆一下单调栈和单调队列，单调栈 —— 对于序列中每个点，求出序列中左/右边第一个比它大/小的点，单调队列 —— 对于序列中每个点，求出序列中距离该点K步范围内的最小/大值。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <stack>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 1e5+100;
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;

int n,a[N],tt1[N],tt2[N];
ll sum[N];
stack<int> st;

int main(){
	scanf("%d",&n);
	rep(i,1,n) scanf("%d",&a[i]);
	rep(i,1,n) tt1[i] = 0, tt2[i] = n+1;
	st.push(1);
	rep(i,2,n){
		while(st.size() && a[i] < a[st.top()]){
			int x = st.top();
			st.pop();
			tt2[x] = i;
			// LOG1("x",x);
		}
		if(st.size()) tt1[i] = st.top();
		st.push(i);
	}
	rep(i,1,n) sum[i] = sum[i-1]+(ll)a[i];
	ll ans = 0;
	int l = 1,r = 1;
	rep(i,1,n){
		if(ans < a[i]*(sum[tt2[i]-1]-sum[tt1[i]])){
			ans = a[i]*(sum[tt2[i]-1]-sum[tt1[i]]);
			l = tt1[i]+1;
			r = tt2[i]-1;
		}
	}
	printf("%lld\n",ans);
	printf("%d %d\n",l,r);
	return 0;
}
