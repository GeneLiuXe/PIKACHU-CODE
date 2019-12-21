int c[N];

inline int lowbit(int x) { return x&(-x); }

inline void update(int x, int c){
	for(int i = x; i <= k; i += lowbit(i)) t[i] += c;
}

inline int ask(int x){
	int tp = 0;
	for(int i = x; i; i -= lowbit(i)) tp += t[i];
	return tp;
}