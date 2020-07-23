// 求小数点后n位
#include <iostream>
#define MAX_C 56000
int a = 10000, b, c = MAX_C, d, e, f[MAX_C + 1], g, n, ans, cnt;
using namespace std;
int main() {
    while(~scanf("%d", &n)){
    for (; b - c; )
        f[b++] = a / 5;
    for (; d = 0, g = c * 2;
         c -= 14, ans = e + d / a, e = d % a, cnt++)
    { if (cnt * 4 > n) break;
     for (b = c; d += f[b]*a, f[b] = d % --g, d /= g--, --b; d *= b); }
    if (n % 4 == 0) cout << (ans / 1000);
    else if (n % 4 == 1) cout << ((ans / 100) % 10);
    else if (n % 4 == 2) cout << ((ans / 10) % 10);
    else if (n % 4 == 3) cout << (ans % 10);
    printf("\n");}
    return 0;
}