#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;
using ll = long long;
using vll = vector<ll>;

ll v[25];
vll nx[25];

ll dp[25][25];
ll fr[25][25];

void out(int i, int j) {
    if (i == 1) {
        cout << j << " ";
        return;
    }
    out(i - 1, fr[i][j]);
    cout << j << " ";
}

void solve() {
    int n;cin >> n;
    for (int i = 1;i <= n;i++) cin >> v[i];
    for (int i = 1;i < n;i++)
        for (int j = i + 1;j <= n;j++) {
            bool ex;cin >> ex;
            if (ex) nx[i].push_back(j);
        }

    for (int i = 1;i <= n;i++) dp[1][i] = v[i];

    for (int i = 1;i <= n - 1;i++)
        for (int src = 1;src <= n;src++)
            for (auto nxt : nx[src])
                if (dp[i][src] + v[nxt] > dp[i + 1][nxt])
                    dp[i + 1][nxt] = dp[i][src] + v[nxt],
                    fr[i + 1][nxt] = src;

    ll ans[3]{};
    for (int i = 1;i <= n;i++)
        for (int j = 1;j <= n;j++)
            if (dp[i][j] > ans[2])
                ans[0] = i, ans[1] = j, ans[2] = dp[i][j];

    out(ans[0], ans[1]);cout << endl;

    cout << ans[2] << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--)
        solve();
    cout.flush();

    system("pause");
}