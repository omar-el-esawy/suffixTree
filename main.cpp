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

        build();
    }

    void build() {
        for (int i = 0; i < sz; i++)
            build(root, i);
    }

    void buildd() {
        for (int i = 0; i < sz; i++) {

            Node *node = new Node(i, sz - i, i);
            int len = -1;
            Node *cnode;
            auto &rchld = root->children;
            int index;
            for (int j = 0; j < root->lastIndex; j++) {
                if (s[i] == s[rchld[j]->ost]) {
                    len = 1;
                    cnode = rchld[j];
                    while (s[i + len] == s[rchld[j]->ost + len] and i + len < sz) {
                        std::cout << len << '\n';
                        len++;
                    }
                    len++;
                    index = j;
                    break;
                }
            }
            if (len == -1)root->add(node);
            else {
                Node *par = new Node(cnode->ost, len, -1);
                Node *chl1 = new Node(i + len, sz - (i + len), i);
                Node *chl2 = new Node(cnode->ost + len, sz - (cnode->ost + len), cnode->ost);
                par->add(chl1);
                par->add(chl2);
                root->add(par, index);
                delete cnode;
            }
        }
    }

    void build(Node *par, int ind) {
        if (ind == sz) return;

        int len = -1;
        int ls = 0;
        Node *opar;
        for (int i = 0; i < par->lastIndex; i++) {
            if (s[ind] == s[par->children[i]->ost]) {
                len = 0;
                ls = i;
                opar = par->children[i];
                while (s[ind + len] == s[par->children[i]->ost + len] and i + len < sz )len++;
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
        } else
            lead = new Node(opar->ost + len, opar->len - len, opar->tst);
        npar->add(child);
        npar->add(lead);
        par->add(npar, ls);
        delete opar;
        build(npar, ind + len);
    }


    void print() {
        auto &children = root->children;
        for (int i = 0; i < root->lastIndex; i++) {
            std::cout << children[i]->ost << ' ' << children[i]->len << ' ' << children[i]->tst << '\n';
        }
    }


};

int main() {

    SuffixTree t("banana$");
    t.print();


    return 0;
}
