#pragma once
// Searching a key on a B-tree in C++

#include <iostream>
#include <fstream>
#include <string>
#define LLINT long long int
#define LLINT long int
#define MAXFARTHEST 9999999999999
using namespace std;

struct Node {
    int data;
    LLINT x;
    LLINT y;
};
class TreeNode {
    Node* nodes[6];
    int* keys;
    int t;
    TreeNode** C;
    int n;
    bool leaf;

public:
    TreeNode(int temp, bool bool_leaf);

    void insertNonFull(Node* node);
    void splitChild(int i, TreeNode* y);
    void traverse();

    int findKey(int k);
    void deletion(int k);
    void removeFromLeaf(int idx);
    void removeFromNonLeaf(int idx);
    Node* getPredecessor(int idx);
    Node* getSuccessor(int idx);
    void fill(int idx);
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);
    void merge(int idx);
    TreeNode* search(int k);
    void QueryTraverseHelper(int &res, int& farthest, long double &farthestDist, LLINT &currX, LLINT &currY, LLINT &currDist);
    friend class BTree;
};

class BTree {
    TreeNode* root;
    int t;

public:
    int queryTraverse(LLINT centerX, LLINT centerY, LLINT dist, int& farthest) {
        int res = 0;
        long double farthestP = MAXFARTHEST;

        root->QueryTraverseHelper(res, farthest, farthestP, centerX, centerY, dist);
        return res;
    }

    BTree(int temp) {
        root = NULL;
        t = temp;
    }

    void traverse() {
        if (root != NULL)
            root->traverse();
    }

    TreeNode* search(int k) {
        return (root == NULL) ? NULL : root->search(k);
    }

    void insert(int k, LLINT x, LLINT y);

    void deletion(int k);
};

TreeNode::TreeNode(int t1, bool leaf1) {
    t = t1;
    leaf = leaf1;
    C = new TreeNode * [2 * t];

    n = 0;
}

void TreeNode::QueryTraverseHelper(int &res, int &farthest, long double &farthestDist, LLINT &currX, LLINT &currY, LLINT &currDist) {
    int i;
    for (i = 0; i < n; i++) {
        if (leaf == false)
            C[i]->QueryTraverseHelper(res, farthest, farthestDist, currX, currY, currDist);
        long double xRes = pow(currX - nodes[i]->x, 2);
        long double yRes = pow(currY - nodes[i]->y, 2);
        long double powRad = pow(currDist, 2);
        long double distance = powRad - (xRes + yRes);
        if (distance > 0) {
            res++;
            if (distance < farthestDist) {
                farthestDist = distance;
                farthest = nodes[i]->data;
            }
        }
        else if (distance == 0) {
            res++;
            if (distance < farthestDist) {
                farthestDist = distance;
                farthest = nodes[i]->data;
            }
        }
    }

    if (leaf == false)
        C[i]->QueryTraverseHelper(res, farthest, farthestDist, currX, currY, currDist);
}

void TreeNode::traverse() {
    int i;
    for (i = 0; i < n; i++) {
        if (leaf == false)
            C[i]->traverse();

        cout << nodes[i]->data << " : (" << nodes[i]->x << " , " << nodes[i]->y << ")"<< endl;
    }

    if (leaf == false)
        C[i]->traverse();
}

TreeNode* TreeNode::search(int k) {
    int i = 0;
    while (i < n && k > nodes[i]->data)
        i++;

    if (nodes[i]->data == k)
        return this;

    if (leaf == true)
        return NULL;

    return C[i]->search(k);
}

void BTree::insert(int k, LLINT x, LLINT y) {
    Node* newNode = new Node; newNode->data = k;  newNode->x = x; newNode->y = y;

    if (root == NULL) {
        root = new TreeNode(t, true);

        root->nodes[0] = newNode;
        root->n = 1;
    }
    else {
        if (root->n == 2 * t - 1) {
            TreeNode* s = new TreeNode(t, false);

            s->C[0] = root;

            s->splitChild(0, root);

            int i = 0;
            if (s->nodes[0]->data < k)
                i++;
            s->C[i]->insertNonFull(newNode);

            root = s;
        }
        else
            root->insertNonFull(newNode);
    }
}

void TreeNode::insertNonFull(Node* newNode) {
    int i = n - 1;

    if (leaf == true) {
        while (i >= 0 && nodes[i]->data > newNode->data) {
            nodes[i + 1] = nodes[i];
            i--;
        }

        nodes[i + 1] = newNode;
        n = n + 1;
    }
    else {
        while (i >= 0 && nodes[i]->data > newNode->data)
            i--;

        if (C[i + 1]->n == 2 * t - 1) {
            splitChild(i + 1, C[i + 1]);

            if (nodes[i + 1]->data < newNode->data)
                i++;
        }
        C[i + 1]->insertNonFull(newNode);
    }
}

