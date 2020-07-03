#pragma once
// Implementing Red-Black Tree in C++

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;


struct Node {
    int data;
    long long int x;
    long long int y;
    Node* parent;
    Node* left;
    Node* right;
    int color;
};

typedef Node* NodePtr;

class RBTree {
private:
    NodePtr root;
    NodePtr TNULL;
    long long int currX;
    long long int currY;
    long long int currDist;

    void initializeNULLNode(NodePtr node, NodePtr parent) {
        node->data = 0;
        node->parent = parent;
        node->left = nullptr;
        node->right = nullptr;
        node->color = 0;
    }

    // Preorder
    void preOrderHelper(NodePtr node) {
        if (node != TNULL) {
            cout << node->data << " ";
            preOrderHelper(node->left);
            preOrderHelper(node->right);
        }
    }

    void queryinOrderHelper(NodePtr node, int &res, int &farthest, long int &farthestDist) {
        if (node != TNULL) {
            queryinOrderHelper(node->left, res, farthest, farthestDist);
            long double xRes = pow(currX - node->x, 2);
            long double yRes = pow(currY - node->y, 2);
            long double powRad = pow(currDist, 2);
            long double distance = powRad - (xRes + yRes);
            if (distance > 0) {
                //cout << node->data << "INSIDE" << endl;
                res++;
                if (distance < farthestDist) {
                    farthestDist = distance;
                    farthest = node->data;
                }
            }
            //else if(distance < 0)
                //cout << node->data << "OUTSIDE" << endl;
            else if(distance == 0){
                //cout << node->data << "ON THE LINE" << endl;
                res++;
                if (distance < farthestDist) {
                    farthestDist = distance;
                    farthest = node->data;
                }
            }

            queryinOrderHelper(node->right, res, farthest, farthestDist);
        }
    }

    // Inorder
    void inOrderHelper(NodePtr node) {
        if (node != TNULL) {
            inOrderHelper(node->left);
            cout << node->data << " ";
            inOrderHelper(node->right);
        }
    }

    // Post order
    void postOrderHelper(NodePtr node) {
        if (node != TNULL) {
            postOrderHelper(node->left);
            postOrderHelper(node->right);
            cout << node->data << " ";
        }
    }

