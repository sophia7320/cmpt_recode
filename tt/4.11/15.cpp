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
        a *= a, b >>= 1;
    }
    return res;
}

void init_sc(int u) {
    vis[u] |= (1ll << u);
    for (auto v : dn[u]) {
        vis[v] |= vis[u];
        init_sc(v);
        sc[u] += sc[v];
    }
    ++sc[u];
}


void _() {
    cin >> n;
    for (int i = 2;i <= n;i++) {
        ll t;cin >> t;
        dn[t].push_back(i);
    }

    for (int i = 1;i <= n;i++) cin >> c[i];

    init_sc(1);

    for (int cur = 1;cur <= n;cur++) dp[cur][1] = pow(c[1], sc[cur]);

    for (int turn = 2;turn <= n;turn++)
        for (int dst = 1;dst <= n;dst++)
            for (int src = 1; src <= n;src++)
                if (!(vis[dst] & (1ll << src)))
                    dp[dst][turn] = (dp[dst][turn] + dp[src][turn - 1] * pow(c[turn], sc[dst]) % M) % M;

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
