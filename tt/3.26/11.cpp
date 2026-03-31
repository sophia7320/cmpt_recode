#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using ll = long long;

int d, p;
int date = 0;

struct person {
    string name, num;
    int status, tm, id;

    bool operator<(const person& other) const {
        if (tm == other.tm) {
            return id > other.id;
        }
        return tm > other.tm;
    }

    bool operator==(const person& other) const { return name == other.name; }

    void output() { cout << name << " " << num << "\n"; }

    bool check() {
        if (num.size() != 18) {
            return false;
        }
        for (auto c : num) {
            if (!('0' <= c && c <= '9'))
                return false;
        }

        return true;
    }
};

const ll N = 1e6 + 10;

vector<person> rd;

inline vector<person> duplicate(vector<person>& v) {
    vector<person> res;
    unordered_set<string> t;
    for (auto& i : v) {
        if (t.find(i.num) == t.end()) {
            t.insert(i.num);
            res.push_back(move(i));
        }
    }

    return res;
}

unordered_map<string, int> rcd;
void solve() {
    date++;
    priority_queue<person> pq;
    int t, s;
    cin >> t >> s;

    for (int i = 0; i < t; i++) {
        person t;
        char c;
        int hh, mm;
        cin >> t.name >> t.num >> t.status >> hh >> c >> mm;
        t.id = i;
        t.tm = hh * 60 + mm;
        if (t.check()) {
            if (t.status) {
                rd.push_back(t);
            }
            pq.push(t);
        }
    }

    while (s && !pq.empty()) {
        auto top = pq.top();
        pq.pop();

        if (rcd.find(top.num) == rcd.end()) {
            top.output();
            rcd[top.num] = date;
            s--;
        } else {
            if (rcd[top.num] + p < date) {
                top.output();
                rcd[top.num] = date;
                s--;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> d >> p;
    while (d--) {
        solve();
    }

    rd = duplicate(rd);
    for (auto& i : rd)
        i.output();

    cout.flush();

    system("pause");

    return 0;
}