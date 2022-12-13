#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>


struct Node {
    int ost;
    int len;
    int tst;
    Node **children;
    int lastIndex;

    Node(int ost, int len, int tst) {
        this->ost = ost;
        this->len = len;
        this->tst = tst;
        lastIndex = 0;

        int n = 26;
        children = new Node *[n];
    }


    void add(Node *node) {
        children[lastIndex++] = node;
    }

    void add(Node *node, int ind) {
        children[ind] = node;
    }
};

class SuffixTree {
    const char *s;
    Node *root;
    int sz;
public:

    SuffixTree(const char *string) {
        int n = 26;
        this->s = string;
        sz = strlen(string);
        root = new Node(0, sz, 0);
        build();
        root->add(new Node(sz - 1, 1, sz - 1));
    }

    void build() {
        for (int i = 0; i < sz; i++)
            build(root, i);
    }


    void build(Node *par, int ind) {
        if (ind == sz - 1) return;

        int len = -1;
        int ls = 0;
        Node *opar;
        for (int i = 0; i < par->lastIndex; i++) {
            if (s[ind] == s[par->children[i]->ost]) {
                len = 0;
                ls = i;
                opar = par->children[i];
                while (s[ind + len] == s[par->children[i]->ost + len] and i + len < sz)
                    len++;
            }
        }
        if (len == -1) {
            Node *node = new Node(ind, sz - ind, ind);
            par->add(node);
            return;
        }
        Node *npar, *child, *lead;
        npar = new Node(opar->ost, len, -1);
        child = new Node(ind + len, sz - (ind + len), ind);
        if (opar->lastIndex != 0) {
            lead = new Node(opar->ost + len, opar->len - len, -1);
            lead->children = opar->children;
            lead->lastIndex = opar->lastIndex;
            build(npar, ind + len);
        } else
            lead = new Node(opar->ost + len, opar->len - len, opar->tst);
        npar->add(child);
        npar->add(lead);
        par->add(npar, ls);
        delete opar;

    }


    void print(Node *root) {
        for (int j = root->ost; j < root->len + root->ost; j++) {
            std::cout << s[j];
        }
        std::cout << '\n';
        auto &children = root->children;
        for (int i = 0; i < root->lastIndex; i++) {
            print(children[i]);
        }
    }

    Node *Search(Node *par, const char *cstring, int ind) {

        int len = -1;
        if (cstring[ind] == s[par->ost]) {
            len = 0;
            while (cstring[ind + len] == s[par->ost + len] and
                   ind + len < strlen(cstring) and
                   par->ost + len <= par->ost + par->len)
                len++;
        }
        if (len == -1 || len + ind >= sz)return nullptr;
        else if (ind + len == strlen(cstring))return par;
        else {
            for (int i = 0; i < par->lastIndex; i++) {
                if (s[par->children[i]->ost] == cstring[ind + len - 1])
                    return Search(par->children[i], cstring, ind + len - 1);
            }
        }
        return nullptr;
    }

    void DFS(Node *node) {
        if (node->tst != -1)std::cout << node->tst << ' ';
        for (int i = 0; i < node->lastIndex; i++) {
            DFS(node->children[i]);
        }
    }

    void Search(const char *cstring) {
        for (int i = 0; i < root->lastIndex; i++) {
            Node *node = Search(root->children[i], cstring, 0);
            if (node != nullptr) {
                DFS(node);
            }
        }

    }

    void print() {

        for (int i = 0; i < root->lastIndex; i++) {
            print(root->children[i]);
            std::cout << "TT\n";
        }
    }

};

int main() {

    SuffixTree t("banana$");
    t.print();

    t.Search("ana"); // Prints: 1 3 7
    std::cout << '\n';
    t.Search("bana"); // Prints: 4 8

    return 0;
}
