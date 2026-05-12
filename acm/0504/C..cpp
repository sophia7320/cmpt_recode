#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

using vint = vector<int>;

vint cds[1000010];
int fr[1000010];

int flg[1000010];
int cnt[1000010];

int n, m, q;

void del(int node) {
    flg[node] = 0;
    int cur = cnt[node];
    if (cur -= 1)
        for (auto nx : cds[node]) cur |= cnt[nx];

    if (cur) return;
    else
        while (true) {
            if (cnt[node] == max(flg[node], cnt[node] - 1)) return;
            cnt[node] -= 1;
            if (node == 1) return;
            node = fr[node];
        }
}

void add(int node) {
    flg[node] = 1;

    if (!cnt[node]) {
        while (true) {
            ++cnt[node];
            if (node == 1) return;
            node = fr[node];
        }
    }
}

void b(int cur) {
    int res = 0;
    for (auto nx : cds[cur]) b(nx), res += cnt[nx];
    cnt[cur] = max(flg[cur], res);
}

void _() {
    cin >> n >> m >> q;

    for (int i = 2;i <= n; i++) {
        cin >> fr[i];
        cds[fr[i]].push_back(i);
    }

    while (m--) { int t; cin >> t;  flg[t] = 1; }

    b(1);
    cout << cnt[1] << endl;

    while (q--) {
        int opt, node;
        cin >> opt >> node;
        if (opt & 1)
            add(node);
        else
            del(node);

        cout << cnt[1] << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr), cout.tie(nullptr);

    int t = 1; // cin>>t ;
    while (t--) _();

    cout.flush();
}