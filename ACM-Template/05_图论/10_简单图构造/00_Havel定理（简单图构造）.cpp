// ---
// 题意：\\
// 给出 $n$ 个点，再给出每个点的出度，问能否构造一个没有自环的简单图，如果可以判断构造是否唯一，输出构造方案。$(2\leq n\leq 100)$\\
// \\
// 思路：\\
// 运用 $Havel_Hakimi$ 定理，将点的度数排序，每次让度数最大的点向度数小的点连边，连完之后重新排序再次让度数最大的点向后连边，过程中不出现度数为负的点则可以构造。\\
// \\
// 然后问题就变成了如何判断是否有多种构造方式。我们只需在度数最大的点向后连边时，连最后一条边的时候查看后一个点的度数是否与该点度数相同，如果相同则说明有多种构造方案。\\
// ---
#include<stdio.h>
#include<string.h>
#include<algorithm>
using namespace std;
const int N=105;
struct node
{
    int d,id;
} e[N],e1[N]; //存点
int n,tot;
pair<int,int>p1[N*N],p2[N*N]; //存图的边
bool cmp(node a,node b)  //按度从大到小排序
{
    if(a.d==b.d) return a.id>b.id;  
    return a.d>b.d;
}
bool Havel_Hakimi()  //判断是否可图化
{
    tot=0;
    for(int i=0;i<n-1;i++)
    {
        sort(e+i,e+n,cmp);
        if(i+e[i].d>=n) return 0;
        for(int j=i+1;j<=i+e[i].d;j++)
        {
            p1[tot++]=make_pair(e[i].id,e[j].id); //记录边
            e[j].d--;
            if(e[j].d<0) return 0;
        }
    }
    if(e[n-1].d!=0) return 0;
    return 1;
} 
bool fun() //判断是否有其他成图方式
{
    int flag=0;
    int tot1=0;
    for(int i=0;i<n-1;i++)
    {
        sort(e1+i,e1+n,cmp);
        int tmp=e1[i].d+i; 
        if(tmp<n-1&&e1[tmp].d==e1[tmp+1].d&&e1[tmp].d!=0)
        {
            flag=1;
            swap(e1[tmp].id,e1[tmp+1].id);
        }
        for(int j=i+1;j<=tmp;j++)
            {
                p2[tot1++]=make_pair(e1[i].id,e1[j].id);
                e1[j].d--;
            }
    }
    return flag;
}
int main()
{
    while(~scanf("%d",&n))
    {
        int sum=0;
        for(int i=0; i<n; i++)
        {
            scanf("%d",&e[i].d);
            e1[i].id=e[i].id=i+1;
            e1[i].d=e[i].d;
            sum+=e[i].d;
        }
        if(sum%2) printf("IMPOSSIBLE\n"); //如果度数和为奇数一定不能成图，一条边连两个点
        else
        {
            if(!Havel_Hakimi()) printf("IMPOSSIBLE\n"); //不可图化
            else
            {
                if(fun()) //可图化且图不唯一，输出两个图的信息
                {
                    printf("MULTIPLE\n");
                    printf("%d %d\n",n,tot);
                    for(int i=0;i<tot;i++) 
                        printf("%d%c",p1[i].first,i==tot-1?'\n':' ');
                    for(int i=0;i<tot;i++)
                        printf("%d%c",p1[i].second,i==tot-1?'\n':' ');
                    printf("%d %d\n",n,tot);
                    for(int i=0;i<tot;i++)
                        printf("%d%c",p2[i].first,i==tot-1?'\n':' ');
                    for(int i=0;i<tot;i++)
                        printf("%d%c",p2[i].second,i==tot-1?'\n':' ');
                }
                else //图唯一
                {
                    printf("UNIQUE\n");
                    printf("%d %d\n",n,tot);
                    for(int i=0;i<tot;i++)
                        printf("%d%c",p1[i].first,i==tot-1?'\n':' ');
                    for(int i=0;i<tot;i++)
                        printf("%d%c",p1[i].second,i==tot-1?'\n':' ');
                    if(tot==0) printf("\n\n"); //特判图只有一个节点没有边，要输出两个换行
                }
            }
        }
    }
    return 0;
}