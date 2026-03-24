#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
using ll = long long;

int bs[1000000];

int root(int id) {
    return bs[id] = bs[id] == id ? bs[id] : root(bs[id]);
}

void solve() {
    int n, m;
    cin >> n >> m;

    for (int i = 1;i <= n;i++) bs[i] = i;

    while (m--) {
        int n;
        cin >> n;

        vector<ll> v;
        v.resize(n);
        for (auto& i : v) cin >> i;

        for (auto i : v) bs[root(i)] = root(v[0]);
    }

    unordered_map<int, int> mp;

    for (int i = 1;i <= n;i++) {
        mp[root(i)]++;
    }

    int ans = 0;
    for (auto [key, va] : mp) {
        ans = max(ans, va);
    }

    cout << ans;
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