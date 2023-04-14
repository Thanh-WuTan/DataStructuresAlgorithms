/*
  cho đồ thị n đỉnh, m cạnh vô hướng mỗi đỉnh có 1 trọng số p[i]
  Q truy vấn thuộc 1 trong 2 dạng : 
  1 v : hỏi từ những đỉnh có đường đi đến v có trọng số max là bao nhiêu, sau đó gán p của đỉnh đó = 0
  2 i : xóa cạnh thứ i
  https://codeforces.com/contest/1416/problem/D
  dưới đây là code tle
*/
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
#define all(v) (v).begin(),(v).end()
#define FU(x,a,b) for(int x=int(a);x<=int(b);x++)
#define FD(x,a,b) for(int x=int(a);x>=int(b);x--)
#define PROB "1416D"

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
const int M = 3e5 + 2, N = 2e5 + 2;
int n, m, q;
bool cut[M]; /// cut[i] = false, cạnh i không bị cut
int p[N], /// p[i] : giá trị của đỉnh thứ i
    par[N]; /// mảng dùng để dsu
pii edg[M], /// edg[i] = {u, v} (cạnh đồ thị)
    que[int(5e5)]; /// các truy vấn 
stack < pii > events; /// để rollback
stack < int > in[N]; /// in[i] : bao gồm các thành phần mà i thuộc, với in[i].top() sẽ là thành phần mà i thuộc ở 'hiện tại' (dùng để rollback)
set < pii , greater < pii > > vals[N]; /// vals[i] : {giá trị, chỉ số} của các đỉnh thuộc tplt i
vector < vector < int > > nodes; /// nodes[i] : {u} các đỉnh u thuộc tplt i
void readinp(){
    scanf("%d %d %d", &n, &m, &q);
    FU(i, 1, n) scanf("%d", &p[i]);
    FU(i, 0, m - 1){
        int u, v;
        scanf("%d %d", &u, &v);
        edg[i] = {u, v};
    }
    FU(i, 0, q - 1){
        int t, x;
        scanf("%d %d", &t, &x);
        que[i] = {t, x - (t == 2)};
    }
}

int root (int u){
    return (par[u] < 0 ? u : par[u] = root(par[u]));
}

void unionn(int u, int v){
    if ((u = root(u)) == (v = root(v))) return;
    if (-par[u] < -par[v]) swap(u, v);
    par[u]+= par[v];
    par[v] = u;
}

void prepare(){
    FU(i, 1, n) par[i] = -1;
    for (auto it : que) if (it.fi == 2) cut[it.se] = true;
    FU(i, 0, m - 1) if (!cut[i])
        unionn(edg[i].fi, edg[i].se);
    nodes.resize(n + 1);
    FU(i, 1, n){
        int rt = root(i);
        in[i].push(rt);
        nodes[rt].pb(i);
    }
  /// đi ngược truy vấn để lưu lại những thay đổi khi 1 cạnh bị cắt ra
    FD(i, q - 1, 0) if (que[i].fi == 2){
        int ide = que[i].se;
        int u = edg[ide].fi, v = edg[ide].se;
        u = in[u].top(), v = in[v].top();
        if (u == v) continue;
        if (sz(nodes[u]) < sz(nodes[v])) swap(u, v);
        for (auto it : nodes[v]){
            in[it].push(u);
            events.push({i, it}); /// lưu lại thay đổi 
            nodes[u].pb(it);
        }
        nodes[v].clear();
    }
    FU(i, 1, n) if (par[i] < 0) {
        for (auto it : nodes[i])
            vals[i].insert(mp(p[it], it));
    }
}

void solve(){
    prepare();
    FU(i, 0, q - 1){

        if (que[i].fi == 1){
            int u = que[i].se;
            int comp = in[u].top();
            auto it = vals[comp].begin();
            int res = it->fi, v = it->se;
            printf("%d\n", res);
            assert(0 <= res && res <= n);
            if (it->fi != 0){
                p[v] = 0;
                vals[comp].erase(vals[comp].begin());
                vals[comp].insert({0, v});
            }
        }
        else{
            /*
              khi cắt cạnh thứ i, ta sẽ dùng stack events để rollback lại
              ta sẽ biết được sau khi cắt cạnh này, những đỉnh u nào bị thay đổi, và những đỉnh u đó sẽ đi vào tplt mới nào (dùng stack in[u])
            */
            while (!events.empty() &&  events.top().fi == i){
                int u = events.top().se;
                int curcomp = in[u].top();
                in[u].pop();
                int newcomp = in[u].top();
                vals[curcomp].erase({p[u], u});
                vals[newcomp].insert({p[u], u});
                events.pop();
            }
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
    return 0;
}
