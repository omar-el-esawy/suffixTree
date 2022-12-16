#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

//the node for suffix tree
struct Node {

    //The starting index of the characters on the edge going to this node.
    int ID;

    //The starting index of the suffix ending at this node
    int suffixStart;

    //Array of pointers to children nodes.
    Node **children;

    //count children of this node
    int node_size;

    Node() {} //default constructor

    //parameterized constructor to initialize the ID and suffix start of node
    Node(int ID, int suffixStart)
    {

        this->ID = ID; //assign the node ID
        this->suffixStart = suffixStart; //assign node suffix start
        node_size = 0; //initialize size to zero

        int n = 27; //The array size equals to the alphabet size + $ sign

        children = new Node *[n]; //dynamic allocation for node children
        for (int i = 0; i < n; i++)
        {
            children[i] = nullptr; //set each child to null
        }
    }

    //function that measures length of a node
    int get_len(int sz) //take the original string size as parameter
    {
        int mn = sz + 1; //set the minimum to a larger number
        int len = 0; //initialize length to zero

        if (node_size == 0) //if the node is terminal
        {
            //if the node is terminal then it's length
            //is equal to the original string size - the start index of this suffix
            len = sz - ID;
        }
        else //if the node is not terminal then it's length is equal to the least difference between it and it's children
        {
            for (int i = 0; i < 27; i++) //loop on all the node's children
            {
            if (children[i] != nullptr) //if child is not empty
                mn = std::min(mn, children[i]->ID - this->ID); //choose the least difference between the node each child
            }
            len = mn; //set length to the found true one
        }
        return len; //return length
    }

    //function that returns the node child according to its first character
    Node *is_here(int ind)
    {
        ind = (ind == 36 ? 'z' - 'a' + 1 : ind - 'a');
        return children[ind];
    }



    void add(Node *node, int ind) //add new child
    {
        //add node into its suitable child according to it's first character
        //example if suffix starts with a add it to index 0 etc..
        ind = (ind == 36 ? 'z' - 'a' + 1 : ind - 'a');
        children[ind] = node; //put the node to it's position
        node_size++; //increment the size
    }
};

class SuffixTree {
    const char *s; //the original string (character array)
    Node *root; //the root node of the tree
    int sz; //size of the original string of the suffix tree
public:

    SuffixTree(const char *string) //initialize the suffix tree
    {
    int n = 27;
    this->s = string; //set the original string
    sz = strlen(string); //set the sz to the size of string
    root = new Node(0, 0); //dynamically create root node
    build(); //this function builds the suffix tree
    root->add(new Node(sz - 1, sz - 1), s[sz - 1]); //add the extra $ to the tree
    }

    void build() //this function builds the whole tree by calling a function that builds each suffix
    {
        //build each suffix of the original string into the tree
        for (int i = 0; i < sz; i++)
        {
            build(root, i, i); //function that build each suffix
        }
    }


    void build(Node *par, int ind, int orignal_ind)
    {
        if (ind == sz - 1) //return if it's end of string
        {
            return;
        }

        int len = -1; //this variable measures the length of common characters between suffix and any node
        Node *opar = par->is_here(s[ind]); //old parent is the one suitable for the first character of suffix (using is_here function)

        if (opar != nullptr) //if the node is not empty will loop through children to get longest matching characters
        {
            len = 0; //initialize length to zero and it will be incremented with each next found common character
            while (s[ind + len] == s[opar->ID + len] and ind + len < sz and
                        opar->ID + len < opar->ID + opar->get_len(sz)) //while there is matching chars
            {
                len++; //increment length
            }
        }

        if (len == -1) //if the root has no child at this char index, the node will just be added
        {
            Node *node = new Node(ind, ind); //create a new node with id and suffix start
            par->add(node, s[ind]); //add this node to the children
            return;
        }

        if (len == opar->get_len(sz)) //if the common length is equal to the parent length
        {

            if (opar->is_here(s[ind + len]) != nullptr and s[ind + len] != '$') //if not the end of string and parent has more children
            {
                //will make recursive call to build the suffix starting from after the common characters end (remaining suffix characters)
                build(opar, ind + len, orignal_ind);

            }
            else //if there's no more children to check then it will just be added
            {

                Node *child = new Node(ind + len, orignal_ind); //create node with the start position from after the common characters
                opar->add(child, s[child->ID]); //add this node
            }
            return; //exit because node is added
        }
        //in this case, we will split the current node,
        //the we will put the remaining suffix from after the common characters,
        //in a new node and add it to the parent node,
        //and the children of old parent will be kept but from after the common characters too
        Node *npar, *child, *lead; //lead is the remaining existing suffix after common words
        npar = new Node(opar->ID, -1); //new parent takes id -1 because it is internal node
        child = new Node(ind + len, orignal_ind); //child will take start index from after the common characters
        if (opar->node_size != 0) //if the old parent had children we will keep them as the new parents children
        {
            lead = new Node(opar->ID + len, -1); //give the lead(rest of the suffix) ID starting from after the common characters length
            lead->children = opar->children; //add the old parent children to the lead node
            lead->node_size = opar->node_size; //give it the size too
        }
        else
        {
            //if the node did not have children will just create lead node that carries the rest of existing suffix
            lead = new Node(opar->ID + len, opar->suffixStart);
        }
        npar->add(child, s[child->ID]); //add the child to new parent
        npar->add(lead, s[lead->ID]);   //add the lead
        par->add(npar, s[npar->ID]); //add the new parent itself to node
        delete opar; //delete the old parent node
    }




