#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>
#include <iomanip>

//#define endl "\n"

using namespace std;
using ll = long long;
using vll = vector<long long>;
using pq = priority_queue<ll>;

ll cnt(int n) {
    int i = 0;set<int> s;
    while (n) {
        i++;
        s.insert(n % 10), n /= 10;
    }
    if (i < 4)s.insert(0);
    return s.size();
}

void _() {
    int y, n;cin >> y >> n;
    int t = y;
    while (cnt(y) != n) y++;
    cout << y - t << " " << setw(4) << y;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cout << setfill('0');

    int t = 1;
    // cin >> t ;
    while (t--) _();

    cout.flush();
    system("pause");
}
