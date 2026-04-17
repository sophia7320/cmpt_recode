#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_map>

#define debug(x) cout<<"#x = "<<x<<endl;

//#define endl "\n"

using namespace std;
using ll = long long;
using vll = vector<long long>;
using pq = priority_queue<ll>;

const ll M = 998244353;

unordered_map<ll, ll > dic;
ll c[100], cv[100], n;

vll dn[100];
ll ns[100];

ll ans = 0;
vll ansv;

ll pow(ll a, ll b) {
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % M;
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

ll lowbit(ll x) {
    x = ~x;
    return x & -x;
}
ll lowbit_loca(ll x) {
    if (dic.find(lowbit(x)) != dic.end())
        return dic[lowbit(x)] <= n ? dic[lowbit(x)] : 0;
    else
        return 0;
}


ll polute(int id, ll& vis) {
    ll t = 1;
    if ((vis >> (id - 1)) & 1) return 0;
    vis |= (1 << (id - 1));
    for (auto& i : dn[id]) {
        t += polute(i, vis);
    }
    return t;
}

void dfs(ll vis, ll cnt) {
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

    ll v = vis;ll pos;

    while (pos = lowbit_loca(v)) {
        ll next = vis;
        v += lowbit(v);
        ll cnt_t = cnt + polute(pos, next);
        ansv.push_back(pos);
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

    for (int i = 0;i <= n;i++) {
        dic[pow(2, i)] = i + 1;
    }

    for (int i = 1; i <= n;i++) {
        ll t;cin >> t;
        c[i] = t;
    }

    init(1);

    ll vis = 0;
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
