#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;
using ll = long long;
using vll = vector<ll>;
using vint = vector<int>;

#define endl "\n"

template<typename T>
void print(T& v) {
    for (auto& it : v)
        cout << it << " ";
    cout << endl;
}

struct igd {
    int id, t;

    bool operator<(const igd& other) const {
        return t > other.t;
    }
};

priority_queue<igd> k;
priority_queue<igd> c;

void print(priority_queue<igd>& q) {
    cout << q.size() << " ";
    while (!q.empty()) {
        auto t = q.top();q.pop();
        cout << t.id << " ";
    }

    cout << endl;
}


void _() {
    int n;cin >> n;
    for (int i = 1;i <= n;i++) {
        int t1, t2, b1, b2;
        cin >> t1 >> t2 >> b1 >> b2;
        if (b1 && b2) {
            if (t1 < t2)
                k.push({ i, t1 });
            else
                c.push({ i, t2 });
            continue;
        }

        if (b1) k.push({ i, t1 });

        if (b2) c.push({ i, t2 });
    }

    print(k);
    print(c);

}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr), cout.tie(nullptr);

    int t = 1;
    // cin>> t;
    while (t--) _();

    cout.flush();
}