#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;
using ll = long long;

const ll N = 1e6 + 10;

struct qwq {
    priority_queue<int, vector<int>, greater<int>> qmi;
    priority_queue<int> qma;

    qwq() : qma(), qmi() {}

    int getans() {
        return qma.top();
    }

    void insert(int n) {
        if (qmi.empty() && qma.empty()) {
            qma.push(n);
            return;
        }
        int t = getans();
        if (n < t)
            qma.push(n);
        else
            qmi.push(n);

        keep();
    }

    void keep() {
        int now = (qmi.size() + qma.size() + 1) / 2;
        while (qma.size() != now) {
            if (qma.size() > now) {
                qmi.push(qma.top());
                qma.pop();
            } else {
                qma.push(qmi.top());
                qmi.pop();
            }
        }
    }
};

void solve() {
    qwq q;

    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int t;
        cin >> t;
        q.insert(t);
        if (i & 1)
            cout << q.getans() << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--) {
        solve();
    }
    cout.flush();

    system("pause");

    return 0;
}