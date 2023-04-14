Mô hình code giống hệt với Dijkstra, chỉ khác ở 2 điểm:
+ Dùng queue thông thường (vào trước ra trước) thay vì queue có độ ưu tiên
+ Dùng một mảng lưu lại danh sách các đỉnh đang có trong queue. Nếu một đỉnh đã có trong queue rồi thì không đưa vào hàng đợi nữa

long long dist[MAX];
bool inQueue[MAX];
vector<int> adj[MAX];

queue<int> q;
dist[1] = 0; inQueue[1] = true; q.push(1);

while (!q.empty()) {
	int u = q.front(); q.pop(); inQueue[u] = false;
	for (pair<int, int> edge : adj[u]) {
		int v = edge.first, c = edge.second;
		if (minimize(dist[v], dist[u] + c) && !inQueue[v]) {
			q.push(v);
			inQueue[v] = true;
		}
	}
}

Thuật toán này dùng được cho đồ thị có trọng số âm. Độ phức tạp trong trường hợp xấu nhất là O(m*n) nhưng tốc độ trung bình ngang với Dijkstra heap.
