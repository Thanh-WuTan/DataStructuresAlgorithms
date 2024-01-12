
/*

    F1 = A
    F2 = B
    FN = F(N-1) + F(N-2) + 1
    
    tính (F1) ^ k + (F2) ^ k + (F3) ^ k + .. + (FN) ^ k
    với k = 1 hoặc 2
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
#define PROB "LFib"
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

/*
    K = 1 :

    f[i+1] = f[i] + f[i-1] + 1
->  f[i-1] = f[i+1] - f[i] - 1

    f1 = f3 - f2 - 1
    f2 = f4 - f3 - 1
    f3 = f5 - f4 - 1
    ...
    f(n-1) = f(n+1) - fn - 1
    fn = f(n+2) - f(n+1) - 1

    sigma(f[i]) = f1 + f2 + f3 + f4 +.. + fn
                = f(n+2) - f2 - n

        -----------------------

    K = 2

 (1) :   f(n+1) = fn + f(n-1) + 1
    ->   f(n+1) - f(n-1) = fn + 1

       fn * f(n+1) - fn * f(n-1)
     = fn * (f(n+1) - f(n-1))
(1)  = fn * (fn + 1)
     = (fn) ^ 2 + fn

again: fn * (fn+1) - fn * f(n-1) = (fn) ^ 2 + fn


->>    (f2)^2 + f2 = f2 * f3 - f2 * f1
       (f3)^2 + f3 = f3 * f4 - f3 * f2
       (f4)^2 + f4 = f4 * f5 - f4 * f3
       ...
       (f(n-1))^2 + f(n-1) = f(n-1) * fn - f(n-1) * f(n-2)
       (fn)^2 + fn = fn * f(n+1) - fn * f(n-1)

sigma((fi)^2) = (f1)^2 + fn * f(n+1) - f2 * f1 - (sigma(fi) - f1)
              = (f1)^2 + fn * f(n+1) - f2 * f1 - (f(n+2) - f2 - n - f1)

*/

struct mt{
    ll a[3][3];
};
mt p, base;
ll f1, f2, n, k, m;

void readinp(){
    cin >> f1 >> f2 >> n >> k >> m;
}

ll mul(ll a, ll b){
    if (b == 0) return 0;
    if (b == 1) return a % m;
    ll t = mul(a, b / 2LL);
    t = (t + t) % m;
    if (b % 2) return (t + a) % m;
    return t;
}

void init(){
    p.a[0][0] = f1;
    p.a[1][0] = f2;
    p.a[2][0] = 1;
}

mt mul_mt(mt A, mt B){
    mt res;
    FU(i, 0, 2)
    FU(j, 0, 2){
        res.a[i][j] = 0;
        FU(k, 0, 2){
            ll t1 = A.a[i][k];
            ll t2 = B.a[k][j];
            res.a[i][j] = (res.a[i][j] + mul(t1, t2)) % m;
        }
    }
    return res;
}

mt poww(mt A, ll m){
    if (m == 1) return A;
    mt t = poww(A, m / 2LL);
    t = mul_mt(t, t);
    if (m % 2) return mul_mt(t, A);
    return t;
}

ll fib(ll N){
    if (N == 1) return f1;
    if (N == 2) return f2;
    mt t = poww(base, N - 1);
    ll res = 0LL;
    FU(i, 0, 2) {
        ll t1 = t.a[0][i];
        ll t2 = p.a[i][0];
        res = (res + mul(t1, t2)) % m;
    }
    return res;
}

void solve(){
    init();

    base.a[0][0] = 0; base.a[0][1] = 1; base.a[0][2] = 0;
    base.a[1][0] = 1; base.a[1][1] = 1; base.a[1][2] = 1;
    base.a[2][0] = 0; base.a[2][1] = 0; base.a[2][2] = 1;

    if (k == 1){
        ll t1 = fib(n+2);
        ll t2 = fib(2);
        cout << (t1 - t2 - n + m * m) % m;
    }
    if (k == 2){
        ll t1 = mul(fib(1), fib(1));
        ll t2 = mul(fib(n), fib(n+1));
        ll t3 = mul(fib(2), fib(1));
        ll t4 = (fib(n+2) - fib(2) - n - fib(1) + m * m) % m;
        cout << (t1 + t2 - t3 - t4 + m * m) % m;
    }
    EL;
}

int main(){
    setIO();
    int t = 1;
    cin >> t;
    while (t--){
        readinp();
        solve();
    }
}
