// ---
// 通常来说，数位 $dp$ 问题都是通过 $dfs$ 解决的，因为 $dfs$ 的做法更容易理解，也能一定地简化代码。\\
// \\
// 对于 $dfs$ 求解的数位 $dp$ 问题，其中设置的状态为 $f[pos][...]$，$pos$ 表示最后 $pos$ 位没有填，$...$ 表示的是从 $pos$ 位之前继承来的信息，然后 $f[pos][...]$ 的数值表示仅考虑这 $pos$ 位所对答案产生的贡献。\\
// \\
// 这 $...$ 所表示的状态通常需要根据题意来进行定义，比较个性化，也是数位 $dp$ 的核心难点。但数位 $dp$ 问题还是非常套路化的，你只需要根据题意想明白想要计算后 $pos$ 位的信息，到底需要从前几位继承哪些信息，想明白这个之后就可以直接套上 $dfs$ 的模板进行求解了。\\
// \\
// 下面的习题给出的都是非套路化问题，具有一定的难度，初学者建议先写一些模板题再来进行挑战。\\
// \\
// 最后给出 $dfs$ 问题的大致模板。(某一模板题的 $AC$ 代码) \\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
using namespace std;

ll f[21][21][2010],a[30]; //左-右 

ll dfs(int pos,int balan,int k,bool flag){ 
	//位置 平衡点 左边继承来的数值 有无继承
	if(pos == 0){
		if(k == 0) return 1;
		else return 0;
	}
	if(!flag && f[pos][balan][k+1000] != -1) return f[pos][balan][k+1000];
	ll ans = 0;
	int end = flag?a[pos]:9;
	rep(i,0,end){
		ll tp = dfs(pos-1,balan,k+(pos-balan)*i,flag && i == end);
		ans += tp;
	}
	if(!flag) f[pos][balan][k+1000] = ans;
	return ans;
}

ll solve(ll n){
	//求a数组
	if(n == -1) return 0;
	int pos = 0;
	memset(a,0,sizeof a);
	while(n){
		a[++pos] = n%10;
		n /= 10;
	}
	ll ans = 0;
	rep(i,1,pos) ans += dfs(pos,i,0,1); //对每一个平衡点分开求
	ans -= pos-1;
	return ans;
}

int main()
{
	//初始化
	rep(i,0,20)
		rep(j,0,20)
			rep(k,0,2000) f[i][j][k] = -1;
	//读入
	int _; scanf("%d",&_);
	while(_--){
		ll L,R; scanf("%lld%lld",&L,&R); L--;
		printf("%lld\n",solve(R)-solve(L));
	}	
	return 0;
}