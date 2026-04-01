#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;
using ll = long long;

int n, m;
char ii[1111][1111];

struct rd {
    int lenth, high;
};


void init() {
    cin >> n >> m;
    for (int i = 1;i <= n;i++) {
        for (int j = 1;j <= m;j++) {
            cin >> ii[i][j];
        }
    }
}

int h[1111][1111];
void solve() {
    int ans = 0;
    for (int i = 1;i <= n;i++) {
        for (int j = 1;j <= m;j++) {
            if (ii[i][j] == '.') {
                h[i][j] = h[i - 1][j] + 1;
            }
            else {
                h[i][j] = 0;
            }
        }
    }

    vector<rd> stk;
    vector<int> s(m + 1);
    for (int i = 1;i <= n;i++) {
        stk.clear();
        s.clear();
        s.resize(m + 1);

        for (int j = 1;j <= m;j++) {
            if (stk.empty()) {
                stk.push_back({ 1,h[i][j] });
                ans += h[i][j];
            }
            else {
                if (!stk.empty()) {
                    auto t = stk.back();
                    if (t.high > h[i][j]) {
                        ans += s[j] = h[i][j] + s[j - 1];
                    }
                    else {
                        int cnt = 1;
                        do {
                            auto t = stk.back();
                            if (t.high > h[i][j])
                                break;
                            cnt += t.lenth;
                            stk.pop_back();
                        } while (!stk.empty());
                        ans += s[j] = h[i][j] * cnt + s[j - cnt + 1];
                        stk.push_back({ cnt,h[i][j] });
                    }
                }
            }
        }
    }

    cout << ans << endl;


}

void output() {

}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    init();

    int t = 1;
    //cin >> t;
    while (t--)
        solve();

    output();

    system("pause");
}