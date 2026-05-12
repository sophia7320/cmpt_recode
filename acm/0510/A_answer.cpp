#include <bits/stdc++.h>
using namespace std;

const int INF = 0x3f3f3f3f;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int N, M;
        cin >> N >> M;

        vector<vector<array<int, 3>>> adj(N + 1);
        for (int i = 0; i < M; i++) {
            int u, v, a, b;
            cin >> u >> v >> a >> b;
            adj[u].push_back({v, a, b});
        }

        int maxA = (N - 1) * 200;
        vector<vector<int>> dp(N + 1, vector<int>(maxA + 1, INF));
        dp[1][0] = 0;

        for (int s = 0; s <= maxA; s++) {
            for (int u = 1; u <= N; u++) {
                int cur = dp[u][s];
                if (cur == INF) continue;
                for (auto [v, a, b] : adj[u]) {
                    int ns = s + a;
                    if (ns <= maxA) {
                        int &ref = dp[v][ns];
                        if (cur + b < ref) ref = cur + b;
                    }
                }
            }
        }

        long long best_prod = LLONG_MAX;
        int best_a = 0, best_b = 0;
        for (int s = 0; s <= maxA; s++) {
            int b = dp[N][s];
            if (b != INF) {
                long long prod = 1LL * s * b;
                if (prod < best_prod) {
                    best_prod = prod;
                    best_a = s;
                    best_b = b;
                }
            }
        }
        cout << best_a << " " << best_b << "\n";
    }
    return 0;
}
