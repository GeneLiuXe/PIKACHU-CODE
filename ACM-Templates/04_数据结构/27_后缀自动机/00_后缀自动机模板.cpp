#include<bits/stdc++.h>
using namespace std;
const int N = 1e6+10;
const int M = N << 1;
typedef long long ll;

namespace SAM{
    int cur,node,last,n;
    int ch[M][27],fa[M],len[M],size[M];
    ll ans;
    void init(int _n){
        cur = 1, node = 1;  n = _n;
    }
    void Build(int c, int id){
        last = cur; cur = ++ node; //node 是总节点数。cur是当前节点，last 是上一个节点。
        int p = last; len[cur] = len[last] + 1;
        for (; p && !ch[p][c]; p = fa[p]) ch[p][c] = cur;
        //这个是 next 指针。沿着 fail 指针一直向前找。
        if (!p) fa[cur] = 1; else { //如果找到根节点了。就直接 连接 fail 指针，
            int q = ch[p][c];
            if (len[q] == len[p] + 1) fa[cur] = q; else{ //满足条件，连接fail指针.
                int nt = ++node; len[nt] = len[p] + 1; //不满足条件，新加一个节点。
                memcpy(ch[nt],ch[q],sizeof(ch[q])); // 把 q 节点的情况 copy 给新加的 节点。
                fa[nt] = fa[q]; fa[q] = fa[cur] = nt;
                for (; ch[p][c] == q; p = fa[p]) ch[p][c] = nt;
            }
        }
        size[cur] = 1;
    }
    int c[N],sa[N<<1];
    ll flower(){  // 模拟 parent 树上的 DFS.
        for (int i = 1; i <= node; ++i) c[len[i]]++;
        for (int i = 1; i <= n; ++i) c[i] += c[i-1];
        for (int i = node; i > 0; --i) sa[c[len[i]]--] = i; //相当于 后缀数组的桶排序。
        for (int i = node; i; --i){ //按照长度排序,排名为 i 的字符串的节点是谁.
            int p = sa[i];
            if (size[p] > 1) ans = max(ans,1ll*size[p]*len[p]); //找 乘积最大。长度和出现次数。
            size[fa[p]] += size[p];
        }
        return ans;
    }

};

char s[N];
int n;
int main(){
    scanf("%s",s+1); 
    n = strlen(s+1);
    SAM::init(n);
    for (int i = 1; i <= n; i++) SAM::Build(s[i] - 'a',i);
    printf("%lld\n",SAM::flower());
    return 0;
}