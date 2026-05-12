#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;
using ll = long long;
using vll = vector<ll>;
using vint = vector<int>;

#define endl "\n"

void _() {
    int n, q;
    cin >> n >> q;
    vint arr(n);
    vector<pair<int, int>> arr2;
    map<int, vector<int>> ma;
    for (int i = 0;i < n;i++) {
        cin >> arr[i];
        ma[arr[i]].push_back(i + 1);
        arr2.push_back({ arr[i], i });
    }
    sort(arr2.begin(), arr2.end());
    while (q--) {
        int a, b;
        cin >> a >> b;
        int k = a ^ b;
        int cc = b - a;
        //cout << "k " << k << endl;
        bool flag = false;
        if (!ma.count(k)) {
            pair aa = { cc,0 };
            auto it = lower_bound(arr2.begin(), arr2.end(), aa) - arr2.begin();

            for (int j = it;j < arr2.size();j++) {

                int i = arr2[j].second - 1;
                if (
                        (i + 1 < n && ((arr[i] ^ a) < b) && ((arr[i + 1] ^ a) > b)) ||
                        (i + 1 < n && ((arr[i] ^ a) > b) && ((arr[i + 1] ^ a) < b)) ||
                        (i + 2 < n && ((arr[i + 1] ^ a) > b) && ((arr[i + 2] ^ a) < b)) ||
                        (i + 2 < n && ((arr[i + 1] ^ a) < b) && ((arr[i + 2] ^ a) > b))
                    ) {
                    flag = true;
                    cout << i + 1 << endl;
                    break;
                }


            }

        }
        else {
            flag = true;
            //cout << " cache " << k << endl;
            if (ma[k].front() - 1 >= 1) cout << ma[k].front() - 1 << endl;
            else cout << 1 << endl;
        }

        // for (int i = 0;i < n - 1;i++) {
        //     if (
        //         ((a ^ arr[i]) - b) * ((a ^ arr[i + 1]) - b) <= 0
        //     ) {
        //         flag = true;
        //         cout << i + 1 << endl;
        //         break;
        //     }
        // }

        if (!flag) cout << -1 << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr), cout.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--) _();

    cout.flush();
}