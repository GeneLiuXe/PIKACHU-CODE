// 用回文自动机求求每个回文串有多少个子串.\\

#include<bits/stdc++.h>
using namespace std;
const int N = 1e5+100;
char s[N];
int vis[N];
namespace PAM {
    int sz, fl[N], len[N], ch[N][26],cnt[N],size[N],vis[N];
    char *s;
    int find(int x, int y) {
        return s[y] == s[y - len[x] - 1] ? x : find(fl[x], y);
    }
    void cal(char *str) {
        s = str;
        int n = strlen(s + 1);
        memset(ch, 0, sizeof(ch));
        memset(fl, 0, sizeof(fl));
        int now = 0;
        sz = 1;
        fl[0] = 1;   
        len[0] = 0;   len[1] = -1;
        for (int i = 1; i <= n; i++) {
            now = find(now, i);
            if (!ch[now][s[i] - 'a']) {
                len[++sz] = len[now] + 2;
                fl[sz] = ch[find(fl[now], i)][s[i] - 'a'];
                ch[now][s[i] - 'a'] = sz;
            }
            now = ch[now][s[i] - 'a'];
        }
    }

    void dfs(int u){
        size[u] = 1;
        cnt[u] = 1 + (vis[fl[u]] == 0);
        vis[fl[u]]++; vis[u]++;
        for (int i = 0; i < 26; ++i) 
            if (ch[u][i]) dfs(ch[u][i]), size[u] += size[ch[u][i]];
        vis[fl[u]]--; vis[u]--;
    }

    long long solve(){
        long long ans = 0;
        dfs(1); dfs(0);
        for (int i = 2; i <= sz; ++i){
            ans = ans + size[i] * cnt[i] - 1;
        }
        return ans;
    }
};
int main(){
    int T;
    scanf("%d",&T);
    for (int cas = 1; cas <= T; ++cas){
        scanf("%s",s+1); 
        PAM::cal(s);
        printf("Case #%d: %lld\n",cas,PAM::solve());
    }
    return 0;
}