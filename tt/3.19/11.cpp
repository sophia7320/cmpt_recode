#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;
using ll = long long;

const int N = 1e6 + 10;

int n, m, k, p;
int s, t;

vector<ll> road[N];
ll val[N];
priority_queue<pair<ll, ll>> pq;
bool visited[N];
ll ans[N];


void solve() {
    cin >> n >> m >> k >> p;
    for (int i = 1;i <= n;i++) cin >> val[i];

    while (m--) {
        int u, v;
        cin >> u >> v;
        road[u].push_back(v);
        road[v].push_back(u);
    }

    cin >> s >> t;
    int c = 1;
    pq.push({ val[s],s });
    while (!pq.empty()) {
        ll u = pq.top().second;
        ll v_sum_cur = pq.top().first;

        pq.pop();
        if (v_sum_cur < ans[u]) continue;

        visited[u] = true;

        for (auto& nei : road[u]) {

        }



    }



}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    //cin >> t;
    while (t--) {
        solve();
    }
    cout.flush();

    system("pause");

    return 0;
}