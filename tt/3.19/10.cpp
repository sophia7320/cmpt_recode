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
    int pre = -1;
    while (p != -1) {

        int a = abs(m[p].v);
        if (us.find(a) == us.end()) {
            us.insert(a);
            pre = p;
        }
        else {
            ru.push_back(m[p]);
            m[pre].nxt = m[p].nxt;
        }

        p = m[p].nxt;
    }

    p = head;
    while (p != -1) {
        cout << right << setw(5) << p << " " << m[p].v << " ";
        if (m[p].nxt != -1)
            cout << right << setw(5) << m[p].nxt << "\n";
        else
            cout << -1 << "\n";
        p = m[p].nxt;
    }
    for (int i = 0;i < ru.size();i++) {
        if (i != ru.size() - 1)
            ru[i].nxt = ru[i + 1].adr;
        else
            ru[i].nxt = -1;
    }

    for (auto& node : ru) {
        cout << right << setw(5) << node.adr << " " << node.v << " ";
        if (node.nxt != -1)
            cout << right << setw(5) << node.nxt << "\n";
        else
            cout << -1 << "\n";
    }
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