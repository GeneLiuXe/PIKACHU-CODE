#include<iostream>
#include<cstdio>
using namespace std;
struct{int l,r,size,dat,bit,add,rev;}a[200010];
int L[200010],R[200010],n,m,tot,root,i,x,y,z;
char str[10];
//size-子树大小, dat-节点真实值, bit-子树中最小值, add-子树区间加标记, rev-子树翻转标记

void spreadadd(int x,int y)
{
	if(!x||!y) return;
	a[x].add+=y; a[x].dat+=y; a[x].bit+=y;
}
//add是lazy标记

void spreadrev(int x)
{
	if(!a[x].rev) return;
	swap(a[x].l,a[x].r);
	if(a[x].l) a[a[x].l].rev^=1; //翻转标记下传
	if(a[x].r) a[a[x].r].rev^=1;
}

void spread(int x)
{
	spreadadd(a[x].l,a[x].add); //标记下传
	spreadadd(a[x].r,a[x].add);
	spreadrev(x); //rev表示是否要翻转
	a[x].add=a[x].rev=0;	
}

inline void update(int x) //更换左右儿子后, 更新其siz及bit
{
	a[x].size=a[a[x].l].size+a[a[x].r].size+1;
	a[x].bit=min(a[x].dat,min(a[a[x].l].bit,a[a[x].r].bit));
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

void INSERT(int x,int y) //将y插入到a[x]之后
{
	splay(root,x); //令x为根
	a[++tot].dat=y; a[tot].r=a[root].r; a[root].r=tot; //令y为x的右儿子, x原右儿子为y右儿子
	update(tot); update(root);
}

void DELETE(int x) //删除数组中第x个数
{
	splay(root,x); //将数组中第x个数旋转到根
	splay(a[root].r,1); //将子树中最小的数转到根, 则根节点的右儿子没有左儿子
	a[a[root].r].l=a[root].l; root=a[root].r;
	update(root);
}

void REVERSE(int x,int y)
{
	splay(root,y+1);
	splay(a[root].l,x-1);
	a[a[a[root].l].r].rev^=1; //子树中所有点都左右翻转
	//此处的操作与add处不同，原因在于区间反转不会导致该区间维护的min发生变化，因此可以这样操作
	//但是正常打标记的操作应该是与ADD函数一致
}

void REVOLVE(int x,int y,int z) //将[x,y-len]与[y-len+1,y]互换位置, len为z%(y-x+1)
{
	z%=y-x+1;
	if(!z)return;
	int mid=y-z; //找到右区间的第一个数
	splay(root,mid); //将mid转到根
	splay(a[root].l,x-1); //根左儿子的右儿子对应区间[x,y-len] (不包括根左儿子的右儿子)
	splay(a[root].r,y-a[a[root].l].size); //根右儿子的左儿子对应区间[y-len+2,y] (不包括根右儿子的左儿子)
	z=a[root].l;
	a[root].l=a[z].r;
	a[z].r=a[a[root].r].l;
	a[a[root].r].l=0;
	update(z); update(a[root].r); update(root);
	splay(root,1); //空出根的左儿子
	a[root].l=z;
	update(root);
}

void MIN(int x,int y)
{
	splay(root,y+1); //将根节点中第y+1小的数转为根
	splay(a[root].l,x-1); //将根节点左儿子子树中第x-1小的数转为根节点左儿子
	printf("%d\n",a[a[a[root].l].r].bit);
}

// splay(root,n+2);
// splay(a[root].l,0);
// Print(a[a[root].l].r);
void Print(int x){ //打印区间[1,n]的数, 输出之前要先把这个区间的数旋转到一个子树中
	spread(x); //标记下传
	if(a[x].l) Print(a[x].l);
	printf("%d ",a[x].dat); //中序遍历
	if(a[x].r) Print(a[x].r);
}

int main()
{ 
	cin>>n;
	a[1].size=1; a[n+2].l=n+1; a[n+2].size=n+2;
	a[1].dat=a[n+2].dat=a[1].bit=a[0].bit=0x3fffffff;
	//所有数右移一格，2表示1，n+1表示n
	for(i=2;i<=n+1;i++) //一开始构建出一根链
	{
		scanf("%d",&a[i].dat); //dat为真实值
		a[i].l=i-1; a[i].size=i; //l表示左儿子，siz表示子树大小
		a[i].bit=min(a[i-1].bit,a[i].dat);  //bit表示最小值
	}
	a[n+2].bit=a[n+1].bit;
	root=tot=n+2; //一共n+2个节点，加了两个前驱和后续
	cin>>m;
	for(i=0;i<m;i++)
	{
		scanf("%s",&str);
		//ADD x y z 区间[x,y]每个数+z
		if(str[0]=='A') {scanf("%d%d%d",&x,&y,&z); ADD(++x,++y,z);}
		//INSERT x y 在序列第x个数之后插入y
		if(str[0]=='I') {scanf("%d%d",&x,&y); INSERT(++x,y);}
		//DELETE x 删除序列第x个数
		if(str[0]=='D')	{scanf("%d",&x); DELETE(++x);}
		//MIN x y 查询区间[x,y]的最小值
		if(str[0]=='M') {scanf("%d%d",&x,&y);	MIN(++x,++y);}	
		//REVERSE x y 将区间[x,y]中的数翻转，如原序列1 2 3 4 5，REVERSE 3 5，得到1 2 5 4 3	
		if(str[0]=='R'&&str[3]=='E') {scanf("%d%d",&x,&y);	REVERSE(++x,++y);}
		//REVOLVE x y z 将区间[x,y]旋转z次，即将最后一个数移到第一个位置，如原序列1 2 3 4 5，REVOLVE 2 4 2，得到1 3 4 2 5
		//区间左右旋转可以互相转换 
		if(str[0]=='R'&&str[3]=='O') {scanf("%d%d%d",&x,&y,&z);	REVOLVE(++x,++y,z);}		
	}
	return 0;
}