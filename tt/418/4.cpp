#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;
using ll = long long;
using vll = vector<ll>;

void _() {
    int n;cin >> n;
    int ans = 0;
    while (n--) {
        int t;cin >> t;
        ans += t < 1700 ? 1 : 0;
    }
    cout << ans << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    //cin >> t;
    while (t--) _();

    cout.flush();

    system("pause");
}