#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <stdint.h>

using namespace std;
using ll = long long;
using vll = vector<ll>;
using vint = vector<int>;

ll qpow(ll a, ll b) {
	ll res = 1;
	while (b) {
		if (b & 1)
			res *= a;
		a *= a, b >>= 1;
	}
	return res;
}

ll f(ll x) {
	ll res = 0;
	while (!(x & 1)) res++, x >>= 1;
	return res;
}

void _() {
	ll a, p;
	cin >> a >> p;

	if (a & 1) {
		cout << 1 << endl;
		return;
	}

	ll k = f(a);
	k = (p + k - 1) / k;
	ll t = (p + a - 1) / a;

	ll cnt = qpow(2, p - t) - (k - 1) / qpow(2, t);

	if ((qpow(a, a) % qpow(2, p)) != 0)
		cnt += 1;

	cout << cnt << endl;

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);

	int t = 1;
	cin >> t;
	while (t--) _();
	cout.flush();
}