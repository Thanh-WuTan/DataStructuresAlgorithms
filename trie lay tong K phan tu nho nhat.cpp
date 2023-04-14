/*
  0 x : thêm 1 ptu x vào S
  1 x : xóa 1 ptu x khỏi S(nếu x có trong S)
  2 x : Thay đổi mỗi ptu c thuộc S thành c ^ x
  3 k : tinh tong k ptu nho nhat trong S
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
#define PROB "d13r1xquery"

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


void setIO() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    if(fopen(PROB".inp", "r")){
        freopen(PROB".inp", "r",stdin);
        freopen(PROB".out", "w",stdout);
    }
}

const bool love = true;
class node{
public:
    node *index[2];
    int cnt; /// -> so luong so di qua nut trie 
    ll sum; /// -> tổng giá trị các số đi qua nút trie
    int d[18]; /// d[i] = số luong số đi qua nút trie và có bit thứ i = 1 
    /// tại sao lại cần thêm mảng d
    /*
      khi nhảy nhánh lúc get giá trị k ptu nhỏ nhất
      tức khi đang ở nút cur
      mình nhảy sang nhánh phải 
      thì tất cả các số có đường đi từ root đi qua cur, xuống nhánh trái và tới lá đều có giá trị có thứ tự <= k
      nên mình cần lấy tổng tất cả các số này ở nhánh trái
      do mình chỉ mới xét tiền tố là từ root đến cur, từ cur đến lá có thể bị xor bởi giá trị T
      mà nhận thấy giá trị sum bị thay đổi chỉ bởi các bit i = 1 của T (tăng nếu đang từ 0 , xor T lên 1, giảm khi đang từ 1 , xor T xuống 0)
    */
    node(){
        cnt = 0;
        sum = 0;
        index[0] = index[1] = NULL;
        FU(j, 0, 17) d[j] = 0;
    }
};
node *root;
int T, t, x;

void readinp(){
    read(t), read(x);
}

int getbit(int x, int i){
    return (x >> i) & 1;
}

void add (int x){
    node *cur = root;
    FD(i, 17, 0){
        int bit = getbit(x, i);
        if (cur->index[bit] == NULL)
            cur->index[bit] = new node();
        cur = cur->index[bit];
        cur->cnt++;
        cur->sum+= ll(x);
        FU(j, 0, 17) cur->d[j]+= getbit(x, j);
    }
}

void del(int x){
    node *cur = root;
    FD(i, 17, 0){
        int bit = getbit(x, i);
        if (cur->index[bit] == NULL || cur->index[bit]->cnt == 0) return;
        else cur = cur->index[bit];
    }
    cur = root;

    FD(i, 17, 0){
        int bit = getbit(x, i);
        assert(cur->index[bit] != NULL);
        cur = cur->index[bit];
        cur->cnt--;
        cur->sum-= ll(x);
        FU(j, 0, 17) cur->d[j]-= getbit(x, j);
    }
}
ll get(int k){
    node *cur = root;
    ll res = 0, x = 0;
    FD(i, 17, 0){
        int bit = getbit(T, i);
        /// xét trường hợp 1 trong 2 nhánh = NULL
        {
            if (cur->index[bit] == NULL){
                cur = cur->index[!bit];
                x|= (!bit << i);
                continue;
            }
            if (cur->index[!bit] == NULL){
                cur = cur->index[bit];
                x|= (bit << i);
                continue;
            }
        }
        assert(cur->index[0] != NULL && cur->index[1] != NULL);
        if (cur->index[bit]->cnt >= k){ /// xét nhánh bên trái trước 
            cur = cur->index[bit];
            x|= (bit << i);
        }
        else{
            /// do các giá trị trong S bị thay đổi bởi các bit = 1 của T
            /// nếu ban đầu số c trong S có bit i = 1, mà bit i của T cũng = 1, thì bị giảm mất 2 ^ i 
            /// tương tự nếu ban đầu số c trong S có bit i = 0, mà bit i của T = 1 thì giá trị tăng lên 2 ^ i
            
            res+= cur->index[bit]->sum;
            FU(j, 0, 17){
                if (getbit(T, j)){
                    res-= ll(cur->index[bit]->d[j]) * ll(1 << j); 
                    res+= ll(cur->index[bit]->cnt - cur->index[bit]->d[j]) * ll(1 << j);
                }
            }
            k-= cur->index[bit]->cnt;
            cur = cur->index[!bit];
            x|= (!bit << i);
        }
    }
    return res + k * ll(x ^ T);
}

void out(node *cur, int i, int x){
    if (i == -1){
        cout << (x) << ' ';
        return;
    }
    if (cur->index[0] != NULL && cur->index[0]->cnt > 0)
        out(cur->index[0], i - 1, x);
    if (cur->index[1] != NULL && cur->index[1]->cnt > 0)
        out(cur->index[1], i - 1, x | (1 << i));
}

/*
  đối với truy vấn 2 mình cần thêm 1 biến T = tổng xor các giá trị x trong truy vấn 2 hiện tại
  thì khi thêm 1 ptu x vào S thay vì add x mình sẽ add(x xor T)
  tương tự khi xóa 1 ptu x thì mình sẽ del(x xor T)
  hãy tự lý giải điều này :3 
*/

void solve(int sub){
    if (t == 0){
        add(x ^ T);
    }
    else if (t == 1){
        del(x ^ T);
    }
    else if (t == 2){
        T^= x;
    }
    else{
        if (!x) cout << 0;
        else  cout << get(x);
        EL;
    }
}

int main(){
    setIO();
    int q = 1;
    read(q); root = new node();
    int t = 0;
    while (q--){
        readinp();
        solve(++t);
    }
    return 0;
}
