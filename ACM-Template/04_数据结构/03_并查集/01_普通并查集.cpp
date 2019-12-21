#include <cstdio>
#include <iostream>
using namespace std;
const int maxn=10000+10;

int n,m,p[maxn];

int find(int x) {
	return p[x]==x?x:p[x]=find(p[x]);
}

void merge(int x,int y){
	int r1=find(x),r2=find(y);
	if(r1!=r2){
		p[r1]=r2;
	}
}

int main()
{
	cin>>n>>m;
	for(int i=1;i<=n;i++){
		p[i]=i;
	}
	for(int i=1;i<=m;i++){
		int x,y;
		cin>>x>>y;
		p[x]=y;
	}	
	return 0;
}
