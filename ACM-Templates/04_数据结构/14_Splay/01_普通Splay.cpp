#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 1e6+100;
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;

int f[N],cnt[N],ch[N][2],siz[N],key[N],sz,rt;
//f[x]-x的父节点, ch[x][0]表示x的左儿子, ch[x][1]表示x的右儿子, key[x]表示x的关键点,
//cnt[x]表示x节点关键字出现的次数(权值), siz[x]表示包括x的这个子树的大小, sz为整棵树的大小, rt为整棵树的根
//空间大小为所有插入点个数, 点被删除之后空间不能复用

void init(){
	memset(f,0,sizeof f); memset(cnt,0,sizeof cnt);
	memset(ch,0,sizeof ch); memset(siz,0,sizeof siz);
	memset(key,0,sizeof key); sz = rt = 0;

}
void clear(int x) { f[x] = cnt[x] = ch[x][0] = ch[x][1] = siz[x] = key[x] = 0; }
bool get(int x) { return ch[f[x]][1] == x;}
void push_up(int x) { siz[x] = siz[ch[x][0]]+siz[ch[x][1]]+cnt[x]; } //更新节点信息, 此处为siz
void rotate(int x){
	int old = f[x], oldf = f[old], which = get(x);
	ch[old][which] = ch[x][which^1]; f[ch[old][which]] = old; //把儿子过继给爸爸, 同时处理父子两个方向上的信息
	ch[x][which^1] = old; f[old] = x; //我给我爸爸当爹, 我爸爸管我叫爸爸
	f[x] = oldf; //我的爷爷成了我的爸爸
	if(oldf) ch[oldf][ch[oldf][1]==old] = x;
	push_up(old); push_up(x); //分别维护信息
}
void splay(int x){ //将x旋为根
	for(int fa; (fa = f[x]); rotate(x))
		if(f[fa]) rotate((get(x) == get(fa))?fa:x); //如果祖父三代连成一线, 则旋转父亲, 否则旋转自己
	rt = x;
}

void insert(int x){ //x为权值
	if(rt == 0) {
		key[++sz] = x, rt = sz;
		cnt[sz] = siz[sz] = 1; f[sz] = ch[sz][0] = ch[sz][1] = 0;
		return;
	} //空树
	int now = rt, fa = 0;
	while(1){
		if(x == key[now]){ //这个数出现过
			cnt[now]++, push_up(now), push_up(fa), splay(now); return;
		}
		fa = now, now = ch[now][key[now]<x];
		if(now == 0){
			++sz; siz[sz] = cnt[sz] = 1;
			ch[sz][0] = ch[sz][1] = 0;
			ch[fa][x>key[fa]] = sz; //根据加入点顺序重新编号
			f[sz] = fa, key[sz] = x, push_up(fa), splay(sz); return;
		}
	}
}

int rnk(int x){ //查询x的排名
	int now = rt, ans = 0;
	while(now){
		if(x < key[now]) now = ch[now][0];
		else{
			ans += siz[ch[now][0]];
			if(x == key[now]) { splay(now); return (ans+1);} //x在树中节点的位置
			ans += cnt[now], now = ch[now][1]; //到达右孩子处
		}
	}
	return -1; //找不到
}
int kth(int x){ //查询排名为x的数
	int now = rt;
	while(1){
		if(ch[now][0] && x <= siz[ch[now][0]]) now = ch[now][0];
		else{
			int tmp = siz[ch[now][0]]+cnt[now];
			if(x <= tmp) return key[now];
			x -= tmp, now = ch[now][1];
		}
	}
}
int pre(){ //进行splay后, x已经在根节点了, 因此只用找左子树最大节点即可, 返回节点编号
	int now = ch[rt][0];
	while(ch[now][1]) now = ch[now][1];
	return now;
}
int next(){ //找右子树最小
	int now = ch[rt][1];
	while(ch[now][0]) now = ch[now][0];
	return now;
}

void del(int x){
	rnk(x); //把x对应节点转到了根
	if(cnt[rt] > 1) {cnt[rt]--; push_up(rt); return;} //有多个相同的数
	if(!ch[rt][0] && !ch[rt][1]) {clear(rt); rt = 0; return;}
	if(!ch[rt][0] || !ch[rt][1]){ //只有一个儿子
		int oldrt = rt; rt = ch[rt][1]+ch[rt][0]; f[rt] = 0; clear(oldrt); return;
	}
	int oldrt = rt, leftbig = pre();
	splay(leftbig);
	ch[rt][1] = ch[oldrt][1];
	f[ch[oldrt][1]] = rt;
	clear(oldrt);
	push_up(rt);
}

int main()
{
    int m; scanf("%d",&m);
	rep(i,1,m){
		// LOG1("i",i);
		int op,x; scanf("%d%d",&op,&x);
		if(op == 1) insert(x);
		else if(op == 2) del(x);
		else if(op == 3) printf("%d\n",rnk(x));
		else if(op == 4) printf("%d\n",kth(x));
		else if(op == 5){
			insert(x);
			printf("%d\n",key[pre()]);
			del(x);
		}
		else if(op == 6){
			insert(x);
			printf("%d\n",key[next()]);	
			del(x);
		}
	}
	return 0;
}