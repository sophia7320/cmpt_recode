#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;
using ll = long long;
using vll = vector<ll>;

int h, m, n;

vll v;
void init() {
    cin >> h >> n >> m;
    v.resize(n);

    for (auto& i : v)
        cin >> i;

    v.push_back(h);
}

inline bool j(int t) {
    int cnt = 0;
    int p = -1, cur = 0;
    while (++p < v.size()) {
        if (v[p] - cur < t) {
            cnt++;
        }
        else {
            cur = v[p];
        }
    }

    return cnt <= m;
}

void _() {
    int l = 0, r = h;
    while (l < r) {
        int mid = (l + r + 1) / 2;
        if (j(mid)) {
            l = mid;
        }
        else {
            r = mid - 1;
        }
    }

    cout << l << endl;
}

void output() {
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    init();

    int t = 1;
    // cin >> t;
    while (t--)
        _();

    output();
}