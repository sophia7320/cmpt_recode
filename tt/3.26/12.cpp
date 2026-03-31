#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using ll = long long;

const ll N = 1e6 + 10;

struct node {
    int x, y;
} g[1100];

int m[500][500];

vector<node> ns;
int gra[110][110];
int p[110][110];
int f[111];

int n, d;

void solve() {
    cin >> n >> d;
    g[0] = { 0, 0 };
    for (int i = 1; i <= n; i++) {
        cin >> g[i].x >> g[i].y;
    }

    for (int i = 0; i <= n + 10; i++)
        for (int j = 0; j <= n + 10; j++)
            if (i != j)
                gra[i][j] = INT32_MAX;

    for (int i = 1; i <= n; i++) {
        int x1 = ns[i].x, y1 = ns[i].y;
        for (int j = 1; j <= n; j++) {
            int x2 = ns[j].x, y2 = ns[j].y;
            int dd = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
            if (dd <= d * d)
                gra[i][j] = 1, p[i][j] = j, p[j][i] = i;
        }
    }

    for (int i = 1;i <= n;i++) {
        int x1 = ns[i].x, y1 = ns[i].y;
        int di = min(50 - abs(x1), 50 - abs(y1));
        if (di <= d) {
            gra[i][n + 1] = 1;
            p[i][n + 1] = n + 1;
        }
    }

    for (int i = 1;i <= n;i++) {
        int x1 = ns[i].x, y1 = ns[i].y;

        if (x1 * x1 + y1 * y1 <= (15 + d) * (15 + d)) {
            gra[0][i] = 1;
            p[0][i] = i;
        }
    }

    for (int k = 1;k <= n + 1;k++)
        for (int i = 1;i <= n + 1;i++)
            for (int j = 1; j <= n + 1;j++) {
                int t = gra[i][k] + gra[k][j];
                if (gra[i][j] > t ||
                    (i == 0 && t == gra[i][j] && f[p[0][k]] < f[p[0][j]])
                    ) {
                    gra[i][j] = gra[i][k] + gra[k][j];
                    p[i][j] = k;
                }
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