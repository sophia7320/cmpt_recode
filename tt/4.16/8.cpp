#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <stdint.h>

#define endl "\n"

using namespace std;
using ll = long long;

using vll = vector<ll>;

struct country {
    ll id;
    ll gold, round, pp;
    ll pg, pr, pag, par;
};

country v[1100];


void _() {
    int n, m;cin >> n >> m;
    for (int i = 0;i < n;i++) {
        v[i].id = i;
        cin >> v[i].gold >> v[i].round >> v[i].pp;
    }

    sort(v, v + n, [](country& a, country& b) {
        return a.gold > b.gold;
        });
    int k = 1, t = v[0].gold;
    for (int i = 0;i < n;i++) {
        if (v[i].gold == t)
            v[i].pg = k;
        else
            t = v[i].gold, v[i].pg = k = i + 1;
    }

    sort(v, v + n, [](country& a, country& b) {
        return a.round > b.round;
        });
    k = 1, t = v[0].round;
    for (int i = 0;i < n;i++) {
        if (v[i].round == t)
            v[i].pr = k;
        else
            t = v[i].round, v[i].pr = k = i + 1;
    }

    sort(v, v + n, [](country& a, country& b) {
        return a.gold / 1.0 / a.pp > b.gold / 1.0 / b.pp;
        });
    k = 1;double h = v[0].gold / 1.0 / v[0].pp;
    for (int i = 0;i < n;i++) {
        if (v[i].gold / 1.0 / v[i].pp == h)
            v[i].pag = k;
        else
            h = v[i].gold / 1.0 / v[i].pp, v[i].pag = k = i + 1;
    }

    sort(v, v + n, [](country& a, country& b) {
        return a.round / 1.0 / a.pp > b.round / 1.0 / b.pp;
        });
    k = 1; h = v[0].round / 1.0 / v[0].pp;
    for (int i = 0;i < n;i++) {
        if (v[i].round / 1.0 / v[i].pp == h)
            v[i].par = k;
        else
            h = v[i].round / 1.0 / v[i].pp, v[i].par = k = i + 1;
    }

    sort(v, v + n, [](country& a, country& b) {
        return a.id < b.id;
        });

    while (m--) {
        int q;cin >> q;
        ll ans, t;
        ans = v[q].pg, t = 1;
        if (v[q].pr < ans) ans = v[q].pr, t = 2;
        if (v[q].pag < ans) ans = v[q].pag, t = 3;
        if (v[q].par < ans) ans = v[q].par, t = 4;

        cout << ans << ":" << t;
        if (m != 0) cout << " ";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    //cin>>t;
    while (t--) _();

    cout.flush();

    system("pause");
}