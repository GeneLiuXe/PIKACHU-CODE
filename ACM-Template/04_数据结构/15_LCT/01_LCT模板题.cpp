// ---
// 一、适用问题: \\
// 动态树主要用于解决操作中带有加边、删边、换根的一系列问题，即树结构发生变化的问题，理论上来说，树链剖分的问题都能用 $LCT$ 进行解决。\\
// \\
// 二、LCT 函数解析 \\
// $LCT$ 本质是上对树进行实链剖分，实链剖分的意思就是将一棵树分成多条链，链中的边称为实边，链与链之间的边则称为虚边，每条链都是一个 $splay$，在 $splay$ 中进行中序遍历即可还原原来的树结构。而 $LCT$ 就是不断进行虚边、实边转换的一个算法。\\
// \\
// $LCT$ 中一共有 $clear$、$pushUp$、$pushDown$、$update$、$rotate$、$splay$、$access$、$makeRoot$、$link$、$cut$、$find$、$split$ 等函数，下面大致介绍一下每个函数的具体作用以及一些坑点，更多的是提纲挈领的作用，想要从最基础的地方开始学的话，推荐 [oiwiki](https://oi-wiki.org/ds/lct/)。\\
// \\
// (1) 简单函数（仅操作单个 $splay$ 的函数）\\
// 1. $clear(x)$：清除一个点的信息，如父亲、左右儿子、标记、维护信息等信息。\\
// 2. $pushUp(x)$：由左右儿子的信息更新父节点的信息，与线段树的 $pushUp()$ 函数没有太大差别。\\
// 3. $pushDown(x)$：将当前节点的标记下放到儿子节点，如加、减、翻转等标记。\\
// 4. $update(x)$：一直递归到根节点，然后把标记信息不断下放，没有涉及任何虚实边的转换。\\
// ---
 void update(int p){ //递归地从上到下pushDown信息
   if(!isRoot(p)) update(f[p]);
   pushDown(p);
 }
// ---
// 5. $rotate(x)$：将当前节点向上旋转一层，可以自己模拟一下。此处改变了 $splay$ 的内部结构，即子节点发生了改变，因此需要进行 $pushUp$，但是仍然没有进行任何虚实边的转换。\\
// ---
 inline void rotate(int x){ //将x向上旋转一层的操作
   int y = f[x], z = f[y], k = Get(x);
   if(!isRoot(y)) ch[z][ch[z][1] == y] = x;
   ch[y][k] = ch[x][!k], f[ch[y][k]] = y;
   ch[x][!k] = y, f[y] = x, f[x] = z;
   pushUp(y); //要先pushUp(y)
   pushUp(x);
 }
// ---
// 6. $splay(x)$：将当前点旋转到 $splay$ 的根节点，$splay$ 到根节点作用在于不需要在向上进行更新。比如你现在要修改 $x$ 的点权，但是每个节点还要维护子树 $sum$ 的信息，如果 $x$ 不是其所在 $splay$ 的根节点，那么修改 $x$ 的点权势必影响到其祖先节点的 $sum$ 信息，因此需要将 $x$ 旋转为其所在 $splay$ 的根后再进行单点修改。注意 $splay$ 函数也没有进行实边和虚边的转换。\\
// ---
 inline void splay(int x){ //把x旋转到当前splay的根
   update(x); //将上面的标记完全下放
   for(int fa; fa = f[x], !isRoot(x); rotate(x)){
     if(!isRoot(fa)) rotate(Get(fa) == Get(x) ? fa : x);
   }
 }
// ---
// 以上函数都属于 $LCT$ 函数中的简单函数，因为这些函数都只是在单个 $splay$ 中进行操作，不涉及任何虚实边的转换。\\
// \\
// (2) 复杂函数（涉及多个 $splay$ 的操作，进行虚实边转换）\\
// 1. $access(x)$：将点 $x$ 到根的路径经过的点放入同一个 $splay$ 中，且这个 $splay$ 中仅包含从 $x$ 到根路径上经过的点。具体操作即是将 $x$ 点不断转成其所在 $splay$ 的根，然后再进行虚实边转换一直到根。此处 $access$ 函数有返回值，返回值为最后构成的 $splay$ 的根节点。\\
// ---
inline int access(int x){ //把从根到x的所有点放在一条实链里, 返回这个splay的根
    int p; //每次改变右儿子的值，因为整棵树是中序遍历，放入右儿子才能保证先遍历父亲再遍历儿子
    for(p = 0; x; p = x, x = f[x]){
      splay(x), ch[x][1] = p, pushUp(x);
    }
    return p;
}
// ---
// 2. $makeRoot(x)$：换根操作，将点 $x$ 变成当前树的根。具体过程为先 $access$ 点 $x$，然后再将点 $x$ 旋转为其 $splay$ 所在根，然后将所有节点的左右儿子翻转即可。\\
// ---
inline void makeRoot(int p){ //使x点成为整棵树的根
    access(p); splay(p);
    swap(ch[p][0],ch[p][1]); //把整条链反向
    rev[p] ^= 1;
}
// ---
// 3. $split(x,y)$：从树中拎出 $x\rightarrow y$ 的路径，返回该路径的 $splay$ 根节点，可以查询路径最大值、点权和、边权和等信息。\\
// ---
inline int split(int x,int y){
	makeRoot(x); 
	return access(y);
} 
// ---
// 4. $find(x)$：即返回点 $x$ 所在树的根节点，不是所在 $splay$ 中的根节点，用于判断两点是否连通。\\
// ---
 inline int find(int p){ //找到x所在树的根节点编号
   access(p), splay(p);
   while(ls) pushDown(p), p = ls;
   return p;
 }
