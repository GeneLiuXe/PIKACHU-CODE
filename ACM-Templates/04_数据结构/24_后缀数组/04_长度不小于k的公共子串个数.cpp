// ---
// 题意: \\
// 给定两个字符串 $A$ 和 $B$，求长度不小于 $k$ 的公共子串的个数，即询问有多少个三元组 $(i,j,len)$ 表示 $A[i]～A[i+len-1]$ 与 $B[j]～B[j+len-1]$ 相同，且 $len\geq k$。\\
// \\
// 思路: \\
// 首先将两个串拼接起来，求出后缀数组之后，按 $k$ 对 $height$ 数组进行分组。对于两个最长公共前缀为 $h$ 的后缀，其对答案的贡献为 $h-k+1$，因此我们分组之后，先考虑 $A$ 对 $B$ 的贡献，维护一个单调栈，存储信息为 $pair(int,int)$ 表示 $height$ 的值与个数，每当有更小的值进入时就不断向前合并，主要是 $second$ 的信息合并，并且记录一个全局的 $total=\sum first*second$，用于计算贡献。\\
// \\
// 之后再计算一遍 $B$ 对 $A$ 的贡献即可结束此题。\\
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

int n,k,height[N],sa[N],rak[N],len; //所有数组都从0开始计数
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

int tot;
pair<int,int> st[N];

int main(){
    while(~scanf("%d",&k)){
        if(k == 0) break;
        scanf("%s",s1);
        scanf("%s",s2);
        int len1 = strlen(s1), len2 = strlen(s2);
        rep(i,0,len1-1) s[i] = s1[i];
        s[len1] = '$';
        rep(i,0,len2-1) s[i+len1+1] = s2[i];
        len = strlen(s);
        get_sa(s);
        get_height(s);
        
        //A:[0,len1-1], B:[len1+1,len1+len2]
        ll ans = 0, total = 0;
        tot = -1;
        // rep(i,0,len1+len2) logs(i,sa[i],height[i]);
        rep(i,1,len1+len2){
            if(height[i] < k) {tot = -1; total = 0; continue;}
            else{
                // logs(i,sa[i]);
                int cnt = 0;
                if(sa[i-1] <= len1-1) cnt = 1;
                while(tot >= 0 && st[tot].first >= height[i]){
                    total -= (ll)(st[tot].first-k+1)*(ll)st[tot].second;
                    cnt += st[tot].second;
                    tot--;
                }
                if(cnt != 0){
                    st[++tot] = make_pair(height[i],cnt);
                    total += (ll)(height[i]-k+1)*(ll)cnt;
                }
                if(sa[i] >= len1+1) ans += total;
            }
        }
        tot = -1; total = 0;
        rep(i,1,len1+len2){
            if(height[i] < k) {tot = -1; total = 0; continue;}
            else{
                int cnt = 0;
                if(sa[i-1] >= len1+1) cnt = 1;
                while(tot >= 0 && st[tot].first >= height[i]){
                    total -= (ll)(st[tot].first-k+1)*(ll)st[tot].second;
                    cnt += st[tot].second;
                    tot--;
                }
                if(cnt != 0){
                    st[++tot] = make_pair(height[i],cnt);
                    total += (ll)(height[i]-k+1)*(ll)cnt;
                }
                // logs(i,sa[i],height[i],total);
                if(sa[i] <= len1-1) ans += total;
            }
        }
        printf("%lld\n",ans);
        rep(i,0,len1+len2) s[i] = '\0';
    }
    return 0;
}