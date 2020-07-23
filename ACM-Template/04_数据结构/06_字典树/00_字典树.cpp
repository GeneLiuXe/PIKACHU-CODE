#include <cstdio>
#include <iostream>
#include <cstring>
using namespace std; 
const int charset = 26;
const int max_node = 100000+10;

struct Trie
{
	int tot,root,child[max_node][charset];  //root:根节点  tot:节点编号
	//child[i][j]=k，表示编号为i的节点的第j个孩子是编号为k的节点
	bool flag[max_node];   //是否以某一个字符为结束
	Trie()
	{
//		memset(child,0,sizeof(child));
		memset(flag,0,sizeof(flag)); 
		root = tot = 1;  //根节点编号为1 
	}	
	void mem()  //将字典树初始化 
	{
		memset(child,0,sizeof(child));
		memset(flag,0,sizeof(flag)); 
		root = tot = 1;  //根节点编号为1 
	}
	void insert(const char *str)
	{
		int cur = root;
		for(int i = 0;str[i];i++)
		{
			int x = str[i]-'a';
			if(child[cur][x] == 0)
				child[cur][x] = ++tot;
			cur = child[cur][x];
		}
	//	flag[cur] = true; 记录单词以该点结束 
	}
	bool query(const char *str)
	{
		int cur = root;
		for(int i = 0;str[i];i++)
		{
			int x = str[i]-'a';
			if(child[cur][x] == 0) return false;
			cur = child[cur][x];
		}
		return true;
		//查询单词时应该 return flag[cur]; 
	}
}tre;