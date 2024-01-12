/*
    tóm tắt d(i) là số lượng ước số nguyên dương của i
    f(n) = cnt
    trong đó cnt được tính như sau :
    for (int i = 1; i <= n; i++)
        if (i là ước của n)
            cnt+= d(i);

    yêu cầu tính f(n!) với n <= 1e6

            -------------

    d(n) là hàm nhân tính
 -> F(n) cũng là hàm nhân tính
    F(n) = tích tất cả (a[i] + 1) * (a[i] + 2) / 2
    trong đó n = (p[1]^a[1]) x (p[2]^a[2]) x .. x (p[k]^a[k])
    với p[i] là thừa số nguyên tố thứ i của n

    với F(n!)
    xét từng snt từ 1->n
    ta cần đếm số mũ của p[i] trong n! bằng công thức :
    [n/p] + [n/p^2] + [n/p^3] + .. + [n/p^k]
    với N < p^(k+1)

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
#define PROB "dividivi"
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

const int N = 1e6 + 5;
const ll mod = 1e7 + 7;
vector < int > nt;
int n; bool p[N]{0};
ll f[N];
void readinp(){
    cin >> n;
    if (!n) exit(0);
}

ll mul(ll a, ll b){
    return ((a % mod) * (b % mod)) % mod;
}

ll poww (ll a, ll b){
    if (b == 0) return 1;
    if (b == 1) return a % mod;
    ll t = poww(a, b / 2LL);
    t = mul(t, t);
    if (b % 2) return mul(t, a);
    return t;
}

void solve(){
    if (f[n]) {
        cout << f[n],EL;
        return;
    }

    ll res = 1LL;
    for (auto i : nt){
        if (i > n) break;
        ll u = i;
        ll cnt = 0;
        while (u <= n){
            cnt+= ll(n / u);
            u = u * ll(i);
        }
        res = res * (cnt + 1) * (cnt + 2) / 2 % mod;
    }
    f[n] = res;
    cout << f[n],EL;
}

int main(){
    setIO();
    FU(i, 2, sqrt(1e6)) if (!p[i])
        for (int j = i * i; j <= 1e6; j+= i) p[j] = true;
    FU(i, 2, 1e6) if (!p[i]) nt.push_back(i);

    int t = 1;
    while (1){
        readinp();
        solve();
    }
}
