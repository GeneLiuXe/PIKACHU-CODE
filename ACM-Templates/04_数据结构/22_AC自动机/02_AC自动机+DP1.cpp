// ---
// 题意:\\ 
// 用 $n$ 个基本字符组成一个长度为 $m$ 的字符串，要求字符串中不能出现给定的 $p$ 个非法串中任何一个，输出方案总数。$(1\leq n,m\leq 50,0\leq p\leq 10)$\\
// \\
// 思路:\\
// 数据范围比较小，因此不难往 $dp$ 上进行思考。又因为有多个非法串，考虑在 $AC$自动机建的整个 $Trie$ 图上进行 $dp$。\\
// \\
// 我们定义状态为 $f[i][j]$，表示长度为 $i$，最后一个字符在 $AC$自动机的第 $j$ 个节点上，则枚举 $j$ 的所有子节点，设 $now=next[j][k]$，即 $now$ 为 $j$ 的第 $k$ 个子节点，则 $f[i][now]=f[i][now]+f[i-1][j]$，当且仅当 $now$ 和 $j$ 不为非法节点。\\
// \\
// 因此我们继续定义非法节点，一个点为非法节点，即该点所代表的字符串中出现了完整的非法串，很明显一个非法串的末尾节点是非法节点，并且若 $fail[now]$ 是非法节点，则 $now$ 也为非法节点，因为 $fail[now]$ 节点所代表的字符串为 $now$ 节点字符串的后缀。\\
// \\
// 除此之外，此题还有两个坑点。\\
// 1. 没有取模，因此需要大整数。\\
// 2. 字符的 $ASCII$ 码范围在 $-128～128$ 之间，$re$ 了一小时...\\
// \\
// 总结:\\
// 在 $AC$自动机上进行 $dp$，就是以 $AC$自动机上的节点作为状态进行转移，本质上与普通 $dp$ 没有差别。\\
// ---
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <queue>
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
using namespace std;

char buf[60],base = 0;
int n,m,p,mp[301];

struct Trie{
	int next[2510][60],fail[2510],end[2510]; //fail[i]是指从root到节点i这一段字母的最长后缀节点
	int root,L;	//L相当于tot
	int newnode()
	{
		for(int i = 0;i < 51;i++)
			next[L][i] = -1; //将root节点的26个子节点都指向-1
		end[L++] = 0; //每一个子节点初始化都不是单词结尾
		return L-1;
	}
 	void init()
 	{
 		L = 0;
 		root = newnode(); //此处返回root = 0
 	}
 	void insert(char buf[])
 	{
		int len = strlen(buf);
		int now = root;
		for(int i = 0;i < len;i++)
		{
			int pos = mp[buf[i]-base+150];
			if(next[now][pos] == -1)
				next[now][pos] = newnode(); //不能在原有字典树中匹配的新字母则新建一个节点
			now = next[now][pos];
 		}
 		end[now] = 1;	//now这个节点是一个单词的结尾
 	}
	void build()
	{
		queue<int>Q;
		fail[root] = root;
		for(int i = 0;i < 51;i++)
		{
			if(next[root][i] == -1)
				next[root][i] = root; //将root的未被访问的子节点指回root
			else
			{
				fail[next[root][i]] = root; //root子节点的失配指针指向root
				Q.push(next[root][i]); //队列中加入新节点
			}
		}
		while( !Q.empty() )
		{
			int now = Q.front();
			if(end[fail[now]]) end[now] = 1; //判断该节点是否非法
			Q.pop();
			for(int i = 0;i < 51;i++)
				if(next[now][i] == -1)
					next[now][i] = next[fail[now]][i]; //now的第i个节点未被访问，则将now的第i个节点指向now的fail节点的第i个节点
				else
				{
					fail[next[now][i]]=next[fail[now]][i];
					Q.push(next[now][i]);
				}
		}
	}
}ac;

struct BigInteger{
    ll A[50];
    enum{MOD = 1000000};
    BigInteger(){memset(A, 0, sizeof(A)); A[0]=1;}
    void set(ll x){memset(A, 0, sizeof(A)); A[0]=1; A[1]=x;}
    void print(){
        printf("%lld", A[A[0]]);
        for (ll i=A[0]-1; i>0; i--){
            if (A[i]==0){printf("000000"); continue;}
            for (ll k=10; k*A[i]<MOD; k*=10ll) printf("0");
            printf("%lld", A[i]);
        }
        printf("\n");
    }
    ll& operator [] (int p) {return A[p];}
    const ll& operator [] (int p) const {return A[p];}
    BigInteger operator + (const BigInteger& B){
        BigInteger C;
        C[0]=max(A[0], B[0]);
        for (ll i=1; i<=C[0]; i++)
            C[i]+=A[i]+B[i], C[i+1]+=C[i]/MOD, C[i]%=MOD;
        if (C[C[0]+1] > 0) C[0]++;
        return C;
    }
    BigInteger operator * (const BigInteger& B){
        BigInteger C;
        C[0]=A[0]+B[0];
        for (ll i=1; i<=A[0]; i++)
            for (ll j=1; j<=B[0]; j++){
                C[i+j-1]+=A[i]*B[j], C[i+j]+=C[i+j-1]/MOD, C[i+j-1]%=MOD;
            }
        if (C[C[0]] == 0) C[0]--;
        return C;
    }
}f[2][2510];

int main()
{
	scanf("%d%d%d",&n,&m,&p);
	scanf("%s",buf);
	rep(i,0,n-1) mp[buf[i]-base+150] = i;
	ac.init();
	rep(i,1,p){
		scanf("%s",buf);
		ac.insert(buf);
	}
	ac.build();
	f[0][0].set(1);
	rep(i,1,m){
		rep(j,0,ac.L-1) f[i%2][j].set(0);
		rep(j,0,ac.L-1)
			rep(k,0,n-1){
				int now = ac.next[j][k];
				if(ac.end[now] == 0 && ac.end[j] == 0) f[i%2][now] = f[i%2][now] + f[(i-1)%2][j];
			}
	}
	BigInteger ans; ans.set(0);
	rep(i,0,ac.L-1) ans = ans+f[m%2][i];
	ans.print();
	return 0;
}