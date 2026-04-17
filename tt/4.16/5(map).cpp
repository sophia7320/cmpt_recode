#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <stdint.h>
#include <map>

#define endl "\n"

using namespace std;
using ll = long long;

using vll = vector<ll>;

ll v[111000];

void _() {
    int n, m;cin >> n >> m;
    for (int i = 1;i <= n;i++) cin >> v[i];

    int k;cin >> k;
    while (k--) {
        vll tv;
        while (true) {
            ll t;cin >> t;
            if (t == 0) break;
            tv.push_back(t);
        }

        map<ll, ll> mp;
        for (auto& i : tv) ++mp[v[i]];

        int c = 0;
        for (auto& node : mp) {
            c++;
            cout << "B" << node.first << "-" << node.second;

            if (c != mp.size()) cout << " "; else cout << endl;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    //cin>>t;
    while (t--) _();

    cout.flush();

    system("pause");
}