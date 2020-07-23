#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const db EPS = 1e-9;
using namespace std;
ll pow_mod(ll a, ll k, ll p) {
    ll ret = 1;
    while(k) {
        if(k & 1)	ret = ret * a % p;
        a = a * a % p;
        k >>= 1;
    }
    return ret % p;
}
ll modsqr(ll a, ll n) { //求解 x^2 = a(mod n)
    ll b, k, i, x;
    if(a == 0)	return 0;
    if(n == 2)	return a % n;
    if(pow_mod(a, (n - 1) / 2, n) == 1) {
        if(n % 4 == 3) {
            x = pow_mod(a, (n + 1) / 4, n);
        } else {
            for(b = 1; pow_mod(b, (n - 1) / 2, n) == 1; ++b);
            i = (n - 1) / 2;
            k = 0;
            do {
                i /= 2;
                k /= 2;
                if((pow_mod(a, i, n) * (ll)pow_mod(b, k, n) + 1) % n == 0)	k += (n - 1) / 2;
            } while(i % 2 == 0);
            x = (pow_mod(a, (i + 1) / 2, n) * (ll)pow_mod(b, k / 2, n)) % n;
        }
        if(x * 2 > n)	x = n - x;
        return x;
    }
    return -1;
}
int main() {
    int T;
    ll p = 1000000007ll;
    ll inv2 = pow_mod(2ll, p - 2, p);
    scanf("%d",&T);
    while (T--) {
        ll b, c;
        scanf("%lld %lld",&b,&c);
        ll a = ((b * b % p) - (4ll * c % p) + p) % p;
        ll x = modsqr(a, p);
        if(x == -1) {
            puts("-1 -1");
        } else {
            ll y = p - x;
            ll X = inv2 * (x + b) % p;
            ll Y = inv2 * (y + b) % p;
            printf("%lld %lld\n", min(X, Y), max(X, Y));
        }
    }
    return 0;
}