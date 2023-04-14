/*
    tóm tắt : cho cây gồm N đỉnh, ban đầu mỗi đỉnh có giá trị = 0
    yêu cầu thực hiện q truy vấn thuộc 1 trong 3 loại sau :
add v k l: Ta thay đổi giá trị các đỉnh thuộc cây con gốc v theo quy tắc sau:
        Giá trị của đỉnh v được tăng lên k.
        Giá trị các đỉnh là con trực tiếp của v được tăng lên k−l.
        Giá trị các đỉnh là con trực tiếp của con trực tiếp của v được tăng lên k−2⋅l.
        Giá trị các đỉnh là con trực tiếp của con trực tiếp của con trực triếp của v được tăng lên k−3⋅l...
get v: Tính giá trị của đỉnh v.
sum v: Tính tổng giá trị các đỉnh thuộc cây con gốc v.
*/


/*
    đối với những bài truy vấn trên cây trước hết để dễ xử lý
    ta thực hiện trải cây bằng cách đánh dấu thứ tự thăm và thứ tự đi ra
    với đỉnh u sẽ là cha của các nút v thuộc [tin[u]..tou[u]]

    h[u] = độ sâu của đỉnh u
    IT sẽ quản lý tổng các node trên đoạn

        truy vấn dạng add v k l:
+ ta sẽ xử lý truy vấn này với 2 mảng lazy được trình bày như sau :

    gọi tầng của v là tầng 0 có độ sâu là h[v]
    các đỉnh con trực tiếp của đỉnh v là tầng 1, có độ sâu là h[v] + 1
    ...
    các đỉnh con thuộc tầng thứ i của đỉnh v sẽ có độ sâu h[u] = h[v] + i hay h[u] - h[v] = i

    thế thì khi update subtree của v tức tăng tất cả đỉnh u thuộc đoạn tin[v].. tou[v]
    lên k - l * (h[u] - h[v]) = k + l * h[v] - l * h[u]
    khi đó giá trị của cả đoạn tin[v]..tou[v] sẽ được tăng lên (tou[v] - tin[v] + 1) * (k + l * h[v]) - l * sigma(h[u])
    trong đó u là các đỉnh thuộc đoạn tin[v] .. tou[v]
    để tình sigma(h[u]) nhanh ta có thể dugnf mảng tiền tố !


    nhận thấy trong lượt truy vấn này có k + l * h[v] là 1 hằng số
    áp dụng tư tưởng của bài https://cses.fi/problemset/task/1736 để thực hiện lazy

    lz1[] quản lý tổng các giá trị k + l * h[v]
    lz2[] quản lý tổng các giá trị l


    như vậy lúc down lazy ta chỉ cần biết được tổng các giá trị k + l * h[v] được cập nhật vào đoạn l..r ( lưu vào lz1[id])
    và tổng các giá trị l được cập nhật vào đoạn l..r (lưu vào lz2[id])
    thì ta dễ dàng suy ra công thức tổng quát để tăng 1 giá trị của 1 đoạn l..r

        truy vấn get u : lấy tổng của đoạn tin[u]..tin[u]
        truy vấn sum u : lấy tổng cảu đoạn tin[u]..tou[u]
*/

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
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define sz(a) int(a.size())
#define FU(x,a,b) for(int x=int(a);x<=int(b);x++)
#define FD(x,a,b) for(int x=int(a);x>=int(b);x--)
#define PROB "talltree"
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


void setIO() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    if(fopen(PROB".inp", "r")){
        freopen(PROB".inp", "r",stdin);
        freopen(PROB".out", "w",stdout);
    }
}

const int N = 5e5 + 1;
const ll mod = 1e9 + 7;
int n, q, h[N], tin[N], tou[N], cnt = 0;
ll st[N*4], lz1[N*4], lz2[N*4], f[N];
/// h : depth u
/// f : sum of depth u
vector < int > a[N];


void readinp(){
    int sub; cin >> sub;
    cin >> n;
    FU(i, 2, n){
        int p; cin >> p;
        a[p].push_back(i);
    }
}

ll MOD(ll x) {
    if (x >= mod) return x % mod;
    if (x < 0) return (x % mod + mod) % mod;
    return x;
}

void dfs (int u){
    tin[u] = ++cnt;
    f[cnt] = MOD(f[cnt - 1] + h[u]);
    for (auto v : a[u]){
        h[v] = h[u] + 1;
        dfs(v);
    }
    tou[u] = cnt;
}

void push (int id, int l, int r, ll x1, ll x2){
    ll t = MOD(x1 * ll(r - l + 1)) - MOD(x2 * (f[r] - f[l-1]));
    st[id] = MOD(st[id] + t);
    lz1[id] = MOD(lz1[id] + x1);
    lz2[id] = MOD(lz2[id] + x2);
}

void down (int id, int l, int r){
    int mid = (l + r) / 2;
    push(id * 2, l, mid, lz1[id], lz2[id]);
    push(id * 2 + 1, mid + 1, r, lz1[id], lz2[id]);
    lz1[id] = lz2[id] = 0;
}

void upd (int id, int l, int r, int u, int v, ll x1, ll x2){
    if (r < u || v < l) return;
    if (u <= l && r <= v){
        push(id, l, r, x1, x2);
        return;
    }
    int m = (l + r) / 2;
    down(id, l, r);
    upd(id * 2, l, m, u, v, x1, x2);
    upd(id * 2 + 1, m + 1, r, u, v, x1, x2);
    st[id] = MOD(st[id * 2] + st[id * 2 + 1]);
}

ll get(int id, int l, int r, int u, int v){
    if (r < u || v < l) return 0;
    if (u <= l && r <= v) return st[id];
    int m = (l + r) / 2;
    down(id, l, r);
    return MOD(get(id * 2, l, m, u, v) + get(id * 2 + 1, m + 1, r, u, v));
}

void solve(){
    dfs(1);
    cin >> q;
    while (q--){
        string ask; cin >> ask;
        if (ask == "add"){
            int u; ll k, l;
            cin >> u >> k >> l;
            upd(1, 1, n, tin[u], tou[u], MOD(k + l * ll(h[u])), l);
        }
        if (ask == "get"){
            int u; cin >> u;
            cout << get(1, 1, n, tin[u], tin[u]) << ' ';
        }
        if (ask == "sum"){
            int u; cin >> u;
            cout << get(1, 1, n, tin[u], tou[u]) << ' ';
        }
    }
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
