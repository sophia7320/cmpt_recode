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

int fr[45];

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
        fr[i] = t;
        dn[t].push_back(i);
    }

    for (int i = 1;i <= n;i++) cin >> c[i];

    init_sc(1);

    for (int turn = 1;turn <= n;turn++) dp[turn][vis[1]] = pow(c[turn], sc[1]), dp[turn][vis[1]] = vis[1];

    //vis[0] = vis[1];

    for (int t = n - 1;t >= 2;t--)
        for (auto& [mask, val] : dp[t]) {
            for (int dst = 1;dst <= n;dst++)
                if (!(mask & (1ll << dst)))
                    dp[t - 1][mask & vis[dst]] = dp[t - 1][mask & vis[dst]] + dp[t][mask] * pow(c[t - 1], sc[dst]);
        }





    ll ans = 0;
    for (auto& [mask, v] : dp[1]) ans += v;
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
