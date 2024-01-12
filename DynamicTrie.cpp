/*
  task : có 4 loại truy vấn : 
      - 1 X     : thêm 1 số X vào bên phải ngoài cùng của mảng
      - 2 L R K : đếm xem trogn đoạn  [L, R] bao nhiêu phần tử bé hơn K
      - 3 L R K : đếm xem trong đoạn  [L, R] bao nhiêu phần tử bằng K
      - 4 L R K : đếm xem trong đoạn  [L, R] bao nhiêu phần tử lớn K
    https://www.hackerearth.com/practice/data-structures/advanced-data-structures/trie-keyword-tree/practice-problems/algorithm/dfdf/
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
#define PROB "Gora"
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



class node{
public:
    vector < int > a;
    node *index[2];
    node(){
        index[0] = index[1] = NULL;
    }
};
node *root;

int getbit (int x, int i){
    return (x >> (i-1)) & 1;
}

void query1 (int x, int id){
    node *cur = root;
    FD(i, 30, 1){
        int bit = getbit(x, i);
        if (cur->index[bit] == NULL)
            cur->index[bit] = new node();
        cur = cur->index[bit];
        cur->a.push_back(id);
    }
}

int query2(int l, int r, int k){
    node *cur = root;
    int res = 0;
    FD(i, 30, 1){
        int bit = getbit(k, i);
        if (bit){
            if (cur->index[0] != NULL){
                node *temp = cur->index[0];
                res+= upper_bound(temp->a.begin(),temp->a.end(),r)
                                        -
                      lower_bound(temp->a.begin(),temp->a.end(),l);
            }
        }
        if (cur->index[bit] == NULL)
            break;
        else cur = cur->index[bit];
    }
    return res;
}


int query3(int l, int r, int k){
    node *cur = root;
    int res = 0;
    FD(i, 30, 1){
        int bit = getbit(k, i);
        if (cur->index[bit] == NULL)
            return 0;
        else cur = cur->index[bit];
    }
    res+= upper_bound(cur->a.begin(), cur->a.end(),r)
                            -
          lower_bound(cur->a.begin(), cur->a.end(),l);
    return res;
}

int query4(int l, int r, int k){
    node *cur = root;
    int res = 0;
    FD(i, 30, 1){
        int bit = getbit(k, i);
        if (!bit){
            if (cur->index[1] != NULL){
                node *temp = cur->index[1];
                res+= upper_bound(temp->a.begin(),temp->a.end(),r)
                                        -
                      lower_bound(temp->a.begin(),temp->a.end(),l);
            }
        }
        if (cur->index[bit] == NULL)
            break;
        else cur = cur->index[bit];
    }
    return res;
}

void solve(){
    root = new node();
    int q; cin >> q;
    int n = 0;
    while (q--){
        int t;
        cin >> t;
        if (t == 1){
            int x; cin >> x;
            query1(x, ++n);
        }
        else{
            int l, r, k;
            cin >> l >> r >> k;
            if (t == 2) cout << query2(l, r, k);
            if (t == 3) cout << query3(l, r, k);
            if (t == 4) cout << query4(l, r, k);
            EL;
        }
    }
}

int main(){
    setIO();
    int t = 1;
//    cin >> t;
    while (t--){
        solve();
    }
}

