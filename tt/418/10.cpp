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

node v[11000];
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
        node nn{ 0,t };
        auto ans = upper_bound(v, v + n, nn, [](const node& a, const node& b) {
            if (a.score == b.score) return a.id < b.id;
            else return a.score > b.score;
        });
        if (ans != v + n)
            cout << ans->id << endl;
        else
            cout << 0 << endl;
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