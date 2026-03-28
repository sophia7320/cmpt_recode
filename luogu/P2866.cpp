#include <bitset>
#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

struct cow {
    ll idx, lookCnt, high, n;
};

using cowLis = vector<cow>;

void solve() {
    int n;
    cin >> n;

    cowLis cows(n);
    for (int i = 0; i < n; i++) {
        auto& c = cows[i];
        c.idx = i;
        cin >> c.high;
    }

    cowLis c_stk;
    c_stk.reserve(n);

    c_stk.emplace_back(cows[0]);

    for (int i = 1; i < n; i++) {
        auto& cur = cows[i];
        auto top = c_stk.back();

        ll temp = 0;

        if (cur.high < top.high) {
            c_stk.emplace_back(cur);
            continue;
        }

        while (cur.high >= top.high && !c_stk.empty()) {
            cows[top.idx].lookCnt = temp++;
            temp += top.n;
            if (cur.high == top.high)
                temp = 0;
            c_stk.pop_back();
            if (!c_stk.empty())
                top = c_stk.back();
        }

        cur.n = temp;
        c_stk.emplace_back(cur);
    }

    ll temp = 0;
    cow top;
    while (!c_stk.empty()) {
        top = c_stk.back();
        cows[top.idx].lookCnt = temp++;
        temp += top.n;
        c_stk.pop_back();
    }

    ll ans = 0;
    for (auto& i : cows)
        ans += i.lookCnt;

    cout << ans << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--)
        solve();
}