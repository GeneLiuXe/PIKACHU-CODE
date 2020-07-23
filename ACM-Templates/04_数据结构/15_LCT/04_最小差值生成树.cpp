// ---
// 题意:\\ 
// $n$ 个点，$m$ 条边的一个无向图，求边权最大值与最小值的差值最小的生成树。$(1\leq n\leq 5*10^4,1\leq m\leq 2*10^5)$\\
// \\
// 思路:\\
// 关于这类特殊生成树问题，一般考虑用 $LCT$ 动态维护树结构然后更新答案。\\
// \\
// 此题也可以这样考虑。将边按边权从小到大排序，如果 $(a,b)$ 两点不连通，则加上该边，如果 $(a,b)$ 两点连通，则将 $a\rightarrow b$ 路径上边权最小的边去除，然后连上当前的边。维护过程不断更新最大值与最小值的差值，不断取 $min$ 即可。\\
// \\
// 因此只需要维护一个边权 $LCT$，并且维护路径最小值以及最小值点的编号，然后动态加边删边即可。还需要对在树中的边打上标记，去除的时候删去标记，用于查找整棵树中的最小边权。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
const int N = 2e5+5e4+100;
const int M = 2e5+10;
int n, m, vis[N], pos = 1, num, ans = 1e9;
struct Edge{
  int a,b,w;
  bool operator < (Edge xx) const {
    return w < xx.w;
  }
}e[N];

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

struct LCT{
  #define ls ch[p][0]
  #define rs ch[p][1]
  #define Get(p) (ch[f[p]][1] == p)
  int ch[N][2], f[N], val[N], minn[N], mpos[N], rev[N];

  inline void clear(int p){ //清除这个点的信息
    ch[p][0] = ch[p][1] = f[p] = val[p] = mpos[p] = minn[p] = 0;
  }

  inline int isRoot(int p){
    return ch[f[p]][0] != p && ch[f[p]][1] != p;
  }

  inline void pushUp(int p){
    minn[p] = val[p]; mpos[p] = p;
    if(ls && minn[ls] < minn[p]) minn[p] = minn[ls], mpos[p] = mpos[ls];
    if(rs && minn[rs] < minn[p]) minn[p] = minn[rs], mpos[p] = mpos[rs];
  }

  inline void pushDown(int p){
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
    int p = 0;
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
    // if (find(x) != find(y)) 
    makeRoot(x), f[x] = y;
  }

  inline void cut(int x,int p){ //把x、y两点间边删掉
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
  scanf("%d%d", &n, &m);
  rep(i,0,n) st.val[i] = st.minn[i] = 1e5;
  rep(i,1,m) scanf("%d%d%d",&e[i].a,&e[i].b,&e[i].w);
  sort(e+1,e+1+m);
  rep(i,1,m) st.val[i+n] = st.minn[i+n] = e[i].w, st.mpos[i+n] = i+n;
  rep(i,1,m){
    if(e[i].a == e[i].b) continue;
    if(st.find(e[i].a) != st.find(e[i].b)){
      st.link(e[i].a,i+n); st.link(i+n,e[i].b);
      num++; vis[i] = 1;
      while(!vis[pos]) pos++;
      if(num == n-1) ans = min(ans,e[i].w-e[pos].w);
    }
    else{
      st.makeRoot(e[i].a);
      int p1 = st.access(e[i].b);
      p1 = st.mpos[p1];
      st.cut(e[p1-n].a,p1); st.cut(p1,e[p1-n].b); vis[p1-n] = 0;
      st.link(e[i].a,i+n); st.link(i+n,e[i].b); vis[i] = 1;
      while(!vis[pos]) pos++;
      if(num == n-1) ans = min(ans,e[i].w-e[pos].w);
    }
  }
  printf("%d\n",ans);
  return 0;
}