// ---
// 5. $link(x,y)$：连接树中 $x$、$y$ 两点之间的边，无边变虚边，如果题目中没有保证操作一定合法，则需要自行判断 $x$、$y$ 是否已经连通。\\
// ---
inline void link(int x,int y){ //在x、y两点间连一条边，连接了虚边
   if (find(x) != find(y)) makeRoot(x), f[x] = y;
 }
// ---
// 6. $cut(x,y)$：断开树中 $x$、$y$ 两点之间的实边，两个点同时断开即可。\\
// ---
inline void cut(int x,int p){ //把x、y两点间边删掉，此处删除的是实边，注意实边和虚边的区别
   makeRoot(x), access(p), splay(p);
   if (ls == x && !rs) ls = f[x] = 0;
 }
// ---
// 三、具体细节\\
// \\
// (1) 单点修改\\
// 由于 $LCT$ 中维护了多个 $splay$，因此单点修改需要把该点修改的信息不断上传，所以我们需要先将点 $x$ 旋转到 $splay$ 的根或者整棵树的根，然后再进行单点修改。\\
// \\
// 如果题中只需要维护实链信息，则只需要旋转到 $splay$ 的根，如果需要同时维护实链和虚链信息，即整棵子树的信息的话，则需要令该点为树根，即调用 $makeRoot()$ 函数。\\
// \\
// (2) 维护边权\\
// 由于 $LCT$ 是不断地进行虚边、实边转换，因此没有固定的边结构，所以直接维护边权十分困难，因此我们将边转成点，边 $(a,b)$ 成为一个点 $x$，$link(a,x)$、$link(x,b)$ 即可。\\
// \\
// (3) 维护子树信息\\
// 普通 $LCT$ 只能维护具有可减性的子树信息，比如子树大小，子树贡献等，而子树 $max$、$min$ 等问题则不具有可减性，难以维护。\\
// \\
// 维护子树信息主要在于维护实边信息和虚边信息，而进行实虚转换的函数只有 $makeRoot()$、$access()$、$link()$ 三个函数，只需要在该三个函数进行一定的修改即可，下面习题中包含了该问题可供参考。\\
// \\
// 四、例题 \\
// \\
// 题意:\\
// $n$ 个点一棵树，支持四种操作。$(1\leq n,q\leq 10^5,0\leq c\leq 10^4)$\\
// 1. $+\  u \ v \ c$，将 $u$ 到 $v$ 的路径上的点的权值都加上 $c$。\\
// 2. $-\  u_1 \ v_1 \ u_2\ v_2$，将树中原有的边 $(u_1,v_1)$ 删除，加入一条新边 $(u_2,v_2)$，保证操作完之后仍然是一颗树。\\
// 3. \ $*\   u \ v \ c$，将 $u$ 到 $v$ 的路径上的点的权值都乘上 $c$。\\
// 4. / $u \ v$，询问 $u$ 到 $v$ 的路径上的点的权值和，答案 $mod\ 51061$。\\
// \\
// 思路: \\
// 三个涉及到路径的操作，都是先把 $u$ 变成树根，然后 $access(v)$，即拉起一条 $u$ 到 $v$ 的路径，使得 $u$ 到 $v$ 路径上的点都在一个 $splay$ 中，然后获得这个 $splay$ 的根节点，即可对根节点打标记完成。\\
// \\
// 删边则是令 $u$ 为根，拉起 $u$ 到 $v$ 的路径，将 $v$ 旋转成 $splay$ 的根，然后儿子与父亲双向断开联系。加边则是令 $u$ 为根，然后使 $u$ 的父亲变成 $v$。\\
// \\
// 总结:\\
// 这题应该算是 $LCT$ 的模板题，涉及的操作都是基础操作，没有太多思维上的难点。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
#define int long long
const int N = 100010;
const int mod = 51061;
int n, q, u, v, c;
char op;

struct LCT{
  #define ls ch[p][0]
  #define rs ch[p][1]
  #define Get(p) (ch[f[p]][1] == p)
  int ch[N][2], f[N], sum[N], val[N], siz[N], rev[N], add[N], mul[N];

  inline void clear(int p){ //清除这个点的信息
    ch[p][0] = ch[p][1] = f[p] = siz[p] = val[p] = sum[p] = rev[p] = add[p] = 0;
    mul[p] = 1;
  }

  inline int isRoot(int p){
    clear(0);
    return ch[f[p]][0] != p && ch[f[p]][1] != p;
  }

