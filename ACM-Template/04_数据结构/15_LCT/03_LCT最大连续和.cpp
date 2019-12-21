// ---
// 题意:\\ 
// $n$ 个点一棵树，每个节点有一个值，支持两种操作。$(1\leq n,q\leq 10^5)$\\
// $1$ $a$ $b$，查询树中点 $a$ 到点 $b$ 的路径中最大连续和\\
// $2$ $a$ $b$ $c$，将树中点 $a$ 到点 $b$ 路径中所有点的值改为 $c$\\
// \\
// 思路:\\ 
// 对每一个点维护一个 $lc[i]$、$rc[i]$、$maxn[i]$ 表示点 $i$ 子树中左连续的最大值、右连续的最大值以及整棵子树中的最大连续值。\\
// \\
// 需要注意一点，交换左右儿子的时候，还需要把每个节点的 $lc$ 和 $rc$ 进行交换，其余细节见代码。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
typedef long long ll;
const ll inf = 1e9+100;
const int N = 1e5+10;
int n,Q;

struct LCT{
  #define ls ch[p][0]
  #define rs ch[p][1]
  #define Get(p) (ch[f[p]][1] == p)
  int ch[N][2], f[N];
  ll maxn[N], sum[N], lc[N], rc[N], val[N], siz[N], lazy[N];
  bool rev[N];

  inline void clear(int p){ //清除这个点的信息
    ch[p][0] = ch[p][1] = f[p] = val[p] = maxn[p] = lc[p] = sum[p] = rc[p] = siz[p] = 0;
  }

  inline int isRoot(int p){
    return ch[f[p]][0] != p && ch[f[p]][1] != p;
  }

  inline void pushUp(int p){
    siz[p] = siz[ls] + 1 + siz[rs];
    sum[p] = val[p] + sum[ls] + sum[rs]; //ls、rs可能为0
    maxn[p] = max(maxn[ls],max(maxn[rs],rc[ls]+lc[rs]+val[p]));
    lc[p] = max(lc[ls],sum[ls]+val[p]+lc[rs]);
    rc[p] = max(rc[rs],sum[rs]+val[p]+rc[ls]);
  }

  inline void pushDown(int p){
    if(rev[p]){ 
      if(ls) rev[ls] ^= 1, swap(ch[ls][0],ch[ls][1]);
      if(rs) rev[rs] ^= 1, swap(ch[rs][0],ch[rs][1]);
      swap(lc[ls],rc[ls]); swap(lc[rs],rc[rs]); //交换左右儿子时还要交换左右连续最大值
      rev[p] = 0;
    }
    if(lazy[p] != -inf){
      if(ls){
        sum[ls] = siz[ls]*lazy[p]; 
        val[ls] = lazy[ls] = lazy[p];
        lc[ls] = rc[ls] = maxn[ls] = lazy[p] > 0 ? sum[ls]:0;
      }
      if(rs){
        sum[rs] = siz[rs]*lazy[p]; 
        val[rs] = lazy[rs] = lazy[p];
        lc[rs] = rc[rs] = maxn[rs] = lazy[p] > 0 ? sum[rs]:0;
      }
      lazy[p] = -inf; 
    }
  }

  void update(int p){ //递归地从上到下pushDown信息
    if(!isRoot(p)) update(f[p]);
    pushDown(p);
  }

  inline void rotate(int x){ //将x向上旋转一层的操作
    int y = f[x], z = f[y], k = Get(x);
    if(!isRoot(y)) ch[z][ch[z][1] == y] = x;
    ch[y][k] = ch[x][!k], f[ch[y][k]] = y;
    ch[x][!k] = y, f[y] = x, f[x] = z;
    pushUp(y); //要先pushUp(y)
    pushUp(x);
  }

  inline void splay(int x){ //把x旋转到当前splay的根
    update(x); //将上面的标记完全下放
    for(int fa; fa = f[x], !isRoot(x); rotate(x)){
      if(!isRoot(fa)) rotate(Get(fa) == Get(x) ? fa : x);
    }
  }

  inline int access(int x){ //把从根到x的所有点放在一条实链里, 返回这个splay的根
    int p;
    for(p = 0; x; p = x, x = f[x]){
      splay(x), ch[x][1] = p, pushUp(x);
    }
    return p;
  }

  inline void makeRoot(int p){ //使x点成为整棵树的根
    access(p);
    splay(p);
    swap(ch[p][0],ch[p][1]); //把整条链反向
    rev[p] ^= 1;
  }

  inline void link(int x,int y){ //在x、y两点间连一条边
    makeRoot(x), f[x] = y; //dfs建树, 每条边都是有效的, 因此不需要判断是否有效
  }
}st;

int main(){
  scanf("%d",&n);
  rep(i,1,n){
    scanf("%lld",&st.val[i]);
    st.siz[i] = 1; st.sum[i] = st.val[i];
    st.lazy[i] = -inf;
    st.lc[i] = st.rc[i] = st.maxn[i] = st.val[i] > 0 ? st.val[i]:0;
  }
  rep(i,1,n-1){
    int a,b; scanf("%d%d",&a,&b);
    st.link(a,b);
  }
  scanf("%d",&Q);
  while(Q--){ 
    int op; scanf("%d",&op);
    if(op == 1){ //a->b max
      int a,b; scanf("%d%d",&a,&b);
      st.makeRoot(a);
      b = st.access(b);
      printf("%lld\n",st.maxn[b]);
    }
    else{ //a->b to c
      int a,b; ll c; scanf("%d%d%lld",&a,&b,&c);
      st.makeRoot(a);
      b = st.access(b);
      st.val[b] = st.lazy[b] = c;
      st.sum[b] = st.siz[b]*c;
      st.lc[b] = st.rc[b] = st.maxn[b] = c > 0 ? st.sum[b]:0;
    }
  }
  return 0;
}