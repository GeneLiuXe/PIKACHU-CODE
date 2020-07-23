// ---
// 题意：\\
// 音符格式转换成长度最小且字典序最小的格式。一共有$7$种音节，分别是$R1$、$R2$、$R4$、$R8$、$R16$、$R32$、$R64$，分别表示1、$\frac{1}{2}$、$\frac{1}{4}$、$\frac{1}{8}$、$\frac{1}{16}$、$\frac{1}{32}$、$\frac{1}{64}$ 拍。对于$R1R2$即为$\frac{3}{2}$拍，也可以表示成$R1.$。即可将相邻的一个音节化为‘.’，因此$R2...$表示$R2R4R16R32$。\\
// \\
// 此题给出一个音符表示格式，要求转化为长度最短且字典序最小的方式。\\
// \\
// 例如 $R1R4R16$ 转化为 $R16R1R4$ 。\\
// \\
// \\
// 思路：\\
// 比赛的时候遇到这题，错误地当成了贪心问题进行考虑，导致始终无法AC。\\
// \\
// 现在我们来观察这个问题，这个表达式所有音节的拍数相加之和是不变的，而每一个音节单元的拍数和长度也都是确定的，因此等价于一个完全背包问题。\\
// \\
// 因此如果此题没有要求给出一个字典序最小的方案，只要求最短长度的话。$dp[i]$表示音符表达式拍数为$i$所需的最短长度，然后直接跑一个完全背包即可。\\
// \\
// 但是这题还要求求出长度最小时字典序最小，因此我们需要考虑状态方程。我一开始的错误做法是两维$for$循环，第一维枚举用到了第$i$个音节单元，第二维枚举当前拍数$j$，然后在转移时若是小于，直接转移，若是等于，则比较当前音节单元字典序与dp[j]的最后一个音节单元字典序。这样会$wa$，因为无法保证字典序最小。\\
// \\
// 但是如果换一种枚举方式就可以通过此题，第一维$for$循环枚举音节拍数$i$，第二维枚举使用了第$j$个音节单元，然后 $==$ 部分的转移仍比较当前$j$与$dp[i]$的上一个音节单元字典序即可通过此题。原因在于求取$dp[i]$的时候，已经使用了所有的音节单元，而前一种枚举方式并没有用上所有的音节单元所以导致出错。\\
// \\
// 当然此题也可以字典序倒序枚举前$i$个音节单元, 也可以通过此题。\\
// \\
// 处理完这部分之后，就是完全背包的路径输出了，由于完全背包$dp[i][j]$是由$dp[i][j-w[i]]$转移而来，即同维转移而来，因此不需要不需要$01$背包的二维记录路径，直接使用一维进行路径记录，即上一次由哪个音节单元转移而来即可。\\
// \\
// 总结：\\
// 此题考查了完全背包的两个内容，一是完全背包求取过程中加上了对于字典序最小的求解，二是完全背包的一维路径输出。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <bitset>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 3200000+10;
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;

char s[N];
int len,n,tot,dp[N],pre[N],ttp[N];
// int str[N][33];
// string str[N];
//v: 拍数, w: 长度
//len: 总拍数
//用最少的长度凑满总拍数
//dp[i]: 表示凑满拍数为i所需的最少长度

struct Node{
	int num,sum,ctt,v,w; //num: 编号，sum: .个数，ctt: 选了多少个, v: 拍数, w: 字符长度
	string ss;
	bool operator < (Node xx) const {
		if(num == xx.num) return sum > xx.sum; 
		else return (ss < xx.ss);
	}
}ans[30];

void solve()
{
	memset(dp,0x3f,sizeof dp);
	dp[0] = 0;
	rep(j,1,len) //音符长度
		rep(i,1,tot){ //枚举所有音符单元
			if(j >= ans[i].v){
				if(dp[j] > dp[j-ans[i].v]+ans[i].w){
					dp[j] = dp[j-ans[i].v]+ans[i].w;
					pre[j] = i;
				}
				else if(dp[j] == dp[j-ans[i].v]+ans[i].w){
					if(pre[j] > i){ //之前的字典序更大
						dp[j] = dp[j-ans[i].v]+ans[i].w;
						pre[j] = i;		
					}
				}
			}
		}
	int tp = len;
	while(tp > 0){ //完全背包一维记录路径, 直接回溯
		ans[pre[tp]].ctt++;
		tp = tp-ans[pre[tp]].v;
	}
	rep(i,1,tot)
		rep(j,1,ans[i].ctt)
			cout << ans[i].ss;
	cout << endl;
}

int main()
{
	// freopen("e.in","r",stdin);
	// freopen("e.out","w",stdout);
	scanf("%s",s+1); n = strlen(s+1);
	int num;
	//获取拍数总长度
	rep(i,1,n){
		num = 0;
		if(s[i] == 'R'){
			i++;
			while(s[i] != 'R' && i <= n && s[i] != '.')
			{
				num = num*10+s[i]-'0';
				i++;
			}
		}
		int tt = 64/num;
		// LOG2("i",i,"tt",tt);
		len += tt;
		if(s[i] == '.'){
			while(s[i] != 'R' && i <= n){
				tt /= 2;
				len += tt;
				i++;
			}
		}
		i--;
	}
	//构造所有音符单元
	rep(i,1,7){
		rep(j,0,i-1){
			int tp = 1<<(i-1);
			++tot;
			ans[tot].v += tp;
			rep(k,1,j){
				tp /= 2, ans[tot].v += tp;
			}
			ans[tot].w = j+1;
			if(i <= 3) ans[tot].w += 2;
			else ans[tot].w += 1;
			ans[tot].sum = j;
			ans[tot].num = 1<<(7-i);
		}
	}
	rep(i,1,tot){
		ans[i].ss = "\0";
		string tpp;
		tpp += 'R';
		tpp += to_string(ans[i].num);
		rep(j,1,ans[i].sum) tpp += ".";
		ans[i].ss = tpp;
	}
	sort(ans+1,ans+1+tot);
	//按字典序构造音符单元 —— 非关键操作, 也可直接打表输入
	Node tp = ans[7];
	rep(i,8,10) ans[i-1] = ans[i];
	ans[10] = tp;
	// rep(i,1,tot)
		// LOG3("i",i,"ss",ans[i].ss,"v",ans[i].v);
	solve();
	return 0;
}