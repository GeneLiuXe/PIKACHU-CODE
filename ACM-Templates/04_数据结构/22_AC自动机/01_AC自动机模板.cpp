// ---
// 一、适用问题: \\
// \\
// $AC$ 自动机主要适用于多个模式串下的匹配问题，常嵌套 $dp$ 进行考察或者利用其 $fail$ 树的性质进行出题。但是这些问题都有一个相同的特点，就是一定会题目给出或者人为建出多个字符串进行匹配，此点可以用于辨别$AC$自动机问题。\\
// \\
// 二、$AC$ 自动机算法解析 \\
// \\
// 基本步骤\\
// 1. 构造一颗 $Trie$ 树，作为 AC自动机的搜索数据结构。\\
// 2. 构造 $fail$ 指针，使当前字符失配时跳转到具有最长公共前后缀的字符继续匹配。如同 $kmp$ 算法一样，$AC$ 自动机在匹配时如果当前字符匹配失败，那么利用 $fail$ 指针进行跳转。由此可知如果跳转，跳转后的串的前缀，必为跳转前的模式串的后缀并且跳转的新位置的深度（匹配字符个数）一定小于跳之前的节点。因此我们可以利用 $bfs$ 在 $Trie$ 上面进行 $fail$ 指针的构造。\\
// \\
// $fail$ 指针的构造 \\
// 【定义】$fail$ 指针指向最长的可匹配后缀，$fail$ 指向点 $p$，表示从根节点到点 $p$ 是当前匹配位置的最长后缀。\\
// 【构建方法】按深度 $bfs$ 整颗字典树，得到当前节点的 $fail$ 指针需要查看其父亲的 $fail$ 指针是否有自己这个儿子，如果没有，需要继续跳 $fail$ 指针。\\
// \\
// 常数优化 \\
// 最后我们在 $AC$ 自动机上跑字符串的时候，我们在失配时，通过 $fail$ 求出匹配点，用该点更新失配节点对应儿子，优化常数。\\
//【举例】假如 $next[x][i]==0$，表示节点 $x$ 没有 $i$ 这个节点，因此我们会不断跳 $x$ 的 $fail$ 指针，直到找到一个节点 $u$，使得 $son[u][i]!=0$，然后我们顺便设置 $son[x][i]=son[u][i]$，优化常数。\\
// \\
// 注意点 \\
// 给出 $n$ 个模式串以及 $1$ 个匹配串，询问每个模式串在匹配串中出现的次数。在 $AC$ 自动机上直接进行匹配时被匹配到的每个节点的 $fail$ 节点也都被匹配到了，不能忘记计算其 $fail$ 节点被匹配的次数。\\
// ---
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
 	void insert(char buf[])
 	{
		int len = strlen(buf);
		int now = root;
		for(int i = 0;i < len;i++)
		{
			if(next[now][buf[i]-'a'] == -1)
				next[now][buf[i]-'a'] = newnode(); //不能在原有字典树中匹配的新字母则新建一个节点
			now = next[now][buf[i]-'a'];
 		}
 		end[now]++;	//now这个节点是一个单词的结尾
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
	void debug()
	{
		for(int i = 0;i < L;i++)
		{
			printf("id = %3d,fail = %3d,end = %3d,chi = [",i,fail[i],end[i]);
			for(int j = 0;j < 26;j++)
				printf("%2d",next[i][j]);
			printf("]\n");
		}
	}
}ac;