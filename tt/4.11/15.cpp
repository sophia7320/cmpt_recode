#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

#define debug(x) cout<<"#x = "<<x<<endl;

//#define endl "\n"

using namespace std;
using ll = long long;
using vll = vector<long long>;
using pq = priority_queue<ll>;

const ll M = 998244353;

ll c[100], cv[100], n;

vll dn[100];
ll ns[100];

vector<bool> vis(50);

ll ans = 0;
vll ansv;

ll pow(ll a, ll b) {
    ll res = 1;
    while (b) {
        if (b & 1)
            res = res * a % M;
        a *= a, b >>= 1;
    }
    return res;
}

void init(int id) {
    for (auto& i : dn[id]) {
        init(i);
        ns[id] += ns[i];
    }
    ++ns[id];
}

vll level[100];

void dfs(int le) {
    vll tmp(level[le].size());
    for (ll bits = 0;bits < pow(2, level[le].size()); bits++) {
        tmp.clear();
    }
}

void _() {
    cin >> n;
    vll v(n);
    for (int i = 2;i <= n;i++) {
        ll up;cin >> up;
        dn[up].push_back(i);
    }

    for (int i = 1; i <= n;i++) {
        ll t;cin >> t;
        c[i] = t;v[i - 1] = i;
    }

    init(1);

    queue<ll> q;
    q.push(1);int p = 0;
    while (!q.empty()) {
        ++p;
        for (int k = 0;k < q.size();k++) {
            auto t = q.front();q.pop();
            for (auto i : dn[t]) level[p].push_back(i), q.push(i);
        }
    }

    dfs(2);

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
