#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int N = 500;

int v[N],w[N],num[N];  //每袋的价格、重量和袋数
int dp[N];  //dp[i]:总经费为i时所能买的最多重量的大米
int n,total;  //大米种类、总经费

void ZeroOnePack(int cost,int weight)
{
	for(int i = total; i >= cost;i--)
		dp[i] = max(dp[i],dp[i-cost]+weight);
}

//完全背包，代价为cost，获得的价值为weight
void CompletePack(int cost,int weight)
{
	for(int i = cost;i <= total;i++) //total为上界
		dp[i] = max(dp[i],dp[i-cost]+weight);
}

//多重背包，代价为cost，获得的价值为weight
void MultiplePack(int cost,int weight,int amount)
{
	if(cost*amount >= total) CompletePack(cost,weight);  //相当于取任意袋数，变为完全背包
	else{
		int k = 1;
		while(k < amount){
			ZeroOnePack(k*cost,k*weight);  //二进制拆分
			amount -= k;
			k<<=1;
		}
		ZeroOnePack(amount*cost,amount*weight);  //把剩余个数用01背包求
		//这个很重要，不要忘记了
	}
}

int main()
{
	int T;
	scanf("%d",&T);
	while(T--)
	{
		memset(dp,0,sizeof dp);  //memset别忘了，很重要！
		scanf("%d%d",&total,&n);
		rep(i,1,n)
			scanf("%d%d%d",&v[i],&w[i],&num[i]);
		rep(i,1,n)
			MultiplePack(v[i],w[i],num[i]);
		printf("%d\n",dp[total]);
	}
	return 0;
}