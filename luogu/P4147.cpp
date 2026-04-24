#include <bitset>
#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

using Bit = bitset<1000>;

vector<Bit> v(1000);

void _() {
    int ans = 0;
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            char t;
            cin >> t;
            if (t == 'R')
                v[i][j] = 1;
        }
    }

    for (int i = 0; i < n; i++) {
        Bit t;
        for (int j = i; j >= 0; j--) {
            t |= v[j];
            int h = i - j + 1;
            int l = 0, r = 0;
            for (; r < m; r++) {
                if (t[r]) {
                    ans = max((r - l) * h, ans);
                    l = r + 1;
                }
            }
            ans = max((r - l) * h, ans);
        }
    }
    cout << ans * 3 << endl;
    cout << ans << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--)
        _();
}