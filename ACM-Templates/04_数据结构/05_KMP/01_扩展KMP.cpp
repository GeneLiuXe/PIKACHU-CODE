// Ｓ串的每个后缀的前缀和Ｔ串的前缀进行匹配．\\
// 就是那 T 串的前缀匹配 S 串的 所有后缀。\\
// ext 数组就是S 串每个后缀可以匹配 T 串前缀的最大长度。\\
// next 数组就是 T 串匹配自己  可以得到的最大长度。 \\

#include<bits/stdc++.h>
using namespace std;
const int N = 5e4 + 100;
int Next[N],ext[N];
void getnext(char T[]){
	int len = strlen(T),po,i = 0,j;
	Next[0] = len; //初始化 Next[0]
	while(T[i] == T[i+1] && i+1 < len) i++;
	Next[1] = i;
	po = 1; //初始化 po
	for (i = 2; i < len; i++){
		if (Next[i - po] + i < Next[po] + po) Next[i] = Next[i - po];
		//第一种情况，可以直接得到Next[i] 的值。 
		else  { //第二种情况，要继续匹配。
			j = Next[po] + po - i;  //如果 i > p ，则要从头开始匹配。
			// Next[po] + po  就是 p，
			if (j < 0) j = 0;
			while(i + j < len && T[i + j] == T[j]) j++; //计算 Next[i] 
			Next[i] = j;
			po = i;
		}
	}
}
void exkmp(char S[], char T[]){
	int Max = 0;
	int slen = strlen(S), tlen = strlen(T);
	int i = 0,j,po;
	getnext(T);  //计算 T的 next 数组。
	while(i < slen && i < tlen && S[i] == T[i]) i++;
	ext[0] = i;
	po = 0; //初始化 po 的位置。 
	for (i = 1; i < slen; i++){
		if (Next[i - po] + i < ext[po] + po) ext[i] = Next[i - po]; 
		else {  // 第二种情况。
			j = ext[po] + po - i; // if i > p  then they star from 0
			if (j < 0) j = 0;
			while(i + j < slen &&  j < tlen && S[i + j] == T[j]) j++;
			ext[i] = j;
			po = i;  // update the position of po.
		}
	}
	for (i = 0; i < slen; i++)
		if (i + ext[i] == slen) Max = max(Max,ext[i]);
	for (i = 0; i < Max; i++){
		printf("%c",T[i]);
	}
	if (Max) printf(" ");
	printf("%d\n",Max);
}
int main(){
	char S[N],T[N];
	while(~scanf("%s",T)){
		scanf("%s",S);
		exkmp(S,T);
	}
	return 0;
}