#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
typedef long long ll;
const int N = 405;
const ll mod = 1e9+7;
using namespace std;

ll A[N][N];
int n, B1[N], B2[N];

ll poww(ll a, ll b) {
    ll base = a, ans = 1;
    while(b) {
        if(b & 1) ans = ans * base % mod;
        base = base * base % mod;
        b >>= 1;
    }
    return ans;
}

bool matrixInv() {
    for(int k = 1; k <= n; k++) {
        for(int i = k; i <= n; i++)
            for(int j = k; j <= n; j++)
                if(A[i][j]) {
                    B1[k] = i, B2[k] = j; break;
                }
        for(int i = 1; i <= n; i++)
            swap(A[k][i], A[B1[k]][i]);
        for(int i = 1; i <= n; i++)
            swap(A[i][k], A[i][B2[k]]);
        if(!A[k][k]) {
            return false; // 不可逆
        }
        A[k][k] = poww(A[k][k], mod-2);
        for(int j = 1; j <= n; j++)
            if(j!=k) (A[k][j] *= A[k][k]) %= mod;
        for(int i = 1; i <= n; i++)
            if(i != k)
                for(int j = 1; j <= n; j++)
                    if(j != k)
                        (A[i][j] += mod - A[i][k] * A[k][j] %mod) %= mod;
        for(int i = 1; i <= n; i++)
            if(i != k)
                A[i][k] = (mod - A[i][k] * A[k][k] % mod) % mod;
    }
    for(int k = n; k; k--) {
        for(int i = 1; i <= n; i++)
            swap(A[B2[k]][i], A[k][i]);
        for(int i = 1; i <= n; i++)
            swap(A[i][B1[k]], A[i][k]);
    }
    return true; // 可逆
}

int main()
{
    scanf("%d",&n);
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= n; j++)
            scanf("%lld", A[i][j]);
    matrixInv();
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= n; j++)
            printf("%lld%c", A[i][j], " \n"[j==n]);
    return 0;
}