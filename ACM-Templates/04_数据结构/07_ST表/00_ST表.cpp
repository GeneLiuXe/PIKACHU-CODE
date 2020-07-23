int st[N][25];

//数组范围为[0,len-1]
void init(){
    for(int i = 0; i < len; i++) st[i][0] = a[i];
    for(int j = 1; (1<<j) <= len; j++){
        for(int i = 0; i + (1<<j) - 1 < len; i++)
            st[i][j] = min(st[i][j-1],st[i+(1<<(j-1))][j-1]);
    }
}

//求[l,r]之间的最小值
int query(int l,int r){
    int k = (int)(log((double)(r - l + 1)) / log(2.0));
    return min(st[l][k],st[r-(1<<k)+1][k]);
}