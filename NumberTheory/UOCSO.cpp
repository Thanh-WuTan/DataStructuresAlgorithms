// i'm wutan
#include <bits/stdc++.h>
#define EL cout<<endl
#define pli pair<ll,ll>
#define pll pair<ll,ll>
#define pii pair<ll,ll>
#define pil pair<ll,ll>
#define can(n, q) round(exp(log(n) / q))
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define sz(a) ll(a.size())
#define all(v) (v).begin(),(v).end()
#define FU(x,a,b) for(ll x=ll(a);x<=ll(b);x++)
#define FD(x,a,b) for(ll x=ll(a);x>=ll(b);x--)
#define PROB "2182"

using namespace std;
/*
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef tree<ll, null_type, less<ll>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;

// ordered_set name_of_set;
// ll pos = *name_of_set.find_by_order(ll(value));
*/
typedef long long ll;
typedef double db;

/*
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
inline void write(T x)
{
    if (x < 0)
    {
        putchar('-'); x = -x;
    }
    T p = 1;
    for (T temp = x / 10; temp > 0; temp /= 10) p *= 10;
    for (; p > 0; x %= p, p /= 10) putchar(x / p + '0');
}
*/

void setIO() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    if(fopen(PROB".inp", "r")){
        freopen(PROB".inp", "r",stdin);
        freopen(PROB".out", "w",stdout);
    }
}

const bool love = true;
const ll N = 1e5 + 2;
const ll md = 1e9 + 7;
ll m;
ll x[N], k[N];

void readinp(){
    cin >> m;
    FU(i, 1, m) cin >> x[i] >> k[i];
}

ll poww(ll a, ll b, ll mod){
    if (b == 0) return 1;
    if (b == 1) return a % mod;
    ll t = poww(a, b / 2, mod);
    t = (t * t) % mod;
    if (b % 2) return  (t * a) % mod;
    return t;
}

/*
  https://usaco.guide/problems/cses-2182-divisor-analysis/solution
  https://cses.fi/problemset/task/2182/
  cho N = x[i] ^ k[i]
  yêu cầu tính c1 = số lượng ước số của N
               c2 = tổng các ước số của N
               c3 = tích các ước số của N
  c1 = c1 * (k[i] + 1)
  
  c2 = c2 * ((pow(x[i], k[i] + 1) - 1) / (x[i] - 1))
  
  Let the product and number of divisors when only considering the first i prime factors be P[i] and C[i] respectively. The answer will be P_N
. P[i] = pow(P[i-1], k[i] + 1) * pow(pow(x[i], k[i] * (k[i] + 1) / 2), c[i-1])
  C[i] = (C[i-1] * (k[i] + 1)) % (Mod - 1)
  
*/
void solve(){
    ll c1 = 1, c2 = 1, c3 = 1, n = 1;
    FU(i, 1, m) n = (n * poww(x[i], k[i], md)) % md;
    FU(i, 1, m) c1 = (c1 * (k[i] + 1)) % md;
    FU(i, 1, m){
        ll t1 = (poww(x[i], k[i] + 1, md) - 1 + md) % md;
        ll t2 = (x[i] - 1 + md) % md;
        ll t3 = (t1 * poww(t2, md - 2, md)) % md;
        c2 = (c2 * t3) % md;
    }
    cout << c1 << ' ' << c2 << ' ';

    c1 = 1;
    FU(i, 1, m){
        ll t = poww(poww(x[i], (k[i] * (k[i] + 1) / 2), md), c1, md);
        c3 = (poww(c3, k[i] + 1, md) * t) % md;
        c1 = (c1 * (k[i] + 1)) % (md - 1);
    }
    cout << c3;
}

int main(){
    setIO();
    ll t = 1;
//    cin >> t;
    while (t--){
        readinp();
        solve();
    }
    return 0;
}
