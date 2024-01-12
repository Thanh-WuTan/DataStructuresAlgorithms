/*
    https://lqdoj.edu.vn/problem/matong
    sử dụng thuật toán đếm số lượng ước trong O (N ^ 1/ 3)
    Note that, we have only these three cases for calculating factors of y since there can be at max two prime factors of y.
    If it would have had more than two prime factors, one of them would surely have been less than equal to n(1/3), 
    and hence it would be included in x and not in y.
    
    
*/

// i'm wutan
/*
    Who the hell you think you are?
    Comin' in to break my heart
    Make it like an operation
    But I ain't one of your patients
    Tired of this waiting and I don't play that game

    I can't get you off my brain
    So I'm poppin' pills for the pain
    And as soon as I go wake up
    You're caught up in your haze
    And I can't get out of my bed

    Tears are almost gone but the symptoms in my heart
    So tell me what's the cure for you (tell me now)
    'Cause boy you make me so confused
    (Boy you got me so confused)

    You got me feeling like a psycho, psycho
    Got me chasin' shots of NyQuil, NyQuil
    Without you got me dizzy and upset
    Got me so obsessed with you
    Look what you started
    I'm psycho, you're heartless
*/
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
#define PROB "matong"
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

const int N = 1e5 + 1;
int n, a[N], p[int(1e4) + 1];
unordered_map < int , bool > s;
vector < int > nt;

void readinp(){
    cin >> n;
    FU(i, 1, n) cin >> a[i];
}

pair<ll, ll> factor(ll n) {
    ll s = 0;
    while ((n & 1) == 0) {
        s++;
        n >>= 1;
    }
    return {s, n};
}

ll pow(ll a, ll d, ll n) {
    ll result = 1;
    a = a % n;
    while (d > 0) {
        if (d & 1) result = result * a % n;
        d >>= 1;
        a = a * a % n;
    }
    return result;
}

bool test_a(ll s, ll d, ll n, ll a) {
    if (n == a) return true;
    ll p = pow(a, d, n);
    if (p == 1) return true;
    for (; s > 0; s--) {
        if (p == n-1) return true;
        p = p * p % n;
    }
    return false;
}

bool miller(ll n) {
    if (n < 2) return false;
    if ((n & 1) == 0) return n == 2;
    ll s, d;
    tie(s, d) = factor(n-1);
    return test_a(s, d, n, 2) && test_a(s, d, n, 3)
        && test_a(s, d, n, 7 && test_a(s, d, n, 61)); 
}

int f(int m){
    if (m == 1) return 1;
    int ans = 1;

    FU(i, 0, sz(nt) - 1){
        ll t = ll(nt[i]);
        if (t * t * t > m) break;
        int cnt = 1;
        while (m % nt[i] == 0){
            m = m / nt[i];
            cnt++;
        }
        ans = ans * cnt;
    }
    if (miller(m)) ans*= 2;
    else if (s[m]) ans*= 3;
    else if (m != 1) ans*= 4;

    return ans;
}

void solve(){
    for (int i = 2; i <= 1e4; i++) if (!p[i]){
        for (int j = 2 * i; j <= 1e4; j+= i)
            p[j] = 1;
        nt.push_back(i);
        ll x = i;
        s[x * x] = 1;
    }
    ll res = 0;

    FU(i, 1, n){
        res = res + ll(a[i]) * ll(f(a[i]));
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