  inline void pushUp(int p){
    clear(0);
    siz[p] = (siz[ls] + 1 + siz[rs]) % mod;
    sum[p] = (sum[ls] + val[p] + sum[rs]) % mod;
  }

  inline void pushDown(int p){
    clear(0);
    if(mul[p] != 1){ //乘法
      if(ls){ //左儿子
        mul[ls] = (mul[ls] * mul[p]) % mod;
        val[ls] = (val[ls] * mul[p]) % mod;
        sum[ls] = (sum[ls] * mul[p]) % mod;
        add[ls] = (add[ls] * mul[p]) % mod;
      }
      if(rs){ //右儿子
        mul[rs] = (mul[rs] * mul[p]) % mod;
        val[rs] = (val[rs] * mul[p]) % mod;
        sum[rs] = (sum[rs] * mul[p]) % mod;
        add[rs] = (add[rs] * mul[p]) % mod;
      }
      mul[p] = 1;
    }
    if(add[p]){
      if(ls){
        add[ls] = (add[ls] + add[p]) % mod;
        val[ls] = (val[ls] + add[p]) % mod;
        sum[ls] = (sum[ls] + add[p] * siz[ls] % mod) % mod;
      }
      if(rs){
        add[rs] = (add[rs] + add[p]) % mod;
        val[rs] = (val[rs] + add[p]) % mod;
        sum[rs] = (sum[rs] + add[p] * siz[rs] % mod) % mod;
      }
      add[p] = 0;
    }
    if(rev[p]){ 
      if(ls) rev[ls] ^= 1, swap(ch[ls][0],ch[ls][1]);
      if(rs) rev[rs] ^= 1, swap(ch[rs][0],ch[rs][1]);
      rev[p] = 0;
    }
  }

  void update(int p){ //递归地从上到下pushDown信息
    //没有将实边变成虚边
    if(!isRoot(p)) update(f[p]);
    pushDown(p);
  }

  inline void rotate(int x){ //将x向上旋转一层的操作
    //没有将实边变成虚边
    int y = f[x], z = f[y], k = Get(x);
    if(!isRoot(y)) ch[z][ch[z][1] == y] = x;
    ch[y][k] = ch[x][!k], f[ch[y][k]] = y;
    ch[x][!k] = y, f[y] = x, f[x] = z;
    pushUp(y); //要先pushUp(y)
    pushUp(x);
  }

  inline void splay(int x){ //把x旋转到当前splay的根
    //没有将实边变成虚边
    update(x); //将上面的标记完全下放
    for(int fa; fa = f[x], !isRoot(x); rotate(x)){
      if(!isRoot(fa)) rotate(Get(fa) == Get(x) ? fa : x);
    }
  }

  inline int access(int x){ //把从根到x的所有点放在一条实链里, 返回这个splay的根
    //进行了边的虚实变换
    int p; //每次改变右儿子的值，因为整棵树是中序遍历，放入右儿子才能保证先遍历父亲再遍历儿子
    for(p = 0; x; p = x, x = f[x]){
      splay(x), ch[x][1] = p, pushUp(x);
    }
    return p;
  }

  inline void makeRoot(int p){ //使x点成为整棵树的根
    access(p); splay(p);
    swap(ch[p][0],ch[p][1]); //把整条链反向
    rev[p] ^= 1;
  }

  inline void link(int x,int y){ //在x、y两点间连一条边，连接了虚边
    if (find(x) != find(y)) makeRoot(x), f[x] = y;
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
  //中序遍历即可还原树结构
  void print(int p){
    if(!p) return;
    pushDown(p);
    print(ls);
    printf("%lld ",p);
    print(rs);
  }
}st;

signed main() {
  scanf("%lld%lld", &n, &q);
  for (int i = 1; i <= n; i++) st.val[i] = 1;
  for (int i = 1; i < n; i++) {
    scanf("%lld%lld", &u, &v);
    st.link(u,v);
  }
  while (q--) {
    scanf(" %c%lld%lld", &op, &u, &v);
    if (op == '+') { //+ u v c, u到v的路径上的点权值+c
      scanf("%lld", &c);
      //u变成树根，拉起v到u的链，把v旋到splay的根
      st.makeRoot(u); v = st.access(v);
      st.val[v] = (st.val[v] + c) % mod;
      st.sum[v] = (st.sum[v] + st.siz[v] * c % mod) % mod;
      st.add[v] = (st.add[v] + c) % mod;
    }
    if (op == '-') { //- u1 v1 u2 v2, 删除(u1,v1), 加上(u2,v2)
      st.cut(u,v);
      scanf("%lld%lld", &u, &v);
      st.link(u,v);
    }
    if (op == '*') { //* u v c, u到v的路径乘上c
      scanf("%lld", &c);
      st.makeRoot(u); v = st.access(v);
      st.val[v] = st.val[v] * c % mod;
      st.sum[v] = st.sum[v] * c % mod;
      st.mul[v] = st.mul[v] * c % mod;
    }
    if (op == '/'){ //u v, 询问u到v路径权值和
      st.makeRoot(u); v = st.access(v);
      printf("%lld\n", st.sum[v]);
    }
  }
  return 0;
}