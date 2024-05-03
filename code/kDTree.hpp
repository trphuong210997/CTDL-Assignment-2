#include "main.hpp"
#include "Dataset.hpp"
/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */
struct kDTreeNode
{
    vector<int> data;
    kDTreeNode *left;
    kDTreeNode *right;
    kDTreeNode(vector<int> data, kDTreeNode *left = nullptr, kDTreeNode *right = nullptr)
    {
        this->data = data;
        this->left = left;
        this->right = right;
    }

    void printData() {
        cout << "(";
        for (int i = 0; i < data.size(); i++) {
            if (i == data.size() - 1)
                cout << data[i];
            else
                cout << data[i] << ", ";
        }
        cout << ")";
    }
};

class kDTree
{
private:
    int count;
    int k;
    kDTreeNode *root;

public:
    kDTree(int k = 2) : k(k), count(0), root(nullptr) {}

    ~kDTree() {
        delete root;
    }

    const kDTree &operator=(const kDTree &other) {
        this->k = other.k;
        this->count = other.count;
        this->root = copyData(other.root);
        return *this;
    }

    kDTreeNode *copyData(kDTreeNode *node) {
        if (node == nullptr) 
            return nullptr;
        
        return new kDTreeNode(node->data, copyData(node->left), copyData(node->right));
    }

    kDTree(const kDTree &other) {
        this->k = other.k;
        this->count = other.count;
        this->root = copyData(other.root);
    }

    void LNR(kDTreeNode *node) const {
        if (node == nullptr) 
            return;
        
        LNR(node->left);
        node->printData();
        LNR(node->right);
    }

    void NLR(kDTreeNode *node) const {
        if (node == nullptr)
            return;
        
        node->printData();
        NLR(node->left);
        NLR(node->right);
    }

    void LRN(kDTreeNode *node) const {
        if (node == nullptr)
            return;
        
        LRN(node->left);
        LRN(node->right);
        node->printData();
    }

    void inorderTraversal() const {
        return LNR(root);
    }

    void preorderTraversal() const {
        return NLR(root);
    }

    void postorderTraversal() const {
        return LRN(root);
    }

    int calHeight(kDTreeNode *node) const {
        if (node == nullptr) 
            return 0;
        
        return 1 + max(calHeight(node->left), calHeight(node->right));
    }

    int height() const {
        return calHeight(root);
    }

    int nodeCount() const {
        return count;
    }

    int TreeLeafCount(kDTreeNode *node) const {
        if (node == nullptr)
            return 0;

        if (node->left == nullptr && node->right == nullptr)
            return 1;
        
        return TreeLeafCount(node->left) + TreeLeafCount(node->right);
    }

    int leafCount() const {
        return TreeLeafCount(root);
    }

    void insert(const vector<int> &point) {
        if (root == nullptr) {
            root = new kDTreeNode(point);
            count++;
            return;
        }

        kDTreeNode *current = root;
        kDTreeNode *parent = nullptr;
        int depth = 0;

        while (current != nullptr) {
            parent = current;
            if (point[depth % k] < current->data[depth % k]) {
                current = current->left;
            } else {
                current = current->right;
            }
            depth++;
        }

        if (point[depth % k] < parent->data[depth % k]) {
            parent->left = new kDTreeNode(point);
        } else {
            parent->right = new kDTreeNode(point);
        }
        count++;
    }

    void remove(const vector<int> &point);

    bool search(const vector<int> &point);

    void buildTree(const vector<vector<int>> &pointList);

    void nearestNeighbour(const vector<int> &target, kDTreeNode *best);

    void kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList);
};

class kNN
{
private:
    int k;
    Dataset *X_train;
    Dataset *y_train;
    int numClasses;

public:
    kNN(int k = 5);
    void fit(Dataset &X_train, Dataset &y_train);
    Dataset predict(Dataset &X_test);
    double score(const Dataset &y_test, const Dataset &y_pred);
};

// Please add more or modify as needed
