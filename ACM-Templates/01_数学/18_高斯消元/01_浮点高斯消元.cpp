#include <bits/stdc++.h>
const int N = 2000+10;
const double eps = 1e-9;
using namespace std;

double a[N][N], x[N];
/*
 equ 为方程数, var 为未知数个数
 a 为方程左边矩阵，x 为等式右边的值
 a[0][0]*x_0 + ... + a[0][var-1]*x_{var-1} = x[0]
*/
int equ, var;

// 返回 0 表示无解，1 表示有解
int Gauss() {
	int i,j,k,col,max_r;
	for(k = 0, col = 0; k < equ && col < var; k++, col++) {
		max_r = k;
		for(i = k+1; i < equ; i++)
			if(fabs(a[i][col]) > fabs(a[max_r][col]))
				max_r = i;
		if(fabs(a[max_r][col]) < eps) return 0;
		if(k != max_r) {
			for(j = col; j < var; j++)
				swap(a[k][j], a[max_r][j]);
			swap(x[k], x[max_r]);
		}
		x[k] /= a[k][col];
		for(j = col+1; j < var; j++) a[k][j] /= a[k][col];
		a[k][col] = 1;
		for(i = 0; i < equ; i++)
			if(i != k) {
				x[i] -= x[k]*a[i][col];
				for(j = col+1; j < var; j++) a[i][j] -= a[k][j]*a[i][col];
				a[i][col] = 0;
			}
	}
	return 1;
}