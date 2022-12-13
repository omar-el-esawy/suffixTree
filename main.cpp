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
    char *s;
    Node *root;
    int sz;
public:

    SuffixTree(char *string) {
        int n = 26;
        this->s = string;
        sz = strlen(string);
        root = new Node(0, sz, 0);
        root->add(new Node(sz - 1, 1, sz - 1));
        build();
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
                while (s[ind + len] == s[par->children[i]->ost + len] and i + len < sz)len++;
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
        } else
            lead = new Node(opar->ost + len, opar->len - len, opar->tst);
        npar->add(child);
        npar->add(lead);
        par->add(npar, ls);
        delete opar;
        build(npar, ind + len);
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


    return 0;
}
