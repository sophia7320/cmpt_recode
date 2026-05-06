#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <stdint.h>

using namespace std;
using ll = long long;
using vll = vector<ll>;
using vint = vector<int>;
int n, m;
vint q, h;
vector<vint> gridq;
vector<vint> gridh;

void bfs(int i) {
	int qn = 0, hn = 0;
	vint qb(n + 1, 0), hb(n + 1, 0);
	queue<int> qq, hq;
	qq.push(i);
	hq.push(i);
	qb[i] = hb[i] = 1;
	while (!qq.empty()) {
		int t = qq.front();
		qq.pop();
		for (int num : gridq[t]) {
			if (!qb[num]) {
				qn++;
				qq.push(num);
				qb[num] = 1;
			}
		}
	}
	while (!hq.empty()) {
		int t = hq.front();
		hq.pop();
		for (int num : gridh[t]) {
			if (!hb[num]) {
				hn++;
				hq.push(num);
				hb[num] = 1;
			}
		}
	}
	q[i] = qn;
	h[i] = hn;
}

void _() {

	cin >> n >> m;
	q.resize(n + 1);
	h.resize(n + 1);
	gridq.resize(n + 1);
	gridh.resize(n + 1);
	while (m--) {
		int a, b;
		cin >> a >> b;
		gridq[a].push_back(b);
		gridh[b].push_back(a);
	}
	for (int i = 1;i <= n;i++) {
		bfs(i);
		cout << (q[i] < (n + 1) / 2 && h[i] < (n + 1) / 2);
	}
	cout << endl;

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);

	int t = 1;
	cin >> t;
	while (t--) _();
	cout.flush();
}