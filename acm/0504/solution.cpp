#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<ll> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    vector<vector<int>> adj(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    ll ans = 0;

    // 1 vertex
    for (int i = 0; i < n; ++i) ans = max(ans, a[i]);

    // 2 vertices (1 edge)
    for (int u = 0; u < n; ++u)
        for (int v : adj[u])
            if (u < v) ans = max(ans, a[u] + a[v]);

    // best1[v] = top neighbours of v sorted by a[u] descending
    const int K1 = 6;
    vector<vector<pair<ll, int>>> best1(n);
    for (int v = 0; v < n; ++v) {
        for (int u : adj[v]) best1[v].push_back({a[u], u});
        sort(best1[v].rbegin(), best1[v].rend());
        if ((int)best1[v].size() > K1) best1[v].resize(K1);
    }

    // 3 vertices (2 edges)
    for (int v = 0; v < n; ++v) {
        if ((int)best1[v].size() >= 2)
            ans = max(ans, a[v] + best1[v][0].first + best1[v][1].first);
    }

    // 4 vertices (3 edges)
    for (int b = 0; b < n; ++b) {
        for (int c : adj[b]) {
            if (b >= c) continue;
            for (auto &[va, aid] : best1[b]) {
                if (aid == c) continue;
                for (auto &[vd, did] : best1[c]) {
                    if (did == b || did == aid) continue;
                    ans = max(ans, va + a[b] + a[c] + vd);
                }
            }
        }
    }

    // best2[v] = top two-step paths v - mid - far, value = a[mid] + a[far]
    const int K2 = 12;
    vector<vector<tuple<ll, int, int>>> best2(n);
    for (int v = 0; v < n; ++v) {
        priority_queue<tuple<ll, int, int>, vector<tuple<ll, int, int>>, greater<>> pq;
        for (int mid : adj[v]) {
            for (int far : adj[mid]) {
                if (far != v) {
                    pq.push({a[mid] + a[far], mid, far});
                    if ((int)pq.size() > K2) pq.pop();
                }
            }
        }
        while (!pq.empty()) {
            best2[v].push_back(pq.top());
            pq.pop();
        }
        reverse(best2[v].begin(), best2[v].end());
    }

    // 5 vertices (4 edges)
    for (int c = 0; c < n; ++c) {
        for (int d : adj[c]) {
            if (c >= d) continue;

            // 2 left (c - mid - far) + 1 right (d - e)
            for (auto &[vl, ml, fl] : best2[c]) {
                if (ml == d || fl == d) continue;
                for (auto &[vr, e] : best1[d]) {
                    if (e == c || e == ml || e == fl) continue;
                    ans = max(ans, a[c] + a[d] + vl + vr);
                    break;
                }
            }

            // 1 left (c - e) + 2 right (d - mid - far)
            for (auto &[vr, mr, fr] : best2[d]) {
                if (mr == c || fr == c) continue;
                for (auto &[vl, e] : best1[c]) {
                    if (e == d || e == mr || e == fr) continue;
                    ans = max(ans, a[c] + a[d] + vl + vr);
                    break;
                }
            }
        }
    }

    cout << ans << "\n";
    return 0;
}
