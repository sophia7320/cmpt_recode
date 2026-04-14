#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

//#define endl "\n"

using namespace std;
using ll = long long;
using vll = vector<long long>;
using pq = priority_queue<ll>;

ll chess[10][10];
ll l, n, ans = 0;
ll sc[10][10];

void dfs(ll i, ll j, ll v) {
    if (i > n) {
        ans++;return;
    }
    if (i >= 2) sc[i - 1][j] = sc[i - 2][j] + chess[i - 1][j];

    ll t = min(l - sc[i - 1][j], v);
    if (t < 0) return;
    if (j < n) for (int k = 0; k <= t;k++) {
        chess[i][j] = k;
        dfs(i, j + 1, v - k);
    }
    else {
        chess[i][j] = v;
        if (v + sc[i - 1][j] > l) return;
        dfs(i + 1, 1, l);
    }

}

void _() {
    cin >> l >> n;

    dfs(1, 1, l);

    cout << ans << endl;
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
