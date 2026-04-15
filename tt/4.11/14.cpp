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
ll pre_col[1100][1100];
ll z[1100][1100];
ll zs[1100][1100];

ll n;

void _() {
    cin >> n;
    for (int y = 1;y <= n;y++) for (int x = 1;x <= n;x++) {
        cin >> chess[y][x];
        if (!chess[y][x]) z[y][x] = 1;
    }

    ll ansv[4];
    ll final_ans = 0;
    while (true) {
        for (int i = 1;i <= n;i++) for (int j = 1;j <= n;j++) {
            pre_col[i][j] = chess[i][j] + pre_col[i - 1][j];
            zs[i][j] = zs[i - 1][j] + z[i][j];
        }

        ll ans = 0;

        for (int up = 1;up <= n;up++) {
            for (int dn = up; dn <= n; dn++) {
                ll i = 1, j = 0; ll t = 0;
                ll zr = 0;

                while (++j <= n) {
                    ll col_sum = pre_col[dn][j] - pre_col[up - 1][j];
                    zr += zs[dn][j] - zs[up - 1][j];
                    if (t < 0 || zr) {
                        t = col_sum, i = j;
                        zr = zs[dn][j] - zs[up - 1][j];
                    }
                    else {
                        t += col_sum;
                    }

                    if (t > ans && !zr) {
                        ansv[0] = i, ansv[1] = up, ansv[2] = j, ansv[3] = dn;
                        ans = t;
                    }
                }
            }
        }

        //cout<<ans<<endl;

        if (ans < 0) break;

        cout << "(" << ansv[0] << ", " << ansv[1] << ")" << " ";
        cout << "(" << ansv[2] << ", " << ansv[3] << ")" << " ";
        cout << ans << endl;

        final_ans += ans;

        // for (int i = ansv[1]; i <= ansv[3]; i++)
        //     for (int j = ansv[0]; j <= ansv[2]; j++) {
        //         chess[i][j] = 0, z[i][j] = 1;
        //     }

        // down
        int py = ansv[3];int high = ansv[3] - ansv[1] + 1;
        while (py > 0) {
            for (int i = ansv[0];i <= ansv[2];i++) {
                if (py - high > 0)
                    chess[py][i] = chess[py - high][i], z[py][i] = z[py - high][i];
                else
                    chess[py][i] = 0, z[py][i] = 1;
            }
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
