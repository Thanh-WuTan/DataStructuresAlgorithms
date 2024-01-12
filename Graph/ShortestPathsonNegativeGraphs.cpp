/*

  cho đồ thị n đỉnh n <= 5e5
  m cạnh vô hướng (ui, vi) có trọng số dương (m / 2 <= 5e5)
  p cạnh có hướng (xi, yi) có trọng số có thể âm / dương(p / 3 <= 5e5), đảm bảo không có đường đi từ yi đến xi, điều này đảm bảo đồ thị khong có chu trình
  tìm đường đi ngắn nhất từ 1 đến n (có trọng số nhỏ nhất)
  
  gọi d[u] là đường đi ngắn nhất từ 1 đến u, ban đầu d[u] = +oo, d[1] = 0
  
  dùng những cạnh vô hướng đề gộp các tplt lại
  đồ thị sau khi gộp sẽ có dạng : 
    đỉnh sẽ là các tplt
    cạnh sẽ là p cạnh có hướng
    là đồ thị DAG
  việc bây giờ là dfs để tính mảng d[u]
  
  khi thăm vào 1 đỉnh u ( 1 tptl ), phải đảm bảo những đỉnh v có cung tới đỉnh u phải dược xử lý trước (hàm void dijkstra)
  sau đó ta thực hiện dijkstra trong tptl đanh thăm
  dijkstra từ nhiều nguồn : với nguồn là những đỉnh u có d[u] != +oo
  sau cùng, ta dùng những cạnh có hướng để đẩy giá trị sang những đỉnh tiếp theo :  FORE(it, comps[index]) FORE(jt, toArc[*it]) minimize(dist[jt->fi], dist[*it] + jt->se);
  
  
  
*/

#include<bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a), _b = (b); i <= _b; i++)
#define FORD(i, b, a) for (int i = (b), _a = (a); i >= _a; i--)
#define REP(i, n) for (int i = 0, _n = (n); i < _n; i++)
#define FORE(i, v) for (__typeof((v).begin()) i = (v).begin(); i != (v).end(); i++)
#define ALL(v) (v).begin(), (v).end()
#define IS_INF(x)   (std::isinf(x))
#define IS_NAN(x)   (std::isnan(x))
#define fi   first
#define se   second
#define MASK(i) (1LL << (i))
#define BIT(x, i) (((x) >> (i)) & 1)
#define div   ___div
#define next   ___next
#define prev   ___prev
#define left   ___left
#define right   ___right
#define __builtin_popcount __builtin_popcountll
using namespace std;
template<class X, class Y>
    bool minimize(X &x, const Y &y) {
        X eps = 1e-9;
        if (x > y + eps) {
            x = y;
            return true;
        } else return false;
    }
template<class X, class Y>
    bool maximize(X &x, const Y &y) {
        X eps = 1e-9;
        if (x + eps < y) {
            x = y;
            return true;
        } else return false;
    }
template<class T>
    T Abs(const T &x) {
        return (x < 0 ? -x : x);
    }

/* Author: Van Hanh Pham */

class DisjointSet {
private:
    vector<int> lab;

public:
    DisjointSet(int n = 0) {
        lab.assign(n + 7, -1);
    }

    int find(int x) {
        return lab[x] < 0 ? x : lab[x] = find(lab[x]);
    }

    bool join(int u, int v) {
        int x = find(u), y = find(v);
        if (x == y) return false;

        if (lab[x] > lab[y]) swap(x, y);
        lab[x] += lab[y];
        lab[y] = x;
        return true;
    }
};

/** END OF TEMPLATE. DRINK A CUP OF TIGERSUGAR BEFORE READING MY CODE. **/

#define MAX   500500
const long long INF = (long long)1e18 + 7LL;

int numNode, numComp, numEdge, numArc;
vector<pair<int, int>> edges[MAX];
vector<pair<int, int>> toArc[MAX];
vector<int> fromArc[MAX];
vector<int> comps[MAX];
int compID[MAX];
bool visited[MAX];
long long dist[MAX];

void loadGraph(void) {
    scanf("%d%d%d", &numNode, &numEdge, &numArc);
    REP(love, numEdge) {
        int u, v, c; scanf("%d%d%d", &u, &v, &c);
        edges[u].push_back(make_pair(v, c));
        edges[v].push_back(make_pair(u, c));
    }
    REP(love, numArc) {
        int u, v, c; scanf("%d%d%d", &u, &v, &c);
        toArc[u].push_back(make_pair(v, c));
        fromArc[v].push_back(u);
    }
}

void prepare(void) {
    DisjointSet dsu(numNode);
    FOR(i, 1, numNode) FORE(jt, edges[i]) dsu.join(i, jt->fi);

    FOR(i, 1, numNode) if (dsu.find(i) == i) {
        compID[i] = ++numComp;
        comps[numComp].push_back(i);
    }
    FOR(i, 1, numNode) {
        int j = dsu.find(i);
        if (i != j) compID[i] = compID[j];
        comps[compID[j]].push_back(i);
    }
}

void dijkstra(int index) {
    if (visited[index]) return;
    visited[index] = true;
    FORE(it, comps[index]) FORE(jt, fromArc[*it]) dijkstra(compID[*jt]);

    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> q;
    FORE(it, comps[index]) if (dist[*it] < INF) q.push(make_pair(dist[*it], *it));

    while (!q.empty()) {
        long long d = q.top().fi;
        int u = q.top().se;
        q.pop();
        if (dist[u] != d) continue;

        FORE(it, edges[u]) {
            int v = it->fi;
            if (minimize(dist[v], dist[u] + it->se)) q.push(make_pair(dist[v], v));
        }
    }

    FORE(it, comps[index]) FORE(jt, toArc[*it]) minimize(dist[jt->fi], dist[*it] + jt->se);
}

void process(void) {
    memset(dist, 0x3f, sizeof dist);
    dist[1] = 0;
    FOR(i, 1, numComp) dijkstra(i);

    FOR(i, 1, numNode) printf("%lld ", dist[i] < INF ? dist[i] : -1LL); printf("\n");
}

int main(void) {
#ifdef ONLINE_JUDGE
    freopen("shortestpath.inp", "r", stdin);
    freopen("shortestpath.out", "w", stdout);
#endif // ONLINE_JUDGE
    loadGraph();
    prepare();
    process();
    return 0;
}

/*** BUBBLE TEA IS GREAT. MY CODE IS AMAZING :D ***/
