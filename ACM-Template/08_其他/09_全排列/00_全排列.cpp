#include <bits/stdc++.h>
using namespace std;

void Permutation(int step, int len, int a[])
{
	if(step == (len-1)) //当选取到最后一个元素时，输出当前排列
	{
		for(int i = 0; i < len; i++) cout<<a[i]<<" ";
		cout<<endl;
		return;
	}
	for(int i = step; i < len; i++)
	{
		int tmp; tmp = a[step]; a[step] = a[i]; a[i] = tmp; //交换a[i]和a[step]
		Permutation(step+1, len, a);
		tmp = a[step]; a[step] = a[i]; a[i] = tmp; //交换a[i]和a[step]
	}
}

int a[20];

int main()
{
	for(int i = 0; i < 6; i++) a[i] = i;
	Permutation(0, 6, a);
	return 0;
}