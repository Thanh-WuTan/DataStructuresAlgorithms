/*
  
  tóm tắt cho N số nguyên dưuong : với mọi a[i] <= N
  yêu cầu chia dãy thành K đoạn liên tiếp , mỗi phần tử bắt buộc phải thuộc đúng 1 đoạn, sao cho tổng độ đẹp của K đoạn này là lớn nhất
  định nghĩa độ đẹp của 1 đoạn = số lượng phần tử khác nhau trong đoạn đó 
  ví dụ đoạn : 1 1 2 1 3 có độ đẹp = 3
        đoạn : 1 1 1 1 1 có độ đẹp = 1
  
  https://codeforces.com/contest/833/problem/B
*/

/*  
  gọi F[i][j] : tổng độ đẹp lớn nhất của j đoạn chia được với i phần tử đầu tiên
  với j = 1 dễ dàng tính được số lượng phần tử khác nhau trong đoạn [1..i]
  với j > 1 , ta sẽ dựa vào F[][j-1] để cập nhật với công thức truy hồi như sau :
      F[i][j] = max(F[u-1][j-1] + c(u,i))
     trong đó c(u, i) sẽ là số lượng phần tử khác nhau trong đoạn [u .. i]
  giả sử đã tính được lớp j-1 (F[i][j-1])
  Khi thực hiện tính đến lớp j, 
  Duyệt qua lần lượt các vị trí i = 1 .. n
  Khi từ i - 1 sang i
  tìm đoạn [l .. r] sao cho đoạn này không chứa phần tử a[i]
  thì số lượng phần tử khác nhau trong các đoạn [j .. i] sẽ tăng lên 1 (khi gắn a[i] vào ) với j thuộc đoạn [l .. r] tìm được
  
  sử dụng IT với j lớp để quản lý các giá trị f[i][j]
  và lazy để thực hiện update 1 đoạn 
  
  thế thì khi xét đến vị trí i trong lớp j
  tăng tất cả f[u][j-1] lên 1 với l[i] <= u <= i - 1
  f[i][j] = max (  + f[i][j-1] : cho a[i] vào đoạn trước đó
                (  + get(1, j - 1, 1, n, 1, i) : tạo 1 đoạn mới chứa a[i]
          
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
#define PROB "833B"
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

const int N = 35001, K = 51;
int n, k, a[N], f[N][K], pos[N], l[N];
int st[N * 4][K], lz[N * 4][K];

void readinp(){
    cin >> n >> k;
    FU(i, 1, n) cin >> a[i];
}

void down (int i, int j){
    int t = lz[i][j];

    st[i*2][j]+= t;
    lz[i*2][j]+= t;

    st[i*2+1][j]+= t;
    lz[i*2+1][j]+= t;

    lz[i][j] = 0;
}

void upd_(int i, int j, int l, int r, int p, int v){
    if (l == r){
        st[i][j] = v;
        return;
    }
    down(i, j);
    int mid = (l + r) / 2;
    if (p >= mid + 1) upd_(i * 2 + 1, j, mid + 1, r, p, v);
    else upd_(i * 2, j, l, mid, p, v);
    st[i][j] = max(st[i*2][j], st[i*2+1][j]);
}

void upd (int i, int j, int l, int r, int u, int v, int g){
    if (u > v || r < u || v < l) return;
    if (u <= l && r <= v){
        st[i][j]+= g;
        lz[i][j]+= g;
        return;
    }
    down(i, j);
    int mid = (l + r) / 2;
    upd(i * 2, j, l, mid, u, v, g);
    upd(i * 2 + 1, j, mid + 1, r, u, v, g);
    st[i][j] = max(st[i*2][j], st[i*2+1][j]);
}

int get(int i, int j, int l, int r, int u, int v){
    if (r < u || v < l) return -1e9;
    if (u <= l && r <= v) return st[i][j];
    down(i, j);
    int mid = (l + r) / 2;
    int t1 = get(i * 2, j, l, mid, u, v);
    int t2 = get(i * 2 + 1, j, mid + 1, r, u, v);
    return max(t1, t2);
}

void cal(int j){
    if (j == 1){
        set < int > s;
        FU(i, 1, n){
            s.insert(a[i]);
            f[i][j] = sz(s);
            upd_(1, j, 1, n, i, f[i][j]);
        }
        return;
    }

    FU(i, 1, n){
        upd(1, j - 1, 1, n, l[i], i - 1, 1);
        f[i][j] = max(f[i][j-1], get(1, j - 1, 1, n, 1, i));
        upd_(1, j, 1, n, i, f[i][j]);
    }
}

void solve(){
    FU(i, 1, n){
        l[i] = pos[a[i]];
        pos[a[i]] = i;
    }


    FU(i, 1, k) cal(i);
//    FU(i, 1, k){
//        FU(u, i, n){
//            set < int > s;
//            FD(v, u, i){
//                s.insert(a[v]);
//                f[u][i] = max(f[u][i], f[v-1][i-1] + sz(s));
//            }
//        }
//    }


    cout << f[n][k];
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
