#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;
using ll = long long;
using vll = vector<ll>;
const ll M = 1e5 + 10;

int n;
ll v[M];
ll top;
void init() {
    cin >> n;
    for (int i = 0; i < n; i++)
        cin >> v[i];
    sort(v, v + n);
}

ll s[M], f[M];

void solve() {
    for (int i = 0; i < n; i++) {
        int t = v[i];
        ll pos = lower_bound(f, f + top, t) - f;

        while (pos < top - 1 && t == f[pos + 1]) pos++;

        if (pos == top || f[pos] != t)
            f[++top - 1] = t + 1, s[top - 1]++;
        else
            f[pos]++, s[pos]++;

    }

    ll ans = INT32_MAX;
    for (int i = 0; i < top; i++) {
        ans = min(s[i], ans);
    }

    cout << ans << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    init();

    int t = 1;
    // cin >> t;
    while (t--)
        solve();
    cout.flush();

    system("pause");
}