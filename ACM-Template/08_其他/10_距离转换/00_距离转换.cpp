// ---
// 欧几里得距离：$\sqrt{(x_1-x_2)^2+(y_1-y_2)^2}$ \\
// 曼哈顿距离：$|x_1-x_2|+|y_1-y_2|$ \\
// 切比雪夫距离：$max(|x_1-x_2|,|y_1-y_2|)$ \\
// \\
// 以原点为中心点，曼哈顿距离为 a 的点构成了一个正方形，四个顶点分别为 (0,a)、(0,-a)、(a,0)、(-a,0) \\
// 以原点为中心点，切比雪夫距离为 a 的点构成了一个正方形，四个顶点分别为 (a,a)、(-a,-a)、(a,-a)、(-a,a) \\
// \\
// 曼哈顿距离 $\rightarrow $ 切比雪夫距离：$(x, y) \rightarrow (x+y, x-y)$ \\
// 切比雪夫距离 $\rightarrow $ 曼哈顿距离：$(x, y) \rightarrow (\displaystyle\frac{x+y}{2}, \displaystyle\frac{x-y}{2})$\\
// ---
#include <bits/stdc++.h>
using namespace std;

pair<int, int> convert1(int x, int y) {
	return {x+y, x-y};
}

pair<int, int> convert2(int x, int y) {
	return {(x-y)/2, (x+y)/2};
}
