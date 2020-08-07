// ---
// 矩阵树定理，用于生成树计数。\\
// ---
#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
using namespace std;
const int N = 110;

struct Matrix {
    typedef long long R;
    static const R mod = 998244353;
    R mat[N][N];

    void init() {memset(mat, 0, sizeof mat);}

    R inv(R a, R m) {
        if(a == 1) return 1;
        return inv(m%a, m) * (m-m/a) % m;
    }

    R det(int n) {
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                mat[i][j] = (mat[i][j]%mod+mod)%mod;
        R res = 1;
        for(int i = 0; i < n; i++) {
            for(int j = i; j < n; j++)
                if(mat[j][i] != 0) {
                    for(int k = i; k < n; k++)
                        swap(mat[i][k], mat[j][k]);
                    if(i != j)
                        res = (-res+mod)%mod;
                    break;
                }
            if(mat[i][i] == 0) {
                res = 0; // 行列式不存在
                break;
            }
            for(int j = i+1; j < n; j++) {
                // ll mut = (mat[j][i]*INV[mat[i][i]]) % mod; 逆元打表
                R mut = (mat[j][i]*inv(mat[i][i], mod)) % mod;
                for(int k = i; k < n; k++)
                    mat[j][k] = (mat[j][k]-(mat[i][k]*mut)%mod+mod)%mod;
            }
            res = (res*mat[i][i])%mod;
        }
        return res;
    }
}ret;

struct Matrix_db {
    typedef double db;
    static constexpr db eps = 1e-8;
    db mat[N][N];

    void init() {memset(mat, 0, sizeof mat);}

    int sgn(db x) {
        if(fabs(x) < eps) return 0;
        if(x < 0) return -1;
        else return 1;
    }

    db det(int n) {
        int i, j, k, sign = 0;
        db ret = 1;
        for(i = 0; i < n; i++) {
            if(sgn(mat[i][i]) == 0) {
                for(j = i + 1; j < n; j++)
                    if(sgn(mat[j][i]) != 0) break;
                if(j == n) return 0;
                for(k = i; k < n; k++)
                    swap(mat[i][k], mat[j][k]);
                sign++;
            }
            ret *= mat[i][i];
            for(k = i + 1; k < n; k++)
                mat[i][k] /= mat[i][i];
            for(j = i + 1; j < n; j++)
                for(k = i + 1; k < n; k++)
                    mat[j][k] -= mat[j][i] * mat[i][k];
        }
        if(sign & 1) ret = -ret;
        return ret;
    }
};

int n, m;
vector<int> G[N];

int main() {
    scanf("%d%d",&n,&m);
    rep(i,1,m) {
        // 无向图，编号从 1~n
        int u,v; scanf("%d%d",&u,&v);
        u--; v--;
        G[u].push_back(v);
        G[v].push_back(u);
    }

    ret.init();
    for(int x = 0; x < n; x++)
        for(int y:G[x]) {
            ret.mat[x][y]--; // 原板子是 = -1，但重边存在则需要 --
            ret.mat[x][x]++;
        }
    long long cnt = ret.det(n-1); // 生成树个数
    return 0;
}