void TreeNode::splitChild(int i, TreeNode* y) {
    TreeNode* z = new TreeNode(y->t, y->leaf);
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++)
        z->nodes[j] = y->nodes[j + t];

    if (y->leaf == false) {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j + t];
    }

    y->n = t - 1;
    for (int j = n; j >= i + 1; j--)
        C[j + 1] = C[j];

    C[i + 1] = z;

    for (int j = n - 1; j >= i; j--)
        nodes[j + 1] = nodes[j];

    nodes[i] = y->nodes[t - 1];
    n = n + 1;
}

// Find the key
int TreeNode::findKey(int k) {
    int idx = 0;
    while (idx < n && nodes[idx]->data < k)
        ++idx;
    return idx;
}

// Deletion operation
void TreeNode::deletion(int k) {
    int idx = findKey(k);

    if (idx < n && nodes[idx]->data == k) {
        if (leaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    }
    else {
        if (leaf) {
            //cout << "The key " << k << " is does not exist in the tree\n";
            return;
        }

        bool flag = ((idx == n) ? true : false);

        if (C[idx]->n < t)
            fill(idx);

        if (flag && idx > n)
            C[idx - 1]->deletion(k);
        else
            C[idx]->deletion(k);
    }
    return;
}

// Remove from the leaf
void TreeNode::removeFromLeaf(int idx) {
    for (int i = idx + 1; i < n; ++i)
        nodes[i - 1] = nodes[i];

    n--;

    return;
}

// Delete from non leaf node
void TreeNode::removeFromNonLeaf(int idx) {
    int k = nodes[idx]->data;

    if (C[idx]->n >= t) {
        Node* pred = getPredecessor(idx);
        nodes[idx] = pred;
        C[idx]->deletion(pred->data);
    }

    else if (C[idx + 1]->n >= t) {
        Node* succ = getSuccessor(idx);
        nodes[idx] = succ;
        C[idx + 1]->deletion(succ->data);
    }

    else {
        merge(idx);
        C[idx]->deletion(k);
    }
    return;
}

Node* TreeNode::getPredecessor(int idx) {
    TreeNode* cur = C[idx];
    while (!cur->leaf)
        cur = cur->C[cur->n];

    return cur->nodes[cur->n - 1];
}

Node* TreeNode::getSuccessor(int idx) {
    TreeNode* cur = C[idx + 1];
    while (!cur->leaf)
        cur = cur->C[0];

    return cur->nodes[0];
}

void TreeNode::fill(int idx) {
    if (idx != 0 && C[idx - 1]->n >= t)
        borrowFromPrev(idx);

    else if (idx != n && C[idx + 1]->n >= t)
        borrowFromNext(idx);

    else {
        if (idx != n)
            merge(idx);
        else
            merge(idx - 1);
    }
    return;
}

// Borrow from previous
void TreeNode::borrowFromPrev(int idx) {
    TreeNode* child = C[idx];
    TreeNode* sibling = C[idx - 1];

    for (int i = child->n - 1; i >= 0; --i)
        child->nodes[i + 1] = child->nodes[i];

    if (!child->leaf) {
        for (int i = child->n; i >= 0; --i)
            child->C[i + 1] = child->C[i];
    }

    child->nodes[0] = nodes[idx - 1];

    if (!child->leaf)
        child->C[0] = sibling->C[sibling->n];

    nodes[idx - 1] = sibling->nodes[sibling->n - 1];

    child->n += 1;
    sibling->n -= 1;

    return;
}

// Borrow from the next
void TreeNode::borrowFromNext(int idx) {
    TreeNode* child = C[idx];
    TreeNode* sibling = C[idx + 1];

    child->nodes[(child->n)] = nodes[idx];

    if (!(child->leaf))
        child->C[(child->n) + 1] = sibling->C[0];

    nodes[idx] = sibling->nodes[0];

    for (int i = 1; i < sibling->n; ++i)
        sibling->nodes[i - 1] = sibling->nodes[i];

    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->n; ++i)
            sibling->C[i - 1] = sibling->C[i];
    }

    child->n += 1;
    sibling->n -= 1;

    return;
}

// Merge
void TreeNode::merge(int idx) {
    TreeNode* child = C[idx];
    TreeNode* sibling = C[idx + 1];

    child->nodes[t - 1] = nodes[idx];

    for (int i = 0; i < sibling->n; ++i)
        child->nodes[i + t] = sibling->nodes[i];

    if (!child->leaf) {
        for (int i = 0; i <= sibling->n; ++i)
            child->C[i + t] = sibling->C[i];
    }

    for (int i = idx + 1; i < n; ++i)
        nodes[i - 1] = nodes[i];

    for (int i = idx + 2; i <= n; ++i)
        C[i - 1] = C[i];

    child->n += sibling->n + 1;
    n--;

    delete (sibling);
    return;
}


void BTree::deletion(int k) {
    if (!root) {
        cout << "The tree is empty\n";
        return;
    }

    root->deletion(k);

    if (root->n == 0) {
        TreeNode* tmp = root;
        if (root->leaf)
            root = NULL;
        else
            root = root->C[0];

        delete tmp;
    }
    return;
}