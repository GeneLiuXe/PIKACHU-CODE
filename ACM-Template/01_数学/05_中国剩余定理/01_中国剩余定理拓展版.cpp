//拓展中国剩余定理 —— m1,m2,m3之间不互质
#include <cstdio>
#define ll long long
// #define ll __int128
// 如果 int128 也爆了的话，建议使用 python
const int MAXN = 2e6+10;
using namespace std;
ll K, C[MAXN], M[MAXN];

ll gcd(ll a, ll b) {
    return b == 0 ? a : gcd(b, a % b);
}
ll exgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {x = 1, y = 0; return a;}
    ll r = exgcd(b, a % b, x, y), tmp;
    tmp = x; x = y; y = tmp - (a / b) * y;
    return r;
}
ll inv(ll a, ll b) {
    ll x = 0, y = 0;
    ll r = exgcd(a, b, x, y);
    while (x < 0) x += b;
    return x;
}
ll excrt(int n, ll M[], ll C[]){ // % m = c
    for (int i = 2; i <= n; i++) {
        ll M1 = M[i - 1], M2 = M[i], C2 = C[i], C1 = C[i - 1], T = gcd(M1, M2);
        if ((C2 - C1) % T != 0) return -1;
        M[i] = (M1 * M2) / T;
        C[i] = ( inv( M1 / T , M2 / T ) * (C2 - C1) / T ) % (M2 / T) * M1 + C1;
        C[i] = (C[i] % M[i] + M[i]) % M[i];
    }
    return C[n];
}

int main() {
    while (~scanf("%lld", &K)) {
        for (int i = 1; i <= K; i++) scanf("%lld%lld", &M[i], &C[i]);
        printf("%lld\n", excrt(K, M, C));
    }
    return 0;
}