#include <stdint.h>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using ll = long long;

const int N = 1e6 + 10;

int n, m, k, p;
int s, t;

struct node {
    int id, dis, v;

    auto operator<(const node& other) const {
        if (dis == other.dis)
            return v < other.v;
        else
            return dis > other.dis;
    }
};

vector<ll> road[N];
ll val[N];
priority_queue<node> pq;
bool visited[N];
ll ans_v[N];
ll ans_dis[N];

void solve() {
    cin >> n >> m >> k >> p;
    for (int i = 0; i <= n; i++)
        ans_dis[i] = INT64_MAX;
    for (int i = 1; i <= n; i++)
        cin >> val[i];

    while (m--) {
        int u, v;
        cin >> u >> v;
        road[u].push_back(v);
        road[v].push_back(u);
    }

    cin >> s >> t;
    int c = 1;

    node st;

    st.v = p == 1 ? val[s] : 0;
    st.id = s, st.dis = 0;

    pq.push(st);
    ans_v[s] = st.v;
    ans_dis[s] = 0;

    while (!pq.empty()) {
        ll u = pq.top().id;
        ll v_sum_cur = pq.top().v;
        ll dis_sum_cur = pq.top().dis;

        pq.pop();
        if (dis_sum_cur != ans_dis[u] || v_sum_cur != ans_v[u])
            continue;

        // if (visited[u])
        //     continue;

        visited[u] = true;

        for (auto nei : road[u]) {
            node nn;
            nn.id = nei;
            nn.dis = dis_sum_cur + 1;
            if ((nn.dis) % k + 1 == p) {
                nn.v = v_sum_cur + val[nei];
            } else {
                nn.v = v_sum_cur;
            }

            if (!visited[nei]) {
                if (nn.dis < ans_dis[nei]) {
                    pq.push(nn);
                    ans_dis[nei] = nn.dis;
                    ans_v[nei] = nn.v;
                } else if (nn.dis == ans_dis[nei] && nn.v > ans_v[nei]) {
                    pq.push(nn);
                    ans_dis[nei] = nn.dis;
                    ans_v[nei] = nn.v;
                }
            }
        }
    }

    cout << ans_v[t] << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--) {
        solve();
    }
    cout.flush();

    system("pause");

    return 0;
}