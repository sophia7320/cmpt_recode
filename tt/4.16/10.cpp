#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

//#define endl "\n"

using namespace std;
using ll = long long;
using vll = vector<long long>;
using pq = priority_queue<ll>;

int n, u, v;

vll ans;
vll egs[222000];

void dfs(int node, int fr) {
    ans.push_back(node);
    if (node == v) {
        for (int i = 0;i < ans.size();i++) {
            cout << ans[i];if (i != ans.size() - 1) cout << " ";
        }
        return;
    }

    for (auto next : egs[node])
        if (fr != next)
            dfs(next, node);

    ans.pop_back();
}

void _() {
    cin >> n >> u >> v;
    for (int i = 1;i < n;i++) {
        int u, v;cin >> u >> v;
        egs[u].push_back(v);
        egs[v].push_back(u);
    }

    dfs(u, 0);

}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    // cin >> t ;
    while (t--) _();

    cout.flush();
    system("pause");
}
