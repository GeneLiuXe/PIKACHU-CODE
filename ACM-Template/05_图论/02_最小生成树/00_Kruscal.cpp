//最小生成树 ——Kruscal 
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

const int maxn = 1000+10;
const int maxm = 4000+10;

struct edge{
	int u,v,w;
}e[maxn];
int n,m,sum;   //sum为总共的路程 
int p[maxn]; //记录每个点的祖先 

bool cmp(edge e1,edge e2)
{
	return e1.w < e2.w;
}

int find(int x)
{
	return p[x]==x?x:p[x]=find(p[x]);
}

bool merge(int x,int y)
{
	int t1 = find(x);
	int t2 = find(y);
	if(t1!=t2)
	{
		p[t1] = t2;
		return true;
	}
	return false;
}

int main()
{
	scanf("%d%d",&n,&m);
	for(int i = 1;i <= m;i++)
	{
		scanf("%d%d%d",&e[i].u,&e[i].v,&e[i].w);
	}
	sort(e+1,e+m+1,cmp);
	for(int i = 1;i <= n;i++) p[i] = i;
	
	//Kruscal算法核心部分 
	int count = 0;  //最小生成树已经有几条边了 
	sum = 0; 
	int maxx = -1;
	int minn = 10010;
	for(int i = 1;i <= m;i++)
	{
		//判断一条边的两个顶点是否已经连通，即判断是否已在同一个集合中
		if(merge(e[i].u,e[i].v))
		{
			count++;
			sum = sum+e[i].w;
			maxx = max(maxx,e[i].w);
			minn = min(minn,e[i].w);	
		} 
		if(count == n-1) break;
	}
	cout<<sum<<endl;
	cout<<maxx-minn<<endl;
	
	return 0;	
} 