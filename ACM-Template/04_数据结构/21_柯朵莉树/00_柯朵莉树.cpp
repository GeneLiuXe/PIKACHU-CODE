// ---
// 柯朵莉树应用范围：\\
// 一般适用于有区间赋值操作的题，且不能出现大量的修改操作，否则会时间爆炸\\
// ---
#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
#define IT set<Node>::iterator
typedef long long ll;
typedef double db;
const int N = 1e5+100;
const db EPS = 1e-9;
using namespace std;

struct Node{
	//节点含义是区间为[l,r]，区间中每个点大小均为v
	int l,r;
	mutable ll v; //否则无法直接修改set中的值
	Node(int L,int R = -1, ll V = 0):l(L),r(R),v(V) {}
	bool operator < (const Node& o) const {
		return l < o.l;
	}
};
set<Node> s;

ll pow_mod(ll a,ll b,ll mod){
	ll base = a%mod, ans = 1;
	while(b){
		if(b&1) ans = (ans*base)%mod;
		base = (base*base)%mod;
		b >>= 1;
	}
	return ans;
}

//分裂区间，返回区间左端点为pos的节点
IT split(int pos){
	IT it = s.lower_bound(Node(pos));
	if(it != s.end() && it->l == pos) return it;
	--it;
	int L = it->l, R = it->r;
	ll V = it->v;
	s.erase(it);
	s.insert(Node(L,pos-1,V));
	//pair<iterator,bool> insert (const value_type& val)
	return s.insert(Node(pos,R,V)).first;
}

//推平操作，合并set，令区间[l,r]中所有数等于val
void assign_val(int l,int r,ll val){
	IT itl = split(l), itr = split(r+1);
	s.erase(itl,itr);
	s.insert(Node(l,r,val));
}

//区间[l,r]中每个数加val
void add(int l,int r,ll val){
	IT itl = split(l), itr = split(r+1);
	for(; itl != itr; ++itl)
		itl->v += val;
}

//区间[l,r]中第k大
ll ranks(int l,int r,int k){
	vector<pair<ll,int> > vp; vp.clear();
	IT itl = split(l), itr = split(r+1);
	for(; itl != itr; ++itl)
		vp.push_back(pair<ll,int>(itl->v,itl->r - itl->l + 1));
	sort(vp.begin(),vp.end());
	for(vector<pair<ll,int> >::iterator it = vp.begin(); it != vp.end(); it++){
		k -= it->second;
		if(k <= 0) return it->first;
	}
}

//区间[l,r]x次方和
ll sum(int l,int r,int ex,ll mod){
	IT itl = split(l), itr = split(r+1);
	ll res = 0;
	for(; itl != itr; itl++)
		res = (res+(ll)(itl->r - itl->l + 1ll) * pow_mod(itl->v,(ll)ex,(ll)mod))%mod;
	return res;
}

int n,m;
ll seed,a[N],vmax;

ll rd(){
	ll ret = seed;
	seed = (seed*7ll+13ll)%((ll)1000000007);
	return ret;
}

int main()
{
	scanf("%d%d%lld%lld",&n,&m,&seed,&vmax);
	rep(i,1,n){
		a[i] = (rd()%vmax)+1ll;
		s.insert(Node(i,i,a[i]));
	}
	s.insert(Node(n+1,n+1,0)); //要在末尾添加一个节点
	rep(i,1,m){
		int op = int(rd()%4)+1;
		int l = int(rd()%n)+1;
		int r = int(rd()%n)+1;
		if(l > r) swap(l,r);
		int x,y;
		if(op == 3) x = int(rd()%(r-l+1))+1;
		else x = int(rd()%vmax)+1;
		if(op == 4)
			y = int(rd()%vmax)+1;
		//上方均为此题读入

		if(op == 1) add(l,r,(ll)x); //区间[l,r]加上x
		else if(op == 2) assign_val(l,r,(ll)x); //区间[l,r]所有数改成x
		else if(op == 3) printf("%lld\n",ranks(l,r,x)); //区间[l,r]中第k大数
		else printf("%lld\n",sum(l,r,x,y)); //区间[l,r]每个数字的x次方和%mod
	}
	return 0;
}