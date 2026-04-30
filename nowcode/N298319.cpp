//小红的双排列
#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <stdint.h>
#include <string>
#include <climits>

#define endl "\n"

using namespace std;
using ll = long long;
using vll = vector<ll>;
using vint = vector<int>;

const ll M = 983244157;
int n;

ll pre3[999999];

ll v[999999];

ll prev3[999999];
ll preo[999999];

ll qpow(ll a, ll b) {
    ll ans = 1;
    while (b) {
        if (b & 1)
            ans = ans * a % M;
        a = (a * a) % M, b >>= 1;
    }
    return ans;
}

void hash_init() {
    for (int i = 1;i * 2 <= n + 100;i++)
        pre3[i] = (pre3[i - 1] + qpow(i, 5)) % M;
    for (int i = 1;i <= n;i++)
        prev3[i] = (prev3[i - 1] + qpow(v[i], 5)) % M;
    for (int i = 1;i <= n;i++)
        preo[i] = preo[i - 1] ^ v[i];
}

bool hash1_check(int i, int j) {
    ll len = j - i + 1;
    if (len & 1) return false;

    ll a = (prev3[j] - prev3[i - 1] + M) % M;
    ll b = pre3[len / 2] * 2 % M;

    return a == b;
}

bool hash2_check(int i, int j) {
    return !(preo[j] ^ preo[i - 1]);
}

void _() {
    int q;
    cin >> n >> q;

    for (int i = 1; i <= n;i++) cin >> v[i];

    hash_init();

    while (q--) {
        int i, j;cin >> i >> j;

        if (hash1_check(i, j) && hash2_check(i, j))
            cout << "Yes" << endl;
        else
            cout << "No" << endl;
    }

}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--) _();

    cout.flush();

    system("pause");
}