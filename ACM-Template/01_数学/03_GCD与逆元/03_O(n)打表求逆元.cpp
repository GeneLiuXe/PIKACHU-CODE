#include <cstdio>
#include <cmath>
using namespace std;
typedef  long long ll;
const int N = 1e5 + 5;

int inv[N];
 
void inverse(int n, int p) { //O(n)求1-n所有逆元
    inv[1] = 1;
    for (int i=2; i<=n; ++i) {
        inv[i] = (ll) (p - p / i) * inv[p%i] % p;
    }
}