    Node *Search(Node *par, const char *cstring, int ind) //search function
    {

        int len = -1; //this var will be used to count number of common characters
        if (cstring[ind] == s[par->ID]) //check if the character at index 'ind' of suffix is the same of ID of given node
        {
            len = 0;
            //loop through the characters of node and suffix
            while (cstring[ind + len] == s[par->ID + len] &&
                   ind + len < strlen(cstring) &&
                   par->ID + len < par->ID + par->get_len(sz))
            {
                len++; //increment the len with each common character
            }
        }
        //if it's not found
        if (len == -1 || len + ind >= sz)
        {
            return nullptr;
        }
        //if it's all found in the given parent node
        else if (ind + len == strlen(cstring))
        {
            return par;
        }
        //will call search again until the cstring is all finished
        else
        {
            Node *child = par->is_here(cstring[ind + len]); //create a new node with suffix start after the already found length
            if (child != nullptr)
            {
                //recursive call
                return Search(child, cstring, ind + len);
            }

        }
        return nullptr;
    }

    //search for all of this string occurrences in the original tree string
    void Search(const char *cstring)
    {
        //go to child node at the index of the first character
        Node *root_children = root->is_here(cstring[0]);

        Node *node = nullptr; //used to search

        if (root_children != nullptr) //if the child at this index is not empty, will recursively search through it
        {
            //search from that node
            node = Search(root_children, cstring, 0);
        }
        //if the node found has more children, will do dfs
        if (node != nullptr)
        {
            DFS(node); //will do dfs until prints the ID of all suffix that has the cstring
        }
        std::cout<<"\n";


    }

    //function that does dfs on node (used for printing and search)
    void DFS(Node *node) {
        if (node->suffixStart != -1)
        {
            std::cout << node->suffixStart << ' '; //print the suffix start
        }
        //loop though the children to print recursively
        for (int i = 0; i < 27; i++)
        {
            if (node->children[i] != nullptr) //if it is not empty
            {
                DFS(node->children[i]); //repeat
            }
        }
    }

    //function that takes node to print ALL its children (used in testing)
    void print(Node *root)
    {
        //print the suffix itself using its length and id
        for (int j = root->ID; j < root->get_len(sz) + root->ID; j++)
        {
            std::cout << s[j]; //printing the suffix char by char
        }
        std::cout << ' ' << root->suffixStart; //print suffix start
        std::cout << '\n'; //new line
        auto &children = root->children; //for ease

        //loop though the children to print recursively
        for (int i = 0; i < 27; i++)
        {
            if (children[i] != nullptr)
            {
                print(children[i]); //recursive call
            }
        }
    }

    //print function (used in testing)
    void print()
    {
        //loop on each node and print it's children
        for (int i = 0; i < 27; i++)
        {
            if (root->children[i] != nullptr)
            {
                //function that does dfs on node to traverse all children
                print(root->children[i]);
                std::cout << "++____++\n";
            }
        }
    }

};

int main()
{
    // Construct a suffix tree containing all suffixes of "bananabanaba$"

    //            0123456789012
    SuffixTree t("bananabanaba$");

    t.Search("ana"); // Prints: 1 3 7
    t.Search("naba"); // Prints: 4 8

    // Add test cases here.

    //             012345678901
    SuffixTree t1("habibaahmed$");
    std::cout<<"test case 1:\n";
    t1.Search("biba");
    t1.Search("a");

    //             012345678901
    SuffixTree t2("omarmohamed$");
    std::cout<<"test case 2:\n";
    t2.Search("m");
    t2.Search("mar");

    //             012345678
    SuffixTree t3("sararady$");
    std::cout<<"test case 3:\n";
    t3.Search("ra");
    t3.Search("sara");

    //             01234567890123
    SuffixTree t4("ibrahimismail$");
    std::cout<<"test case 4:\n";
    t4.Search("him");
    t4.Search("i");

    //             012345678901
    SuffixTree t5("nadamohamed$");
    std::cout<<"test case 5:\n";
    t5.Search("a");
    t5.Search("nada");

    //             0123456
    SuffixTree t6("banana$");
    std::cout<<"test case 6:\n";
    t6.Search("ana");
    t6.Search("ba");

    //             0123456789
    SuffixTree t7("papatpapa$");
    std::cout<<"test case 7:\n";
    t7.Search("pa");
    t7.Search("t");

    //             012345678901
    SuffixTree t8("testestesty$");
    std::cout<<"test case 8:\n";
    t8.Search("a");
    t8.Search("nada");

    //             0123456789
    SuffixTree t9("fixsuffix$");
    std::cout<<"test case 9:\n";
    t9.Search("fix");
    t9.Search("x");

    //              0123456789012
    SuffixTree t10("testingtests$");
    std::cout<<"test case 10:\n";
    t10.Search("a");
    t10.Search("nada");

    return 0;
}
