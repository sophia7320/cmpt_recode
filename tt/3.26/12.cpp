#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

#undef INT32_MAX
#define INT32_MAX 99999

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

void output(int i, int j) {
    if (p[i][j] == j)
        return;

    auto& t = ns[p[i][j]];
    cout << t.x << " " << t.y << endl;

    output(p[i][j], j);
}

void solve() {
    cin >> n >> d;
    ns.resize(n + 3);
    for (int i = 1; i <= n; i++) {
        cin >> ns[i].x >> ns[i].y;
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
                gra[i][j] = 1, p[i][j] = j;
        }
    }

    for (int i = 1; i <= n; i++) {
        int x1 = ns[i].x, y1 = ns[i].y;
        int di = min(50 - abs(x1), 50 - abs(y1));
        if (di <= d) {
            gra[i][n + 1] = 1;
            p[i][n + 1] = n + 1;
        }
    }

    for (int i = 1; i <= n; i++) {
        int x1 = ns[i].x, y1 = ns[i].y;
        f[i] = INT32_MAX;

        if (x1 * x1 + y1 * y1 <= (7.5 + d) * (7.5 + d)) {
            gra[0][i] = 1;
            p[0][i] = i;
            f[i] = x1 * x1 + y1 * y1;
        }
    }

    for (int k = 1; k <= n; k++)
        for (int i = 0; i <= n; i++)
            for (int j = 1; j <= n + 1; j++) {
                ll t = gra[i][k] + gra[k][j];
                if (gra[i][j] > t ||
                    (i == 0 && t == gra[i][j] && f[p[0][k]] < f[p[0][j]])) {
                    gra[i][j] = gra[i][k] + gra[k][j];
                    p[i][j] = p[i][k];
                }
            }

    if (gra[0][n + 1] == INT32_MAX)
        cout << 0 << endl;
    else {
        cout << gra[0][n + 1] << endl;
        output(0, n + 1);
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