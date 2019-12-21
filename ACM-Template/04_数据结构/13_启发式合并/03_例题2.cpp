// ---
// 题意：\\
// 给定一棵有根树，每条边的权值是 $[a,v]$ 的一个字母。现对于每个树上点，求出最长的一条 “回文” 路径。“回文” 路径的含义是将路径上所有的字母取出，可以组成一个回文串。$(1\leq n\leq 5*10^5)$\\
// \\
// 思路：\\
// 树上的这类问题，我们可以依次思考点分治、树上启发式合并、长链剖分，根据该题题意，不难识别这是一道树上启发式合并问题。\\
// \\
// 确认是树上启发式合并之后，我们需要定状态。由于 “回文” 路径只要求所有字母可以组成一个回文串，因此我们可以利用状压的思想给每一个字母进行赋值，然后对于每个点求一个从根节点到当前点的异或和。\\
// \\
// 令 $f[i]$ 表示对于当前点 $now$，其子树中存在一个点 $x$，$value[x]=i,f[i]=dep[x]$，$x$ 为此中情况下深度最深的点。然后计算点 $now$ 答案时，我们只需考虑三种情况。\\
// 1. $now$ 为最长路径的一个端点 \\
// 2. 最长路径经过 $now$ \\
// 3. 最长路径在 $now$ 子树中，不经过 $now$ \\
// \\
// 三种情况在代码中有比较清晰的注释，不太清楚细节的朋友可以看看代码。总体来说，这题应该属于树上启发式合并的经典问题。\\
// \\
// 总结:\\
// 此题有几个思想比较可取。\\
// 1. 对每一个字母进行状压编码\\
// 2. 每一个点维护的权值是从根到该点的异或和\\
// 3. 分 $3$ 种情况对答案进行了枚举\\
// ---
#include <bits/stdc++.h>
#define mem(a,b) memset(a,b,sizeof a);
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
typedef long long ll;
typedef double db;
const int inf = 1e8;
const int N = 5e5+10;
const db EPS = 1e-9;
using namespace std;

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

int n,a[N],tot,head[N],sz[N],son[N],f[1<<22],dep[N],ans[N]; //f[i]:表示子树中异或和为f[i]的最大深度
struct Node{
	int to,next;
}e[N];

void add(int x,int y){
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot;
}

void dfs1(int x){
	ans[x] = -inf; sz[x] = 1;
	for(int i = head[x]; i; i = e[i].next){
		int y = e[i].to;
		dep[y] = dep[x]+1; a[y] ^= a[x];
		dfs1(y); sz[x] += sz[y];
		if(sz[y] > sz[son[x]]) son[x] = y;
	}
}

void Delete(int x){
	f[a[x]] = -inf;
	for(int i = head[x]; i; i = e[i].next) Delete(e[i].to);
}

void modify(int x,int fa){
	ans[fa] = max(ans[fa],f[a[x]]+dep[x]-2*dep[fa]);
	for(int i = 0; i < 22; i++)
		ans[fa] = max(ans[fa],f[a[x]^(1<<i)]+dep[x]-2*dep[fa]);
	for(int i = head[x]; i; i = e[i].next) modify(e[i].to,fa);
}

void ins(int x){
	f[a[x]] = max(f[a[x]],dep[x]);
	for(int i = head[x]; i; i = e[i].next) ins(e[i].to);
}

void dfs2(int x){
	ans[x] = 0;
	for(int i = head[x]; i; i = e[i].next)
		if(e[i].to != son[x]) dfs2(e[i].to), Delete(e[i].to);
	if(son[x]) dfs2(son[x]);
	f[a[x]] = max(f[a[x]],dep[x]);
	//路径经过x
	for(int i = head[x]; i; i = e[i].next) 
		if(e[i].to != son[x]) modify(e[i].to,x), ins(e[i].to);
	//x为路径端点
	ans[x] = max(ans[x],f[a[x]]-dep[x]);
	for(int i = 0; i < 22; i++)
		ans[x] = max(ans[x],f[a[x]^(1<<i)]-dep[x]);
	//路径不经过x
	for(int i = head[x]; i; i = e[i].next) ans[x] = max(ans[x],ans[e[i].to]);
}

int main()
{
	scanf("%d",&n); tot = 1;
	rep(i,0,(1<<22)-1) f[i] = -inf; //不要忘记赋初值
	rep(i,2,n){
		int p; char s[10];
		scanf("%d%s",&p,s);
		add(p,i); a[i] = 1<<(s[0]-'a');
	}
	dfs1(1); dfs2(1);
	rep(i,1,n) printf("%d%c",ans[i]," \n"[i==n]);
	return 0;
}