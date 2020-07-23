// ---
// 题意：\\
// 给出一个$n*m$的矩阵，矩阵每个点为$1$或$0$，每行每列均可翻转且只能翻转一次。问能否将矩形通过翻转变成一个从$a_{1.1}...a_{1.m}\ a_{2.1}...a_{n.m}$数值不下降的状态，如果可以给出每行每列的翻转状态，否则输出$NO$。$(1\leq n,m\leq 200)$\\
// \\
// 思路：\\
// 稍微考虑一下这个题目，就可以发现此题难点主要在于一个点是否翻转由该点所代表的行列同时决定，因此我们难以进行判断。\\
// \\
// 所以我们可以思考能否事先确定行或列的状态，这样判断起来就很方便了。然后就可以发现只要确定了矩阵的最终状态，再确定了第一行的状态，我们就可以确定每一列的状态。确定了每一列状态和矩形最终状态，就可以不断向下循环确定每一行的状态，如果一直都不发生冲突，则为$YES$。\\
// \\
// 我们来考虑一下复杂度。首先枚举矩形的最终状态，即从$(n,m)$向$(1,1)$不断填$1$，因此一共有$n*m$个最终状态。对于每个最终状态，我们枚举第一行是否翻转，然后确定每一列的状态，再对剩下的每一行进行判断。因此复杂度为$O(n*m*n*m)$。\\
// \\
// 这样的复杂度肯定是会$T$的，因此考虑$bitset$优化，对于每一行直接按位异或得到答案，可以将复杂度优化到$O(n*m*n*m)/32$，因此可以通过此题。具体异或细节见代码。\\
// \\
// ---
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <bitset>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 200+10;
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;

bitset<N> row,col,norml[2];
bitset<N> mp[N],jud;
bitset<N> base[N];
int n,m;

void test()
{
	col = mp[0]^base[0]^norml[row[0]]; //枚举第一行状态, 确定每一列是否翻转
	rep(i,1,n-1){
		jud = base[i]^mp[i]^col; //判断这一行的row是否翻转
		if(jud.count() == m) row.set(i,1);
		else if(jud.count() == 0) row.set(i,0);
		else return;
	}
	printf("YES\n");
	rep(i,0,n-1) printf("%d",row[i]==1); printf("\n");
	rep(i,0,m-1) printf("%d",col[i]==1); printf("\n");
	exit(0);
}

int main()
{
	scanf("%d%d",&n,&m);
	rep(i,0,n-1)
		rep(j,0,m-1){
			int xx; scanf("%d",&xx);
			if(xx == 1) mp[i].set(j);
		}
	rep(i,0,m-1) norml[1].set(i);
	for(int i = n-1; i >= 0; i--)
		for(int j = m-1; j >= 0; j--){
			base[i].set(j);
			row.set(0,1);
			test();
			row.set(0,0);
			test();
		}
	printf("NO\n");
	return 0;
}