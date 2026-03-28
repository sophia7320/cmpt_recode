#include <stdint.h>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using ll = long long;

const ll N = 1e6 + 10;

vector<ll> v;
vector<ll> ans;

void solve() {
    int n;
    cin >> n;
    v.resize(n);
    ans.resize(n);
    for (auto& i : v)
        cin >> i;

    int p = 0, t = 200, i = 1;
    ans[0] = 200;
    while (i < n) {
        if (v[i] > v[i - 1]) {
            t += 100;
            ans[i++] = t;
            p = i;
        } else if (v[i] < v[i - 1]) {
            t = ans[i] = 200;
            p = i;
            while (p - 1 >= 0 && ans[p - 1] == ans[p] && v[p - 1] != v[p]) {
                ans[p - 1] += 100;
                p--;
            }
            i++;
        } else {
            ans[i] = ans[i - 1];
            i++;
        }
    }
    ll s = 0;
    for (auto& i : ans)
        s += i;

    cout << s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--) {
        solve();
    }
    cout.flush();

    system("pause");

    return 0;
}