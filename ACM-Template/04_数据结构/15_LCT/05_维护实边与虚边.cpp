// ---
// 题意:\\ 
// $n$ 个点，一共 $q$ 次操作。一共有两种操作类型，$A \ x \ y$ 表示连通 $(x,y)$，保证操作合法，且始终是棵森林。$Q\ x\ y$ 表示查询去除 $(x,y)$ 边之后，$x$ 所在树的节点数 $*$ $y$ 所在树的节点数。$(1\leq n,q\leq 10^5)$\\
// \\
// 思路:\\ 
// 我们一般遇到的都是维护链上节点个数的问题，而此题要求这颗树上的节点个数，因此我们需要同时维护虚边和实边的信息。\\
// \\
// 我们令 $sz[x]$ 表示节点 $x$ 子树中节点个数，$sz2[x]$ 表示节点 $x$ 虚儿子的节点个数和。因此 $sz[x]=sz[ls]+sz[rs]+1+sz2[x]$，而这也正是 $pushUp$ 函数。\\
// \\
// 因此我们只需要维护 $sz2[x]$ 即可，然后观察哪些函数会改变 $sz2[x]$ 的值，不难发现，只有 $makeRoot$、$access$、$link$、$cut$ 会改变边的虚实关系，其中 $makeRoot$ 主要修改在于调用了 $access$ 函数，而 $cut$ 只是删除实边不会修改虚边，因此真正关键的函数即为 $link$ 和 $access$ 函数，具体操作见代码，不难思考。\\
// \\
// 这里主要讲解 $link$ 中修改 $sz2[y]$ 信息时为什么需要将节点 $y$ $makeRoot(y)$，原因在于单点修改之后，其祖先节点维护的信息都会发生变化，因此一般的问题需要 $splay(y)$，因为一般问题只需要维护实链信息。然后在该问题中还维护了虚链信息，因此需要 $makeRoot(y)$ 而不是 $splay(y)$。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
typedef long long ll;
const int N = 100010;
int n, q;

struct LCT{
  #define ls ch[p][0]
  #define rs ch[p][1]
  #define Get(p) (ch[f[p]][1] == p)
  int ch[N][2], f[N], siz[N], siz2[N], rev[N];

  inline void clear(int p){ //清除这个点的信息
    ch[p][0] = ch[p][1] = f[p] = siz[p] = siz2[p] = rev[p] = 0;
  }

  inline int isRoot(int p){
    clear(0);
    return ch[f[p]][0] != p && ch[f[p]][1] != p;
  }

  inline void pushUp(int p){
    clear(0);
    siz[p] = siz[ls] + 1 + siz[rs] + siz2[p];
  }

  inline void pushDown(int p){
    clear(0);
    if(rev[p]){ 
      if(ls) rev[ls] ^= 1, swap(ch[ls][0],ch[ls][1]);
      if(rs) rev[rs] ^= 1, swap(ch[rs][0],ch[rs][1]);
      rev[p] = 0;
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
    int p; //每次改变右儿子的值，因为整棵树是中序遍历，放入右儿子才能保证先遍历父亲再遍历儿子
    for(p = 0; x; p = x, x = f[x]){
      splay(x), siz2[x] += siz[ch[x][1]]-siz[p], ch[x][1] = p, pushUp(x);
    }
    return p;
  }

  inline void makeRoot(int p){ //使x点成为整棵树的根
    access(p); splay(p);
    swap(ch[p][0],ch[p][1]); //把整条链反向
    rev[p] ^= 1;
  }

  inline void link(int x,int y){ //在x、y两点间连一条边
    //makeRoot(x)的作用是使得x无父亲
    //makeRoot(y)的作用是使得y无父亲，因此可以修改y的信息，不用去更新y的祖先
    makeRoot(x), makeRoot(y), f[x] = y, siz2[y] += siz[x]; pushUp(y);
  }

  inline void cut(int x,int p){ //把x、y两点间边删掉，此处删除的是实边，注意实边和虚边的区别
    makeRoot(x), access(p), splay(p);
    if (ls == x && !rs) ls = f[x] = 0;
  }

  inline int find(int p){ //找到x所在树的根节点编号
    access(p), splay(p);
    while(ls) pushDown(p), p = ls;
    return p;
  }
}st;

signed main() {
  scanf("%d%d", &n, &q);
  rep(i,1,n) st.siz[i] = 1;
  while (q--) {
    char op[10]; int x,y; scanf("%s%d%d",op,&x,&y);
    if(op[0] == 'A') st.link(x,y);
    else{
      st.cut(x,y);
      st.makeRoot(x); st.splay(x);
      int a1 = st.siz[x];
      st.makeRoot(y); st.splay(y);
      int a2 = st.siz[y];
      st.link(x,y);
      printf("%lld\n",(ll)a1*(ll)a2);
    }
  }
  return 0;
}