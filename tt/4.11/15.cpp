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
    vis[u] |= (1ll << u);
    ++sc[u];
}


void dfs(int cur) {
    ///dp[cur][1] = 1;

    for (auto v : dn[cur]) dfs(v);

    dp[cur][1] = pow(c[1], sc[cur]);

    for (int v = 1;v <= n;v++) {
        if (dp[v][1] == 0) continue;

        for (int i = 2;i <= sc[cur];i++)
            for (int p = 1;p <= n;p++)
                if (vis[v] & (1 << p))
                    dp[v][i] += dp[p][i - 1] * pow(c[v], sc[v]);

    }

}

void _() {
    cin >> n;
    for (int i = 2;i <= n;i++) {
        ll t;cin >> t;
        dn[t].push_back(i);
    }

    for (int i = 1;i <= n;i++) cin >> c[i];

    init_sc(1);

    for (int cur = 1;cur <= n;cur++)dp[cur][1] = pow(c[1], sc[cur]);

    for (int turn = 2;turn <= n;turn++)
        for (int dst = 1;dst <= n;dst++)
            for (int src = 1; src <= n;src++)
                if (vis[dst] & (1ll << src))
                    if (src != dst)
                        dp[dst][turn] = (dp[dst][turn] + dp[src][turn - 1] * pow(c[dst], sc[dst]) % M) % M;

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
