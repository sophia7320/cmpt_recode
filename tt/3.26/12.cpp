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

int n, d;

void solve() {
    cin >> n >> d;
    g[0] = {0, 0};
    for (int i = 1; i <= n; i++) {
        cin >> g[i].x >> g[i].y;
    }

    for (int i = 0; i <= n + 10; i++)
        for (int j = 0; j <= n + 10; j++)
            if (i != j)
                gra[i][j] = INT32_MAX;

    for (int i = 1; i <= n; i++) {
        int x1 = ns[i].x, y1 = ns[i].y;
        for (int j = 1; i <= n;)
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