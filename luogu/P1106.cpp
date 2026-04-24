#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <stdint.h>

using namespace std;
using ll = long long;
using vll = vector<ll>;
using vint = vector<int>;

void _() {
    string n; cin >> n; n += '0';
    int k;cin >> k;
    while (k--) {
        int p; for (p = 0;p < n.length();p++) if (n[p] > n[p + 1]) break;

        n = n.substr(0, p) + n.substr(p + 1);
    }

    int p = 0;
    while (n[p] == '0' && p < n.length()) p++;
    while (p < n.length() - 1) cout << n[p++];
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