// ---
// 题意:\\ 
// 给定两个字符串，询问满足条件的四元组个数，$(x,y,u,v)$ 满足条件当且仅当 $s[x...y]=h[u...v]$，且 $s[x...y]$ 为回文串。$(1\leq n\leq 10^5)$\\
// \\
// 思路:\\ 
// 建两颗回文自动机，然后从奇根和偶根分别遍历，同时遍历两棵树，如果到达相同位置则计算对答案的贡献，也算是比较裸的一道题目。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
const int N = 4*1e5+10;
typedef long long ll;
using namespace std;

char buf1[N],buf2[N];
ll ans = 0;

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
        per(i,tot-1,2) cnt[fail[i]] += cnt[i];
    }
}pam[2];

void dfs(int t1,int t2){
    rep(i,0,KIND-1){
        int y1 = pam[0].trie[t1][i], y2 = pam[1].trie[t2][i];
        if(y1 && y2){
            dfs(y1,y2);
            ans = ans+pam[0].cnt[y1]*pam[1].cnt[y2];
        }
    }
}

int main(){
    int _; scanf("%d",&_);
    rep(Ca,1,_){
        ans = 0;
        pam[0].init(); pam[1].init();
        scanf("%s",buf1); scanf("%s",buf2);
        int len1 = strlen(buf1), len2 = strlen(buf2);
        rep(i,0,len1-1) pam[0].insert(buf1[i]);
        rep(i,0,len2-1) pam[1].insert(buf2[i]);
        pam[0].count(); pam[1].count();
        dfs(0,0); dfs(1,1);
        printf("Case #%d: %lld\n",Ca,ans);
    }
    return 0;
}