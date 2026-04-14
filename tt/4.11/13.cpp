#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

//#define endl "\n"

using namespace std;
using ll = long long;
using vll = vector<long long>;
using pq = priority_queue<ll>;

ll chess[1100][1100];
ll pre_col[1100][1100];
ll z[1100][1100];

ll n;

void _() {
    cin >> n;
    for (int i = 1;i <= n;i++) for (int j = 1;j <= n;j++) {
        cin >> chess[i][j];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    // cin >> t ;
    while (t--) _();

    cout.flush();
    system("pause");
}
