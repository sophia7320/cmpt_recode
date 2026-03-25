#include <stdint.h>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using ll = long long;

const ll N = 1e6 + 10;

ll gra[1100][1100];
ll n, m;

void solve() {
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            gra[i][j] = INT32_MAX;

    for (int i = 1; i <= n; i++)
        gra[i][i] = 0;

    while (m--) {
        int u, v;
        cin >> u >> v;
        gra[u][v] = 1;
        gra[v][u] = 1;
    }

    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                gra[i][j] = min(gra[i][j], gra[i][k] + gra[k][j]);
            }
        }
    }

    for (int i = 1; i <= n; i++) {
        int zz = 0, mm = n;
        for (int j = 1; j <= n; j++) {
            if (1 || i != j) {
                zz += gra[i][j] <= 6 ? 1 : 0;
                // mm += gra[i][j] != INT32_MAX ? 1 : 0;
            }
        }
        double res = zz / 1.0 / mm * 100;
        cout << i << ": " << fixed << setprecision(2) << res << "%\n";
    }
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