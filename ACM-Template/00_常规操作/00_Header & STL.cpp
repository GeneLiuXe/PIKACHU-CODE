#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 1e5+100;
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

/*快速读入、输出*/
namespace fastIO {
    #define BUF_SIZE 10000000        // 预定义缓冲区大小, 读入数据大的话就多开点
    #define LL long long
    //fread->read
    bool IOerror=0;
    inline char nc(){
        static char buf[BUF_SIZE],*p1=buf+BUF_SIZE,*pend=buf+BUF_SIZE;
        if (p1==pend){
            p1=buf; pend=buf+fread(buf,1,BUF_SIZE,stdin);
            if (pend==p1){IOerror=1;return -1;}
        }
        return *p1++;
    }
    inline bool blank(char ch){return ch==' '||ch=='\n'||ch=='\r'||ch=='\t';}
    inline void read(int &x){
        bool sign=0; char ch=nc(); x=0;
        for (;blank(ch);ch=nc());
        if (IOerror)return;
        if (ch=='-')sign=1,ch=nc();
        for (;ch>='0'&&ch<='9';ch=nc())x=x*10+ch-'0';
        if (sign)x=-x;
    }
    inline void read(LL &x){
        bool sign=0; char ch=nc(); x=0;
        for (;blank(ch);ch=nc());
        if (IOerror)return;
        if (ch=='-')sign=1,ch=nc();
        for (;ch>='0'&&ch<='9';ch=nc())x=x*10+ch-'0';
        if (sign)x=-x;
    }
    inline void read(double &x){
        bool sign=0; char ch=nc(); x=0;
        for (;blank(ch);ch=nc());
        if (IOerror)return;
        if (ch=='-')sign=1,ch=nc();
        for (;ch>='0'&&ch<='9';ch=nc())x=x*10+ch-'0';
        if (ch=='.'){
            double tmp=1; ch=nc();
            for (;ch>='0'&&ch<='9';ch=nc())tmp/=10.0,x+=tmp*(ch-'0');
        }
        if (sign)x=-x;
    }
    inline void read(char *s){
        char ch=nc();
        for (;blank(ch);ch=nc());
        if (IOerror)return;
        for (;!blank(ch)&&!IOerror;ch=nc())*s++=ch;
        *s=0;
    }
    inline void read(char &c){
        for (c=nc();blank(c);c=nc());
        if (IOerror){c=-1;return;}
    }
    #undef LL
    #undef BUF_SIZE
};
using namespace fastIO;

inline void write(int x)
{
  static const int maxlen = 100;
  static char s[maxlen];
    if(x < 0) {putchar('-');x=-x;}
  if(!x){putchar('0'); return;}
  int len = 0; for(;x;x/=10) s[len++] = x % 10+'0';
  for(int i = len-1; i >= 0; --i) putchar(s[i]);
}

int n; read(n); wirte(n); printf("\n");
/*------------------------------------------------------------*/

/*疯狂加速*/
#pragma GCC optimize(2)
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
/*------------------------------------------------------------*/

/*__int128快速读入、输出*/
//读入
inline char nc() { 
  static char buf[100000], *p1 = buf, *p2 = buf; 
  return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, 100000, stdin), p1 == p2) ? EOF : *p1++; 
} 
template <typename T> bool rn(T& v) { 
  static char ch; 
  while (ch != EOF && !isdigit(ch)) ch = nc(); 
  if (ch == EOF) return false; 
  for (v = 0; isdigit(ch); ch = nc()) v = v * 10 + ch - '0'; 
  return true; 
}
//输出
template <typename T> void o(T p) {
    static int stk[70], tp;
    if (p == 0) {
        putchar('0');
        return;
    }
    if (p < 0) {
        p = -p;
        putchar('-');
    }
    while (p) stk[++tp] = p % 10, p /= 10;
    while (tp) putchar(stk[tp--] + '0');
}
/*------------------------------------------------------------*/

/*离散化*/
sort(base+1,base+1+n);

int tot = unique(base+1,base+1+n)-base-1;

int find(ll x){
	return lower_bound(base+1,base+1+tot,x)-base;
}
/*------------------------------------------------------------*/


