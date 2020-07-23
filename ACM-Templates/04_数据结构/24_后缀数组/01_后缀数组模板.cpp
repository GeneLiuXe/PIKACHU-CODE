// ---
// 一、适用问题: \\
// \\
// 后缀数组的题目非常灵活多变，主要涉及字符串所有后缀的字典序比较以及最长公共前缀。\\
// \\
// 本文主要介绍后缀数组的一些经典应用，虽然是经典应用，但是其思想应该属于后缀数组类问题的本质思想。\\
// \\
// 二、算法介绍: \\
// \\
// 求解后缀数组的算法主要有倍增法、$DC3$ 算法，具体的算法实现此处就略过了，想要具体了解的话可以自行 $google$ 搜索。\\
// \\
// 下文给出的后缀数组模板是 $DC3$ 算法，时间复杂度为 $O(n)$。\\
// (1) 后缀数组中各大数组介绍\\
// 1. $suffix[i]:$ 以 $i$ 为起始位置的后缀\\
// 2. $sa[i]:$ 排名第 $i$ 的后缀的起始位置 \\
// 3. $rak[i]:$ 表示 $suffix[i]$ 的排名\\
// 4. $height[i]:$ $suffix(sa[i-1])$ 和 $suffix(sa[i])$ 的最长公共前缀\\
// 5. $h[i] = height[rak[i]]$，$h[i] \geq h[i-1]-1$\\
// \\
// 有了上述数组，我们便可以求出字符串任意两个后缀的最长公共前缀。\\
// $suffix[i]$ 和 $suffix[j]$ 之间的最长公共前缀 $= min(height[rak[i]+1],...,height[rak[j]])$\\
// \\
// 后缀数组所有的题目都是在这几个数组上做文章，具体的题目类型分类见下文例题。\\
// ---
#include <bits/stdc++.h>
const int N = 1e6+10;
using namespace std;

int height[N],sa[N],rak[N],len;
/*
    suffix[i]: 以i为起始位置的后缀
    sa[i]: 排名第i的后缀的起始位置
    rak[i]: 表示suffix[i]的排名
    height[i]: suffix(sa[i-1])和suffix(sa[i])的最长公共前缀
        · h[i] = height[rak[i]], h[i] >= h[i-1]-1
        · suffix[i]和suffix[j]之间的最长公共前缀 = min(height[rak[i]+1]...height[rak[j]])
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

int main(){
    scanf("%s",s);
    len = strlen(s);
    get_sa(s);
    get_height(s);
    return 0;
}