#include <bits/stdc++.h>
typedef long long ll;
using namespace std;

struct BigInteger{
    ll A[50];
    enum{MOD = 1000000};
    BigInteger(){memset(A, 0, sizeof(A)); A[0]=1;}
    void set(ll x){memset(A, 0, sizeof(A)); A[0]=1; A[1]=x;}
    void print(){
        printf("%lld", A[A[0]]);
        for (ll i=A[0]-1; i>0; i--){
            if (A[i]==0){printf("000000"); continue;}
            for (ll k=10; k*A[i]<MOD; k*=10ll) printf("0");
            printf("%lld", A[i]);
        }
        printf("\n");
    }
    ll& operator [] (int p) {return A[p];}
    const ll& operator [] (int p) const {return A[p];}
    BigInteger operator + (const BigInteger& B){
        BigInteger C;
        C[0]=max(A[0], B[0]);
        for (ll i=1; i<=C[0]; i++)
            C[i]+=A[i]+B[i], C[i+1]+=C[i]/MOD, C[i]%=MOD;
        if (C[C[0]+1] > 0) C[0]++;
        return C;
    }
    BigInteger operator * (const BigInteger& B){
        BigInteger C;
        C[0]=A[0]+B[0];
        for (ll i=1; i<=A[0]; i++)
            for (ll j=1; j<=B[0]; j++){
                C[i+j-1]+=A[i]*B[j], C[i+j]+=C[i+j-1]/MOD, C[i+j-1]%=MOD;
            }
        if (C[C[0]] == 0) C[0]--;
        return C;
    }
};

int main(){
	BigInteger A,B,C,D;
	A.set(10000);
	B.set(10000);
	C = A+B;
	D = A*B;
	C.print();
	D.print();
}