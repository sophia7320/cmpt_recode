#include <stdint.h>

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;
using ll = long long;
using vll = vector<ll>;

struct node {
    ll id, dis, val, cnt;

    bool operator<(const node& other) const {
        if (dis == other.dis)
            return val < other.val;
        else
            return dis > other.dis;
    }
};

using pq = priority_queue<node>;

ll n, m, s, d;

struct eg {
    ll dst, w;
};

vector<eg> link[1100];

ll ans[1010][3];
ll v[1100];

bool vis[1100];

ll f[1100];

void out(ll id) {
    if (id == s) {
        cout << id << " ";
        return;
    }
    out(f[id]);
    cout << id;
    if (id != d)
        cout << " ";
}

void _() {
    cin >> n >> m >> s >> d;

    for (int i = 0; i < n; i++)
        f[i] = i;

    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;

        link[u].push_back({ v, w });
        link[v].push_back({ u, w });
    }

    for (int i = 0; i < n; i++) {
        ans[i][0] = INT32_MAX;
    }
    pq q;

    node first;
    first.id = s;
    ans[s][1] = first.val = v[s];
    ans[s][0] = first.dis = 0;
    ans[s][2] = first.cnt = 0;

    q.push(first);

    while (!q.empty()) {
        auto t = q.top();
        q.pop();

        if (t.dis > ans[t.id][0] || t.val < ans[t.id][1])
            continue;

        vis[t.id] = true;

        ll cur_id = t.id, cur_dis = t.dis, cur_val = t.val;

        for (auto& eg : link[cur_id]) {
            ll elen = eg.w, nxval = v[eg.dst], dstid = eg.dst;

            if (vis[dstid])
                continue;

            if (cur_dis + elen < ans[dstid][0]) {
                node nn;
                nn.id = dstid;
                nn.dis = ans[dstid][0] = cur_dis + elen;
                nn.val = ans[dstid][1] = cur_val + v[dstid];
                nn.cnt = ans[dstid][2] = t.cnt + 1;
                f[dstid] = cur_id;
                q.push(nn);
            }
            else if (cur_dis + elen == ans[dstid][0] &&
                     cur_val + v[dstid] > ans[dstid][1]) {
                node nn;
                nn.id = dstid;
                nn.dis = ans[dstid][0] = cur_dis + elen;
                nn.val = ans[dstid][1] = cur_val + v[dstid];
                nn.cnt = ans[dstid][2] = t.cnt + 1;
                f[dstid] = cur_id;
                q.push(nn);
            }
        }
    }

    cout << ans[d][2] << " " << ans[d][1] << endl;
    out(d);
    // cout << s << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cout.tie(nullptr);
    cin.tie(nullptr);

    int t = 1;
    // cin>>t;
    while (t--)
        _();
    cout.flush();

    system("pause");
}