#include <stdint.h>

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;
using ll = long long;
using vll = vector<ll>;

struct qnode {
    ll id, cnt;

    bool operator<(const qnode& other) const {
        return cnt > other.cnt;
    }
};


using pq = priority_queue<qnode>;

vll v;
void _() {
    ll n, k;
    cin >> n >> k;
    v.resize(n);
    pq q;
    for (int i = 0;i < n;i++) {
        qnode nd{ i,0 };
        cin >> nd.cnt;
        if (nd.cnt) q.push(nd);
    }

    ll has_eat = 0;
    while (!q.empty()) {
        auto t = q.top().cnt;
        ll eat = (t - has_eat) * q.size();
        if (k - eat < 0) break;
        k -= eat; has_eat = t;
        q.pop();
        while (!q.empty() && q.top().cnt == has_eat) q.pop();
    }

    ll cnt = q.size();
    while (!q.empty()) {
        v[q.top().id] = q.top().cnt - has_eat;
        q.pop();
    }

    if (cnt == 0) {
        for (int i = 0;i < n;i++) cout << 0 << " ";
        return;
    }

    ll ofs = k / cnt;
    ll e = k % cnt;
    //cout << "E=" << e << endl;
    for (int i = 0;i < n;i++) {
        if (v[i]) {
            v[i] = v[i] - ofs;
            if (e > 0)  --v[i], --e;
        }
    }

    for (int i = 0;i < n;i++) cout << v[i] << " ";
}

int main() {
    // ios::sync_with_stdio(false);
    // cout.tie(nullptr);
    // cin.tie(nullptr);

    int t = 1;
    // cin>>t;
    while (t--) _();
    cout.flush();

    //system("pause");
}
