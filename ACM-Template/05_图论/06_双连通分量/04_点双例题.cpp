// ---
// 题意：\\
// \quad n个点，m条边\\
// \quad 如果把第i个点去掉，将有多少对点不能互通\\
// ---
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int N = 1e5+10, M = 5*1e5+10;
typedef long long ll;

struct Edge{
	int to,next;
}e[M*2];
int head[N],dfn[N],low[N],siz[N];   //dfn:记录该点的dfs序	low:记录该点所能达到的最小dfn值
ll ans[N];
bool cut[N];	//记录该点是否为割点
int n,m,tot,num,root;

void init()
{
	tot = 1;
	memset(head,0,sizeof head);
	memset(cut,0,sizeof cut);
	memset(low,0,sizeof low);
	memset(dfn,0,sizeof dfn);
	memset(ans,0,sizeof ans);
	memset(siz,0,sizeof siz);
}

void add(int x,int y)
{
	e[++tot].to = y; e[tot].next = head[x]; head[x] = tot;
}

void tarjan(int x)
{
	dfn[x] = low[x] = ++num;  //记录步长
	siz[x] = 1;  //记录该点之下有几个点
	int flag = 0,sum = 0;  //标记有几个子节点 low[y] >= dfn[x]
	for(int i = head[x]; i ; i = e[i].next)
	{
		int y = e[i].to;
		if(!dfn[y])
		{
			tarjan(y);  //每一次被dfs，都是因为该点未被访问过
			siz[x] += siz[y];
			low[x] = min(low[x],low[y]);
			if(low[y] >= dfn[x]){
				flag++; //根节点需要满足两次这个条件	非根节点只需满足一次这个条件 即为割点
				ans[x] += (ll)siz[y]*(n-siz[y]);  //flag++说明如果去掉x，则y与外界不再连通
				sum += siz[y];
				if(x != 1 || flag > 1) cut[x] = true;
			}
		}
		else{
			//siz[x] += siz[y];	//此处不能加这句话，因为y点之前已经被访问过了，说明就算去掉x，y仍然可以连通，因此不能算入
			low[x] = min(low[x],dfn[y]);  //因为该点之前已经被搜过了	
		} 
	}
	if(cut[x])
		ans[x] += (ll)(n-sum-1)*(sum+1)+(n-1);
	else
		ans[x] = 2*(n-1);
}

int main()
{
	while(~scanf("%d%d",&n,&m))
	{
		init();
		rep(i,1,m){
			int x,y;
			scanf("%d%d",&x,&y);
			if(x == y) continue;
			add(x,y); add(y,x);
		}
		tarjan(1);  //这里不需要从每一个点进行dfs，是因为题目保证所有城镇连通
		rep(i,1,n)
			printf("%lld\n",ans[i]);
	}
	return 0;
}