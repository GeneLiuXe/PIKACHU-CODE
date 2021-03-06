// ---
// 题意: \\
// 给定一个长度为 $n$ 的字符串，询问该字符串中长度最长的双倍回文串的长度。双倍回文串指 $AA^TAA^T$，如 $abbaabba$ 即是双倍回文串，而 $ababa$ 则不是。$(1\leq n\leq 5*10^5)$ \\
// \\
// 思路: \\
// 由于只需要求最长回文串，并不要求输出个数，因此我们直接在 $Manacher$ 的 $dp$ 过程中求出答案即可。\\
// \\
// 首先明确一点，$Manacher$ 右端点递增过程中，涉及到了所有的本质不同回文串，因此我们在每次右端点递增时，令当前节点为双倍回文串的中心节点，然后查询左半部分是否也为一个回文串，如果是则更新答案。\\
// \\
// 变型: \\
// 当然此题也可以进行延伸，可以将查询内容改成有多少个双倍回文串。如果询问有多少个双倍回文串的话，我的想法是用回文自动机+ $Manacher$ 算法进行解决。\\
// \\
// 首先 $Manacher$ 求出每一个节点为中心所能扩展的最远距离。然后在根据当前字符串建立回文自动机，并且在构建时对于每个节点记录构成该节点的末尾坐标 $pos$，之后再遍历回文自动机上的所有节点，利用 $Manacher$ 计算的 $p$ 数组来验证该节点所代表的回文串是否是双倍回文串即可。\\
// ---
#include <bits/stdc++.h>
const int N = 5e5+100;
using namespace std;

int n,tot,p[2*N],R,pos,ans;
char s[N],s2[2*N];

void manacher(){
    s2[0] = '$';
    s2[tot = 1] = '#';
    for(int i = 1; i <= n; i++)
        s2[++tot] = s[i], s2[++tot] = '#';
    for(int i = 1; i <= tot; i++){
        if(R >= i) p[i] = min(p[pos*2-i],R-i+1);
        if(p[i] + i > R){
            for(; s2[R+1] == s2[i*2-R-1]; R++){
                int y = (3*i-R-1)/2;
                if(s2[i] == '#' && s2[R+1] == '#' && (R+1-i)%4 == 0 && p[y] >= i-y+1) ans = max(ans,2*(i-y));
            }
            pos = i;
            p[i] = R-i+1;
        }
    }
    printf("%d\n",ans);
}

int main(){
    scanf("%d",&n); scanf("%s",s+1);
    manacher();
}