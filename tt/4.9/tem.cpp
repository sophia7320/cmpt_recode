#include <stdint.h>

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;
using ll = long long;
using vll = vector<ll>;

using pq = priority_queue<int>;

ll n, t;

void _() {
    int x;
    cin >> x >> n;
    int cnt = 0;
    while (true) {
        cnt++;
        cin >> t;

        if (t < 0 || cnt > n) {
            cout << "Game Over" << endl;
            return;
        }

        if (t == x) {
            if (cnt == 1) {
                cout << "Bingo!" << endl;
            } else if (cnt < 4) {
                cout << "Lucky You!" << endl;
            } else {
                cout << "Good Guess!" << endl;
            }
        }

        if (t < x) {
            cout << "Too small" << endl;
        } else {
            cout << "Too big" << endl;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cout.tie(nullptr);
    cin.tie(nullptr);

    int t = 1;
    // cin>>t;
    while (t--) _();
    cout.flush();

    system("pause");
}