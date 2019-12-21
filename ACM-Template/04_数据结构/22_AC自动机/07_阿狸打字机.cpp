// ---
// 题意:\\ 
// 给出一个长串。其中 'B' 表示最后一个字母被删除，'P' 表示将当前字符打印出来。一共 $m$ 次询问，每次询问给出一个 $(x,y)$，表示查询第 $x$ 次打印的字符串在第 $y$ 次打印的字符串中出现了多少次。$(1\leq n,m\leq 10^5)$\\
// \\
// 思路:\\ 
// 首先先将长串加入到$AC$自动机中，然后记录每个节点的父节点，遇到 'B' 就往上走，遇到 $P$ 就标记第 $x$ 次打印对应的是$AC$自动机上的哪个节点。\\
// \\
// 构造完$AC$自动机后，问题就变成了如何查询其中一个节点在另一个节点中出现的次数。我们首先考虑暴力匹配，其实就是取出字符串 $y$，然后在 $AC$ 自动机中跑，所有经过的节点都标记一下，然后查询有多少个被标记的节点中包含 $x$。即在 $y$ 跑完之后，倒着将标记压到 $fail$ 节点上，然后查询 $x$ 节点的标记次数即可。\\
// \\
// 在这个暴力过程中，我们可以发现一个节点在另一个节点中出现的次数，取决于其所有 $fail$ 子孙被标记的次数，因此我们将查询离线，按照右端点排序。然后根据 $AC$ 自动机的 $fail$ 指针建立 $fail$ 树，求出 $dfs$ 序，用树状数组维护每个节点子树中被标记的次数。\\
// \\
// 总结:\\
// 此题最大的收获是彻底加深了对于 $fail$ 指针的理解，一个节点在另一个字符串中出现的次数，取决了该节点所有 $fail$ 子树中被标记的次数之和。\\
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