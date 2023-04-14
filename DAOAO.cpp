/*


	Tóm tắt : Cho tọa độ của N điểm, khác nhau đôi một, yêu cầu tìm 4 điểm tạo thành 1 hình chữ nhật có diện tích lớn nhất trong cho không có điểm nào nằm trong hay nằm trên cạnh của hình chữ nhật được tạo thành, trừ 4 góc được chọn
	Kiến thức : IT, TRIE, SWEEPLINE, COMPRESS
	Giải : 
	vector < int > px[N] : vector[i] lưu các tung độ (đã được nén) của các điểm có hoành độ là i ( đã được nén )
	vector < pii> in[N] : vector[j] lưu các hoành độ của các điểm có tung độ là j và vị trí của tung độ j trong px[i]
	trong đó .fisrt lưu hoành độ, .se lưu vị trí của nó trong px[.first]
	
	Nhận xét: 
	các cạnh của hcn phải song song với 2 trục tọa độ, nên A(xa, ya) B(xb, yb), C(xc, yc) , D(xd, yd) 
	lần lượt là tọa độ ô trái dưới, ô phải dưới, trái trên, phải trên sẽ có 
	xa = xc, xb = xd
	ya = yb, yc = yd
	Khi đó, sweepline qua các hoành độ ( dựa trên vector px ), 
	ở mỗi hoành độ đang duyệt, ta duyệt lần lượt 2 tung độ cạnh nhau trong vector px[i] :  y1 = px[i][j], y2 = px[i][j+1]
  	tại sao lại là 2 tung độ kề nhau : để dễ tìm được hcn mà không có điểm nào khác nằm trên cạnh của nó.
	Việc còn lại ta chỉ việc tìm 2 điểm đối diện. ta dựa vào vector in[y1] và in[y2]
	cần duyệt 2 tung độ cạnh nhau của y1 và y2 ( ở đây mình chỉ duyệt 2 hoành độ cuối cùng của in[y1]),
	cũng phải đảm bảo y1 và y2 trong px[in[y1][l1-2].fi] phải kề nhau.
	khi đó ta đã có được tọa độ 4 đỉnh, cần kiểm tra xem trong hcn ABCD có tất cả bao nhiêu điểm, nếu có 4 điểm ( tức 4 điểm ở góc ) 
	thì hcn ABCD này thỏa, cập nhật diện tích
  
  1 điểm (x, y) nằm trong hcn ABCD nếu : xa <= x <= xb && ya <= y <= yc
  dưới đây trình bày kĩ thuật trie + mảng động để đếm xem trong đoạn [L,R] có bao nhiêu phần tử < K thay vì dùng BIT 2 chiều 1 cách ngu muội

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
#define PROB "DAOAO"
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

const int N = 50002;
class node{
public:
    vector < int > v;
    node *index[2];
    node(){
        index[0] = index[1] = NULL;
    }
}; node *root;
int n, m, X[N], temp[N], Y[N]; //// X[] : hoành độ trước khi nén, Y[] : tung độ trước khi nén
int nf;
pii a[N];
vector < int > px[N];
vector < pii > in[N]; 

void readinp(){
    cin >> n;
    FU(i, 1, n) cin >> a[i].fi >> a[i].se;
}

bool cmp1(pii a, pii b){
    return a.se < b.se;
}

void compress(){ /// nén theo y
    sort(a + 1, a + 1 + n, cmp1);
    int cnt = 0;
    FU(i, 1, n){
        if (i == 1 || a[i].se != a[i-1].se) cnt++;
        temp[i] = cnt;
        Y[cnt] = a[i].se;
    }
    nf = cnt;
    FU(i, 1, n) a[i].se = temp[i];

}

void make_px(){ /// nén theo x
    sort(a + 1, a + 1 + n);
    int cnt = 0;
    FU(i, 1, n){
        if (i == 1 || a[i].fi != a[i-1].fi) cnt++;
        X[cnt] = a[i].fi;
        px[cnt].push_back(a[i].se);
     }
     m = cnt;
}


int getbit(int x, int i){
    return (x >> (i-1)) & 1;
}

void add(int id, int k){
    node *cur = root;
    FD(i, 30, 1){
        int bit = getbit(k, i);
        if (cur->index[bit] == NULL)
            cur->index[bit] = new node();
        cur = cur->index[bit];
        cur->v.push_back(id);
    }
}

int get(int l, int r, int k){ /// trả về số lượng số < k trong đoạn [L,R]
    node *cur = root;
    int res = 0;
    FD(i, 30, 1){
        int bit = getbit(k, i);
        if (bit){
            if (cur->index[0] != NULL){
                node *temp = cur->index[0];
                res+= upper_bound(temp->v.begin(),temp->v.end(),r)
                    - lower_bound(temp->v.begin(),temp->v.end(),l);
            }
        }
        if (cur->index[bit] == NULL)
            break;
        else cur = cur->index[bit];
    }
    return res;
}

void solve(){
    compress();
    make_px();
    root = new node();
    ll res = 0;
    FU(i, 1, m){
        for (auto j : px[i]) add(i, j);
        FU(j, 0, sz(px[i]) - 1){
            int p = px[i][j];
            in[p].push_back({i, j});
        }

        if (i == 1 || sz(px[i]) < 2) continue;

        FU(j, 0, sz(px[i]) - 2){
            int y1 = px[i][j];
            int y2 = px[i][j + 1]; 
            if (sz(in[y1]) < 2 || sz(in[y2]) < 2) continue;
            int l1 = sz(in[y1]), l2 = sz(in[y2]);
            if (in[y1][l1-2].fi != in[y2][l2-2].fi) continue; /// xét cùng hoành độ hay không ? 
            if (in[y1][l1-2].se + 1 != in[y2][l2-2].se) continue; /// xét kề nhau trong px[in[y1][l1-2].fi] hay không ?
            int p = in[y1][l1-2].fi;
            int x2 = X[i], x1 = X[in[y1][l1-2].fi];
            int g = get(p, i, y2 + 1) - get(p, i, y1); /// số lượng điểm trong hcn tìm được : trong đó (p, y1) là ô trái dưới, (i, y2) là ô phải trên
            if (g != 4) continue;
            ll dai = 1LL * abs(x1 - x2);
            ll rong = 1LL * abs(Y[y1] - Y[y2]);
            res = max(res, dai * rong);
        }
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


