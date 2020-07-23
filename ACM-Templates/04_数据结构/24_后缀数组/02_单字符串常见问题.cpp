// ---
// 一、最长公共前缀 \\
// \\
// 题意: 给定一个字符串，询问某两个后缀的最长公共前缀。\\
// \\
// 思路: 模板题。对字符串求出后缀数组后，用线段树或 $st$ 表进行 $rmq$ 即可。\\
// \\
// 二、可重叠最长重复子串\\
// \\
// 题意: 给定一个字符串，求最长重复子串，这两个子串可以重叠。\\
// \\
// 思路: 模板题。即求字符串中任意两个后缀最长公共前缀的最大值，即输出 $height$ 数组中的最大值即可。\\
// \\
// 三、不可重叠最长重复子串 \\
// \\
// 题意: 给定一个字符串，求最长不可重复子串，这两个子串不可以重叠。\\
// \\
// 思路: 这个题与上面那个题最大的区别在于，此题找到的两个子串不可以重叠。因此我们在此处引入后缀数组中一个重要的思想，二分 $+\ height$ 分组。\\
// \\
// 我们二分一个答案 $w$，然后根据这个答案对 $height$ 数组进行分组，即每一组中的 $height$ 数组值大于 $w$。然后记录一个 $maxx$ 和 $minn$ 分别表示该组中所有后缀起始位置的最大值和最小值，然后判断两个位置之间的差是否大于等于 $w$，如果大于等于则 $w$ 可行。\\
// \\
// 四、可重叠的 $k$ 次最长重复子串 \\
// 题意: 给定一个字符串，求至少出现 $k$ 次的最长重复子串，这 $k$ 个子串可以重叠。\\
// \\
// 思路: 利用上面那个题的 ==二分 $+\ height$ 分组== 思想可以轻松解决此题。即分完组之后判断该组内的字符串数是否大于 $w$ 即可。\\
// \\
// 五、不相同的子串的个数\\
// \\
// 题意: 给定一个字符串，求不相同的子串个数。\\
// \\
// 思路: 求出该字符串的后缀数组，然后遍历 $height$ 数组，排名为 $i$ 的后缀的起始位置为 $sa[i]$，如果可以相同的话，对答案的贡献为 $n-sa[i]+1$。但是现在要求不相同的子串个数，因此要减去最长的相同前缀，即贡献为 $n-sa[i]+1-height[i]$。\\
// \\
// 六、连续重复子串 [整个串] \\
// 题意: 给定一个字符串 $L$，已知这个字符串是由某个字符串 $S$ 重复 $R$ 次得到的，求 $R$ 的最大值。\\
// \\
// 思路: 设 $LEN$ 为字符串 $L$ 的长度。枚举字符串 $S$ 的长度 $len$，保证 $len$ 能够被 $LEN$ 整除。然后再判断 $suffix[0]$ 与 $suffix[len]$ 的最长公共前缀长度是否为 $LEN-len$，如果是则表示该长度符合答案。\\
// \\
// 这里求后缀最长公共前缀时不需要 $rmq$，由于 $suffix[0]$ 是固定的，因此只需要求出 $height[i]$ 数组到 $height[rak[0]]$ 的最小值即可，总时间复杂度为 $O(n)$。\\
// \\
// 七、重复次数最多的连续重复子串 [子串] \\
// 题意: 给定一个字符串 $L$，在该字符串的所有子串中找到一个重复度最多的子串，重复度相同则输出字典序最小。重复度即上题定义的重复次数，例如 "$ababab$" 重复次数为 $3$，"$abcd$" 重复次数为 $1$。\\
// \\
// 思路: 我们枚举 $len$，去求解长度为 $len$ 下最大的重复度 $R$，即有无一个子串由 $R$ 个长度为 $len$ 的子串连续拼接而成。\\
// \\
// 因此我们将字符串 $L$ 分段，$0、len、2*len、3*len、...、k*len$，对于 $suffix[i*len]$ 与 $suffix[(i+1)*len]$ 求出最长公共前缀 $x$，再另 $pos=i*len-(len-x\%len)$，求 $suffix[pos]$ 与 $suffix[pos+len]$ 的最长公共前缀，然后更新答案。\\
// \\
// 这样操作的原因其实是枚举所有的以长度 $len$ 进行重复的子串，这也是对 $len$ 取模数的原因。可能这样说还是有些难以理解，推荐自己静下来想一想，或者看看代码。\\
// \\
// 最后分析下时间复杂度，$\displaystyle\frac{n}{1}+\displaystyle\frac{n}{2}+\displaystyle\frac{n}{3}+...+\displaystyle\frac{n}{n}=O(nlogn)$。\\
// ---
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
#define mem(a,b) memset(a,b,sizeof a);
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
typedef long long ll;
typedef double db;
const int N = 1e5+10;
const db EPS = 1e-9;
using namespace std;

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

