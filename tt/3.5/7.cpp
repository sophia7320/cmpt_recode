#include <iostream>
#include <string>
#include <vector>

using namespace std;
using ll = long long;

ll gcd(ll a, ll b) {
    ll t;
    while (t = a % b) a = b, b = t;
    return b;
}

int k, m;

ll num = 0;

void dfs(int sum, int id) {
    if (id == k) {
        ll t = num + 1;
        ll t_sum = 0;
        while (t) t_sum += t % 10, t /= 10;

        ll sum = 0;
        while (num) sum += num % 10, num /= 10;

        if (gcd(t_sum, sum) > 2)
    }
}

void solve() {
    static int times = 0;
    times++;
    cout << "case " << times;

    int m;
    cin >> k >> m;

    dfs(m, 0);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    cin >> t;
    while (t--) {
        solve();
    }
    cout.flush();

    system("pause");

    return 0;
}