#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;
using ll = long long;
using vll = vector<ll>;

void _() {
    int a, b;cin >> a >> b;
    cout << b - a << endl;
    if (b - a > 250) cout << "jiu ting tu ran de..." << endl;
    else if (b - a < 0)  cout << "hai sheng ma?" << endl;
    else cout << "nin tai cong ming le!" << endl;
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