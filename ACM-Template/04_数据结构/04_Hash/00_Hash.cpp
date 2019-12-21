// ---
// 题意：\\
// 给出n个串，m组询问。每组询问均为一个字符串，询问在初始n个串中是否存在一个串与该询问串恰好只有一个位置不相同。输出YES or NO。\\
// \\
// Hash思路：\\
// 首先先讲讲Hash的算法，利用bkdr算法将每个字符串hash成一个数值，hash函数如下：\\
// \\
// hash: abccac\\
// hash[1] = 0\\
// hash[6] = $0*131^5+1*131^4+2*131^3+2*131^2+0*131+2$\\
// \\
// 此处hash[6]即是这个字符串的hash值，131为seed，即hash种子，然后还要取一个模数，即mod。由此可以发现hash值即是字符串中每一个位置的贡献，所以本题要求恰好只有一个位置不相同，即可以枚举不相同位置，减去原有贡献，加上新贡献即可。然后本题就可以解决，但是由于此题的数据卡的很e xin，所以seed和mod取的不好的话，会被卡成zz。\\
// \\
// 这里补充一下常见的seed和mod，seed一般取质数，3、5、7、13、131、13131这些均可，mod一般也取质数，1e9+7，1e11+7，1e13+7，1e18+7均可，也可以直接将数据类型取为unsigned long long，即可对$2^64-1$直接取模。\\
// \\
// 有一个注意点，在计算过程中，seed*mod不能超过数据类型的最大值，否则相当于在计算过程中又模上了一个不是mod的数，会导致结果错误。\\
// \\
// 由于本题只有三个字符，所以可以将seed定为3，mod定一个很大的数，类似于三进制。\\
// ---
#include <cstdio>
#include <iostream>
#include <cstring>
#include <map>
#include <algorithm>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
typedef long long ull;
const int N = 6*1e5+1000;
const ull mod = 1e11+7;
const ull ttp1 = 3;
const ull ttp2 = 5;
int n,m;
ull base[5] = {0,1,2,131313,123};
ull seed1[N],seed2[N];
char s[N];
struct Node{
	ull a,b;
}gn;
map<Node,int> mp;
 
bool operator == (Node x, Node y)
{
	if(x.a == y.a && x.b == y.b) return true;
	else return false;
}
 
bool operator < (Node x, Node y)
{
	return x.a < y.a;
}
 
int solve(ull hash1, ull hash2)
{
	int len = strlen(s);
	// printf("s:%s,len:%d\n",s,len);
	rep(j,0,len-1)
	{
		rep(k,0,2)
		{
			if(s[j]-'a' == k) continue;
			ull tmp1 = hash1;
			tmp1 = (tmp1+(-base[s[j]-'a']+base[k])*seed1[len-1-j]%mod)%mod;
			if(tmp1 < 0) tmp1 += mod;
 
			ull tmp2 = hash2;
			tmp2 = (tmp2+(-base[s[j]-'a']+base[k])*seed2[len-1-j]%mod)%mod;
			if(tmp2 < 0) tmp2 += mod;
 
			gn.a = tmp1, gn.b = tmp2;
			// printf("k:%d,tmp:%llu\n",k,tmp);
			if(mp[gn] == 1) return 1;
		}
	}
	return 0;
}
 
int main()
{
	// printf("mod:%lld\n",mod);
	mp.clear();
	seed1[0] = 1;
	seed1[1] = ttp1;
	seed2[0] = 1;
	seed2[1] = ttp2;
	int _ = 6*1e5+100;
	rep(i,2,_)
	{
		seed1[i] = (seed1[i-1]*ttp1)%mod;
		if(seed1[i] < 0) seed1[i] += mod;
		
		seed2[i] = (seed2[i-1]*ttp2)%mod;
		if(seed2[i] < 0) seed2[i] += mod;
	}
	scanf("%d%d",&n,&m);
	rep(i,1,n)
	{
		scanf("%s",s);
		int len = strlen(s);
		ull hash1 = 0;
		ull hash2 = 0;
		rep(j,0,len-1)
		{
			hash1 = (hash1*ttp1%mod+base[s[j]-'a'])%mod;
			if(hash1 < 0) hash1+=mod;
 
			hash2 = (hash2*ttp2%mod+base[s[j]-'a'])%mod;
			if(hash2 < 0) hash2+=mod;
		}
		// printf("s:%s,hash:%llu\n",s,hash);
		gn.a = hash1, gn.b = hash2;
		mp[gn] = 1;
	}
	rep(i,1,m)
	{
		scanf("%s",s);
		int len = strlen(s);
		ull hash1 = 0;
		ull hash2 = 0;
		rep(j,0,len-1)
		{
			hash1 = (hash1*ttp1%mod+base[s[j]-'a'])%mod;
			if(hash1 < 0) hash1+=mod;
 
			hash2 = (hash2*ttp2%mod+base[s[j]-'a'])%mod;
			if(hash2 < 0) hash2+=mod;
		}
		if(solve(hash1,hash2))
			printf("YES\n");
		else printf("NO\n");
	}
	return 0;
}
 
//mod: 1e9+7, seed: 257
//mod: 1e18+3, seed: 3、5
//mod: 1e11+7, seed: 3、5、1313、...
//如果mod*seed会越界的话，那么结果就会错误，因为计算过程中出现了两个seed，多了自动溢出的那个seed