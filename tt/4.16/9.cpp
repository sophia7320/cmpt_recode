#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_set>

//#define endl "\n"

using namespace std;
using ll = long long;
using vll = vector<long long>;
using pq = priority_queue<ll>;

double lv[1100][1100];
bool sex[2200];
unordered_set<int> ph[1100];

int real(int n) {
    return sex[n] ? -n : n;
}

void _() {
    int n, m; cin >> n >> m;

    for (int i = 0;i < m;i++) {
        int k;cin >> k;
        while (k--) {
            string x; cin >> x;
            if (x[0] == '-') sex[-stoi(x)] = true;
            ph[i].insert(abs(stoi(x)));
        }
    }

    int xx, yy;cin >> xx >> yy;
    double res[2][1100]{};

    for (int p = 0;p < n;p++)
        if (ph[p].find(abs(xx)) != ph[p].end())
            for (auto i : ph[p])
                if (sex[abs(xx) ^ sex[i]]) res[0][i] += 1.0 / ph[p].size();

    for (int p = 0;p < n;p++)
        if (ph[p].find(abs(yy)) != ph[p].end())
            for (auto i : ph[p])
                if (sex[abs(xx) ^ sex[i]]) res[1][i] += 1.0 / ph[p].size();

    auto maxxx = max_element(res[0], res[0] + n);
    auto maxyy = max_element(res[1], res[1] + n);

    if (maxxx - res[0] == abs(yy)) {
        cout << xx << " " << yy << endl;
    }
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
