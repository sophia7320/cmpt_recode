#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

//#define endl "\n"

using namespace std;
using ll = long long;
using vll = vector<long long>;
using pq = priority_queue<ll>;

const ll M = 998244353;

ll c[11100];

vll dn[11100];

ll sc[110];

ll vis[45];

ll dp[50][50];

int n;

ll pow(ll a, ll b) {
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % M;
        a *= a, b >>= 1;
    }
    return res;
}

void init_sc(int u) {
    for (auto v : dn[u]) {
        init_sc(v);
        sc[u] += sc[v];
        vis[u] |= vis[v];
    }
    vis[u] |= (1 << u);
    ++sc[u];
}


void dfs(int cur) {
    ///dp[cur][1] = 1;

    for (auto v : dn[cur]) dfs(v);

    dp[cur][1] = pow(c[1], sc[cur]);

}

void _() {
    cin >> n;
    for (int i = 2;i <= n;i++) {
        ll t;cin >> t;
        dn[t].push_back(i);
    }

    for (int i = 1;i <= n;i++) cin >> c[i];

    init_sc(1);
    dfs(1);

    ll ans = 0;
    for (int k = 1;k <= n;k++) {
        ans = (ans + dp[1][k]) % M;
    }
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
