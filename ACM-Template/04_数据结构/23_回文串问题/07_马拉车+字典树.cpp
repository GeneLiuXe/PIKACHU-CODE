// ---
// 题意:\\
// 给出 $n$ 个字符串，求解在其中 $n*n$ 个拼接中，回文串的数量。$(\sum len(i)\leq 2*10^6)$\\
// \\
// 思路:\\
// 考虑马拉车+字典树解决该问题。\\
// \\
// 两个串拼在一起是回文串，即 $s$ 与 $h$ 拼接之后为回文串，主要有两种情况。\\
// 1. $|s|\leq |h|$，则将 $s$ 与 $h$ 的反串进行匹配，$h$ 中剩下的未匹配部分为回文串。\\
// 2. $|h|\leq |s|$，则将 $s$ 与 $h$ 的反串进行匹配，$s$ 中剩下的未匹配部分为回文串。\\
// \\
// 分类完之后即可用马拉车算法求出回文串范围，然后用字典树进行反串匹配。\\
// ---
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
#define mem(a,b) memset(a,b,sizeof a);
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
typedef long long ll;
typedef double db;
const int N = 2e6+1000;
const db EPS = 1e-9;
using namespace std;

int n,trie[N][26],tot,p[2*N],flag[N],L[N],sum1[N],sum2[N];
char s[N],s1[N],s2[2*N];
ll ans = 0;

void init(char base[],int len,int op = 0){
    //manacher
    int ln = 1, R = 0, pos = 0;
    rep(i,1,len) s1[i] = base[i-1];
    s2[0] = '$'; s2[ln = 1] = '#';
    for(int i = 1; i <= len; i++)
        s2[++ln] = s1[i], s2[++ln] = '#';
    for(int i = 1; i <= ln; i++){
        if(R >= i) p[i] = min(p[pos*2-i],R-i+1);
        if(p[i]+i > R){
            for(; s2[R+1] == s2[i*2-R-1] && R+1 <= ln && i*2-R-1 <= ln; R++);
            pos = i;
            p[i] = R-i+1;
        }
    }
    //预处理
    if(op){
        for(int i = 1; i <= len; i++){
            int tp = i+(len-i)/2;
            flag[i] = 0;
            tp *= 2;
            if((len-i) % 2 == 1) tp += 2;
            else tp++;
            if(p[tp] >= ln-tp+1) flag[i] = 1; 
        }
        return;
    }
    for(int i = 1; i <= len; i++){
        int tp = i-(i-1)/2;
        flag[i] = 0;
        tp *= 2;
        if((i-1)%2 == 1) tp -= 2;
        else tp--;
        if(i != 1 && p[tp] >= tp) flag[i] = 1;
    }
    //插入字典树
    int now = 0;
    for(int i = len; i >= 1; i--){
        if(!trie[now][s1[i]-'a']){
            trie[now][s1[i]-'a'] = ++tot, sum1[tot] = sum2[tot] = 0;
            rep(j,0,25) trie[tot][j] = 0;
        }
        now = trie[now][s1[i]-'a'];
        if(flag[i]) sum2[now]++; //后半部分回文
        if(i == 1) sum1[now]++; //末尾
    }
}

void solve(char base[],int len){
    init(base,len,1);
    rep(i,1,len) s1[i] = base[i-1];
    int now = 0;
    for(int i = 1; i <= len; i++){
        if(!trie[now][s1[i]-'a']) return;
        now = trie[now][s1[i]-'a'];
        if(i == len) ans += sum2[now]+sum1[now];
        else if(flag[i]) ans += sum1[now];
    }
}

int main()
{
    while(~scanf("%d",&n)){
        tot = 0; ans = 0;
        rep(i,0,25) trie[0][i] = 0;
        int l = 0;
        rep(i,1,n){
            int tp; scanf("%d%s",&tp,s+l);
            init(s+l,tp);
            L[i] = l; L[i+1] = l+tp;
            l += tp;
        }
        ans = 0;
        rep(i,1,n) solve(s+L[i],L[i+1]-L[i]);
        printf("%lld\n",ans);
    }
    return 0;
}