// ---
// 题意：\\
// 给定一个前序遍历、后序遍历，求有多少个符合条件的中序遍历，即有多少个不同的二叉树？\\
// \\
// 思路：\\
// 本题的关键在于识别这样一对 (i, j) 满足，a[i] == a[j] 且 a[i+1] == a[j-1]，则 a[i] 可作为子树根节点，而 a[i+1] 可为左儿子，也可为右儿子，即总方案数 * 2。\\
// ---
#include <bits/stdc++.h>
using namespace std;

char a[30], b[30];

int main() {
	int ans = 1;
	scanf("%s%s", a, b);
	for(int i = 0; a[i]; i++)
		for(int j = 1; b[j]; j++)
			if(a[i] == b[j] && a[i+1] == b[j-1])
				ans *= 2;
	printf("%d\n", ans);
	return 0;
}