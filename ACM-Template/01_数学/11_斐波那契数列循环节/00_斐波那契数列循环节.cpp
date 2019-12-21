// ---
// $(f[1],f[2])-(f[3],f[4])-(f[5],f[6])-(f[7],f[8])$，给定 $n$ 求 $m$，满足 $f[1+m]\%n=f[1]\%n$ \\
// 在此题中，由于每次增加两个数，因此最后需要将答案/2。 \\
// ---
#include <bits/stdc++.h>

#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
using namespace std;

#define ll unsigned long long

ll gcd(ll a, ll b) {
    return b == 0 ? a : gcd(b, a % b);
}

ll lcm(ll a, ll b) {
    return a / gcd(a, b) * b;
}

struct Matrix {
    ll mat[2][2];

    Matrix() { memset(mat, 0, sizeof mat); }
};

Matrix mul_M(Matrix a, Matrix b, ll mod) {
    Matrix ans;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            ans.mat[i][j] = 0;
            for (int k = 0; k < 2; ++k) {
                ans.mat[i][j] += a.mat[i][k] * b.mat[k][j] % mod;
                if (ans.mat[i][j] >= mod)ans.mat[i][j] -= mod;
            }
        }
    }
    return ans;
}

Matrix pow_M(Matrix a, ll n, ll mod) {
    Matrix ans;
    for (int i = 0; i < 2; ++i) ans.mat[i][i] = 1;
    Matrix tmp = a;
    while (n) {
        if (n & 1)ans = mul_M(ans, tmp, mod);
        tmp = mul_M(tmp, tmp, mod);
        n >>= 1;
    }
    return ans;
}

ll pow_m(ll a, ll n, ll mod) {
    ll ans = 1, tmp = a % mod;
    while (n) {
        if (n & 1)ans = ans * tmp % mod;
        tmp = tmp * tmp % mod;
        n >>= 1;
    }
    return ans;
}

const int maxn = 1e6 + 10;

int prime[maxn];

void getprime() {
    memset(prime, 0, sizeof prime);
    for (int i = 2; i <= maxn; ++i) {
        if (!prime[i])prime[++prime[0]] = i;
        for (int j = 1; j <= prime[0] && prime[j] * i <= maxn; ++j) {
            prime[prime[j] * i] = 1;
            if (i % prime[j] == 0)break;
        }
    }
}

ll factor[100][2];
int fatcnt;

int getfactors(ll x) {
    fatcnt = 0;
    ll tmp = x;
    for (int i = 1; prime[i] * prime[i] <= tmp; ++i) {
        factor[fatcnt][1] = 0;
        if (tmp % prime[i] == 0) {
            factor[fatcnt][0] = prime[i];
            while (tmp % prime[i] == 0) {
                factor[fatcnt][1]++;
                tmp /= prime[i];
            }
            fatcnt++;
        }
    }
    if (tmp != 1) {
        factor[fatcnt][0] = tmp;
        factor[fatcnt++][1] = 1;
    }
    return fatcnt;
}

int legendre(ll a, ll p) {
    if (pow_m(a, (p - 1) >> 1, p) == 1)return 1;
    else return -1;
}

int f0 = 1;
int f1 = 1;

ll getfib(ll n, ll mod) {
    if (mod == 1)return 0;
    Matrix A;
    A.mat[0][0] = 0;
    A.mat[1][0] = 1;
    A.mat[0][1] = 1;
    A.mat[1][1] = 1;
    Matrix B = pow_M(A, n, mod);
    ll ans = f0 * B.mat[0][0] + f1 * B.mat[1][0];
    return ans % mod;
}

ll fac[maxn];

ll G(ll p) {
    ll num;
    if (legendre(5, p) == 1)num = p - 1;
    else num = 2 * (p + 1);
    int cnt = 0;
    for (ll i = 1; i * i <= num; ++i) {
        if (num % i == 0) {
            fac[cnt++] = i;
            if (i * i != num) {
                fac[cnt++] = num / i;
            }
        }
    }
    sort(fac, fac + cnt);
    ll ans;
    for (int i = 0; i < cnt; ++i) {
        if (getfib(fac[i], p) == f0 && getfib(fac[i] + 1, p) == f1) {
            ans = fac[i];
            break;
        }
    }
    return ans;
}

ll find_loop(ll n) {
    getfactors(n);
    ll ans = 1;
    for (int i = 0; i < fatcnt; ++i) {
        ll record = 1;
        if (factor[i][0] == 2)record = 3;
        else if (factor[i][0] == 3)record = 8;
        else if (factor[i][0] == 5)record = 20;
        else record = G(factor[i][0]);
        for (int j = 1; j < factor[i][1]; ++j) {
            record *= factor[i][0];
        }
        ans = lcm(ans, record);
    }
    return ans;
}

int main() {
    __;
    getprime();
    ll n;
    while (cin >> n) {
        ll ans = find_loop(n);
        cout << ans / 2ll << endl;
    }
    return 0;
}