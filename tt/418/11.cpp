#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;
using ll = long long;
using vll = vector<ll>;

struct eg {
    int dst, w;
};

vector<eg> vp[11001];

void _() {
    int n;cin >> n;
    for (int i = 1;i <= n - 1;i++) {
        int fr, w;cin >> fr >> w;
        vp[fr].push_back({ i,w });
    }

    queue<eg> q;
    q.push({ 0, 101 });
    vector<eg> trea;
    while (!q.empty()) {
        auto t = q.front();q.pop();
        if (!vp[t.dst].empty()) {
            for (auto& e : vp[t.dst])
                q.push({ e.dst,min(t.w,e.w) });
        }
        else {
            trea.push_back({ t.dst,t.w });
        }
    }

    sort(trea.begin(), trea.end(), [](eg& a, eg& b) {
        if (a.w == b.w) return a.dst < b.dst;
        else return a.w > b.w;
    });

    cout << trea[0].w << endl;

    for (int i = 0;i < trea.size();i++) {
        if (i == 0) cout << trea[i].dst;
        else if (trea[i].w == trea[i - 1].w) cout << " " << trea[i].dst;
        else break;
    }

}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    //cin >> t;
    while (t--) _();

    cout.flush();

    system("pause");
}