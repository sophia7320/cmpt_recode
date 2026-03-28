#include <iostream>
#include <set>
#include <vector>

using namespace std;
using ll = long long;

string s;

void solve() {
    cin >> s;
    ll n = s.length();

    vector<ll> dp(n + 1);
    ll ans = 0;
    for (ll i = 1; i <= n; i++) {
        for (ll j = 0; j < i; j++) {
            if (s[j] <= s[i])
                ans = max(ans, dp[i] = max(dp[j] + 1, dp[i]));
        }
    }

    cout << ans << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--)
        solve();

    system("pause");
}