#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;
using ll = long long;
using vll = vector<ll>;

int n;

struct number {
    vector<ll> dt;
    number(int n) {
        dt.reserve(100);
        while (n) {
            dt.push_back(n % 10);
            n /= 10;
        }
    }

    number& operator*(ll other) {
        ll tmp = 0, p = 0;
        while (p < dt.size()) {
            tmp += dt[p] * other;
            dt[p++] = tmp % 10;
            tmp /= 10;
        }

        while (tmp) {
            dt.push_back(tmp % 10);
            tmp /= 10;
        }

        return *this;
    }

    void out() {
        for (int i = dt.size() - 1; i >= 0; i--) {
            cout << dt[i];
        }
        cout << endl;
    }
};

void init() {
    cin >> n;
}

vll v;
void solve() {
    ll s = 0, i = 2;
    while (s < n) {
        s += i;
        v.push_back(i++);
    }

    if (s > n + 1)
        v[s - n - 2] = 0;
    else if (s == n + 1)
        v.back()++, v[0] = 0;
}

void output() {
    number num(1);
    for (int i : v) {
        if (i) {
            cout << i << " ";
            num = num * i;
        }
    }
    cout << endl;
    num.out();
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

    output();
    system("pause");
}