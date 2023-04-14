/*

  task : 
  cho N số nguyên a[1], a[2], a[3], .. , a[n]
  F(l, r) = gcd(a[l], a[l + 1], .. , a[r-1], a[r])
  yêu cầu tính tất cả F(l, r) với (1 <= l <= r <= n)
  
  https://vn.spoj.com/problems/VOGCDSUM/
*/

/*
  giải :
    Ý tưởng trâu : duyệt tất cả cập l <= r O( N * N / 2 ) để tính F(l, r)
    Cải tiến từ ý tưởng trâu : nhận thấy F(l, r) = F(l, p) với r <= p, mình sẽ nhảy cóc nhay từ r đến p bằng mảng p[i][j]
    trong đó p[i][j] sẽ là gcd của các phần tử liên tiếp độ dài 2 ^ j bắt đầu từ i
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
#define PROB "VOGCDSUM"
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
const ll mod = 1e9 + 7;
int n; ll a[N], p[N][18];

void readinp(){
    cin >> n;
    FU(i, 1, n) cin >> a[i];
}


ll add (ll a, ll b){
    return (a + b) % mod;
}

ll mul (ll a, ll b){
    if (b == 0) return 0;
    if (b == 1) return a % mod;
    ll t = mul(a, b / 2LL);
    t = add(t, t);
    if (b % 2) return add(t, a);
    return t;
}

ll get(int l, int r){
    ll ret = 0LL;
    FD(k, 17, 0){
        int j = r - (1 << k) + 1;
        if (j >= l){
            ret = __gcd(ret, p[j][k]);
            r = j - 1;
        }
        if (r < l) break;
    }
    return ret;
}

void solve(){
    FU(i, 1, n) p[i][0] = a[i];
    FU(k, 1, 17) FU(i, 1, n){
        ll t1 = p[i + (1 << k-1)][k-1];
        ll t2 = p[i][k-1];
        p[i][k] = __gcd(t1, t2);
    }

    ll res = 0;
    FU(i, 1, n){
        int j = i;
        ll T = a[i];
        while (j <= n){
            int pos = j;
            ll Y = T;
            FD(k, 17, 0){
                int q = pos + (1 << k);
                if (q > n) continue;
                ll X = __gcd(Y, p[pos + 1][k]);
                if (X != T) continue;
                Y = T;
                pos = q;
            }
            res = add(res, mul(T, ll(pos - j + 1)));
            j = pos + 1;
            T = __gcd(T, a[j]);
            if (T == 1 && j <= n){
                res = add(res, mul(T, n - j + 1));
                break;
            }
        }
    }
    cout << res;
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

