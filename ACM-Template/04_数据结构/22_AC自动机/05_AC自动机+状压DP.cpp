// ---
// 题意:\\ 
// 给出 $L$ 和 $N$，表示密码的长度和观察到的子串个数。再给出 $N$ 个子串，询问长度为 $L$ 的密码包含这 $N$ 个子串的所有可能方案。如果方案总数小于 $42$，则输出所有方案。$(1\leq L\leq 25,1\leq N\leq 10)$\\
// \\
// 思路:\\ 
// $N$ 范围比较小，因此考虑状压 $dp$，$f[i][S][j]$ 表示长度为 $i$，包含的子串状态为 $S$，当前在$AC$自动机的第 $j$ 个节点上，所有可能的方案数，直接转移即可求取答案。\\
// \\
// 但是此题还需要输出方案，因此我们对于每个 $f[i][S][j]$ 开一个 $vector$ 存储所有可能情况，但是需要剪枝，不然会 $mle$，不能让任何一个 $vector$ 中的方案数大于 $42$，如此即可完成此题。\\
// ---
#include <bits/stdc++.h>
typedef long long ll;
using namespace std;

struct Trie{
	int next[200][26],fail[200],end[200]; //fail[i]是指从root到节点i这一段字母的最长后缀节点
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
 	void insert(char buf[],int flag)
 	{
		int len = strlen(buf);
		int now = root;
		for(int i = 0;i < len;i++)
		{
			if(next[now][buf[i]-'a'] == -1)
				next[now][buf[i]-'a'] = newnode(); //不能在原有字典树中匹配的新字母则新建一个节点
			now = next[now][buf[i]-'a'];
 		}
 		end[now] = flag;	//now这个节点是一个单词的结尾
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
			end[now] += end[fail[now]];
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

char buf[110];
int n,m;
ll dp[27][1050][80];
vector<string> base[27][1050][80];

void solve(){
	memset(dp,0,sizeof dp);
	dp[0][0][0] = 1;
	for(int i = 1; i <= n; i++)
		for(int S = 0; S <= (1<<m)-1; S++)
			for(int j = 0; j <= ac.L-1; j++)
				for(int k = 0; k <= 25; k++){
					int now = ac.next[j][k];
					dp[i][S|ac.end[now]][now] += dp[i-1][S][j];
				}
	ll ans = 0;
	for(int j = 0; j <= ac.L-1; j++) ans += dp[n][(1<<m)-1][j];
	if(ans <= 42){
		for(int i = 0; i <= n; i++)
			for(int S = 0; S <= (1<<m)-1; S++)
				for(int j = 0; j <= ac.L-1; j++){
					dp[i][S][j] = 0; base[i][S][j].clear();
				}
		dp[0][0][0] = 1;
		for(int i = 1; i <= n; i++)
			for(int S = 0; S <= (1<<m)-1; S++)
				for(int j = 0; j <= ac.L-1; j++){
					if(dp[i-1][S][j] == 0) continue;
					for(int k = 0; k <= 25; k++){
						int now = ac.next[j][k];
						dp[i][S|ac.end[now]][now] += dp[i-1][S][j];
						if((int)base[i][S|ac.end[now]][now].size()+(int)base[i-1][S][j].size() > ans) continue;
						if(dp[i-1][S][j] != 0 && (int)base[i-1][S][j].size() == 0){
							string tp = "\0"; tp += (char)('a'+k);
							base[i][S|ac.end[now]][now].push_back(tp);
						}
						else{
							for(auto &v:base[i-1][S][j]){
								string tp = v+(char)('a'+k);
								base[i][S|ac.end[now]][now].push_back(tp);
							}
						}
					}
				}
		vector<string> fin; fin.clear();
		for(int j = 0; j < ac.L; j++){
			for(auto &v:base[n][(1<<m)-1][j])
				fin.push_back(v);
		}
		printf("%lld\n",ans);
		sort(fin.begin(),fin.end());
		for(auto &v:fin)
			cout << v << endl;
	}
	else printf("%lld\n",ans);
}

int main()
{
	scanf("%d%d",&n,&m);
	ac.init();
	for(int i = 0;i < m;i++){
		scanf("%s",buf);
		ac.insert(buf,1<<i);
	}
	ac.build();
	solve();
	return 0;
}