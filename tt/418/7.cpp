#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;
using ll = long long;
using vll = vector<ll>;

int v[11100];

void _() {
    int n;cin >> n;int sum = 0;
    for (int i = 0;i < n;i++) cin >> v[i], sum += v[i];

    auto maxe = max_element(v, v + n);
    auto mine = min_element(v, v + n);
    int avg = sum / n;

    cout << *maxe << " " << *mine << " " << avg << endl;

    vll ans;
    for (int i = 0;i < n;i++)
        if (v[i] > 2 * avg) ans.push_back(i + 1);

    if (ans.size() == 0) {
        cout << "Normal" << endl;
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