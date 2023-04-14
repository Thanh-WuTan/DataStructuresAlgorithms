/*
https://codeforces.com/contest/1486/problem/F
    tóm tắt : cho cây n đỉnh và m đường đi đơn path[i] = {x, y}
    yêu cầu : đếm xem có bao nhiêu cặp i != j sao cho path[i] và path[j]
    giao nhau ĐÚNG 1 đỉnh

    giải :

    đặt lca[i] = LCA(path[i].x, path[i].j)
    nhận xét quan trọng : 2 đường đi giao nhau đúng 1 đỉnh, gọi đỉnh đó là u
    thì u sẽ là LCA của ít nhất 1 trong 2 đường

    từ nhận xét như vậy ta sẽ hình dung đường đi thứ i từ x -> y có dạng như sau
        Hoặc là từ subtree1 sang subtree2
        Hoặc là x = lca(x, y) hoặc y = lca(x, y) tức là đường đi có dạng đường thẳng
    subtree1 và subtree2 mình dễ dàng tìm được bằng cách
        trong path[i] = {x, y}
        nhảy từ x lên lca(x,y) rồi lùi xuống 1 tầng = subtree1[i]
        nhảy từ y lên lca(x,y) rồi lùi xuống 1 tầng = subtree2[i]

    lưu vào truy vấn có dạng {x, y, subtree1, subtree2, lca(x,y)}
    xắp xếp lại các truy vấn ưu tiên theo độ sâu của lca{x,y} tăng dần
    tức lca{x,y} ở gần root hơn thì xếp trước , root = 1

    duyệt lần lượt qua các đường đi path[i] (i = 1.. m)
    khi xét đến đường đi i
        nếu tồn tại path[j] nào giao với path[i] tại đúng 1 đỉnh
        thì đỉnh đó sẽ là lca[i], tức mình lấy lca[i] làm tâm của giao điểm
        TH1 : lca[j] = lca[i],
        như vậy mình chỉ việc đếm xem có bao nhiêu  path[j] có lca[j] = lca[i]
        và path[j] có subtree1[j] và subtree2[j] đồng thời khác với subtree1[i] và subtree2[i]
        (dùng bù trừ)
        TH2 : path[j] có 1 đoạn đi từ trên đâm vào lca[i] rồi đi xuống dưới cây con gốc lca[i]
        như vậy muốn path[j] có duy nhất 1 điểm chung với path[i] là lca[i]
        thì đoạn đó không được đâm vào subtree1[i] và subbtre2[i]
        dùng cấu trúc cây phân đoạn để quản lý cái này

        đến đây có thể lý giải tại sao phải sort theo h[lca] tăng dần



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
    'Cause girl you make me so confused
    (girl you got me so confused)

    You got me feeling like a psycho, psycho
    Got me chasin' shots of NyQuil, NyQuil
    Without you got me dizzy and upset
    Got me so obsessed with you
    Look what you started
    I'm psycho, you're heartless
*/
// i'm wutan
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
#define PROB "1486F"
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

const int N = 3e5 + 2;
int n, m;
int par[N][20], fen[N], h[N], tin[N], tou[N], base;
struct query{
    int x, y, u, v, anc;
};
vector < query > q;
vector < vector < int > > a;


void readinp(){
    read(n);
    a.resize(n + 1);
    FU(i, 2, n){
        int x, y;
        read(x), read(y);
        a[x].pb(y);
        a[y].pb(x);
    }
}

void dfs (int u, int p){
    tin[u] = ++base;
    for (auto v : a[u]) if (v != p){
        h[v] = h[u] + 1;
        par[v][0] = u;
        dfs(v, u);
    }
    tou[u] = base;
}

int lca(int u, int v){
    if (h[u] < h[v]) swap(u, v);
    FD(k, 19, 0) if (h[par[u][k]] >= h[v]) u = par[u][k];
    FD(k, 19, 0) if (par[u][k] != par[v][k])
        u = par[u][k], v = par[v][k];
    return (u == v ? u : par[u][0]);
}

int jump(int u, int k){
	FD(i, 19, 0)
        if(k >> i & 1) u = par[u][i];
	return u;
}

bool cmp(query t1, query t2){
    if (h[t1.anc] != h[t2.anc]) return h[t1.anc] < h[t2.anc];
    return t1.anc < t2.anc;
}

void upd (int p, int v){
    for (int i = p; i <= n; i+= i&-i)
        fen[i]+= v;
}

int get(int p){
    int ret = 0;
    for (int i = p; i > 0; i-= i&-i)
        ret+= fen[i];
    return ret;
}

void solve(){
    par[1][0] = 1;
    dfs(1, 1);
    FU(k, 1, 19) FU(u, 1, n)
        par[u][k] = par[par[u][k-1]][k-1];
    read(m);
    FU(i, 1, m){
        int x, y;
        read(x), read(y);
        int anc = lca(x, y);
        int u = (x == anc) ? -1 : jump(x, h[x] - h[anc] - 1);
        int v = (y == anc) ? -1 : jump(y, h[y] - h[anc] - 1);
        if (u > v) swap(u, v), swap(x, y);
        q.push_back({x, y, u, v, anc});
    }
    sort(q.begin(), q.end(), cmp);
    unordered_map < int , int > cnt;
    map < pii, int > _cnt;
    ll res = 0;
    FU(i, 0, m - 1){
        int j = i;
        while (j + 1 < m && q[j + 1].anc == q[i].anc) j++;
        int anc = q[i].anc;
        cnt.clear(), _cnt.clear();
        FU(j2, i, j){
            int u = q[j2].u, v = q[j2].v;
            int t1 = 0, t2 = 0, t3 = 0;
            // xet TH1
            if (u != -1) t1 = cnt[u]++;
            if (v != -1) t2 = cnt[v]++;
            if (u != -1 && v != -1) t3 = _cnt[{u, v}]++;
            res+= ll(j2 - i - t1- t2 + t3);

            // TH2 :
            res+= ll(get(tou[anc]) - get(tin[anc] - 1));
            if (u != -1) res-= ll(get(tou[u]) - get(tin[u] - 1));
            if (v != -1) res-= ll(get(tou[v]) - get(tin[v] - 1));
        }

        FU(j2, i, j)
            upd(tin[q[j2].x], 1), upd(tin[q[j2].y], 1);
        i = j;
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
