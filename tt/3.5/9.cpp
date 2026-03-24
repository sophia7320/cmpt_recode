#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
using ll = long long;

ll n, k;

struct person {
    string id;
    ll score;
    ll in_arr;
    ll arrid;
};

vector<vector<person>> arr;
vector<person> total;

void solve() {
    ll n;
    cin >> n;
    arr.reserve(n);
    for (int j = 1;j <= n;j++) {
        cin >> k;

        vector<person> v(k);
        for (auto& i : v) {
            string id;
            ll score;
            cin >> id >> score;
            i.id = id, i.score = score, i.arrid = j;
        }

        sort(v.begin(), v.end(), [](auto& a, auto& b) {
            return a.score > b.score;
            });

        for (int i = 0;i < k;i++) {
            if (i > 0 && v[i].score == v[i - 1].score)
                v[i].in_arr = v[i - 1].in_arr;
            else
                v[i].in_arr = i + 1;
        }

        for (int i = 0;i < k;i++) {
            total.emplace_back(move(v[i]));
        }
    }

    sort(total.begin(), total.end(), [](auto& a, auto& b) {
        if (a.score == b.score) a.id < b.id;
        return a.score > b.score;
        });

    int t = 1;
    cout << total.size() << endl;
    for (int i = 0;i < total.size();i++) {
        auto& tp = total[i];
        if (i > 0 && total[i].score == total[i - 1].score)
            cout << tp.id << " " << t << " " << tp.arrid << " " << tp.in_arr << "\n";
        else {
            t = i + 1;
            cout << tp.id << " " << t << " " << tp.arrid << " " << tp.in_arr << "\n";
        }
    }

}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    //cin >> t;
    while (t--) {
        solve();
    }
    cout.flush();

    system("pause");

    return 0;
}