#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <stdint.h>
#include <climits>

using namespace std;
using ll = long long;
using vll = vector<ll>;
using vint = vector<int>;

#define int long long
#define endl "\n"


template<typename T>
void print(T& v) {
    for (auto& it : v)
        cout << it << " ";
    cout << endl;
}

struct edge {
    int dst, a, b;
};

vector<vector<edge>> v;

int dp[61000][310]{};
int f[61000][310]{};

void _() {

    int n, m;cin >> n >> m;
    v.assign(n + 1, vector<edge>());
    for (int i = 0;i <= (n - 1) * 200;i++)
        for (int j = 0;j < 310;j++)
            f[i][j] = 0;

    while (m--) {
        int src, dst, a, b;
        cin >> src >> dst >> a >> b;
        v[src].push_back({ dst,a,b });
    }

    f[0][1] = 1, dp[0][1] = 0;

    for (int i = 0;i <= (n - 1) * 200;i++)
        for (int u = 1;u <= n;u++)
            if (f[i][u])
                for (auto [vt, a, b] : v[u]) {
                    int suma = i + a;
                    int& ref = dp[suma][vt];

                    if (suma > 60000) continue;

                    if (!f[suma][vt]) ref = dp[i][u] + b, f[suma][vt] = 1;
                    else ref = min(ref, dp[i][u] + b);
                }

    int temp = LLONG_MAX;

    int ansa = 0, ansb = 0;

    for (int i = 1;i <= (n - 1) * 200;i++)
        if (f[i][n])
            if (dp[i][n] * i < temp)
                ansa = i, ansb = dp[i][n], temp = dp[i][n] * i;

    cout << ansa << " " << ansb << endl;

}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr), cout.tie(nullptr);

    int t = 1;
    cin >> t;
    while (t--) _();

    cout.flush();
}