#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

using vstr = vector<string>;
using vint = vector<int>;

void solve() {
    int n;
    cin >> n;
    vstr datas(n);
    for (auto& i : datas)
        cin >> i;
    vint ids(n);
    for (int i = 0; i < n; i++)
        ids[i] = i;

    do {
        for (auto id : ids)
            cout << datas[id] << " ";

        cout << endl;
    } while (next_permutation(ids.begin(), ids.end()));
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
}