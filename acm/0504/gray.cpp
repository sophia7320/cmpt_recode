#include <iostream>
using namespace std;
using ll = long long;

const ll MOD = 998244353;

ll mpow(ll a, ll b, ll m) {
    ll r = 1;
    a %= m;
    while (b) {
        if (b & 1) r = r * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return r;
}

int main() {
    ll n;
    cin >> n;
    ll e = mpow(2, n - 1, MOD - 1);
    e = (e - n) % (MOD - 1);
    if (e < 0) e += MOD - 1;
    cout << mpow(2, e, MOD) << "\n";
}
