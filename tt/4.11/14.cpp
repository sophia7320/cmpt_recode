#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

//#define endl "\n"

using namespace std;
using ll = long long;
using vll = vector<long long>;
using pq = priority_queue<ll>;

ll chess[1100][1100];
ll pre_row[1100][1100];
ll z[1100][1100];
ll zs[1100][1100];

ll n;

bool check(ll a[], ll b[]) {
    const int n = 4;
    for (int i = 0;i < n - 1;i++)
        if (a[i] == b[i])
            if (a[i + 1] != b[i + 1])
                return a[i + 1] < b[i + 1];

    return false;
}

void _() {
    cin >> n;
    for (int y = 1;y <= n;y++) for (int x = 1;x <= n;x++) {
        cin >> chess[y][x];
        if (!chess[y][x]) z[y][x] = 1;
    }

    ll ansv[4];
    ll final_ans = 0;
    while (true) {

        for (int i = 1;i <= n;i++)
            for (int j = 1;j <= n;j++) {
                pre_row[i][j] = pre_row[i][j - 1] + chess[i][j];
                zs[i][j] = zs[i][j - 1] + z[i][j];
            }

        ll ans = 0;

        for (int lf = 1;lf <= n;lf++) {
            for (int rh = lf; rh <= n; rh++) {
                ll i = 1, j = 0; ll t = 0;
                ll zr = 0;

                while (++j <= n) {
                    ll col_sum = pre_row[j][rh] - pre_row[j][lf - 1];
                    zr += zs[j][rh] - zs[j][lf - 1];

                    if (t < 0 || zr)
                        t = col_sum, i = j, zr = zs[j][rh] - zs[j][lf - 1];
                    else
                        t += col_sum;

                    ll u[] = { lf,i,rh,j };
                    if ((t > ans || (t == ans && check(u, ansv))) && !zr)
                        memcpy(ansv, u, sizeof(u)), ans = t;

                }
            }
        }

        //cout<<ans<<endl;
        if (ans <= 0) break;

        cout << "(" << ansv[0] << ", " << ansv[1] << ")" << " ";
        cout << "(" << ansv[2] << ", " << ansv[3] << ")" << " ";
        cout << ans << endl;

        final_ans += ans;

        // drop
        int py = ansv[3];int high = ansv[3] - ansv[1] + 1;
        while (py > 0) {
            for (int i = ansv[0];i <= ansv[2];i++)
                if (py - high > 0)
                    chess[py][i] = chess[py - high][i], z[py][i] = z[py - high][i];
                else
                    chess[py][i] = 0, z[py][i] = 1;

            py--;
        }
    }

    cout << final_ans << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    // cin >> t ;
    while (t--) _();

    cout.flush();
    system("pause");
}
