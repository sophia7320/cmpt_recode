#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <stdint.h>

using namespace std;
using ll = long long;
using vll = vector<ll>;
using vint = vector<int>;


ll v[1100][2];

void _() {
    int n;cin >> n;
    for (int i = 0;i <= n;i++) {
        cin >> v[i][0] >> v[i][1];
    }

    sort(v + 1, v + n + 1, [](ll(&a)[2], ll(&b)[2]) {
        if ()
    });
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