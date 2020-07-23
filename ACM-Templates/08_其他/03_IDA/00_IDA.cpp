#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>

int n, shell[21];

// swap [x1, x2] and [x2 + 1, x3]
void move (int x1, int x2, int x3)
{
     int tmp[21], i, j;
     for (i = x2 + 1, j = 0; i <= x3; i++, j++)
         tmp[j] = shell[i];
     for (i = x1; i <= x2; i++, j++)
         tmp[j] = shell[i];
     for (i = x1, j = 0; i <= x3; i++, j++)
         shell[i] = tmp[j];
     return;
}

int hfunc ()
{
    int i, ans = 0;
    for (i = 0; i < n - 1; i++)
        if (shell[i + 1] != shell[i] + 1) ans++;
    if (shell[n - 1] != n) ans++;
    return ans;
}

int maxdepth;
int dfs (int depth)
{
    int x1, x2, x3, h;
    for (x1 = 0; x1 <= n - 2; x1++) //枚举移动区间的左端点
    {   
        for (x2 = x1; x2 <= n - 2; x2++)  //枚举移动区间的右端点
        {
            for (x3 = x2 + 1; x3 <= n - 1; x3++)  //枚举插入点
            {
                move(x1, x2, x3);   //进行移动
                h = hfunc();
                if (h == 0) return 1;
                else if (3 * depth + h <= 3 * maxdepth) //IDA*，限制深度
                {
                     if (dfs(depth + 1)) return 1;
                }
                move(x1, x1 - x2 + x3 - 1, x3); //如果不可行，则返回原状
            }
        }
    } return 0;
}

int main ()
{
    int kase, i;
    scanf("%d", &kase);
    for (; kase > 0; kase--)
    {
        scanf("%d", &n);       //几本书
        for (i = 0; i < n; i++)
            scanf("%d", &shell[i]); //各个位置的序号
        maxdepth = (int)ceil((double)hfunc() / 3);
        if (maxdepth) while (maxdepth < 5 && dfs(1) == 0) maxdepth++;   //不断加大搜索深度
        if (maxdepth == 5) printf("5 or more\n");
        else printf("%d\n", maxdepth);
    }
    return 0;
}

/*
    题意：
1-n本书，可以选择任意一段连续的书，再将这一段书插入到其他某个位置。
目标是将书按照1-n的顺序依次排列，求最少需要多少次操作。
如果更换次数 >= 5，则直接输出 “5 or more”

    解法：
限制dfs的搜索深度，并且设计一个估价函数。
可以发现，将一段书插入到某一个位置，最多改变3个数的后继值，因此对于每一个状态，记录该状态下i与i+1不匹配的个数
然后ceil(这个个数/3)的值就是该状态达到目标状态所至少需要的操作数

对于每个状态，当前深度+目标操作数 < 5才继续进行深度搜索，由此大大降低了搜索的复杂度
*/
