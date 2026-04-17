#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

//#define endl "\n"

using namespace std;
using ll = long long;
using vll = vector<long long>;
using pq = priority_queue<ll>;

ll hnt(int src, int dst, int cnt) {
    if (cnt == 1) return 1;
    return hnt(1, 2, cnt - 1) + hnt(2, 3, cnt - 1) + 1;
}

void _() {
    int n;cin >> n;
    cout << hnt(1, 3, n);
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
