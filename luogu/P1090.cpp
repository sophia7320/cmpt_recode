#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

using namespace std;
using ll = long long;
using vll = vector<ll>;

int n;

priority_queue<ll, vector<ll>, greater<ll>> pq;
void init() {
    cin >> n;
    while (n--) {
        int t;
        cin >> t;
        pq.push(t);
    }
}

void solve() {
    ll ans = 0;
    while (pq.size() != 1) {
        ll t = pq.top();
        pq.pop();
        t += pq.top();
        pq.pop();

        ans += t;
        pq.push(t);
    }

    cout << ans << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    init();

    int t = 1;
    // cin >> t;
    while (t--)
        solve();
}