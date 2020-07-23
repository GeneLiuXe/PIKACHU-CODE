// ---
// 适用问题：一个图中，有若干个关键点，将这几个关键点连在一起的最小花费。直观的理解，就是带关键节点的最小生成树。\\
// \\
// 时间复杂度：$O(n*3^k+c*|E|*2^k)$。$n$ 为点数，$|E|$ 为边数，$k$ 为关键点数，$c$ 为 $spfa$ 常数，前一部分为子集枚举的复杂度，第二部分为枚举边松弛的复杂度。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
const int N = 30+5;
const int K = 10;
const int inf = 1e8;
using namespace std;

int dp[N][1<<K], st[N], endS;
bool vis[N][1<<K];
queue<int> q;
//dp[i][state]: 以i为根，关键点在当前斯坦纳树中的连通状态为state的最小花费。

//初始化函数
void init(){
	//定义末状态
	endS = (1<<K)-1;

	//初始化
	rep(i,0,n)
		rep(S,0,endS) dp[i][S] = inf, vis[i][S] = 0;
	
	//求每个点的状态, 假设前k个点为特殊点
	rep(i,1,k) st[i] = 1<<(i-1);

	//dp函数关键点赋初值
	rep(i,1,k) dp[i][st[i]] = 0;
}

//斯坦纳树第二部分转移
void SPFA(int state){
	while(q.size()){
		int x = q.front(); q.pop(); vis[x][state] = 0;
		//枚举连边
		for(int i = head[x]; i; i = e[i].next){
			int y = e[i].to;
			//松弛操作
			if(dp[y][st[y]|state] > dp[x][state]+e[i].w){
				dp[y][st[y]|state] = dp[x][state]+e[i].w;
				//状态保持一致或已经在队列中了
				if((st[y]|state) != state || vis[y][state]) continue;
				vis[y][state] = 1;
				q.push(y);
			}
		}

	}
}

//斯坦纳树主函数
void steinerTree(){
	rep(S,1,endS){
		rep(i,1,n){
			//i为关键节点，判断i是否在状态S中
			if(st[i] && (S|st[i]) != S) continue;
			//第一部分转移，枚举子集
			for(int sub = S&(S-1); sub; sub = (sub-1)&S){	
				int x = st[i]|sub, y = st[i]|(S-sub);
				if(dp[i][x] != inf && dp[i][y] != inf)
					dp[i][S] = min(dp[i][S],dp[i][x]+dp[i][y]);
			}
			if(dp[i][S] != inf)
				q.push(i), vis[i][S] = 1;
		}
		//第二部分转移，对于每个状态进行一次转移
		SPFA(S);
	}
}