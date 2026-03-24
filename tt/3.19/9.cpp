#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
using ll = long long;
using vll = vector<ll>;

const ll N = 1e6 + 10;

ll a1[N];
ll a2[N];

void solve() {
    int n; cin >> n;
    for (int i = 0;i < n;i++) cin >> a1[i];
    cin >> n;
    for (int i = 0;i < n;i++) cin >> a2[i];

    sort(a1, a1 + n);
    sort(a2, a2 + n);

    ll ans = 0;
    for (int i = 0;i < n;i++) {
        ans += a1[i] * a2[i] > 0 ? a1[i] * a2[i] : 0;
    }

    cout << ans << endl;

}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    //cin >> t;
    while (t--) {
        solve();
    }
    cout.flush();

    system("pause");

    return 0;
}