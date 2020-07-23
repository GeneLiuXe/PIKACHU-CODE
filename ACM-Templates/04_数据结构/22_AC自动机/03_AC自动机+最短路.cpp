// ---
// 题意:\\ 
// 给定 $n$ 和 $m$，表示一共有 $n$ 个点，每个点都有其所对应的坐标，$m$ 条非法路径。先要从 $1$ 号点走到 $n$ 号点，但路径中不能出现 $m$ 条非法路径中任意一条，求最短距离。$(1\leq n\leq 50,1\leq m\leq 100)$\\
// \\
// 思路:\\
// $AC$自动机上跑最短路，思路比较明显。$AC$自动机上的每一个节点都代表一个状态，且不能通过任何非法节点。\\
// \\
// 需要在$AC$自动机的根节点的所有儿子上都建立新节点，且如果一个节点的 $fail$ 节点为非法节点，则该节点也为非法节点。\\
// \\
// 建出 $Trie$ 图后，直接在图上跑 $dijkstra$ 最短路即可。\\
// \\
// 小坑点：坐标之差会爆 $int$...（找了半小时 $bug$）\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
const int N = 100+10;
typedef double db;
const db inf = 1e15;
using namespace std;

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

int n,m,base[100],vis[25100];
db X[N],Y[N],dis[25100];
struct Node{
	db ans; int a,b;
	bool operator < (Node xx) const {
		return ans > xx.ans;
	}
};
priority_queue<Node> q;

struct Trie{
	int next[25100][60],fail[25100],end[25100]; //fail[i]是指从root到节点i这一段字母的最长后缀节点
	int root,L;	//L相当于tot
	int newnode()
	{
		for(int i = 0;i < 51;i++)
			next[L][i] = -1; //将root节点的26个子节点都指向-1
		end[L++] = 0; //每一个子节点初始化都不是单词结尾
		return L-1;
	}
 	void init()
 	{
 		L = 0;
 		root = newnode(); //此处返回root = 0
 	}
 	void insert(int len)
 	{
		int now = root;
		for(int i = 0;i < len;i++)
		{
			int pos = base[i];
			if(next[now][pos] == -1)
				next[now][pos] = newnode(); //不能在原有字典树中匹配的新字母则新建一个节点
			now = next[now][pos];
 		}
 		end[now] = 1;	//now这个节点是一个单词的结尾
 	}
	void build()
	{
		queue<int> Q;
		fail[root] = root;
		for(int i = 0;i < 51;i++)
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
			if(end[fail[now]]) end[now] = 1; //判断该节点是否非法
			Q.pop();
			for(int i = 0;i < 51;i++)
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

db dist(int i,int j){
	//坐标之差会爆int...
	db tmp = (X[i]-X[j])*(X[i]-X[j])+(Y[i]-Y[j])*(Y[i]-Y[j]);
	tmp = sqrt(tmp);
	return tmp;
}

void dijkstra(){
	while(q.size()) q.pop();
	rep(i,0,50)
		if(ac.next[ac.root][i] == 0){
			int p = ac.newnode();
			ac.next[ac.root][i] = p;
			rep(j,0,50) ac.next[p][j] = 0;
		}
	rep(i,0,ac.L) dis[i] = inf, vis[i] = 0;
	q.push({0,ac.next[ac.root][1],1}); dis[ac.next[ac.root][1]] = 0;
	db ans = inf;
	while(q.size()){
		int now = q.top().a, id = q.top().b; q.pop();
		if(vis[now]) continue;
		vis[now] = 1;
		if(id == n) ans = min(ans,dis[now]);
		rep(i,id+1,n){
			db tp = dis[now]+dist(id,i);
			int y1 = now, y2 = ac.next[now][i];
			while(y2 == 0){
				y1 = ac.fail[y1];
				y2 = ac.next[y1][i];
			}
			ac.next[now][i] = y2;
			if(!vis[y2] && !ac.end[y2] && dis[y2] > tp){
				dis[y2] = tp;
				q.push({dis[y2],y2,i});
			}
		}
	}
	if(ans == inf) printf("Can not be reached!\n");
	else printf("%.2f\n",ans);
}

int main(){
	while(~scanf("%d%d",&n,&m)){
		if(n == 0 && m == 0) break;
		rep(i,1,n) scanf("%lf%lf",&X[i],&Y[i]);
		ac.init();
		rep(i,1,m){
			int k; scanf("%d",&k);
			rep(j,0,k-1) scanf("%d",&base[j]);
			ac.insert(k);
		}
		ac.build();
		dijkstra();
	}
}	