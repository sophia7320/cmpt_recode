#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;
using ll = long long;

const ll N = 1e6 + 10;

vector<ll> ii;
vector<ll> v(51);
vector<vector<ll>> vc(51);
int n;
bool ans[1100000];

void solve() {
    cin >> n;
    ii.reserve(n);
    for (int i = 1; i <= 50; i++) {
        vc.reserve(50);
        v.reserve(50);
    }
    while (n--) {
        int t;
        cin >> t;
        ii.push_back(t);
        v[t]++;
    }
    int m1 = -1, m2 = -1;
    for (int i = 50; i >= 1; i--) {
        if (v[i] > 1) {
            m1 = m2 = i;
            break;
        } else if (v[i] == 1) {
            m1 = i;
            i--;
            while (i >= 0 && v[i] == 0)
                i--;
            m2 = i;
            break;
        }
    }
    for (int i = 50; i >= 1; i--) {
        if (!v[i])
            continue;
        int p = i + 1;
        while (p <= 50) {
            if (p <= m2 && v[p]) {
                vc[i].push_back(p - i);
            }
            p++;
        }
    }

    ll ma = 0;
    for (auto i : ii) {
        ma += vc[i].empty() ? 0 : vc[i].back();
    }
    ans[0] = true, ans[ma] = true;
    vector<ll> tmp;
    tmp.reserve(100000);
    unordered_set<ll> jt;
    for (int i = 1; i < ma; i++) {
        jt.insert(i);
    }
    for (auto i : ii) {
        tmp.clear();
        for (auto j : jt)
            for (auto offset : vc[i])
                if (j - offset >= 0 && ans[j - offset])
                    tmp.push_back(j);
        for (auto& tt : tmp) {
            jt.erase(tt);
            ans[tt] = true;
        }
    }
    for (int i = 0; i <= ma; i++) {
        if (ans[i])
            cout << i << " ";
    }
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