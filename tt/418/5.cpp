#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>

using namespace std;
using ll = long long;
using vll = vector<ll>;

set<int> s;
int a[111000];
void _() {
    int n;cin >> n;
    while (n--) {
        int u, v;cin >> u >> v;
        a[u] |= v;
        s.insert(u);
    }
    vll ans;
    //ans.reserve(n);
    for (auto i : s)
        if (!a[i]) ans.push_back(i);

    if (ans.size() == 0) {
        cout << "NONE" << endl;
        return;
    }

    for (int i = 0;i < ans.size();i++) {
        cout << ans[i];if (i != ans.size() - 1) cout << " ";
    }

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