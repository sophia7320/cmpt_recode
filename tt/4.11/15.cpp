#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>
#include <unordered_map>

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

unordered_map<ll, ll> dp[50];

int n;

ll pow(ll a, ll b) {
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % M;
        a = (a * a) % M, b >>= 1;
    }
    return res;
}

ll qpow[45][45];

void init_sc(int u) {
    vis[u] |= (1ll << u);
    for (auto v : dn[u]) {
        vis[v] |= vis[u];
        init_sc(v);
        sc[u] += sc[v];
    }
    ++sc[u];
}

int cnt1(ll v) {
    int cnt = 0;
    while (v) {
        v &= v - 1;
        cnt++;
    }
    return n - cnt;
}

void _() {
    cin >> n;
    for (int i = 2;i <= n;i++) {
        ll t;cin >> t;
        dn[t].push_back(i);
    }

    for (int i = 1;i <= n;i++) cin >> c[i];

    init_sc(1);

    for (int i = 1;i <= n;i++)
        for (int j = 1;j <= n;j++)
            qpow[i][j] = pow(c[i], sc[j]);

    for (int t = 1;t <= n;t++) dp[t][vis[1]] = pow(c[t], sc[1]);

    for (int t = n;t >= 2;t--)
        for (auto& [mask, val] : dp[t])
            for (int dst = 1;dst <= n;dst++) {
                if (cnt1(mask) < t - 1) break;

                if (!(mask & (1ll << dst)))
                    dp[t - 1][mask | vis[dst]] = (dp[t - 1][mask | vis[dst]] + val * qpow[t - 1][dst] % M) % M;
            }

    ll ans = 0;
    for (auto& [m, v] : dp[1]) ans = (ans + v) % M;
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
