// 具体转换方式:\\
// 1.从左到右进行遍历\\
// 2.运算数,直接输出.\\
// 3.左括号,直接压入堆栈,(括号是最高优先级,无需比较)(入栈后优先级降到最低,确保其他符号正常入栈)\\
// 4.右括号,(意味着括号已结束)不断弹出栈顶运算符并输出直到遇到左括号(弹出但不输出)\\
// 5.运算符,将该运算符与栈顶运算符进行比较,\\
// 如果优先级高于栈顶运算符则压入堆栈(该部分运算还不能进行),\\
// 如果优先级低于等于栈顶运算符则将栈顶运算符弹出并输出,然后比较新的栈顶运算符.\\
// (低于弹出意味着前面部分可以运算,先输出的一定是高优先级运算符,等于弹出是因为同等优先级,从左到右运算)\\
// 直到优先级大于栈顶运算符或者栈空,再将该运算符入栈.\\
// 6.如果对象处理完毕,则按顺序弹出并输出栈中所有运算符.\\



// 注意负数的情况。 \\

// 1.如果“-”出现在表达式首位并且后面紧跟数字，那么此时为负号，合并到后面的数字字符串里去\\

// 2.如果“-”出现在表达式首位并且后面紧跟左括号，那么此时为负号，在整个字符串数组首位插个0处理成 “ 0 - （xxx）”的形式\\
// 所以在前面加个 0， \\
// 3.如果“-”出现在左括号后面，那么此时为负号，合并到后面的数字字符串中 ，\\
// 这个情况特判前面是不是左括号。 \\


// 如果减号前面是数字，或者是右括号。那么它就是减号，否则它就是负号。 下面的程序用的是这个方法。\\


// (-1)-(-1)  = 0 n\\


#include<bits/stdc++.h>
#define LL long long
const int MOD=1000000007;
const int N=10000+5;
using namespace std;
int n,m;
void dbg() {cout << endl;}
template<typename T, typename... A> void dbg(T a, A... x) {cout << a << ' '; dbg(x...);}
#define logs(x...) {cout << #x << " -> "; dbg(x);}
stack<int> s1;//操作数栈
stack<char> s2;//运算符栈
int lev(char x)//运算符优先级
{
    if(x=='+'||x=='-')
        return 1;
    if(x=='*'||x=='/')
        return 2;
    if(x=='^')
        return 3;
    return 0;
}
 
void calculate(stack<int> &s1,stack<char> &s2)//弹出栈顶元素并计算
{
    /*取出后弹出栈*/
    int y=s1.top();
        s1.pop();
    int x=s1.top();
        s1.pop();
    char z=s2.top();
        s2.pop();
 
    /*根据运算符计算,并压入栈*/
    if(z=='+')
        s1.push(x+y);
    if(z=='-')
        s1.push(x-y);
    if(z=='*')
        s1.push(x*y);
    if(z=='/')
        s1.push(x/y);
    if(z=='^')
        s1.push(pow(x,y));
}
 

char str[1000000];
int sum[1000000];
 
int main(){
 
    scanf("%s",str+1);
    n=strlen(str+1);
 	str[0] = '#';
    stack<int> s1;
    stack<char> s2;
    int temp=0;
    int sign = 1;
    bool flag=false;
    for(int i=1;i<=n;i++){
        if('0'<=str[i]&&str[i]<='9'){ //判断当前字符是否为数字
            temp = temp * 10 + str[i] - '0';
            flag=true;
        }
        else {
            if(flag){
                s1.push(temp*sign);
                temp=0;
                sign = 1;
                flag=false;
            }
            if (str[i] == '-'){
            	if (str[i-1] != ')' && !isdigit(str[i-1])) {
            		sign = -1;
            		continue;
            	}
            }
            if(str[i]=='(') {
                s2.push(str[i]);
                continue;
            }
            if(str[i]==')') {
                while(s2.top()!='(')
                    calculate(s1,s2);
                s2.pop();
                continue;
            }
            while(!s2.empty()&&lev(s2.top())>=lev(str[i]))//优先级判断, 栈中的优先级大于当前的，要先计算栈中的。
                calculate(s1,s2);
            s2.push(str[i]);//运算符入栈
        }
    }
    if(flag){
        s1.push(temp*sign);
        temp=0;
        flag=false;
    }
    while(!s2.empty())
        calculate(s1,s2);
    cout<<s1.top()<<endl;
    return 0;
}

                    
