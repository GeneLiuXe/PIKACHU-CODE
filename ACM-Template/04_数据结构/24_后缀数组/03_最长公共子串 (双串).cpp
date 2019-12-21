// ---
// 题意:\\ 
// 给定两个字符串 $A$ 和 $B$，求最长公共子串。\\
// \\
// 思路:\\
// 这里介绍一下后缀数组的另一个套路，即涉及两个或多个串时，往往会将两个串用一个没出现的符号（比如 '\$'），将两个串连接在一起。\\
// \\
// 比如此题即可将两个串拼接在一起，求出后缀数组之后，枚举 $height$ 数组，判断 $height$ 数组中相邻的两个后缀是否来自不同的串，如果是则更新答案。\\
// ---
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
#define mem(a,b) memset(a,b,sizeof a);
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
typedef long long ll;
typedef double db;
const int N = 2*1e5+10;
const db EPS = 1e-9;
using namespace std;

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

int n,height[N],sa[N],rak[N],len; //所有数组都从0开始计数
/*
    suffix[i]: 以i为起始位置的后缀
    sa[i]: 排名第i的后缀的起始位置
    rak[i]: 表示suffix[i]的排名
    height[i]: suffix(sa[i-1])和suffix(sa[i])的最长公共前缀
        · h[i] = height[rak[i]], h[i] >= h[i-1]-1
        · suffix[i]和suffix[j]之前的最长公共前缀 = min(height[rak[i]+1]...height[rak[j]])
*/

bool cmp(int *y,int a,int b,int k)
{
    int a1 = y[a],b1 = y[b];
    int a2 = a + k >= len ? -1 : y[a + k];
    int b2 = b + k >= len ? -1 : y[b + k];
    return a1 == b1 && a2 == b2;
}

int t1[N],t2[N],cc[N];

void get_sa(char s[])
{
    int *x = t1,*y = t2,m = 200;
    len = strlen(s);
    for(int i = 0;i < m;i ++) cc[i] = 0;
    for(int i = 0;i < len;i ++) ++ cc[x[i] = s[i]];
    for(int i = 1;i < m;i ++) cc[i] += cc[i - 1];
    for(int i = len - 1;~i;i --) sa[-- cc[x[i]]] = i;
    for(int k = 1;k < len;k <<= 1)
    {
        int p = 0;
        for(int i = len - k;i < len;i ++)  y[p ++] = i;
        for(int i = 0;i < len;i ++) if(sa[i] >= k) y[p ++] = sa[i] - k;
        for(int i = 0;i < m;i ++) cc[i] = 0;
        for(int i = 0;i < len;i ++) ++ cc[x[y[i]]];
        for(int i = 1;i < m;i ++) cc[i] += cc[i - 1];
        for(int i = len - 1;~i;i --) sa[-- cc[x[y[i]]]] = y[i];
        swap(x,y); m = 1; x[sa[0]] = 0;

        for(int i = 1;i < len;i ++)
            x[sa[i]] = cmp(y,sa[i - 1],sa[i],k) ? m - 1 : m ++;
        if(m >= len) break;
    }
}

void get_height(char s[])
{
    len = strlen(s);
    for(int i = 0;i < len;i ++) rak[sa[i]] = i;
    int h = 0;
    height[0] = 0;
    for(int i = 0;i < len;i ++)
    {
        if(!rak[i]) continue;
        int j = sa[rak[i] - 1];
        if(h) h --;
        while(s[i + h] == s[j + h]) h ++;
        height[rak[i]] = h;
    }
}

char s1[N],s2[N],s[N];

int main(){
    scanf("%s",s1);
    scanf("%s",s2);
    int len1 = strlen(s1), len2 = strlen(s2);
    rep(i,0,len1-1) s[i] = s1[i];
    s[len1] = '$';
    rep(i,0,len2-1) s[i+len1+1] = s2[i];
    len = strlen(s);
    get_sa(s);
    get_height(s);
    //[0,len1-1] [len1+1,len1+len2]
    int ans = 0;
    rep(i,1,len1+len2){
        int p1 = sa[i], p2 = sa[i-1];
        if(p1 <= len1-1 && p2 >= len1+1) ans = max(ans,height[i]);
        else if(p2 <= len1-1 && p1 >= len1+1) ans = max(ans,height[i]);
    }
    printf("%d\n",ans);
    return 0;
}
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