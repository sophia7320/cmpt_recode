#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <iomanip>

using namespace std;
using ll = long long;

const int N = 1e6 + 10;

struct node {
    int adr, v, nxt;
};

node m[N];

void solve() {
    int head, n;
    cin >> head >> n;

    for (int i = 0;i < N;i++) m[i].nxt = -1, m[i].adr = i;

    while (n--) {
        int id, va, nxt;
        cin >> id >> va >> nxt;
        m[id].v = va, m[id].nxt = nxt;
    }

    int p = head;
    unordered_set<int> us;
    vector<node> ru;
    while (p != -1) {
        int a = abs(m[p].v);
        if (us.find(a) == us.end()) {
            us.insert(a);
            m[p].nxt = m[m[p].nxt].nxt;
        }
        else {
            ru.push_back(m[p]);
        }
        p = m[p].nxt;
    }

    p = head;
    while (p != -1) {
        cout << right << setw(5) << p << " " << m[p].v << " " << right << setw(5) << m[p].nxt << "\n";
        p = m[p].nxt;
    }

    for (auto& node : ru)
        cout << right << setw(5) << node.adr << " " << node.v << " " << right << setw(5) << node.nxt << "\n";

}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cout << setfill('0');

    int t = 1;
    //cin >> t;
    while (t--) {
        solve();
    }
    cout.flush();

    system("pause");

    return 0;
}