#include <cstdio>
#include <iostream>
#include <stack>
#include <cstring>
using namespace std;
const int N = 100+10;
const int inf = 0x3fffffff;

int e[N][N],dis[N],path[N],vis[N];
int n,m;

void dijkstra(int x)
{
	//初始化dis数组
	for(int i = 1;i <= n;i++)
		dis[i] = e[x][i];
	
	memset(vis,0,sizeof(vis));
	vis[x] = 1;
	for(int i = 1;i <= n-1;i++)
	{
		int minn = inf;
		int u;
		for(int j = 1;j <= n;j++)
		{
			if(!vis[j] && dis[j] < minn)
			{
				minn = dis[j];
				u = j;
			}
		}
		vis[u] = 1;
		for(int j = 1;j <= n;j++)
		{
			if(e[u][j] < inf)
			{
				if(e[u][j]+dis[u] < dis[j])
				{
					dis[j] = e[u][j]+dis[u];
					path[j] = u;    //记录该点是被哪一个点松弛的 
				}
			}
		}
	}
}

void print(int x,int y)   //x为起点，y为终点 
{
	stack<int> q;
	int tmp = y;
	while(path[y] != -1)
	{
		q.push(y);
		y = path[y];
	}
	q.push(y);
	
	//打印路径 
	printf("%d=>%d, length:%d, path: %d ",x,tmp,dis[tmp],x);  
    while(!q.empty())       //先进后出,获得正序  
	{  
    	printf("%d ",q.top());//打印堆的头节点  
    	q.pop();            //将堆的头节点弹出  
    }  
    printf("\n"); 
} 

int main()
{
	cin>>n>>m;
	memset(path,-1,sizeof(path));  
	//把图初始化 
	for(int i = 1;i <= n;i++)
	{
		for(int j = 1;j <= n;j++)
		{
			if(i == j) e[i][j] = 0;
			else e[i][j] = inf;	
		}	
	} 
	//读入边
	for(int i = 1;i <= m;i++)
	{
		int t1,t2,t3;
		cin>>t1>>t2>>t3;
		e[t1][t2] = t3;
	}  
	dijkstra(1);
	cout<<"n:"<<n<<endl;
	print(1,n); 
	return 0;
}