// ---
// 题意:\\ 
// 给定 $m$，表示构成字符串只能使用前 $m$ 个字符，再给定一个包含 $n$ 个禁止串的集合。然后询问是否能够构成一个无限长的串，满足以下三个条件。\\
// 1. 该串中只使用了前 $m$ 个字符\\
// 2. 该串中不包含任何禁止串\\
// 3. 该串不能在任何位置开始出现循环节\\
// \\
// 如果可以构造则输出 $Yes$，否则输出 $No$。$(1\leq n\leq 100,1\leq m\leq 26)$\\
// \\
// 思路:\\
// 先不考虑循环节的问题，如果单纯考虑能否构造一个只使用了前 $m$ 个字符，不包含禁止串且长度无限的串，这个问题就是一个经典问题。\\
// \\
// 我们将禁止串插入$AC$自动机中，所有的 $next$ 出边组成一个有向图，我们将所有非法节点从图中去除，得到一个合法的有向图，我们可以在这个合法的图上任意走。因此仅当这个图中出现了环，我们才可以构建出一个无限长的串，否则不行。\\
// \\
// 在上面的基础上，我们考虑如何构建一个不出现循环节的长度无限的串，无限长则说明有环，那如果这是个简单环，则在不断绕行的过程中一定会出现循环节。而如果这个环中有好几个环，则可以在左边的环绕一圈，右边的再绕两圈，因此不会出现循环节。\\
// \\
// 因此此题只需要构建$AC$自动机之后，去除无效节点，然后对剩下的有向图跑强连通分量，对于每个强连通分量查看是否有多个环即可。一个简单环中最多只有 $n$ 条边，如果大于 $n$ 条边，则非简单环，因此只需判定一个环中的边数量即可。\\
// ---
#include <bits/stdc++.h>
const int N = 1e5+10;
typedef long long ll;
using namespace std;

char buf[N];
int n,m,pos[N],tot,head[N],dfn[N],top,sz[N],vis[N]; //top记录dfs序
vector<pair<int,int> > v[N];
ll c[N],ans[N];
struct Node{
	int to,next;
}e[N];
void add(int x,int y){
	e[++tot].to = y, e[tot].next = head[x], head[x] = tot;
}

struct Trie{
	int next[N][26],fail[N],end[N],fa[N]; //fail[i]是指从root到节点i这一段字母的最长后缀节点
	int root,L;	//L相当于tot
	int newnode()
	{
		for(int i = 0;i < 26;i++)
			next[L][i] = -1; //将root节点的26个子节点都指向-1
		fa[L] = 0; end[L++] = 0; //每一个子节点初始化都不是单词结尾
		return L-1;
	}
 	void init()
 	{
 		L = 0;
 		root = newnode(); //此处返回root = 0
 	}
 	void insert(char buf[])
 	{
		int len = strlen(buf), cnt = 0;
		int now = root;
		for(int i = 0;i < len;i++)
		{
			if(buf[i] == 'B') now = fa[now];
			else if(buf[i] == 'P') pos[++cnt] = now;
			else{
				if(next[now][buf[i]-'a'] == -1){
					next[now][buf[i]-'a'] = newnode(); //不能在原有字典树中匹配的新字母则新建一个节点
					fa[next[now][buf[i]-'a']] = now;
				}
				now = next[now][buf[i]-'a'];
			}
 		}
 	}
	void build()
	{
		queue<int>Q;
		fail[root] = root;
		for(int i = 0;i < 26;i++)
		{
			if(next[root][i] == -1)
				next[root][i] = root; //将root的未被访问的子节点指回root
			else
			{
				fail[next[root][i]] = root; //root子节点的失配指针指向root
				Q.push(next[root][i]); //队列中加入新节点
			}
		}
		while( !Q.empty() )
		{
			int now = Q.front();
			add(fail[now],now);
			Q.pop();
			for(int i = 0;i < 26;i++)
				if(next[now][i] == -1)
					next[now][i] = next[fail[now]][i]; //now的第i个节点未被访问，则将now的第i个节点指向now的fail节点的第i个节点
				else
				{
					fail[next[now][i]]=next[fail[now]][i];
					Q.push(next[now][i]);
				}
		}
	}
}ac;

inline int lowbit(int x) {return x&(~x+1);}
inline void update(int x,ll v) {for(;x<=ac.L;x+=lowbit(x)) c[x] += v;}
inline ll ask(int x){
	ll tp = 0;
	while(x) tp += c[x], x -= lowbit(x);
	return tp;
}

void dfs(int x){
	dfn[x] = ++top; sz[x] = 1;
	for(int i = head[x]; i; i = e[i].next){
		int y = e[i].to; dfs(y);
		sz[x] += sz[y];
	}
}

void solve(){
	int len = strlen(buf), now = ac.root;
	int cnt = 0;
	for(int i = 0; i < len; i++){
		if(buf[i] == 'B'){
			update(dfn[now],-1);
			now = ac.fa[now];
			vis[now] = 0;
			continue;
		}
		else if(buf[i] == 'P'){
			cnt++;
			for(auto &hp:v[cnt]){
				int x = hp.first, y = cnt, id = hp.second;
				ll mp = ask(dfn[pos[x]]+sz[pos[x]]-1)-ask(dfn[pos[x]]-1);
				ans[id] = mp;
			}
			continue;
		}
		else now = ac.next[now][buf[i]-'a'];
		if(!vis[now]){
			vis[now] = 1;
			update(dfn[now],1);
		}
		else{
			vis[now] = 0;
			update(dfn[now],-1);
		}
	}
	for(int i = 1; i <= m; i++) printf("%lld\n",ans[i]);
}

int main()
{
	scanf("%s",buf);
	scanf("%d",&m);
	for(int i = 1; i <= m; i++){
		int l,r; scanf("%d%d",&l,&r);
		v[r].push_back(make_pair(l,i));
	}
	ac.init();
	ac.insert(buf);
	tot = 1;
	ac.build();
	for(int i = 0; i < ac.L; i++)
		if(!dfn[i]) dfs(i);
	solve();
	return 0;
}