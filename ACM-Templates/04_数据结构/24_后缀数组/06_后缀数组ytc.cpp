

#include<bits/stdc++.h>
#define N 1000050
using namespace std;

namespace SA{
    int t1[N],t2[N];
    int n, c[N], sa[N], rk[N], height[N];
    int *x = t1,*y = t2; // 这里用指针是因为，交换的比较块。 
    char *s;
    /*
        sa:排名为i的后缀的位置
        rk:位置为i的后缀的排名
    */
    void get_SA(char *str, int m) {
        s = str; n = strlen(s+1);
        for (int i = 1; i <= n; ++i) ++c[x[i] = s[i]];   // 如果多次使用， c 要清零。
        for (int i = 2; i <= m; ++i) c[i] += c[i - 1];  // m 是从1开始的， 所以给的数组，最小要是1.
        for (int i = n; i >= 1; --i) sa[c[x[i]]--] = i;
        for (int k = 1; k <= n; k <<= 1) {
            int num = 0;
            for (int i = n - k + 1; i <= n; ++i) y[++num] = i;
            for (int i = 1; i <= n; ++i) if (sa[i] > k) y[++num] = sa[i] - k;
            for (int i = 1; i <= m; ++i) c[i] = 0;
            for (int i = 1; i <= n; ++i) ++c[x[i]];
            for (int i = 2; i <= m; ++i) c[i] += c[i - 1]; 
            for (int i = n; i >= 1; --i) sa[c[x[y[i]]]--] = y[i], y[i] = 0;
            swap(x, y);
            x[sa[1]] = 1;   num = 1;
            for (int i = 2; i <= n; ++i)
                x[sa[i]] = (y[sa[i]] == y[sa[i - 1]] && y[sa[i] + k] == y[sa[i - 1] + k]) ? num : ++num;
            if (num == n) break;
            m = num;
        }
    }
    void get_height() {
        int k = 0;
        for (int i = 1; i <= n; ++i) rk[sa[i]] = i;
        for (int i = 1; i <= n; ++i) {
            if (rk[i] == 1) continue; //第一名height为0
            if (k) --k;//h[i]>=h[i-1]+1;
            int j = sa[rk[i] - 1];
            while (j + k <= n && i + k <= n && s[i + k] == s[j + k]) ++k;
            height[rk[i]] = k; //h[i]=height[rk[i]];
        }
    }
};

char s[N];
int n;
int main() {
    scanf("%s", s + 1);
    n = strlen(s + 1);
    SA::get_SA(s, 122);
    SA::get_height();
    for (int i = 1; i <= n; ++i)
        printf("%d ",SA::sa[i]);
}