    bool searchTreeHelper(NodePtr node, int key) {
        if (node == TNULL) {
            return 0;
        }

        if (key == node->data) {
            return 1;
        }
        if (key < node->data) {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }

    // For balancing the tree after deletion
    void deleteFix(NodePtr x) {
        NodePtr s;
        while (x != root && x->color == 0) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (s->color == 1) {
                    s->color = 0;
                    x->parent->color = 1;
                    leftRotate(x->parent);
                    s = x->parent->right;
                }

                if (s->left->color == 0 && s->right->color == 0) {
                    s->color = 1;
                    x = x->parent;
                }
                else {
                    if (s->right->color == 0) {
                        s->left->color = 0;
                        s->color = 1;
                        rightRotate(s);
                        s = x->parent->right;
                    }

                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->right->color = 0;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else {
                s = x->parent->left;
                if (s->color == 1) {
                    s->color = 0;
                    x->parent->color = 1;
                    rightRotate(x->parent);
                    s = x->parent->left;
                }

                if (s->right->color == 0 && s->right->color == 0) {
                    s->color = 1;
                    x = x->parent;
                }
                else {
                    if (s->left->color == 0) {
                        s->right->color = 0;
                        s->color = 1;
                        leftRotate(s);
                        s = x->parent->left;
                    }

                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->left->color = 0;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = 0;
    }

    void rbTransplant(NodePtr u, NodePtr v) {
        if (u->parent == nullptr) {
            root = v;
        }
        else if (u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void deleteNodeHelper(NodePtr node, int key) {
        NodePtr z = TNULL;
        NodePtr x, y;
        while (node != TNULL) {
            if (node->data == key) {
                z = node;
            }

            if (node->data <= key) {
                node = node->right;
            }
            else {
                node = node->left;
            }
        }

        if (z == TNULL) {
            //cout << "Key not found in the tree" << endl;
            return;
        }

        y = z;
        int y_original_color = y->color;
        if (z->left == TNULL) {
            x = z->right;
            rbTransplant(z, z->right);
        }
        else if (z->right == TNULL) {
            x = z->left;
            rbTransplant(z, z->left);
        }
        else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            }
            else {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (y_original_color == 0) {
            deleteFix(x);
        }
    }

    // For balancing the tree after insertion
    void insertFix(NodePtr k) {
        NodePtr u;
        while (k->parent->color == 1) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u->color == 1) {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    leftRotate(k->parent->parent);
                }
            }
            else {
                u = k->parent->parent->right;

                if (u->color == 1) {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = 0;
    }

    void printHelper(NodePtr root, string indent, bool last) {
        if (root != TNULL) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "   ";
            }
            else {
                cout << "L----";
                indent += "|  ";
            }

            string sColor = root->color ? "RED" : "BLACK";
            cout << root->data << "(" << sColor << ")" << endl;
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }

public:
    RBTree() {
        TNULL = new Node;
        TNULL->color = 0;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }

    void preorder() {
        preOrderHelper(this->root);
    }

    void inorder() {
        inOrderHelper(this->root);
    }

    int queryinorder(int centerX, int centerY, int dist, int farthest) {
        int res = 0;
        long int farthestP = 9999999;
        currX = centerX; currY = centerY; currDist = dist;

        queryinOrderHelper(this->root, res, farthest, farthestP);
        return res;
    }
    void postorder() {
        postOrderHelper(this->root);
    }

    bool searchTree(int k) {
        return searchTreeHelper(this->root, k);
    }

    NodePtr minimum(NodePtr node) {
        while (node->left != TNULL) {
            node = node->left;
        }
        return node;
    }

    NodePtr maximum(NodePtr node) {
        while (node->right != TNULL) {
            node = node->right;
        }
        return node;
    }

    NodePtr successor(NodePtr x) {
        if (x->right != TNULL) {
            return minimum(x->right);
        }

        NodePtr y = x->parent;
        while (y != TNULL && x == y->right) {
            x = y;
            y = y->parent;
        }
        return y;
    }

    NodePtr predecessor(NodePtr x) {
        if (x->left != TNULL) {
            return maximum(x->left);
        }

        NodePtr y = x->parent;
        while (y != TNULL && x == y->left) {
            x = y;
            y = y->parent;
        }

        return y;
    }

    void leftRotate(NodePtr x) {
        NodePtr y = x->right;
        x->right = y->left;
        if (y->left != TNULL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(NodePtr x) {
        NodePtr y = x->left;
        x->left = y->right;
        if (y->right != TNULL) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        }
        else if (x == x->parent->right) {
            x->parent->right = y;
        }
        else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    // Inserting a node
    void insert(int key, int pointX, int pointY) {
        NodePtr node = new Node;
        node->parent = nullptr;
        node->data = key;
        node->x = pointX;
        node->y = pointY;
        node->left = TNULL;
        node->right = TNULL;
        node->color = 1;

        NodePtr y = nullptr;
        NodePtr x = this->root;

        while (x != TNULL) {
            y = x;
            if (node->data < x->data) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }

        node->parent = y;
        if (y == nullptr) {
            root = node;
        }
        else if (node->data < y->data) {
            y->left = node;
        }
        else {
            y->right = node;
        }

        if (node->parent == nullptr) {
            node->color = 0;
            return;
        }

        if (node->parent->parent == nullptr) {
            return;
        }

        insertFix(node);
    }

    NodePtr getRoot() {
        return this->root;
    }

    void deleteNode(int data) {
        deleteNodeHelper(this->root, data);
    }

    void printTree() {
        if (root) {
            printHelper(this->root, "", true);
        }
    }
};


//int main() {
//    string line;
//    RedBlackTree rbtree;
//
//    ofstream outfile;
//    outfile.open("output_redblack.txt");
//    float totalDur = 0;
//    int duration = 0;
//    int worstCase = 0;
//
//    int count = 1;
//    for (int j = 1; j <= 10; j++)     //EVERY STEP CONTAIN 10000 ELEMENT INPUT
//    {
//
//        //INSERT OPERATION
//        fstream in("input_step-" + to_string(j) + ".txt");
//        while (getline(in, line))
//        {
//            auto start = high_resolution_clock::now();
//            rbtree.insert(stoi(line));
//
//            auto stop = high_resolution_clock::now();
//            duration = duration_cast<microseconds>(stop - start).count();
//            totalDur += duration;
//
//        }
//        //WRITE INSERT AVERAGE TIME
//        outfile << "INSERT -- " << j << " : " << totalDur / 10000 << " microseconds " << " microseconds" << endl;
//        totalDur = 0;
//
//        fstream in_search("search-" + to_string(j) + ".txt");
//        while (getline(in_search, line))
//        {
//            auto start = high_resolution_clock::now();
//
//            if (rbtree.searchTree(stoi(line)))
//                cout << "NUMBER FOUND" << endl;
//            else
//                cout << "NOT FOUND" << endl;
//
//            auto stop = high_resolution_clock::now();
//            duration = duration_cast<microseconds>(stop - start).count();
//            totalDur += duration;
//
//            if (duration > worstCase)
//                worstCase = duration;
//        }
//
//        outfile << "SEARCH -- " << j << " : " << totalDur / 10 << " microseconds " << "worstcase : " << worstCase << " microseconds" << endl
//            << endl << endl;
//        totalDur = 0;
//        worstCase = 0;
//        in.close();
//        in_search.close();
//    }
//    outfile.close();
//}