/*生成随机数*/
#include <ctime>
srand(time(0));
int x = rand()%((int)1000)+1; //1~1000范围

#include<random>
#include<ctime>
std::mt19937 rnd(time(0));
printf("%lld\n",rnd()); //周期长度为2^19937-1, 比rand()优秀很多
/*------------------------------------------------------------*/


/*浮点数取模*/
void FLoat_Mod(long double a, long double b){
	printf("%.9Lf\n",a-b*(long long)(a/b));
}
/*------------------------------------------------------------*/

/* 相关于char[]的输入输出 */
  char s[105];
  scanf("%s", s); // 读入一个字符串，且首指针放到s+0
  scanf("%s%s%s", s+1, s+21, s+41); // 读入三个字符串，且首指针分贝放到s+1、s+21、s+41，注意本样例若前两个字符串长度超过19，就会在内存有重叠而造成使用出错
  scanf("\n%[^\n]%*c",s); // 读入一行(第一种形式)，首指针为s+0。也可以换为"%[^\n]%*c"，%*c表示忽略后一个字符
  //注意此种读法会忽略每行开始的空格
  //^表示非, [^/n]表示读入换行字符就结束读入, %*c表示该输入项读入后不赋予任何变量
  scanf("%[^\n]", s); getchar(); // 读入一行(第二种形式)，首指针为s+0。getchar()表示把\n取掉
  printf("%s %s", s+0, s+1); // 分别表示从第0、1位开始输出，直到遇到'\0'，所以想截断输出一个字符串还可以将字符串的某个位置置为'\0'即可输出前半部分
/*------------------------------------------------------------*/

/* scanf */
  scanf("%*[ ]%[^/]/%d",t[i].s,&t[i].a); //%*[]表示越过[]中的字符, %[a-z]表示读入字符串, 直到遇到不是a-z中的字符为止
  //%[^a]表示读入字符串知道遇到字符a为止, 但a并没有被读入
  getline(cin,s); //s为一个string, 推荐使用 getline 进行整行读入
/*------------------------------------------------------------*/ 

/* 相关于string的输入输出 */
  string s;s.resize(100); // 想用scanf读string必须预先设置大小
  scanf("%s", &s[0]); // 读入到string
  scanf("\n%[^\n]%*c", &s[0]); // 读入1行到string
  char tmp[100];
  scanf("%s", tmp+1); s=tmp+1; // 先读入到char[]再令string=char[]
  printf("%s", s.c_str()); // 输出string
/*------------------------------------------------------------*/


/*map中存放string*/
  mp[s.c_str()] //最好传入string形式，避免发生错误
  std::unordered_map<int,int> tmp; mp.swap(tmp); //设置一个空的map用于交换，即mp.clear()
  mp.reserve(1000); //预留map中元素个数
  mp.rehash(1000); //给元素个数预留足够的bucket用于hash
/*------------------------------------------------------------*/


/*string函数*/
  /* 数字转字符串 */
  strings s = to_string(2323.232);
  /* 取长度 */
  int sz = s.size();
  /* 取下标字符 */
  char c = s[2];
  /* 截取字符串 */
  string ss = s.substr(2, 3);  // 从第2个字符开始截取长度为3的串
  /* 查找串 */
  size_t found = s.find("23", 0); // 从位置0开始找到第1个串"23"，找到返回下标，否则found=string::npos
  size_t found = s.rfind("23", s.size()-1); // 从右侧开始找到第1个串"23"，至多找到至pos
  size_t可以强转为int, int pos = (int)s.find("23",0);
  int pos = (int)s.find(':');  //string也可以直接查找字符
  /* 拼接 */
  s += "aaaa";
/*------------------------------------------------------------*/


