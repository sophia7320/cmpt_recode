#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;
using ll = long long;

class SegTree {
    using map = vector<char>;

private:
    struct Node {
        map m;
        bool lazy;

        Node() {
            m.reserve(26);
            for (char c = 'a'; c <= 'z'; c++) {
                m.push_back(c);
            }
            lazy = false;
        }

        Node(char src, char dst) {
            m.reserve(26);
            for (char c = 'a'; c <= 'z'; c++) {
                m.push_back(c);
            }
            lazy = false;
            m[src - 'a'] = dst;
        }

        auto& operator[](const char src) const { return m[src - 'a']; }

        auto& operator[](const char src) { return m[src - 'a']; }

        Node& operator=(Node&& other) {
            this->m = move(other.m);
            this->lazy = other.lazy;

            return *this;
        }
    };

    ll n;
    vector<Node> tree;

    void merge(Node& pre, const Node& late) {
        for (char c = 'a'; c <= 'z'; c++) {
            pre[c] = late[pre[c]];
        }
        pre.lazy = true;
    }

    void pushdown(ll cur_node) {
        auto& t = tree[cur_node];
        if (!t.lazy) {
            return;
        }

        merge(tree[cur_node * 2], t);
        merge(tree[cur_node * 2 + 1], t);

        t = Node();
    }

    void __update(ll cur_node, ll l, ll r, ll L, ll R, char src, char dst) {
        if (L <= l && r <= R) {
            Node t(src, dst);
            merge(tree[cur_node], t);
            tree[cur_node].lazy = true;
            return;
        }

        pushdown(cur_node);

        ll mid = (l + r) / 2;

        if (L <= mid)
            __update(cur_node * 2, l, mid, L, R, src, dst);
        if (mid + 1 <= R)
            __update(cur_node * 2 + 1, mid + 1, r, L, R, src, dst);
    }

    void __output(string& s, ll node, ll l, ll r) {
        if (l == r) {
            const auto& cg = tree[node];
            s[l] = cg[s[l]];
            return;
        }

        pushdown(node);

        ll mid = (l + r) / 2;

        __output(s, node * 2, l, mid);
        __output(s, node * 2 + 1, mid + 1, r);
    }

public:
    SegTree() = default;

    SegTree(size_t size) : tree(4 * size), n(size) {};

    SegTree& operator=(SegTree&& other) {
        this->tree = move(other.tree);
        this->n = other.n;
        return *this;
    }

    void update(ll l, ll r, char src, char dst) {
        __update(1, 0, n - 1, l, r, src, dst);
    }

    void outputAns(string& s) { __output(s, 1, 0, n - 1); }
};

SegTree segTree;

string s;

void init() {
    cin >> s;
    segTree = SegTree(s.length());
}

void _() {
    ll l, r;
    char x, y;
    cin >> l >> r >> x >> y;
    l--, r--;

    segTree.update(l, r, x, y);
}

void output() {
    segTree.outputAns(s);
    cout << s << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    init();

    int t;
    cin >> t;
    while (t--)
        _();

    output();
}