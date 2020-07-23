#include <cstdio>

struct HighPrec{
    int L , A[10001];
};
inline HighPrec Init(){
    // 函数作用： 返回一个初值为 1 的高精度 
    HighPrec H;
    H.L = 1 , H.A[1] = 1;
    return H;
}
HighPrec Mul(HighPrec A , int k){
    // 函数作用： 高精度乘上单精度
    HighPrec H;
    for(int i = 1 ; i <= A.L ; i++)
        H.A[i] = A.A[i] * k; // 乘
    for(int i = 2 ; i <= A.L ; i++)
        H.A[i] += H.A[i - 1] / 10,
        H.A[i - 1] %= 10; // 进位
    H.L = A.L;
    while(H.A[H.L] > 10) // 看原先最高位能不能进位 
        H.A[H.L + 1] = H.A[H.L] / 10,
        H.A[H.L] %= 10, // 进位 
        H.L++; // 增加长度
    return H; 
}
HighPrec Div(HighPrec A , int k){
    // 函数作用： 高精度除以单精度
    HighPrec H;
    int t = 0;
    for(int i = A.L ; i >= 1 ; i--)
        t = t * 10 + A.A[i], // 小学竖式
        H.A[i] = t / k , t %= k;
    H.L = A.L;
    while(H.A[H.L] == 0)
        H.L--; // 看原先最高位有没有被除为 0 并不断退位 
    return H;
}
void OutPut(HighPrec A){
    // 函数作用：输出高精度
    for(int i = A.L ; i >= 1 ; i--)
        printf("%d" , A.A[i]); // 正常套路，逆序输出 
}

HighPrec T;
int n;

int main(){
    scanf("%d" , &n);
    T = Init(); // 赋初值 1
    // 下面的乘除是原公式简化的结果，大家请手动去简化 
    for(int i = n + 2 ; i <= n * 2 ; i++)
        T = Mul(T , i);
    for(int i = 1 ; i <= n ; i++)
        T = Div(T , i);
    OutPut(T); // 输出 
    return 0;
}