int n,height[N],sa[N],rak[N],len; //所有数组都从0开始计数
/*
    suffix[i]: 以i为起始位置的后缀
    sa[i]: 排名第i的后缀的起始位置
    rak[i]: 表示suffix[i]的排名
    height[i]: suffix(sa[i-1])和suffix(sa[i])的最长公共前缀
        · h[i] = height[rak[i]], h[i] >= h[i-1]-1
        · suffix[i]和suffix[j]之前的最长公共前缀 = min(height[rak[i]+1]...height[rak[j]])
*/

bool cmp(int *y,int a,int b,int k)
{
    int a1 = y[a],b1 = y[b];
    int a2 = a + k >= len ? -1 : y[a + k];
    int b2 = b + k >= len ? -1 : y[b + k];
    return a1 == b1 && a2 == b2;
}

int t1[N],t2[N],cc[N];

void get_sa(char s[])
{
    int *x = t1,*y = t2,m = 200;
    len = strlen(s);
    for(int i = 0;i < m;i ++) cc[i] = 0;
    for(int i = 0;i < len;i ++) ++ cc[x[i] = s[i]];
    for(int i = 1;i < m;i ++) cc[i] += cc[i - 1];
    for(int i = len - 1;~i;i --) sa[-- cc[x[i]]] = i;
    for(int k = 1;k < len;k <<= 1)
    {
        int p = 0;
        for(int i = len - k;i < len;i ++)  y[p ++] = i;
        for(int i = 0;i < len;i ++) if(sa[i] >= k) y[p ++] = sa[i] - k;
        for(int i = 0;i < m;i ++) cc[i] = 0;
        for(int i = 0;i < len;i ++) ++ cc[x[y[i]]];
        for(int i = 1;i < m;i ++) cc[i] += cc[i - 1];
        for(int i = len - 1;~i;i --) sa[-- cc[x[y[i]]]] = y[i];
        swap(x,y); m = 1; x[sa[0]] = 0;

        for(int i = 1;i < len;i ++)
            x[sa[i]] = cmp(y,sa[i - 1],sa[i],k) ? m - 1 : m ++;
        if(m >= len) break;
    }
}

void get_height(char s[])
{
    len = strlen(s);
    for(int i = 0;i < len;i ++) rak[sa[i]] = i;
    int h = 0;
    height[0] = 0;
    for(int i = 0;i < len;i ++)
    {
        if(!rak[i]) continue;
        int j = sa[rak[i] - 1];
        if(h) h --;
        while(s[i + h] == s[j + h]) h ++;
        height[rak[i]] = h;
    }
}

char s[N];

int st[N][20];

void init(){
    for(int i = 0; i < len; i++) st[i][0] = height[i];
    for(int j = 1; (1<<j) <= len; j++){
        for(int i = 0; i + (1<<j) - 1 < len; i++)
            st[i][j] = min(st[i][j-1],st[i+(1<<(j-1))][j-1]);
    }
}

int query(int l,int r){
    int k = (int)(log((double)(r - l + 1)) / log(2.0));
    return min(st[l][k],st[r-(1<<k)+1][k]);
}

int main(){
    int _ = 0;
    while(~scanf("%s",s)){
        if(s[0] == '#') break;
        len = strlen(s);
        get_sa(s);
        get_height(s);
        init();
        int ans = 0, LEN = 0;
        rep(L,1,len){
            for(int j = L; j < len; j += L){
                int p1 = rak[j];
                int p2 = rak[j-L];
                if(p1 > p2) swap(p1,p2);
                int thp = query(p1+1,p2);
                if((thp/L)+1 > ans)
                    ans = (thp/L)+1, LEN = L;

                thp = L-thp%L;
                if(j-L-thp < 0) continue;
                p1 = rak[j-thp], p2 = rak[j-L-thp];
                if(p1 > p2) swap(p1,p2);
                thp = query(p1+1,p2);
                if((thp/L)+1 > ans)
                    ans = (thp/L)+1, LEN = L;
            }
        }
        int pos = -1;
        rep(i,0,len-1-LEN){
            int p1 = rak[i], p2 = rak[i+LEN];
            if(p1 > p2) swap(p1,p2);
            int thp = query(p1+1,p2);
            if((thp/LEN)+1 == ans){
                if(pos == -1) pos = i;
                else if(rak[i] < rak[pos]) pos = i;
            }
        }
        s[pos+LEN*ans] = '\0';
        printf("Case %d: %s\n",++_,s+pos);
    }
    return 0;
}