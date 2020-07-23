// ---
// 一、定义 \\
// 1. 设数集$T$为一组数任意子集异或得到的集合。\\
// 2. $T$的线性基是$T$的一个子集 $A=$ { $a_1,a_2,a_3,...,a_n$}。$A$中元素互相$xor$所形成的异或集合，等价于原数集$T$的元素互相$xor$形成的异或集合。可以理解为将原数集进行了压缩。\\
// \\
// 二、性质 \\
// 1. 线性基没有异或为$0$的子集。\\
// 2. 线性基的异或集合中每个元素的异或方案唯一，其实这个跟性质1是等价的。\\ 
// 3. 线性基二进制最高位互不相同。\\ 
// 4. 线性基中元素互相异或，异或集合不变。\\ 
// \\
// 三. 求$x$在$Q$中的排名 (允许重复) \\
// 1. 共有 $cnt$ 个基底, 则每个数出现的次数均为 $2^{n-cnt}-1$。 因为没被选入基底的数有 $n-cnt$ 个, 其能用基底唯一表示, 再把基底对应选中异或即为 $0$ 向量, 因此共有 $2^{n-cnt}-1$ 个零向量。\\
// 2. $x$由第 $a_0,…,a_i,…,a_m$ 个向量异或而来, 没有参与的向量置 $0$, 令 $P=a_0,…,a_i，…,a_m$的二进制表示。因此如果 $0$ 可取, 则 $x$ 排名为$(P-1)*(2^{n-cnt}-1)+1$; 如果 $0$ 不可取, 则 $x$ 排名为P。\\
// \\
// 四. 求 $T$ 集合中第 $i$ 位为 $1$ 的数的个数 $H$ \\
// $T$ 集合大小为 $2^{|Q|}$, 令 $ALL\  |= p[i], i\in [0,62]$。若 $ALL$ 第 $i$ 位为 $1$，则 $H=2^{|Q|-1}$, 否则 $H=0$。\\
// ---
struct Linear_Basis{
	ll p[65],d[65];
	int cnt;
	Linear_Basis() {memset(p,0,sizeof p);}
	~Linear_Basis() {}
	bool ins(ll x){ //向线性基中插入一个数
		for(int i = 62; i >= 0; i--){
			if(x&(1ll<<i)){
				if(!p[i]) {p[i] = x; break;}
				x ^= p[i];
			}
		}
		return x > 0ll;
	}
	ll MAX(ll x){ //求线性空间与ans异或的最大值
		for(int i = 62; i >= 0; i--)
			if((x^p[i]) > x) x ^= p[i];
		return x;
	}
	//如果是求一个数与线性基的异或最小值，则需要先rebuild，再从高位向低位依次进行异或
	ll MIN(){
		rep(i,0,62)
			if(p[i]) return p[i];
	}
	//将线性基改造成每一位相互独立，即对于二进制的某一位i，只有pi的这一位是1，其它都是0
	//注意此处只是避免多个基位上同时有第i位，但是可能会出现p[5]同时有第5位和第3位，但是p[3]为空
	void rebuild(){
		cnt = 0;
		for(int i = 62; i >= 0; i--)
			for(int j = i-1; j >= 0; j--)
				if(p[i]&(1ll<<j))
					p[i]^=p[j];
		rep(i,0,62)
			if(p[i]) d[cnt++] = p[i];
	}
	//求线性基能够组成的数中的第K大
	ll Kth(ll k){
		ll ret = 0;
		if(k >= (1ll<<cnt)) return -1; //k大于子集总数, 找不到
		for(int i = 62; i >= 0; i--)
			if(k&(1ll<<i)) ret ^= d[i];
		return ret;
	}
	//合并两个线性基
	Linear_Basis& merge(const Linear_Basis &xx){
		for(int i = 62; i >= 0; i--)
			if(xx.p[i]) ins(xx.p[i]);
		return *this;
	}
}LB;

//两个线性基求交
Linear_Basis merge(Linear_Basis a, Linear_Basis b){
	Linear_Basis A = a, tmp = a, ans; //tmp不断构建A+(B\ans)
	ll cur,d;
	rep(i,0,33) //从低到高，使得不存在一个基底可以仅由(tmp\A)表示
		if(b.p[i]){ //b中有这个基底
			cur = 0, d = b.p[i];
			per(j,i,0)
				if((d>>j)&1){
					if(tmp.p[j]){
						d ^= tmp.p[j], cur ^= A.p[j];
						if(d) continue;
						ans.p[i] = cur; //cur的第i位不为0
					}
					else tmp.p[j] = d, A.p[j] = cur;
					//如果不能被表示, A的赋值是为了让高位中含有j这位的基底下放到A中j的位置
					break;
				}
		}
	return ans;
}