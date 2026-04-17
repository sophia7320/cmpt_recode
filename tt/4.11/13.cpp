#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

//#define endl "\n"

using namespace std;
using ll = long long;
using vll = vector<long long>;
using pq = priority_queue<ll>;

ll n;

bool mi(vll& tree, ll pos) {
    if (pos > n) return true;

    bool tmp = true;
    if (pos * 2 <= n) tmp &= tree[pos * 2] > tree[pos];
    if (pos * 2 + 1 <= n) tmp &= tree[pos * 2 + 1] > tree[pos];

    return tmp && mi(tree, pos * 2) && mi(tree, pos * 2 + 1);
}

bool ma(vll& tree, ll pos) {
    if (pos > n) return true;

    bool tmp = true;
    if (pos * 2 <= n) tmp &= tree[pos * 2] < tree[pos];
    if (pos * 2 + 1 <= n) tmp &= tree[pos * 2 + 1] < tree[pos];

    return tmp && ma(tree, pos * 2) && ma(tree, pos * 2 + 1);
}

void back(vll& tree, ll pos) {
    if (pos > n) return;

    back(tree, pos * 2);
    back(tree, pos * 2 + 1);

    cout << tree[pos]; if (pos != 1) cout << " ";
}

void _() {

    vll tree(n + 1);
    for (int i = 1;i <= n;i++) cin >> tree[i];

    if (ma(tree, 1)) cout << "Max Heap" << endl;
    else if (mi(tree, 1)) cout << "Min Heap" << endl;
    else  cout << "Not Heap" << endl;

    back(tree, 1);cout << endl;

}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    cin >> t >> n;
    while (t--) _();

    cout.flush();
    system("pause");
}
