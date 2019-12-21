// ---
// 题意:\\ 
// 给定 $n$ 和 $m$，表示要组成一个长度为 $n$ 的字符串，再给出 $m$ 个字符串，每个串都有一个能量值，现让你组成一个串，使得你组成的串的总能量值最大。若能量值相同输出长度小的，长度相同则输出字典序小的。$(0\leq n\leq 50,0\leq m\leq 100)$\\
// \\
// 思路:\\
// 涉及多个串的问题，一般都采用$AC$自动机进行解决，我们以$AC$自动机中每一个节点为状态，进行状态转移。定义 $f[i][j]$ 表示长度为 $i$ 的串，当前处于$AC$自动机上的 $j$ 节点的最大能量值，定义 $s[i][j]$ 为 $f[i][j]$ 状态下所组成的串，然后直接进行 $dp$ 转移即可。\\
// \\
// 注意，此问题中每个节点的贡献值不单只是其自身串的贡献，还有其所有 $fail$ 节点贡献的累加值。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
const int inf = 1e8;
using namespace std;

struct Trie{
	int next[500010][26],fail[500010],end[500010]; //fail[i]是指从root到节点i这一段字母的最长后缀节点
	int root,L;	//L相当于tot
	int newnode()
	{
		for(int i = 0;i < 26;i++)
			next[L][i] = -1; //将root节点的26个子节点都指向-1
		end[L++] = 0; //每一个子节点初始化都不是单词结尾
		return L-1;
	}
 	void init()
 	{
 		L = 0;
 		root = newnode(); //此处返回root = 0
 	}
 	void insert(char buf[],int tp)
 	{
		int len = strlen(buf);
		int now = root;
		for(int i = 0; i < len; i++){
			if(next[now][buf[i]-'a'] == -1)
				next[now][buf[i]-'a'] = newnode(); //不能在原有字典树中匹配的新字母则新建一个节点
			now = next[now][buf[i]-'a'];
 		}
 		end[now] = tp;	//now这个节点是一个单词的结尾
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
		while( !Q.empty() ) //按长度进行bfs
		{
			int now = Q.front();
			end[now] += end[fail[now]]; //继承前面节点的value
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
	int query(char buf[])
	{
		int len = strlen(buf);
		int now = root;
		int res = 0;
		for(int i = 0;i < len;i++)
		{
			now = next[now][buf[i]-'a'];
			int temp = now;
			while( temp != root )
			{
				res += end[temp]; //访问到了这里说明，从root到tmp都已经匹配成功
				end[temp] = 0; //加上这句话则同一模式串在匹配串中只会匹配一次，而不会多次匹配
				temp = fail[temp]; //循环遍历从root到tmp这串字符串的所有后缀
			}
		}
		return res;
	}
}ac;

char buf[211][111];
int n,m,f[55][1200]; //pre记录从哪个节点来以及是啥字符
string s[55][1200];

void solve(){
	rep(i,0,n)
		rep(j,0,ac.L) f[i][j] = -inf, s[i][j] = "\0";
	f[0][0] = 0;
	rep(i,1,n)
		rep(j,0,ac.L-1){
			rep(k,0,25){
				int now = ac.next[j][k];
				int tp = f[i-1][j]+ac.end[now];
				string hp = s[i-1][j]+(char)(k+'a');
				if(f[i][now] < tp || (f[i][now] == tp && hp < s[i][now])){
					f[i][now] = tp;
					s[i][now] = hp;
				}
			}
		}
	int ans = 0;
	rep(i,1,n)
		rep(j,0,ac.L-1) ans = max(ans,f[i][j]);
	if(ans == 0) printf("\n");
	else{
		string str = "\0"; int len = 100;
		rep(i,1,n){
			rep(j,0,ac.L-1)
				if(f[i][j] == ans){
					if(i < len || (i == len && s[i][j] < str))
							len = i, str = s[i][j];
				}
		}
		cout << str << endl;
	}
}

int main()
{
	int _; scanf("%d",&_);
	while(_--)
	{
		scanf("%d%d",&n,&m);
		ac.init();
		rep(i,1,m) scanf("%s",buf[i]);
		rep(i,1,m){
			int tp; scanf("%d",&tp);
			ac.insert(buf[i],tp);
		}
		ac.build();
		solve();
	}
	return 0;
}