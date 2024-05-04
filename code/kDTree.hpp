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

    kDTreeNode *copyData(kDTreeNode *node) {
        if (node == nullptr) 
            return nullptr;
        
        return new kDTreeNode(node->data, copyData(node->left), copyData(node->right));
    }

    const kDTree &operator=(const kDTree &other) {
        this->k = other.k;
        this->count = other.count;
        this->root = copyData(other.root);
        return *this;
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
        int treeLevel = 0;

        while (current != nullptr) {
            parent = current;

            if (point[treeLevel % k] < current->data[treeLevel % k])
                current = current->left;
            else
                current = current->right;

            treeLevel++;
        }

        if (point[treeLevel % k] < parent->data[treeLevel % k])
            parent->left = new kDTreeNode(point);
        else 
            parent->right = new kDTreeNode(point);
        
        count++;
    }

    bool searchHelper(kDTreeNode *node, const vector<int> &point, int treeLevel) {
        if (node == nullptr)
            return false;
        
        if (node->data == point)
            return true;

        if (point[treeLevel % k] < node->data[treeLevel % k])
            return searchHelper(node->left, point, treeLevel + 1);
        else
            return searchHelper(node->right, point, treeLevel + 1);
    }

    bool search(const vector<int> &point) {
        return searchHelper(root, point, 0);
    }

    kDTreeNode *findMin(kDTreeNode *node) {
        if (node == nullptr)
            return nullptr;
        
        while (node->left != nullptr)
            node = node->left;
        
        return node;
    }

    void removeHelper(kDTreeNode *node, const vector<int> &point, int treeLevel) {
        if (node == nullptr)
            return;
        
        if (search(point) == false)
            return;

        if (node->data == point) {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                count--;
                return;
            }

            if (node->left == nullptr) {
                kDTreeNode *temp = node;
                node = node->right;
                delete temp;
                count--;
                return;
            }

            if (node->right == nullptr) {
                kDTreeNode *temp = node;
                node = node->left;
                delete temp;
                count--;
                return;
            }

            kDTreeNode *minNode = findMin(node->right);
            node->data = minNode->data;
            removeHelper(node->right, minNode->data, treeLevel + 1);
        }

        if (point[treeLevel % k] < node->data[treeLevel % k])
            removeHelper(node->left, point, treeLevel + 1);
        else
            removeHelper(node->right, point, treeLevel + 1);
    }

    void remove(const vector<int> &point) {
        return removeHelper(root, point, 0);
    }
    

    kDTreeNode *buildTreeHelper(const vector<vector<int>> &pointList, int treeLevel) {
        if (pointList.empty())
            return nullptr;
        
        int axis = treeLevel % k;
        int median = pointList.size() / 2;

        sort(pointList.begin(), pointList.end(), [axis](const vector<int> &a, const vector<int> &b) {
            return a[axis] < b[axis];
        });

        kDTreeNode *node = new kDTreeNode(pointList[median]);
        node->left = buildTreeHelper(vector<vector<int>>(pointList.begin(), pointList.begin() + median), treeLevel + 1);
        node->right = buildTreeHelper(vector<vector<int>>(pointList.begin() + median + 1, pointList.end()), treeLevel + 1);

        return node;
    }

    void buildTree(const vector<vector<int>> &pointList) {
        this->root = buildTreeHelper(pointList, 0);
    }

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
