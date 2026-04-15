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

ll polute(int id, vector<bool>& vis) {
    ll t = 1;
    if (vis[id]) return 0;
    vis[id] = true;
    for (auto& i : dn[id]) {
        t += polute(i, vis);
    }
    return t;
}

void dfs(vector<bool>& vis, ll cnt) {
    if (cnt == n) {
        ll tmp = 1;
        for (int i = 0;i < ansv.size();i++) {
            //cout << ansv[i] << " ";
            tmp = tmp * pow(c[i + 1], ns[ansv[i]]) % M;
            //cout << "i=" << i << " " << "c[i]= " << c[i] << " " << "ns[i]= " << ns[i] << " ";
        }

        ans = (ans + tmp) % M;
        return;
    }


    for (int i = 1;i <= n;i++) if (!vis[i]) {
        vector<bool> next(vis);
        ll cnt_t = cnt + polute(i, next);
        ansv.push_back(i);
        dfs(next, cnt_t);
        ansv.pop_back();
    }
}

void _() {
    cin >> n;
    for (int i = 2;i <= n;i++) {
        ll up;cin >> up;
        dn[up].push_back(i);
    }

    for (int i = 1; i <= n;i++) {
        ll t;cin >> t;
        c[i] = t;
    }

    init(1);

    dfs(vis, 0);

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
