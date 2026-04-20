#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;
using ll = long long;
using vll = vector<ll>;

struct node {
    int id, score;

    bool operator<(const node& other) const {
        if (score == other.score)
            return id < other.id;
        else
            return score > other.score;
    }
};

node v[110000];
void _() {
    int n;cin >> n;
    for (int i = 0;i < n;i++) v[i].id = i + 1, cin >> v[i].score;

    sort(v, v + n);

    for (int i = 0;i < n;i++) {
        if (i == 0) cout << v[i].id;
        else if (v[i].score == v[i - 1].score) cout << " " << v[i].id;
        else break;
    }
    cout << endl;

    int m;
    cin >> m;
    while (m--) {
        int t;cin >> t;

        int l = 0, r = n - 1;
        while (l < r) {
            int mid = (l + r + 1) / 2;
            if (v[mid].score > t)
                l = mid;
            else
                r = mid - 1;
        }

        int ans = l;
        r = l, l = 0;
        while (l < r) {
            int mid = (l + r) / 2;
            if (v[mid].score <= v[ans].score)
                r = mid;
            else
                l = mid + 1;
        }

        if (v[ans].score > t) {
            cout << v[r].id << endl;
        }
        else {
            cout << 0 << endl;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    //cin >> t;
    while (t--) _();

    cout.flush();

    system("pause");
}