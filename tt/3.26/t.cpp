#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using ll = long long;

const ll N = 1e6 + 10;

ll g[1100][1100];
ll k, n, m;

vector<int> xyf;

int disjoint_set[1200];

int root(int id) {
    return disjoint_set[id] =
               disjoint_set[id] == id ? id : root(disjoint_set[id]);
}

void un(int a, int b) {
    disjoint_set[root(a)] = root(b);
}

map<int, vector<int>> ans;

void solve() {
    cin >> k >> n >> m;

    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u][v] += w;
    }

    for (int i = 1; i <= n; i++) {
        int z = 0, m = 0;
        for (int j = 1; j <= n; j++) {
            auto t = g[i][j];
            if (t > 0 && t <= 5) {
                m++;
                if (g[j][i])
                    z++;
            }
        }
        if (m > k && z * 5 <= m) {
            xyf.push_back(i);
        }
    }

    if (xyf.empty()) {
        cout << "None" << endl;
        return;
    }

    // cout << "xyf.size=" << xyf.size() << endl;

    // sort(xyf.begin(), xyf.end());

    for (int i = 1; i <= n; i++)
        disjoint_set[i] = i;

    for (auto i : xyf)
        for (auto j : xyf) {
            if (g[i][j] && g[j][i])
                un(max(i, j), min(i, j));
        }

    for (auto i : xyf) {
        ans[root(i)].push_back(i);
    }

    // cout << "--------ans------------" << endl;
    //  cout << ans.size();
    for (auto [k, v] : ans) {
        for (int i = 0; i < v.size(); i++) {
            if (i != 0)
                cout << " ";
            cout << v[i];
        }
        cout << endl;
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