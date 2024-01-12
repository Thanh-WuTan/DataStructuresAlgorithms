/*
  tóm tắt yêu cầu chia dãy N số nguyên dương thành những đoạn liên tiếp sao cho mỗi đoạn có tổng không vượt quá M và tổng các giá trị lớn nhất của mỗi đoạn là nhỏ nhất

*/

#include <bits/stdc++.h>
#define io_faster ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0);
#define EL cout<<'\n'
#define pli pair<ll,int>
#define pii pair<int,int>
#define fi first
#define se second
#define PROB "CUTSEQS"

using namespace std;
typedef long long ll;

void file(){
#ifndef ONLINE_JUDGE
	freopen(PROB".inp","r",stdin);
	freopen(PROB".out","w",stdout);
#endif
}

const int N  = 1e5 + 5;
const ll INF = 2e18;
int n; ll m, a[N], f[N], st[N*6];
void readinp(){
    cin >> n >> m;
    for (int i=1;i<=n;i++){
        cin >> a[i];
        if (a[i] > m){
            cout << -1;
            exit(0);
        }
    }
}

void upd (int id, int l, int r, int p, ll v){
    if (l == r && r == p){
        st[id] = v;
        return;
    }
    int mid = (l + r) >> 1;
    if (p >= mid + 1)
        upd(id*2+1,mid+1,r,p,v);
    else upd(id*2,l,mid,p,v);
    st[id] = min(st[id*2], st[id*2+1]);
}

ll get (int id, int l, int r, int u, int v){
    if (v < l || r < u) return INF;
    if (u <= l && r <= v) return st[id];
    int mid = (l + r) >> 1;
    ll t1 = get(id*2, l,mid, u, v);
    ll t2 = get(id*2+1, mid+1, r, u, v);
    return min(t1, t2);
}

/// f[i] : giá trị tối ưu nhất chia được khi xét đến vị trí i

void solve(){
    for (int i=0;i<=n*6;i++) st[i] = INF;
    deque < int > d; /// xây dựng deque quản lý các chỉ số của các phần tử giảm dần từ front đến back
    d.push_back(1);
    upd(1,1,n,1,a[1]);
    f[1] = a[1];
    ll s = a[1]; int j = 1;
    /// con trỏ j cho biết vị trí xa nhất sao cho : tổng a[j] + a[j+1] + .. + a[i] <= m
    for (int i=2;i<=n;i++){
        s+= a[i];
        while (s > m)
            s-= a[j++]; 
        while (d.size() && a[d.back()] <= a[i]){  
            upd(1,1,n, d.back(), INF);
            d.pop_back();
        }
        while (d.size() && d.front() < j){ /// những vị trí < j sẽ không thỏa điều kiện tổng <= m nên loại ra
            upd(1,1,n, d.front(), INF);
            d.pop_front();
        }
        if (d.size()){
            /// tạo 1 đoạn mới với a[i] là max nhất trong đoạn này
            /// khi đó d.back() + 1 sẽ là vị trí xa nhất mà a[i] là max trên đoạn [d.back() + 1, i]
            upd(1, 1, n, i, f[d.back()] + a[i]); 
            
                
            /// gắn a[i] vào đoạn [j,i-1] với a[d.front()] là max
            /// do deque mình lưu các chỉ số của các phần tử giảm dần từ front đến back
            /// cho nên d.front() là vị trí của phần tử lớn nhất từ j đến i
            /// tại sao lại gắn vào đoạn [j, i-1] : ở đây mình tham lam chọn đoạn càng dài càng tốt
            upd(1, 1, n, d.front(), f[j - 1] + a[d.front()]);
        }
        else{
            upd(1, 1, n, i, f[j - 1] + a[i]);
        }
        d.push_back(i);
        f[i] = get(1,1,n,1,n);
    }
    cout << f[n];
}

int main(){
    io_faster
    file();
    int t = 1;
//    cin >> t;
    while (t--){
        readinp();
        solve();
    }
}
