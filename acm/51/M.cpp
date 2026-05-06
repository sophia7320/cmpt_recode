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

ll daytoi(string& day) {
	if (day == "Monday") return 0;
	if (day == "Tuesday") return 1;
	if (day == "Wednesday") return 2;
	if (day == "Thursday") return 3;
	if (day == "Friday") return 4;
}

string dtos(ll day) {
	if (day == 0) return "Monday";
	if (day == 1) return "Tuesday";
	if (day == 2) return "Wednesday";
	if (day == 3) return "Thursday";
	if (day == 4) return "Friday";
}

void _() {
	ll yy1, mm1, dd1, d1;string sday1;
	cin >> yy1 >> mm1 >> dd1 >> sday1;
	d1 = daytoi(sday1);

	ll yy2, mm2, dd2, d2;string sday2;
	cin >> yy2 >> mm2 >> dd2;
	d2 = daytoi(sday2);

	ll sum1 = yy1 * 12 * 30 + mm1 * 30 + dd1;
	ll sum2 = yy2 * 12 * 30 + mm2 * 30 + dd2;

	ll dis = sum2 - sum1;

	ll ans = ((d1 + dis) % 5 + 5) % 5;

	cout << dtos(ans) << endl;
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