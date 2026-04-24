#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;
using ll = long long;
using vll = vector<ll>;

ll v[25];
vll nx[25];

ll dp[25];
ll fr[25];

void out(int n) {
    if (n == 0)return;
    out(fr[n]);
    cout << n << " ";
}

void solve() {
    int n;cin >> n;
    for (int i = 1;i <= n;i++) cin >> v[i];
    for (int i = 1;i < n;i++)
        for (int j = i + 1;j <= n;j++) {
            bool ex;cin >> ex;
            if (ex) nx[i].push_back(j);
        }

    for (int i = 1;i <= n;i++) dp[i] = v[i];

    for (int i = 1;i <= n;i++)
        for (auto nxt : nx[i])
            if (dp[i] + v[nxt] > dp[nxt])
                dp[nxt] = dp[i] + v[nxt], fr[nxt] = i;

    int ans, t = 0;
    for (int i = 1;i <= n;i++) if (dp[i] > 0) ans = i, t = dp[i];

    out(ans);cout << endl;
    cout << t << endl;

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