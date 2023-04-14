/*
  tóm tắt cho đồ thị có hướng không trọng số G
  gọi V là tập hợp gồm N đỉnh của G , các đỉnh được đánh số từ 1 đến N
  gọi E1 là tập hợp M cạnh ban đầu của đồ thị G
  cho 1 tập hợp E2 gồm m2 cạnh có hướng, có trọng số (có thể âm hoặc dương )
  yêu cầu thêm một số cạch từ tập V2 vào đồ thị G sao cho đồ thị G thành 1 thành phần liên thông mạnh, và tổng trọng số các cạnh thêm vào là ít nhất
  đảm bảo đồ thị G tồn tại 1 cặp đỉnh (s, t) sao cho : 
    + từ s có thể đi đến mọi đỉnh thuộC V \ {s} (1)
    + mọi đỉnh thuộc V \ {t} luôn tồn tại đường đi đến t
 
 
  từ đảm bảo trên, để G trở thành phần liên thông mạnh, ta chỉ việc thêm một số cạnh sao cho tồn tại đường đi từ t đến s và là đường đi ngắn nhất !
  trước hết phài tìm được đỉnh s và t
  để tìm s và t ta cần dựng đồ thị DAG bằng cách gộp các thành phần liên thông mạnh lại thành 1 đỉnh
  khi đó đỉnh s sẽ là đỉnh không có bậc vào của đồ thị mới 
  và đỉnh t sẽ là đỉnh không có bậc ra của đồ thị mới
  
  đến đây bài toán gần như đã xong, việc còn lại ta chỉ việc dijkstra tìm đường đi ngắn nhất từ t đến s
  thêm 1 nhận xét : do tập hợp V2 có thể có cạnh có trọng số âm, 
  để tối ưu kết quả mình tham lam chọn tất cả những cạnh có trọng số âm, và gán lại những cạnh đó có trọng số = 0
  
  kết hợp cả 2 tập hợp cạnh E1 ( mọi cạnh có trọng số = 0 ) và E2 để tìm đường đi ngắn nhất
  
  nếu chỉ dùng mỗi tập E2 sẽ có trường hợp sai : 
    giả sử kết quả tối ưu chỉ dựng 1 số cạnh để t có thể đi đến được u, nhưng với những cạnh trong tập E1 , từ u có thể đi đến s với chi phí = 0
    
    
  
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
#define PROB "EDGES"
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

const int N = 1e5 + 2, M = 500001;
bool ine[N]{0}, oue[N]{0};
int n, m1, m2, in[N], tp = 0;
int num[N], low[N], cnt = 0, t, s;
vector < vector < int > > a;
vector < vector < pii > > g;
int tplt[N];
pii e[M];
ll d[N], res = 0;
struct canh {
    int u, v, w;
}; canh edg[M];

void readinp(){
    read(n);
    a.resize(n + 1); /// tập hợp E1
    g.resize(n + 1); /// tập hợp E2
    read(m1);
    FU(i, 1, m1){
        int u, v;
        read(u), read(v);
        e[i] = {u, v};
        a[u].push_back(v);

    }
    read(m2);
    FU(i, 1, m2){
        int u, v, w;
        read(u), read(v), read(w);
        if (w < 0) res+= w, w = 0; /// tham lam dùng những cạnh trọng số âm
        edg[i] = {u, v, w};
        g[u].push_back({w, v});
    }
}

int top = 0, st[N];
void dfs (int u){
    low[u] = num[u] = ++cnt;
    st[++top] = u;
    for (auto v : a[u]) if (!in[v]){
        if (num[v]) low[u] = min(low[u], num[v]);
        else{
            dfs(v);
            low[u] = min(low[u], low[v]);
        }
    }
    if (num[u] == low[u]){
        int v = 0;
        tp++;
        while (v != u){
            v = st[top]; top--;
            in[v] = tp;
            tplt[tp] = v;
        }
    }
}

bool check (){
    FU(i, 1, m2) if (edg[i].w < 0) return false;
    return true;
}

void solve(){
  /// tìm thành phần liên thông mạnh : 
    FU(i, 1, n) if (!num[i]) dfs(i); 
  
  /// dựng đồ thị mới : 
    map < pii , int > D;
    FU(i, 1, m1){
        int u = e[i].fi;
        int v = e[i].se;
        int U = in[u];
        int V = in[v];
        if (U == V) continue;
        if (D.find({U, V}) != D.end()) continue;
        D[{U, V}] = 1;
        ine[V] = true;
        oue[U] = true;
    }
  
  /// tìm đỉnh S và T : 
    int S , T;
    FU(i, 1, tp){
        if (ine[i] == false) S = i;
        if (oue[i] == false) T = i;
    }
    s = tplt[S];
    t = tplt[T];

    


  /// dijstra
    FU(i, 1, n) d[i] = 1e18;
    d[t] = 0;
    priority_queue < pli , vector < pli > , greater < pli > > Q;
    Q.push({0, t});
    while (!Q.empty()){
        int u = Q.top().se;
        ll  w = Q.top().fi;
        Q.pop();
        if (w > d[u]) continue;
        for (auto v : g[u]){ /// duyệt các đỉnh v kề u trong tập hợp E2
            if (d[v.se] > w + ll(v.fi)){
                d[v.se] = w + ll(v.fi);
                Q.push({d[v.se], v.se});
            }
        }
        for (auto v : a[u]){ /// duyệt các đỉnh v kề u trong tập hợp E1
            if (d[v] > w){
                d[v] = w;
                Q.push({d[v], v});
            }
        }
    }
    if (d[s] == 1e18){
        cout << "NO";
    }
    else{
        cout << "YES";EL;
        cout << res + d[s];
    }
}

int main(){
    setIO();
    int tc = 1;
//    cin >> t;
    while (tc--){
        readinp();
        solve();
    }
}

