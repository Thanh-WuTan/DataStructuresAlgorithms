/*
  tóm tắt : đếm xem có bao nhiêu đoạn [L,R] : a[L] + a[R] + min(a[L], a[L+1], ..,  a[R]) <= m
https://www.hackerearth.com/practice/data-structures/advanced-data-structures/segment-trees/practice-problems/algorithm/interval-counting-bf6ebe8a/
*/

/*
    L[i] : vị trí nhỏ nhất <= i sao cho a[i] là min trên đoạn [L[i], i]
    R[i] : vị trí lớn nhất >= i sao cho a[i] là min trên đoạn [i, R[i]]
    duyệt từng a[i], cho a[i] đóng vai trò làm min trên đoạn L[i], R[i], ( 2 chỉ số này có thể tìm bằng stack ) 
    như vậy ta cần đếm cặp (u, v) : (L[i] <= u <= i <= v <= R[i]) sao cho a[u] + a[v] <= K - a[i]
    chỉ có cách duyệt 1 trong 2 giá trị a[u] để đếm a[v] hoặc duyệt a[v] để đếm a[u]
    để tối ưu thì mình duyệt đoạn có độ dài nhỏ hơn , tức nếu i - L[i] < R[i] - i thì mình sẽ duyệt a[u], ngược lại duyệt a[v]
    cách đếm số lượng phần tử trong đoạn [L,R] có giá trị <= K có nhiều cách giải
    sau đây sẽ trình bày kĩ thuật IT + cấu trúc mảng động
    đã thử dùng trie nhưng lại quá bộ nhớ @@
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
#define PROB "Countingthenumberofintervals"
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

const int N = 5e5 + 2;
vector < ll > st[N * 4];
int n, l[N], r[N];
ll a[N], m;

void readinp(){
    cin >> n >> m;
    FU(i, 1, n) cin >> a[i];
}

void build (int id, int l, int r){
    if (l == r){
        st[id].push_back(a[l]);
        return;
    }
    int mid = (l + r) / 2;
    build(id * 2, l, mid);
    build(id * 2 + 1, mid + 1, r);
    st[id].resize(sz(st[id * 2]) + sz(st[id * 2 + 1]));
    merge(st[id * 2].begin(), st[id * 2].end(),
          st[id * 2 + 1].begin(), st[id * 2 + 1].end(),
          st[id].begin());
}

int cal (int id, int l, int r, int u, int v, ll k){
    if (r < u || v < l) return 0;
    if (u <= l && r <= v){
        return upper_bound(st[id].begin(), st[id].end(), k) - st[id].begin();
    }
    int mid = (l + r) / 2;
    return cal(id * 2, l, mid ,u ,v, k) + cal(id * 2 + 1, mid + 1, r, u, v, k);
}

void init(){
    build(1, 1, n);
    stack < int > st;
    FU(i, 1, n){
        while (!st.empty() && a[i] <= a[st.top()]) st.pop();
        l[i] = (st.empty()) ? 1 : st.top() + 1;
        st.push(i);
    }
    while (!st.empty()) st.pop();
    FD(i, n, 1){
        while (!st.empty() && a[i] < a[st.top()]) st.pop();
        r[i] = (st.empty()) ? n : st.top() - 1;
        st.push(i);
    }
}

ll cnt = 0;
void solve(){
    init();
    FU(i, 1, n){
        ll k = m - a[i];
      /// a[u] + a[v] + a[i] <= m
      /// -> a[u] <= m - a[i] - a[v]
        if (i - l[i] < r[i] - i)
            FU(u, l[i], i)
                cnt+= cal(1, 1, n, i, r[i], k - a[u]);

        else
            FU(v, i, r[i])
                cnt+= cal(1, 1, n, l[i], i, k - a[v]);

    }
    cout << cnt;

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
