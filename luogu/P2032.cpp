#include <iostream>
#include <set>
#include <vector>

using namespace std;
using ll = long long;
using mset = multiset<ll>;

mset s;
vector<ll> v;

void solve() {
    int n, k;
    cin >> n >> k;
    v.reserve(n);
    for (int i = 0; i < n; i++) {
        ll t;
        cin >> t;

        v.push_back(t);

        if (i >= k) {
            cout << *s.crbegin() << "\n";
            s.erase(s.find(v[i - k]));
        }
        s.insert(t);

        if (i == n - 1) {
            cout << *s.crbegin() << "\n";
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--)
        solve();
}