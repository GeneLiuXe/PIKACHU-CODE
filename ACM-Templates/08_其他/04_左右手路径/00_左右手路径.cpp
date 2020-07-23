#include <bits/stdc++.h>
#define __ ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
typedef long long ll;
typedef double db;
const int N = 1e5+100;
const int M = 1e5+100;
const db EPS = 1e-9;
using namespace std;

//左手路径, 优先序循环为顺时针, 第一个优先选择为前一个方位
//右手路径, 优先序循环为逆时针, 第一个优先选择为后一个方位
string choice[4] = {"UP","RIGHT","DOWN","LEFT"},op;
int now = 2;

int main()
{
	//右手路径, 初始 now = "DOWN"
	while(1){
		rep(i,0,3){
			string next = choice[(now-i+5)%4];
			cout << "LOOK " << next << endl;
			cin >> op;
			if(op == "SAFE"){
				cout << "GO " << next << endl;
				cin >> op;
				now = (now-i+5)%4;
				if(op == "YES") exit(0);
				break;
			}
		}
	}

	//左手路径, 初始 now = "RIGHT"
	// while(1){
	// 	rep(i,0,3){
	// 		string next = choice[(now+i+3)%4];
	// 		cout << "LOOK " << next << endl;
	// 		cin >> op;
	// 		if(op == "SAFE"){
	// 			cout << "GO " << next << endl;
	// 			cin >> op;
	// 			now = (now+i+3)%4;
	// 			if(op == "YES") exit(0);
	// 			break;
	// 		}
	// 	}
	// }
	return 0;
}