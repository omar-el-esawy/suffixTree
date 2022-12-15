#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>


struct Node {
    int ost;
    int tst;
    Node **children;
    int node_size;

    Node() {}

    Node(int ost, int tst) {
        this->ost = ost;

        this->tst = tst;
        node_size = 0;

        int n = 27;
        children = new Node *[n];
        for (int i = 0; i < n; i++) {
            children[i] = nullptr;
        }
    }


    int get_len(int sz) {
        int mn = sz + 1;
        int len = 0;
        if (node_size == 0)len = sz - ost;
        else {
            for (int i = 0; i < 27; i++) {
                if (children[i] != nullptr)
                    mn = std::min(mn, children[i]->ost - this->ost);
            }
            len = mn;
        }
        return len;
    }

    Node *is_here(int ind) {
        ind = (ind == 36 ? 'z' - 'a' + 1 : ind - 'a');
        return children[ind];
    }

//    void add(Node *node) {
//        children[node_size++] = node;
//    }

    void add(Node *node, int ind) {
        ind = (ind == 36 ? 'z' - 'a' + 1 : ind - 'a');
        children[ind] = node;
        node_size++;
    }
};

class SuffixTree {
    const char *s;
    Node *root;
    int sz;
public:

    SuffixTree(const char *string) {
        int n = 27;
        this->s = string;
        sz = strlen(string);
        root = new Node(0, 0);
        build();
        root->add(new Node(sz - 1, sz - 1), s[sz - 1]);
    }

    void build() {
        for (int i = 0; i < sz; i++)
            build(root, i, i);
    }


    void build(Node *par, int ind, int orignal_ind) {
        if (ind == sz - 1) return;

        int len = -1;
        Node *opar = par->is_here(s[ind]);

        if (opar != nullptr) {
            len = 0;
            while (s[ind + len] == s[opar->ost + len] and ind + len < sz and
                   opar->ost + len < opar->ost + opar->get_len(sz))
                len++;
        }

        if (len == -1) {
            Node *node = new Node(ind, ind);
            par->add(node, s[ind]);
            return;
        }

        if (len == opar->get_len(sz)) {
            if (opar->is_here(s[ind + len]) != nullptr and s[ind + len] != '$') {
                build(opar, ind + len, orignal_ind);
            } else {
                Node *child = new Node(ind + len, orignal_ind);
                opar->add(child, s[child->ost]);

            }
            return;
        }

        Node *npar, *child, *lead;
        npar = new Node(opar->ost, -1);
        child = new Node(ind + len, orignal_ind);
        if (opar->node_size != 0) {
            lead = new Node(opar->ost + len, -1);
            lead->children = opar->children;
            lead->node_size = opar->node_size;
        } else
            lead = new Node(opar->ost + len, opar->tst);
        npar->add(child, s[child->ost]);
        npar->add(lead, s[lead->ost]);
        par->add(npar, s[npar->ost]);
        delete opar;
    }


    void print(Node *root) {
        for (int j = root->ost; j < root->get_len(sz) + root->ost; j++) {
            std::cout << s[j];
        }
        std::cout << ' ' << root->tst;
        std::cout << '\n';
        auto &children = root->children;
        for (int i = 0; i < 27; i++) {
            if (children[i] != nullptr)
                print(children[i]);
        }
    }

    Node *Search(Node *par, const char *cstring, int ind) {

        int len = -1;
        if (cstring[ind] == s[par->ost]) {
            len = 0;
            while (cstring[ind + len] == s[par->ost + len] and
                   ind + len < strlen(cstring) and
                   par->ost + len < par->ost + par->get_len(sz))
                len++;
        }
        if (len == -1 || len + ind >= sz)return nullptr;
        else if (ind + len == strlen(cstring))return par;
        else {
            Node *child = par->is_here(cstring[ind + len]);
            if (child != nullptr)return Search(child, cstring, ind + len);
//            for (int i = 0; i < par->node_size; i++) {
//                if (s[par->children[i]->ost] == cstring[ind + len - 1])
//                    return Search(par->children[i], cstring, ind + len - 1);
//            }
        }
        return nullptr;
    }

    void DFS(Node *node) {
        if (node->tst != -1)std::cout << node->tst << ' ';
        for (int i = 0; i < 27; i++) {
            if (node->children[i] != nullptr)
                DFS(node->children[i]);
        }
    }

    void Search(const char *cstring) {
        Node *root_children = root->is_here(cstring[0]);
        Node *node = nullptr;

        if (root_children != nullptr)
            node = Search(root_children, cstring, 0);


        if (node != nullptr)
            DFS(node);


    }

    void print() {

        for (int i = 0; i < 27; i++) {
            if (root->children[i] != nullptr) {
                print(root->children[i]);
                std::cout << "++____++\n";
            }
        }
    }

};

int main() {


    //                 0123456789012
    SuffixTree t("bananabanaba$");
    t.print();
    t.Search("ana"); // Prints: 1 3 7
    std::cout << '\n';
    t.Search("naba"); // Prints: 4 8

    // Add test cases here.


    return 0;
}
