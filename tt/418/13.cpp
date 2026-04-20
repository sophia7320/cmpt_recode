#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>

using namespace std;
using ll = long long;
using vll = vector<ll>;

struct person {
    int jz, yy;
    string id;int age;

    bool operator<(const person& other) const {
        if (age == other.age)
            return yy > other.yy;
        else
            return age < other.age;
    }
};

ostream& operator<<(ostream& out, person& per) {
    return out << per.id;
}

person ayy[999999];
priority_queue<person> wait;

bool vis[999999];
bool arr[999999];

void _() {
    int n;cin >> n;
    vector<person> tmp;
    for (int i = 0;i < n;i++) {
        int jz, yy;string id;int age;
        cin >> jz >> yy >> id >> age;
        tmp.push_back({ jz,yy,id,age >= 80 ? 999999 : 0 });
        ayy[yy] = tmp.back();
    }

    sort(tmp.begin(), tmp.end(), [](person& a, person& b) {
        return a.jz > b.jz;
    });

    int tm = 1;
    do {
        while (!tmp.empty() && tmp.back().jz <= tm)
            wait.push(tmp.back()), arr[stoi(tmp.back().id)] = true, tmp.pop_back();

        if (tm <= n && arr[stoi(ayy[tm].id)] && !vis[stoi(ayy[tm].id)]) {
            auto t = ayy[tm];
            cout << tm << " " << t << endl;
            vis[stoi(t.id)] = true;
        }
        else while (!wait.empty()) {
            auto t = wait.top();wait.pop();
            if (vis[stoi(t.id)]) continue;

            cout << tm << " " << t << endl;
            vis[stoi(t.id)] = true;
            break;
        }

        tm++;
    } while (!wait.empty() || !tmp.empty());

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