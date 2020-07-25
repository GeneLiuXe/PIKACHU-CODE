// 在 SAM 里面增加几个地方就可以了。 //

// 提前是先建好 trie 树， 是在 trie 树的基础上建立SAM， 但是trie树在心里建就好， 不需要真的建出来， 只要知道每个点的前面一个节点是谁就可以了。//
#include<bits/stdc++.h>
using namespace std;
const int N = 1e6+10;
const int M = N << 1;
typedef long long ll;

namespace SAM{
    int cur,node,last,n;
    int ch[M][27],fa[M],len[M],size[M];
    ll ans=0;
    void init(int _n){
        cur = 1, node = 1;  n = _n;
    }
    int Build(int c, int last){
        if (ch[last][c] && len[ch[last][c]] == len[last] + 1) return ch[last][c]; // 特判一的地方。 

        cur = ++ node; //node 是总节点数。cur是当前节点，last 是上一个节点。
        int p = last; len[cur] = len[last] + 1;
        for (; p && !ch[p][c]; p = fa[p]) ch[p][c] = cur;
        //这个是 next 指针。沿着 fail 指针一直向前找。
        if (!p) fa[cur] = 1; else { //如果找到根节点了。就直接 连接 fail 指针，
            int q = ch[p][c];
            if (len[q] == len[p] + 1) fa[cur] = q; else{ //满足条件，连接fail指针.
                int nt = (p == last) ? cur : (++node);  // 特判二的地方
                len[nt] = len[p] + 1; //不满足条件，新加一个节点。
                memcpy(ch[nt],ch[q],sizeof(ch[q])); // 把 q 节点的情况 copy 给新加的 节点。
                if (p != last) fa[cur] = nt; // 特判二的地方
                fa[nt] = fa[q]; fa[q] = nt;
                for (; ch[p][c] == q; p = fa[p]) ch[p][c] = nt;
            }
        }
        return cur;  // 记得返回一个值。 
    }
    ll solve(){   // 求不同的子串个数。 
        for (int i = 2; i <= node; ++i)
            ans = ans + (len[i] - len[fa[i]]);
        return ans;
    }
};

char s[N];
int n;
int node[N],f[N][11];
int main(){
    scanf("%s",s+1); 
    n = strlen(s+1);
    SAM::init(n);
    for (int i = 0; i < 10; ++i)
        f[n+1][i] = n+1;
    node[n+1] = 1;
    for (int i = n; i; --i){
        int c =s[i] - 'a';
        int mn = n+1;
        for (int j = c; j < 10; ++j)
            mn = min(mn, f[i+1][j]);
        int tmp = node[mn];
        for (int j = mn; j > i; --j){
            tmp = SAM::Build(c, tmp);
        }
        node[i] = tmp;
        for (int j = 0; j < 10; ++j)
            f[i][j] = f[i+1][j];
        f[i][c] = i;
    }

    ll ans = SAM::solve();
    printf("%lld\n",ans);
    return 0;
}