/*bitset函数*/
  /* 声明 */
  bitset<16> a;           // 空，全零 0000000000000000
  bitset<16> b(0x3fff);   // 整数参数 0011111111111111
  bitset<16> c("00101");  // 字符串   0000000000000101

  /* 赋值 */
  a[0] = 1;    // 低位第0位设为1
  a.set(0, 1); // 低位第0位设为1
  a.set(0);    // 低位第0位设为1
  a.set(0, 0); // 低位第0位设为0
  a.set();     // 设为全1
  a.reset();   // 设为全0，该函数同样有两个参数，参数1为pos，参数2位value(为空时是0)
  a.flip(1);   // 翻转第1位
  a.flip();    // 翻转整个01串
  a<<=1;       // a左移1位

  /* 遍历 */
  cout << a << endl;                     // cout输出整个01串
  printf("%s\n", a.to_string().c_str()); // printf输出整个01串，较为麻烦一些
  cout << a[0] << endl;                  // cout输出低位第0位
  printf("%d\n", a[0]==1);               // printf输出低位第0位

  /* 判位、换型 */
  a.count();              //统计1的个数
  a = a1 & a2;            //按位与
  a = a1 | a2;            //按位或
  a = a1 ^ a2;            //按位异或
  a = ~a1;              //按位补
  a = a1 << 3;            //移位
  a.text(0);                          // 判断第0位是否为1
  a.any();                            // 判断是否至少1位为1
  s.none();                           // 判断是否全0
  int one = a.count;                  // 获得1的个数
  string s = a.to_string();           // 转换为字符串
  unsigned long = a.to_ulong();       // 转换为无符号整形
  unsigned long long = a.to_ullong(); // 转换为无符号整形
/*------------------------------------------------------------*/


/*vector*/
  /* 声明 */
  vector<int> v;
  /* 排序 */
  sort(v.begin(),v.end());
  /* 遍历方式一（推荐）*/
  for(auto &tp : v){}
  /* 遍历方式二（不推荐）*/
  if(v.size()){ //一定要确保v不为空，否则会RE
    rep(i,0,v.size()-1){}
  }
  /* resize() 和 reserve() 函数 */
  v.reserve(100);  //改变了vector的capacity，但是没有改变其空间大小
  v.resize(100);   //同时改变了vector的capacity和size，且原有元素不会被覆盖
/*-------------------------------------------------------------------*/


/*set函数*/
  /* 声明 */
  set<int> s;
  /* 赋值 */
  s.insert(1);
  s.insert(2);
  /* 遍历 */
  if(s.find(1) != s.end()) printf("had");
  for(set<int>::iterator it=s.begin();it!=s.end();it++) printf("%d", *it); // 全部遍历
  for(auto &x : s) printf("%d\n", x); 
  int sz = s.size(); // 大小
  /* 清空 */
  s.clear();
  s.erase(x); //x为s中存的内容
  *(s.begin()); //取出set中首位元素
  multiset<int>::iterator it1 = st.begin(), it2 = st.end();
  it2--; //取出set/multiset首尾元素
  set<pair<int,int> >::iterator it = st1.lower_bound(make_pair(-pos,0)); //it是指针
  lower_bound: 找到第一个大于等于这个数的值，若要找第一个小于的可以考虑往set里丢负值
  /* multiset删除 */
  st.erase(st.find(x)) //只删一个x
  st.erase(x) //删除所有x
/*------------------------------------------------------------*/


/*regex分割*/
  regex re("[\\.,!\\? ]"); //全局变量, .与?需要转义, 此处分割的内容有'.'、','、'!'、'?'、' '
  //分割之后，最后的一个字符串可能会含有大量空格
  /* 切割单词后放入一个vector */
  vector<string> ans{
      sregex_token_iterator(yy.begin()+pos+1, yy.end(), re, -1),
      sregex_token_iterator()
  };
  for(auto &it: ans) printf("%s\n",it.c_str());
/*------------------------------------------------------------*/

/*线段树节点编号*/
inline int get_id(int l,int r) {return (l+r)|(l!=r);}
/*------------------------------------------------------------*/    

/*二进制中01操作*/
template<class T> int getbit(T s, int i) { return (s >> i) & 1; }
template<class T> T onbit(T s, int i) { return s | (T(1) << i); }
template<class T> T offbit(T s, int i) { return s & (~(T(1) << i)); }
template<class T> int cntbit(T s) { return __builtin_popcount(s);}
/*------------------------------------------------------------*/

/*运行时间*/
  #include <ctime> //程序开头
  clock_t startTime,endTime;
  startTime = clock(); //计时开始
  
  //程序末尾
  endTime = clock();//计时结束
  cout << "The run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl; 
/*------------------------------------------------------------*/