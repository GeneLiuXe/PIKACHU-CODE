// ---
// 题意：\\
// \ 一个n*n的网格图，每行每列各涂一次，每行每列均有26种涂法，输出涂色方案。\\
// \\
// 思路：\\
// \ 可以发现本题有个性质，最后涂的行或列，一定只有一个元素。\\
// \ 因此开一个结构体记录每行每列，各个颜色的涂色情况，以及该行或列一共有几种不同的颜色。\\
// \ 然后类似于跑拓扑排序，将只有一种颜色的行列加入队列，再依次撤销。\\
// \ 将新的颜色数变为1的行列加入队列，即可完成本题。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <queue>
#include <algorithm>
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x) cout << "x: " << x << endl;
#define LOG2(x,y) cout << "x: " << x << ", y: " << y << endl;
typedef long long ll;
typedef double db;
const db EPS = 1e-9;
using namespace std;
const int N = 3000+100;
 
int n;
char mp[N][N];
struct Node{
	int total;
	int num[30];
}t[2*N];
int ans[2*N][4],tot;
int vis[2*N];
 
void solve()
{
	tot = 0;
	queue<int> q;
	while(q.size()) q.pop();
	rep(i,1,n*2) {
		if(t[i].total == 1) q.push(i);
		else if(t[i].total == 0) ans[++tot][1] = i, ans[tot][2] = 0, vis[i] = 1;
	}
	while(q.size()){
		int x = q.front();
		q.pop();
		if(vis[x]) continue;
		t[x].total = 0;
		ans[++tot][1] = x;
		vis[x] = 1;
		rep(i,0,27)
			if(t[x].num[i] >= 1) ans[tot][2] = i;
		int pp = ans[tot][2];
		if(x <= n){
			rep(i,n+1,n*2){
				if(mp[x][i-n]-'a' != pp) continue;
				t[i].num[pp]--;
				if(t[i].num[pp] == 0){
					t[i].total--;
					if(t[i].total == 1) q.push(i);
				} 
			}
		}
		else{
			rep(i,1,n){
				if(mp[i][x-n]-'a' != pp) continue;
				t[i].num[pp]--;
				if(t[i].num[pp] == 0){
					t[i].total--;	
					if(t[i].total == 1) q.push(i);
				} 
			}
		}
	}
	for(int i = 2*n; i >= 1; i--)
	{
		int x = ans[i][1], y = ans[i][2];
		if(x <= n) printf("h %d ",x);
		else printf("v %d ",x-n);
		printf("%c\n",'a'+y);
	}
}
 
int main()
{
	scanf("%d",&n);
	rep(i,1,n) scanf("%s",mp[i]+1);
	rep(i,1,n)
		rep(j,1,n){
			if(mp[i][j] == '?') continue;
			int x = mp[i][j]-'a';
			t[i].num[x]++;
			if(t[i].num[x] == 1) t[i].total++;
			t[j+n].num[x]++;
			if(t[j+n].num[x] == 1) t[j+n].total++;
		}
	solve();
	return 0;
}