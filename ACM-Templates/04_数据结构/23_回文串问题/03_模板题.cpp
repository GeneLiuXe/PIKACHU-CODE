// ---
// 题意:\\ 
// $n$ 个字符，找出其中所有的奇长度回文串，按长度降序排列，求出前 $k$ 个回文串长度的乘积。$(1\leq n\leq 10^6,1\leq k\leq 10^{12})$\\
// \\
// 思路:\\
// 模板题。直接对于所有本质不同回文串求出其个数，然后用 $pair$ 存到 $vector$ 中，再利用快速幂计算答案即可。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
typedef long long ll;
const int N = 1e6+1000;
const ll mod = 19930726;
using namespace std;

struct PAM{
	#define KIND 26
	int n,last,tot;
	int len[N],trie[N][KIND],fail[N],S[N],num[N];
	ll cnt[N];
	//len[i]: 节点i所代表的回文串长度, fail[i]: 当前回文串的最长回文后缀（不包括自身）
	//cnt[i]: 节点i所代表的回文串的个数, S[i]: 第i次添加的字符, num[i]: 以第i个字符为结尾的回文串个数
	//last: 上一个字符构成最长回文串的位置，方便下一个字符的插入
	//tot: 总结点个数 = 本质不同的回文串的个数+2, n: 插入字符的个数 
	int newnode(int l){
		rep(i,0,KIND-1) trie[tot][i] = 0;
		cnt[tot] = 0, len[tot] = l, num[tot] = 0;
		return tot++;
	}
	inline void init(){
		tot = n = last = 0, newnode(0), newnode(-1);
		S[0] = -1, fail[0] = 1;
	}
	int get_fail(int x){ //获取fail指针
		while(S[n-len[x]-1] != S[n]) x = fail[x];
		return x;
	}
	inline int insert(int c){ //插入字符
		c -= 'a';
		S[++n] = c;
		int cur = get_fail(last);
		//在节点cur前的字符与当前字符相同，即构成一个回文串
		if(!trie[cur][c]){ //这个回文串没有出现过
			int now = newnode(len[cur]+2);
			fail[now] = trie[get_fail(fail[cur])][c];
			trie[cur][c] = now;
			num[now] = num[fail[now]]+1; //更新以当前字符为结尾的回文串的个数
		}
		last = trie[cur][c];
		cnt[last]++; //更新当前回文串的个数
		return num[last]; //返回以当前字符结尾的回文串的个数
	}
	void count(){ //统计每个本质不同回文串的个数
		per(i,tot-1,0) cnt[fail[i]] += cnt[i];
	}
}pam;

int n;
ll k;
char buf[N];
vector<pair<int,ll> > v;

ll pow_mod(ll a,ll b){
	ll ans = 1, base = a;
	while(b){
		if(b&1) ans = (ans*base)%mod;
		base = (base*base)%mod;
		b /= 2ll;
	}
	return ans;
}

int main(){
	scanf("%d%lld",&n,&k);
	scanf("%s",buf);
	pam.init();
	rep(i,0,n-1) pam.insert(buf[i]);
	pam.count();
	ll num = 0;
	rep(i,0,pam.tot-1)
		if(pam.len[i] > 0 && pam.len[i]%2 == 1){
			v.push_back(make_pair(pam.len[i],pam.cnt[i]));
			num += pam.cnt[i];
		}
	sort(v.begin(),v.end());
	if(num < k) printf("-1\n");
	else{
		ll ans = 1;
		int pos = v.size()-1;
		while(k > 0){
			if(k >= v[pos].second){
				ans = (ans*pow_mod(v[pos].first,v[pos].second))%mod;
				k -= v[pos].second;
			}
			else{
				ans = (ans*pow_mod(v[pos].first,k))%mod;
				k = 0;
			}
			pos--;
		}
		printf("%lld\n",ans);
	}
	return 0;
}