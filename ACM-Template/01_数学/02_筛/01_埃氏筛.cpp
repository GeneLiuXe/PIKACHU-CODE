#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#define rep(i,a,b) for(int i = a;i <= b;i++)
using namespace std;
const int N = 1.5*1e7+1000;

int b[N],tot,prime[N];

int main()
{
	tot = 0; //素数个数
 
	//埃氏筛
	for(int i = 2; i <= 1e5; i++)
	{
		if(b[i] == 0)
			prime[++tot] = i; //哪几个是素数
		else continue;
		int base = i;
		while(base <= 1e5){
			base += i;
			b[base] = 1; //这个数是不是素数, 为1则非素数
		}
	}
	return 0;
}