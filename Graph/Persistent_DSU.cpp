/*
  task : có 3 dạng truy vấn
    1 u v thêm cạnh u v
    2 u v xóa cạnh u v
    3 u v hỏi xem u và v có đường đi đến với nhau hay không ? 
    https://vn.spoj.com/problems/VOSCOMPS/
    
    
*/

/*

  giải : dùng chia để trị kết hợp dsu with rollback để thực hiện offline các truy vấn
  L[i] = idx , nếu query[i] = 2 u v trong đó idx là thứ tự truy vấn < i và idx lớn nhất : query[idx] = 1 u[i], v[i] tức thêm cạnh u[i], v[i]
  R[i] = idx, nếu query[i] = 1 u v, trong đó idx là thứ tự truy vấn < i và idx là nhỏ nhất : query[idx] = 2 u[i], v[i], tức xóa cạnh u[i], v[i]
  chia đôi các khoảng truy vấn để thực hiện lần lượt
  
  khi đến đoạn lefr, right
  mình sẽ đi lần lượt vào left, mid và mid + 1, right
  
  trước khi đi vào left, mid : duyệt qua tất cả truy vấn 2 u v từ mid + 1 đến right, truy vấn 2 u[i] v[i] nào có L[i] < left nghĩa là cạnh u[i] v[i]
   được thêm trước thời gian truy vấn left, nên sử dụng dsu gộp đỉnh u[i] v[i] lại , nhớ lưu lại để lúc sau thực hiện roll back
  
  trước khi đi vào mid + 1, right : duyệt qua tất cả các truy vấn 1 u v từ left đến mid, truy vấn 1 u[i] v[i] nào có R[i] > right nghĩa là cạnh u[i] v[i]
    sẽ được xóa sau thời gian truy vấn right, nên dsu gộp đỉnh u[i] v[i] lại, cũng tương tự như trên, lưu lại nhớ để thực hiện rollback
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
#define PROB "VOSCOMPS"
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

const int N = 1e5 + 2;
struct query{
    int t, x, y;
}; query que[N];
struct tan{
    int v, u, x;
};
int n, q, L[N], R[N], res[N], par[N];
map < pii, int > pos;
stack < int > lis[N];

void readinp(){
    cin >> n >> q;
    FU(i, 1, q){
        int t, x, y;
        cin >> t >> x >> y;
        if (x > y) swap(x, y);
        que[i] = {t, x, y};
    }
}

int root(int x){
    return (par[x] < 0 ? x : root(par[x]));
}

tan dsu (int u, int v){
    tan res = {0, 0, 0};
    if (-par[u] < -par[v]) swap(u, v);
    res = {v, u, par[v]};
    par[u]+= par[v];
    par[v] = u;
    return res;
}

void go(int l, int r){
    if (l == r){
        if (que[l].t == 3){
            if (root(que[l].x) == root(que[l].y))
                res[l] = 1;
            else res[l] = 0;
        }
        return;
    }

    int mid = (l + r) / 2;

    stack < tan > ds;
    FU(i, mid + 1, r) if (que[i].t == 2 && L[i] < l){
        int x = que[i].x, y = que[i].y;
        int u = root(x), v = root(y);
        if (u == v) continue;
        ds.push(dsu(u, v));
    }
    go(l, mid);
    while (!ds.empty()){
        tan t = ds.top();
        ds.pop();
        par[t.u]-= t.x;
        par[t.v] = t.x;
    }



    FU(i, l, mid) if (que[i].t == 1 && R[i] > r){
        int x = que[i].x, y = que[i].y;
        int u = root(x), v = root(y);
        if (u == v) continue;
        ds.push(dsu(u, v));
    }
    go(mid + 1, r);
    while (!ds.empty()){
        tan t = ds.top();
        ds.pop();
        par[t.u]-= t.x;
        par[t.v] = t.x;
    }
}

void solve(){
    int cnt = 0;
    FD(i, q, 1){
        int t = que[i].t, x = que[i].x, y = que[i].y;
        if (t == 2){
            if (!pos.count({x, y})) pos[{x, y}] = ++cnt;
            int id = pos[{x, y}];
            lis[id].push(i);
            L[i] = q + 1;
        }
        if (t == 1){
            if (!pos.count({x, y})) pos[{x, y}] = ++cnt;
            int id = pos[{x, y}];
            if (lis[id].empty()) R[i] = q + 1;
            else{
                R[i] = lis[id].top();
                L[lis[id].top()] = i;
                lis[id].pop();
            }
        }
    }
    FU(i, 1, n) par[i] = -1;
    go(1, q);
    FU(i, 1, q) if (que[i].t == 3) cout << res[i];

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

