#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;
using ll = long long;

int n;

struct number {
    vector<ll> dt;
    number() {
        dt.reserve(100);
    }

    number& operator*(ll other) {
        ll tmp = 0, p = 0;
        while (p < dt.size()) {
            tmp = dt[p] * other;
            dt[p] = tmp % 10;
            tmp /= 10;
        }

        while (tmp) {
            dt.push_back(tmp % 10);
            tmp /= 10;
        }

        return *this;
    }
};

void init() {
    cin >> n;
}

void solve() {
}

void output() {
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    init();

    int t;
    cin >> t;
    while (t--)
        solve();

    output();
}