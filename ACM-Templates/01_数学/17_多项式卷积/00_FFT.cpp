#include <bits/stdc++.h>
const double PI = acos(-1.0);
using namespace std;

struct FastFourierTransform {
	typedef complex<double> cp;
	typedef int R;
	static const int MAXN = 1e5+5;
	// a: a[0]*x^0+a[1]*x^1+...+a[n-1]*x^(n-1)
	// b: b[0]*x^0+b[1]*x^1+...+b[m-1]*x^(m-1)
	// c: c[0]*x^0+c[1]*x^1+...+c[n+m-2]*x^(n+m-2)

	int n, m, len;
	cp a[MAXN], b[MAXN];
	R c[2*MAXN], rev[2*MAXN];

	void FFT(cp *a, R n, R inv) {
		R bit = 0;
		while((1<<bit)<n) bit++;
		for(R i = 0; i < n; i++) {
			rev[i] = (rev[i>>1]>>1) | ((i&1)<<(bit-1));
			if(i < rev[i]) swap(a[i], a[rev[i]]);
		}
		for(int len = 2; len <= n; len <<= 1) {
			R mid = len >> 1;
			cp unit(cos(PI/mid), inv*sin(PI/mid)); // 单位根
			for(int i = 0; i < n; i += len) {
				// mid << 1 是准备合并序列的长度, i 是合并到了哪一位
				cp omega(1, 0);
				for(R j = 0; j < mid; j++, omega *= unit) {
					// 只扫左边部分, 得到右半部分的答案
					cp x = a[i+j], y = omega*a[i+j+mid];
					a[i+j] = x+y;
					a[i+j+mid] = x-y;
				}
			}
		}
		if(inv == -1) for(R i = 0; i <= n; i++) a[i] /= n;
	}

	void multiply(int len1, R *A, int len2, R *B) {
		R bitn = 1;
		n = len1, m = len2, len = n+m;
		while(bitn < n+m) bitn <<= 1;
		memset(a, 0, sizeof(cp)*(bitn+1));
		memset(b, 0, sizeof(cp)*(bitn+1));
		memset(c, 0, sizeof(R)*(bitn+1));

		for(int i = 0; i < n; i++) a[i] = A[i];
		for(int i = 0; i < m; i++) b[i] = B[i];

		FFT(a, bitn, 1); FFT(b, bitn, 1);
		for(int i = 0; i < bitn; i++) a[i] = a[i]*b[i];
		FFT(a, bitn, -1); // IFFT
		
		for(int i = 0; i < bitn; i++) c[i] += a[i].real()+0.5;
	}
}fft;


int main()
{
	return 0;
}