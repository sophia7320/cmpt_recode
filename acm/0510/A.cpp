#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;
using ll = long long;
using vll = vector<ll>;
using vint = vector<int>;

#define endl "\n"


template<typename T>
void print(T& v) {
    for (auto& it : v)
        cout << it << " ";
    cout << endl;
}

struct node {
    int id, sa, sb;

    bool operator<(const node& other) const {
        if (sa + sb != other.sa + other.sb)
            return sa + sb > other.sa + other.sb;
        else
            return sa > other.sa;
    }
};

struct edge {
    int dst, a, b;
};

vector<vector<edge>> v;
vint f;

void _() {
    v.clear();f.clear();
    int n, m;cin >> n >> m;
    v.resize(n + 1);

    while (m--) {
        int src, dst, a, b;
        cin >> src >> dst >> a >> b;
        v[src].push_back({ dst , a ,b });
    }

    //Djstrastla

    node res[n + 1]; for (int i = 1;i <= n;i++) res[i] = node{ i,99999999,99999999 };
    int vis[n + 1]{};
    priority_queue<node> q;

    q.push({ 1,0,0 });
    res[1] = { 1,0,0 };
    while (!q.empty()) {
        auto t = q.top();q.pop();

        if (t < res[t.id]) continue;
        vis[t.id] = true;

        for (auto e : v[t.id]) {
            if (vis[e.dst]) continue;

            node nn{ e.dst,t.sa + e.a,t.sb + e.b };

            if (res[e.dst] < nn) {
                res[e.dst].sa = nn.sa;
                res[e.dst].sb = nn.sb;
                q.push(nn);
            }
        }
    }

    node ans = res[n];

    cout << ans.sa << " " << ans.sb << endl;


}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr), cout.tie(nullptr);

    int t = 1;
    cin >> t;
    while (t--) _();

    cout.flush();
}