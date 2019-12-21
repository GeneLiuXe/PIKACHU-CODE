// ---
// Replace a b c, 将[a,b]中所有括号改为c
// Swap a b, 将[a,b]中所有字符翻转
// Invert a b, 将[a,b]中所有'('变为')', ')'变为'('
// Query a b, 询问[a,b]之间的字符至少要改变多少位才能变成合法的括号序列
// ---
#include <iostream>
#include <cstdio>
#define rep(i,a,b) for(int i = a; i <= b; i++)
const int N = 1e5+100;
using namespace std;
struct Node{int l,r,size,dat,add,rev,res,l1,l2,r1,r2,sum;}a[N];
int L[N],R[N],n,m,tot,root;
char str[10];
//size-子树大小, dat-节点真实值, add-子树区间赋值标记, rev-子树翻转标记, res-子树取反标记
//l1、l2-最大、小前缀和，r1、r2-最大、小后缀和
void spreadadd(int x,int y)
{
	if(!x||!y) return;
	a[x].add=y; a[x].dat=y;
	a[x].rev = 0, a[x].res = 0;
	if(y < 0) a[x].sum = -a[x].size;
	else a[x].sum = a[x].size;
	a[x].r1 = a[x].l1 = (y==1?a[x].sum:0);
	a[x].r2 = a[x].l2 = (y==1?0:a[x].sum);
}
//add是lazy标记

void spreadrev(int x) //翻转
{
	if(!x) return;
	swap(a[x].l1,a[x].r1);
	swap(a[x].l2,a[x].r2);
	swap(a[x].l,a[x].r);
	a[x].rev ^= 1;
}
 
void spreadres(int x) //取反
{
	if(!x) return;
	swap(a[x].l1,a[x].l2);
	swap(a[x].r1,a[x].r2);
	a[x].sum = -a[x].sum; a[x].dat = -a[x].dat;
	a[x].l1 = -a[x].l1, a[x].l2 = -a[x].l2;
	a[x].r1 = -a[x].r1, a[x].r2 = -a[x].r2;
	a[x].res ^= 1;
}

void spread(int x)
{
	if(a[x].rev){
		spreadrev(a[x].l); 
		spreadrev(a[x].r);
	}
	if(a[x].add){
		spreadadd(a[x].l,a[x].add); //标记下传
		spreadadd(a[x].r,a[x].add);
	}
	if(a[x].res){
		spreadres(a[x].l);
		spreadres(a[x].r);
	}
	a[x].add=a[x].rev=a[x].res=0;	
}

inline void update(int x)
{
	int L = a[x].l, R = a[x].r;
	a[x].size=a[L].size+a[R].size+1;
	a[x].sum = a[L].sum+a[R].sum+a[x].dat;
	a[x].l1 = max(a[L].l1,a[L].sum+a[R].l1+a[x].dat);
	a[x].l2 = min(a[L].l2,a[L].sum+a[R].l2+a[x].dat);	
	a[x].r1 = max(a[R].r1,a[R].sum+a[L].r1+a[x].dat);
	a[x].r2 = min(a[R].r2,a[R].sum+a[L].r2+a[x].dat);
}

void turnleft(int &x) //x左旋, 即x的右儿子左旋到x位置
{
	int y=a[x].r; a[x].r=a[y].l; a[y].l=x;
	update(x); update(y);	x=y;
}

void turnright(int &x) //x右旋, 即x的左儿子右旋到x位置
{
	int y=a[x].l; a[x].l=a[y].r; a[y].r=x;
	update(x); update(y);	x=y;
	//先更新x, 因为x是儿子
}

void splay(int &x,int y) //将x子树中第y小的数转为根
{
	if(!x) return;
	L[0]=R[0]=0; //L存储了y转到x之后, 左子树中的各个节点
	//R存储y转到x之后，右子树中的各个节点
	while(1)
	{
		//lazy、旋转标记下传
		spread(x),spread(a[x].l),spread(a[x].r);
		int temp=a[a[x].l].size+1; //<=x的数的个数
		//完成了目标
		if(y==temp||(y<temp&&!a[x].l)||(y>temp&&!a[x].r)) break;
		if(y<temp) //让y的左儿子旋转上来
		{
			if(a[a[x].l].l&&y<=a[a[a[x].l].l].size) {turnright(x); if(!a[x].l) break;}
			R[++R[0]]=x; x=a[x].l;
		}
		else //让y的右儿子旋转上来
		{
			y-=temp;
			int temp=a[a[a[x].r].l].size+1;
			if(a[a[x].r].r&&y>temp) {y-=temp; turnleft(x); if(!a[x].r) break;}
			L[++L[0]]=x; x=a[x].r;
		}
	}
	L[++L[0]]=a[x].l; R[++R[0]]=a[x].r; //此处的x已经成为y所对应的节点
	//将左右子树一一连接
	for(int i=L[0]-1;i>0;i--) {a[L[i]].r=L[i+1]; update(L[i]);}
	for(int i=R[0]-1;i>0;i--) {a[R[i]].l=R[i+1]; update(R[i]);}
	a[x].l=L[1]; a[x].r=R[1]; update(x);
}

void ADD(int x,int y,int z)
{
	splay(root,y+1); //把后继转到树根
	splay(a[root].l,x-1); //把前驱转到树根左儿子
	spreadadd(a[a[root].l].r,z); //区间+, 打lazy标记
}

void SWAP(int x,int y)
{
	splay(root,y+1);
	splay(a[root].l,x-1);
	spreadrev(a[a[root].l].r);
}

void Invert(int x,int y)
{
	splay(root,y+1);
	splay(a[root].l,x-1);
	spreadres(a[a[root].l].r);
}

void Query(int x,int y)
{
	splay(root,y+1);
	splay(a[root].l,x-1);
	int x1 = a[a[a[root].l].r].l2;
	int x2 = a[a[a[root].l].r].r1;
	x1 = -x1;
	int ans = ((x1+1)/2)+((x2+1)/2); //左端最小前缀和/2上取整+右端最大前缀和上取整
	//abs(左端最小前缀和)=x,表示左端至少有x个'('未匹配
	//abs(右端最大前缀和)=x,表示右端至少有x个')'未匹配
	printf("%d\n",ans);
}

char s[N];

int main()
{ 
	cin>>n>>m;
	a[1].size=1; a[n+2].l=n+1; a[n+2].size=n+2;
	a[1].dat = a[n+2].dat; update(1);
	scanf("%s",s+1);
	rep(i,2,n+1) a[i].dat = (s[i-1]=='('?1:-1);
	rep(i,2,n+2)
	{
		a[i].l=i-1; a[i].size=i; //l表示左儿子，siz表示子树大小
		update(i);
	}
	root=tot=n+2; //一共n+2个节点，加了两个前驱和后续
	rep(i,1,m)
	{
		int x,y,z; char op[5];
		scanf("%s",str);
		if(str[0]=='R'){
			scanf("%d%d",&x,&y); scanf("%s",op);
			if(op[0] == '(') z = 1;
			else z = -1;
			ADD(++x,++y,z);
		}
		else if(str[0]=='S'){
			scanf("%d%d",&x,&y);
			SWAP(++x,++y);
		}
		else if(str[0] == 'I'){
			scanf("%d%d",&x,&y);
			Invert(++x,++y);
		}
		else if(str[0] == 'Q'){
			scanf("%d%d",&x,&y);
			Query(++x,++y);
		}
	}
	return 0;
}