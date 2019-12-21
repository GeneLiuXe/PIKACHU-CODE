// ---
// 题意：$n*m$ 的格子，每个格子上的值表示这个点上的志愿者个数，若该点为 $0$，表示该点为景区。问最少需要多少个志愿者，可以连通所有的景区，需要输出路径。$(1\leq n,m,k\leq 10)$\\
// \\
// 思路：答案直接上板子即可得到。（板子还是理解完记住比较好，养成学一个记住一个的好习惯）。路径输出的话，我们考虑一下 $dp$ 的两部分转移方程。\\
// \\
// 可以发现 $(i,j,state)$ 构成了一个状态，$(i,j)$ 表示坐标。每个状态可能由一个或两个子状态转移而来，因此我们用 $pre[i][j][state]$ 结构体记住两个子状态，输出路径的时候 $dfs$ 递归所有子状态进行格点标记即可。\\
// ---
#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const db EPS = 1e-9;
const int N = 11;
const int inf = 1e8;
using namespace std;

int n,m,a[N][N],st[N][N],cnt,dp[N][N][1<<N],endS,mp[N][N];
bool vis[N][N][1<<N];
int dir[4][2] = {{-1,0},{1,0},{0,1},{0,-1}};
queue<pair<int,int> > q;
struct Node{
	int x,y,s1,s2;
	Node() {x = y = s1 = s2 = 0;}
	Node(int a,int b,int c,int d):x(a),y(b),s1(c),s2(d) {}
}pre[N][N][1<<N];

void init(){
	rep(i,1,n)
		rep(j,1,m){
			scanf("%d",&a[i][j]);
			if(a[i][j] == 0) st[i][j] = 1<<cnt, cnt++;
		}
	endS = (1<<cnt)-1;
	rep(i,0,n)
		rep(j,0,m)
			rep(S,0,endS) dp[i][j][S] = inf, vis[i][j][S] = 0;
	//DP赋初值
	rep(i,0,n)
		rep(j,0,m)
			if(st[i][j]) dp[i][j][st[i][j]] = 0;
}

void SPFA(int state){
	while(q.size()){
		int x = q.front().first, y = q.front().second; q.pop(); vis[x][y][state] = 0;
		rep(i,0,3){
			int tx = x+dir[i][0], ty = y+dir[i][1];
			if(tx < 1 || tx > n || ty < 1 || ty > m) continue;
			if(dp[tx][ty][state|st[tx][ty]] > dp[x][y][state]+a[tx][ty]){
				dp[tx][ty][state|st[tx][ty]] = dp[x][y][state]+a[tx][ty];
				//更新前驱
				pre[tx][ty][state|st[tx][ty]] = {x,y,state,state};
				if((state|st[tx][ty]) != state || vis[tx][ty][state]) continue;
				q.push(make_pair(tx,ty)); vis[tx][ty][state] = 1;
			}
		}
	}
}

void steinertree(){
	rep(S,1,endS){
		rep(i,1,n)
			rep(j,1,m){
				if(st[i][j] && (st[i][j]|S) != S) continue;
				for(int sub = S&(S-1); sub; sub = (sub-1)&S){
					int x = sub|st[i][j], y = st[i][j]|(S-sub);
					if(dp[i][j][x] != inf && dp[i][j][y] != inf){
						if(dp[i][j][S] > dp[i][j][x]+dp[i][j][y]-a[i][j]){
							dp[i][j][S] = dp[i][j][x]+dp[i][j][y]-a[i][j];
							pre[i][j][S] = {i,j,x,y};
						}
					}
				}
				if(dp[i][j][S] != inf)
					q.push(make_pair(i,j)), vis[i][j][S] = 1;
			}
		SPFA(S);
	}
}

void dfs(int x,int y,int state){
	if(vis[x][y][state] || x < 1 || x > n || y < 1 || y > m || !state) return;
	mp[x][y] = 1; vis[x][y][state] = 1;
	int tx = pre[x][y][state].x, ty = pre[x][y][state].y, s1 = pre[x][y][state].s1, s2 = pre[x][y][state].s2;
	if(s1 != s2){
		dfs(tx,ty,s1); dfs(tx,ty,s2);
	}
	else{
		dfs(tx,ty,s1);
	}
}

void solve(){
	int ans = inf, xp = 0, yp = 0;
	rep(i,1,n)
		rep(j,1,m)
			if(dp[i][j][endS] < ans){
				ans = dp[i][j][endS]; xp = i, yp = j;
			}
	memset(vis,0,sizeof vis);
	dfs(xp,yp,endS);
	printf("%d\n",ans);
	rep(i,1,n){
		rep(j,1,m){
			if(a[i][j] == 0) printf("x");
			else if(mp[i][j] == 1) printf("o");
			else printf("_");
		}	
		printf("\n");
	}
}

int main()
{
	scanf("%d%d",&n,&m);
	init();
	steinertree();
	solve();
	return 0;
}