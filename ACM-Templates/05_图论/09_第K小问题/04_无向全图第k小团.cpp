// ---
// 题意：给出一个 $n$ 个点的无向图，每个点都有一个权重，求出图中第 $k$ 小的团，找不到输出 $-1$。团的定义为完全图。$(1\leq n\leq 100,1\leq k\leq 10^6)$\\
// \\
// 思路：此类的问题的思路比较统一，先定一个状态，然后再将这个状态丢入堆中，去更新其它状态。\\
// \\
// 我们定状态为团的权重，并用 $bitset$ 记录团中所有出现的点，并记录每个团中编号最大的节点。\\
// \\
// 我们更新时每次选一个编号比当前团中任意节点都大的一个节点加入团中。判断一个点是否可以加入团，只需判断这个点的邻接矩阵和记录团中点的 $bitset$ 进行与运算，如果最终结果仍然是团中点的 $bitset$，则证明当前点和团中所有点都有连边，因此加入这个点仍可以构成一个团。\\
// ---
#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 100+10;
const db EPS = 1e-9;
using namespace std;

char s[N];
int n,k;
ll w[N];
bitset<105> mp[N];
struct Node{
    ll w; int id;
    bitset<105> vis;
    Node() {w = id = 0;vis.reset();}
    bool operator < (Node xx) const {
        return w > xx.w;
    }
};

priority_queue<Node> q;

int main()
{
    scanf("%d%d",&n,&k);
    rep(i,1,n) scanf("%lld",&w[i]);
    rep(i,1,n){
        scanf("%s",s+1);
        rep(j,1,n)
            if(s[j] == '1') mp[i].set(j);
    }
    while(q.size()) q.pop();
    Node empty; ll ans = 0;
    q.push(empty);
    while(q.size()){
        Node x = q.top(); q.pop();
        k--;
        if(k == 0) {ans = x.w; break;}
        rep(i,x.id+1,n)
            if(x.vis[i] == 0 && (x.vis&mp[i]) == x.vis){
                Node nw = x;
                nw.w += w[i];
                nw.vis[i] = 1;
                nw.id = i;
                q.push(nw);
            }
    }
    if(k != 0) printf("-1\n");
    else printf("%lld\n",ans);
    return 0;   
}