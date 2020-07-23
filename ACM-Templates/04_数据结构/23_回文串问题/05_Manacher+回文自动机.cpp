// ---
// 题意:\\ 
// 给定一个长度为 $n$ 的字符串，先可以选择该字符串中的一个节点，将其替换成 $\&$ 符号，问替换之后该字符串中本质不同回文串个数。 $(1\leq n\leq 5*10^5)$\\
// \\
// 思路: \\
// 一个比较明显的考虑方式就是枚举替换的位置，然后计算该位置改变后增加的回文串数量与减少的回文串数量。\\
// \\
// 首先我们考虑增加的回文串数量，显然就是以该节点为中心所形成的回文串总数，因此我们直接用马拉车的 $p$ 数组进行求解即可。\\
// \\
// 然后再来考虑减少的回文串数量。我们对于回文自动机中所有本质不同回文串的末尾出现位置记录一个 $mx$ 和 $mi$，表示最远出现的位置和最早出现的位置，记该回文串长度为 $len$。则如果 $mx-len+1\leq mi$ ，则修改位置出现在 $[mx-len+1,mi]$ 中时，本质不同回文串数量就会减 $1$，因此我们维护一个差分数组，$sum[mx-len+1]$ 加 $1$，$sum[mi]$ 减 $1$，然后对于每个自动机上的节点都这样处理一遍即可。\\
// \\
// 最后再枚举替换点统计答案。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define per(i,a,b) for(int i = a; i >= b; i--)
const int N = 1e5+10;
using namespace std;

char buf[N],s2[2*N];
int sum[N],p[2*N];

void dbg() {cout << "\n";}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}

struct PAM{
    #define KIND 26
    int n,last,tot;
    int len[N],trie[N][KIND],fail[N],cnt[N],S[N],num[N],mx[N],mi[N];
    //len[i]: 节点i所代表的回文串长度, fail[i]: 当前回文串的最长回文后缀（不包括自身）
    //cnt[i]: 节点i所代表的回文串的个数, S[i]: 第i次添加的字符, num[i]: 以第i个字符为结尾的回文串个数
    //last: 上一个字符构成最长回文串的位置，方便下一个字符的插入
    //tot: 总结点个数 = 本质不同的回文串的个数+2, n: 插入字符的个数 
    int newnode(int l){
        rep(i,0,KIND-1) trie[tot][i] = 0;
        cnt[tot] = 0, len[tot] = l, num[tot] = 0, mx[tot] = 0, mi[tot] = 2*1e5;
        return tot++;
    }
    inline void init(){
        tot = n = last = 0, newnode(0), newnode(-1);
        S[0] = -1, fail[0] = 1;
    }
    int get_fail(int x){ //获取fail指针
        while(S[n-len[x]-1] != S[n]) x = fail[x];
        return x;
    }
    inline int insert(int c,int pos){ //插入字符
        c -= 'a';
        S[++n] = c;
        int cur = get_fail(last);
        //在节点cur前的字符与当前字符相同，即构成一个回文串
        if(!trie[cur][c]){ //这个回文串没有出现过
            int now = newnode(len[cur]+2);
            fail[now] = trie[get_fail(fail[cur])][c];
            trie[cur][c] = now;
            num[now] = num[fail[now]]+1; //更新以当前字符为结尾的回文串的个数
        }
        last = trie[cur][c];
        cnt[last]++; //更新当前回文串的个数
        mx[last] = max(mx[last],pos);
        mi[last] = min(mi[last],pos); //更新每个回文串的最早/晚出现位置
        return num[last]; //返回以当前字符结尾的回文串的个数
    }
    void count(){ //统计每个本质不同回文串的个数
        per(i,tot-1,2){
            cnt[fail[i]] += cnt[i];
            mx[fail[i]] = max(mx[fail[i]],mx[i]);
            mi[fail[i]] = min(mi[fail[i]],mi[i]);
            if(mi[i] >= mx[i]-len[i]+1){
                sum[mx[i]-len[i]+1] += 1;
                sum[mi[i]+1] += -1;
            }
        }
    }
}pam;

void manacher(){
    int len = strlen(buf+1), tot = 1, R = 0, pos = 0;
    s2[0] = '$'; s2[tot = 1] = '#';
    for(int i = 1; i <= len; i++)
        s2[++tot] = buf[i], s2[++tot] = '#';
    for(int i = 1; i <= tot; i++){
        if(R >= i) p[i] = min(p[pos*2-i],R-i+1);
        if(p[i] + i > R){
            for(; s2[R+1] == s2[i*2-R-1] && R+1 <= tot && i*2-R-1 <= tot; R++);
            pos = i;
            p[i] = R-i+1;
        }
    }
}

int main(){
    int _; scanf("%d",&_);
    while(_--){
        int ans = 0, ct = 0;
        scanf("%s",buf+1);
        pam.init();
        int len = strlen(buf+1);
        rep(i,0,len) sum[i] = 0;
        rep(i,1,len) pam.insert(buf[i],i);
        pam.count();
        manacher();
        rep(i,1,len){
            sum[i] += sum[i-1];
            int to = pam.tot-2-sum[i]+((p[i*2]-2)/2+1);
            if(ans < pam.tot-2-sum[i]+((p[i*2]-2)/2+1)){
                ans = pam.tot-2-sum[i]+((p[i*2]-2)/2+1);
                ct = 1;
            }
            else if(ans == pam.tot-2-sum[i]+((p[i*2]-2)/2+1)) ct++;
        }
        printf("%d %d\n",ans,ct);
    }
    return 0;
}