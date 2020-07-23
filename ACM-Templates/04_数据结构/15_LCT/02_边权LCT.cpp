// ---
// 题意:\\
// $n$ 个点一棵树，支持两种操作。$(1\leq n,q\leq 10^4)$ \\
// 1. $\text{CHANGE}$ $i$ $t_i$，将第 $i$ 条边的边权改为 $t_i$。\\
// 2. $\text{QUERY}$ $a$ $b$，查询树中点 $a$ 到点 $b$ 的路径中的边权最大值。\\
// \\
// 思路: \\
// 边权 $LCT$，需要对于每一条边建一个节点，即树中一共有 $2*n-1$ 个节点，每个边节点与上下两个节点连边。\\
// \\
// 建边需要先确定每个节点的边权之后再进行 $link$，因为点修改会对该节点的祖先节点产生影响，需要将该点旋至 $splay$ 端点后才能进行修改。\\
// \\
// 总结: \\
// 总结一下构建 $LCT$ 构建的关键，构建 $LCT$ 需要先对各个顶点赋值然后再进行 $link$ 操作，若是 $link$ 之后再赋值相当于点修改，而点修改需要将点旋为 $splay$ 根之后才能进行更改。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
const int N = 20000+10;
int n,val[N];

struct LCT{
  #define ls ch[p][0]
  #define rs ch[p][1]
  #define Get(p) (ch[f[p]][1] == p)
  int ch[N][2], f[N], maxn[N], val[N], siz[N], rev[N];

  inline void clear(int p){ //清除这个点的信息
    ch[p][0] = ch[p][1] = f[p] = siz[p] = val[p] = maxn[p] = 0;
  }

  inline int isRoot(int p){
    clear(0);
    return ch[f[p]][0] != p && ch[f[p]][1] != p;
  }

  inline void pushUp(int p){
    clear(0);
    siz[p] = siz[ls] + 1 + siz[rs];
    maxn[p] = max(val[p],max(maxn[ls],maxn[rs]));
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
    int p;
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

  inline void link(int x,int y){ //在x、y两点间连一条边
    makeRoot(x), f[x] = y; //dfs建树, 每条边都是有效的, 因此不需要判断是否有效
  }
}st;

int main(){
  int _; scanf("%d",&_);
  while(_--){
    scanf("%d",&n);
    rep(i,0,2*n) st.clear(i);
    rep(i,1,n-1){
      int a,b,c; scanf("%d%d%d",&a,&b,&c);
      st.val[i+n] = c;
      st.link(a,i+n);
      st.link(i+n,b);
    }
    while(1){
      char s[20]; scanf("%s",s);
      if(s[0] == 'D') break;
      else if(s[0] == 'C'){
        int a,b; scanf("%d%d",&a,&b);
        st.splay(a+n); //先转为splay根节点
        st.val[a+n] = b;
      }
      else{
        int a,b; scanf("%d%d",&a,&b);
        st.makeRoot(a);
        b = st.access(b);
        printf("%d\n",st.maxn[b]);
      }
    }
  }
  return 0;
}