/*
    cho M  đoạn thẳng đoạn thẳng thứ i có đầu trái là S[i], đầu phải là E[i]
     giá trị của đoạn thẳng i ở vị trí S[i] là V[i], 
     ở vị trí S[i] + 1 sẽ là V[i] + 1 * D[i]
     ở vị trí S[i] + 2 sẽ là V[i] + 2 * D[i] 
     ở vị trí E[i] sẽ là V[i] + (E[i] - S[i]) * D[i]
    
     hỏi ở vị trí i ( i = 1..N) đoạn thẳng nào có giá trị lớn nhất và là bao nhiêu
     https://codeforces.com/group/FLVn1Sc504/contest/274814/problem/X
*/

/*

  giải : xem cmt trong code để hiểu thêm
*/

// i'm wutan
#include <bits/stdc++.h>
#define EL cout<<'\n'
#define pli pair<ll,int>
#define pll pair<ll,ll>
#define pii pair<int,int>
#define pb(x) push_back(x)
#define fi first
#define se second
#define sz(a) int(a.size())
#define FU(x,a,b) for(int x=int(a);x<=int(b);x++)
#define FD(x,a,b) for(int x=int(a);x>=int(b);x--)
#define PROB "JEWELNB"
using namespace std;
typedef long long ll;
typedef double db;


template <typename T>
inline void read(T& x){
    bool Neg = false;
    char c;
    for (c = getchar(); c < '0' | c > '9'; c = getchar())
        if (c == '-') Neg = !Neg;
    x = c - '0';
    for (c = getchar(); c >= '0' && c <= '9'; c = getchar())
        x = x * 10 + c - '0';
    if (Neg) x = -x;
}

template <typename T>
inline void write(T x){
    if (x < 0)
        putchar('-'); x = -x;
    T p = 1;
    for (T temp = x / 10; temp > 0; temp /= 10) p *= 10;
    for (; p > 0; x %= p, p /= 10) putchar(x / p + '0');
}


void setIO() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    if(fopen(PROB".inp", "r")){
        freopen(PROB".inp", "r",stdin);
        freopen(PROB".out", "w",stdout);
    }
}

const int N = 2e5 + 2;
struct jew{
    int s, e, v, d;
}; jew a[N];
pll st[N * 4];
int n, m;

void readinp(){
    cin >> n >> m;
    FU(i, 1, m) cin >> a[i].s >> a[i].e >> a[i].v >> a[i].d;
}

ll getY(pll line, ll X){
    return line.fi * X + line.se;
}

void upd (int id, int l, int r, int u, int v, pll g){
    if (r < u || v < l) return;
    int m = (l + r) / 2;
    if (u <= l && r <= v){
        /// st[id] nam o tren g
        if (getY(st[id], l) >= getY(g, l) && getY(st[id], r) >= getY(g, r))
            return;
        /// st[id] nam o duoi g
        if (getY(st[id], l) <= getY(g, l) && getY(st[id], r) <= getY(g, r)){
            st[id] = g;
            return;
        }

        /// l -> m : st[id] nam tren g
        if (getY(st[id], l) >= getY(g, l) && getY(st[id], m) >= getY(g, m)){
            upd(id * 2 + 1, m + 1, r, u, v, g);
            return;
        }

        /// l -> m : st[id] nam duoi g,
        /// down st[id] xuong m+1 -> r
        /// gan g vao st[id]
        if (getY(st[id], l) <= getY(g, l) && getY(st[id], m) <= getY(g, m)){
            upd(id * 2 + 1, m + 1, r, u, v, st[id]);
            st[id] = g;
            return;
        }

        /// m + 1 -> r : st[id] nam tren g
        if (getY(st[id], m) >= getY(g, m) && getY(st[id], r) >= getY(g, r)){
            upd (id * 2, l, m, u, v, g);
            return;
        }

        if (getY(st[id], m) <= getY(g, m) && getY(st[id], r) <= getY(g, r)){
            upd(id * 2, l, m, u, v, st[id]);
            st[id] = g;
            return;
        }
    }
    upd(id * 2, l, m, u, v, g);
    upd(id * 2 + 1, m + 1, r, u, v, g);
}

ll get(int id, int l, int r, int p){
    if (l > p || r < p) return -1e18;
    ll ans = getY(st[id], p);
    if (l == r) return ans;
    int m = (l + r) / 2;
    ans = max(ans, get(id * 2, l, m, p));
    ans = max(ans, get(id * 2 + 1, m + 1, r, p));
    return ans;
}

void solve(){

    /// y = (X - s) * d + v
    /// y = X * d - s * d + v
    FU(i, 1, n * 4) st[i] = {0LL, 0LL};
    FU(i, 1, m){
        ll A = 1LL * a[i].d;
        ll B = -1LL * a[i].s * 1LL * a[i].d + 1LL * a[i].v;
        upd(1, 1, n, a[i].s, a[i].e, {A, B});
    }

    FU(i, 1, n)
        cout << get(1, 1, n, i) ,EL;

}

int main(){
    setIO();
    int t = 1;
//    cin >> t;
    while (t--){
        readinp();
        solve();
    }
}
