// i'm wutan
#include <bits/stdc++.h>
#define EL cout<<endl
#define pli pair<ll,int>
#define pll pair<ll,ll>
#define pii pair<int,int>
#define pil pair<int,ll>
#define can(n, q) round(exp(log(n) / q))
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define sz(a) int(a.size())
#define FU(x,a,b) for(int x=int(a);x<=int(b);x++)
#define FD(x,a,b) for(int x=int(a);x>=int(b);x--)
#define PROB "t"

using namespace std;
/*
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;

// ordered_set name_of_set;
// int pos = *name_of_set.find_by_order(int(value));
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
const int M = 4;
int n, a[1 << M];
int dp[2][1 << M];

void readinp(){
    cin >> n;
    FU(i, 1, n) cin >> a[i];
}

void cnt1(){ /// dp[s] = number of a[i] such that : a[i] is a subset of s (a[i] & s = a[i])
    FU(i, 1, n) dp[0][a[i]]++;
    FU(i, 0, M - 1){
        FU(s, 0, (1 << M) - 1){
            int bit = (s >> i) & 1;
            dp[1][s] = dp[0][s];
            if (bit) dp[1][s] = dp[1][s] + dp[0][s ^ (1 << i)]; /// the difference is here
        }
        FU(s, 0, (1 << M) - 1){
            dp[0][s] = dp[1][s];
            dp[1][s] = 0;
        }
    } 
}

void cnt2(){ /// dp[s] = number of a[i] such that : s is a subset of a[i] (a[i] & s = s)
    FU(i, 1, n) dp[0][a[i]]++;
    FU(i, 0, M - 1){
        FU(s, 0, (1 << M) - 1){
            int bit = (s >> i) & 1;
            dp[1][s] = dp[0][s];
            if (!bit) dp[1][s] = dp[1][s] + dp[0][s | (1 << i)]; /// the difference is here
        }
        FU(s, 0, (1 << M) - 1){
            dp[0][s] = dp[1][s];
            dp[1][s] = 0;
        }
    } 
}

void solve(){
  
    cnt1();
    cn2t();
    
}

int main(){
    setIO();
    int t = 1;
//    cin >> t;
    while (t--){
        readinp();
        solve();
    }
    return 0;
}
