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

    kDTree(int k, int count, kDTreeNode *root) : k(k), count(count), root(root) {}

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

    kDTreeNode *findMin(kDTreeNode *node, int dimension) {
        if (node == nullptr)
            return nullptr;
        
        kDTreeNode *current = node;
        kDTreeNode *minNode = node;

        while (current != nullptr) {
            if (current->data[dimension] < minNode->data[dimension])
                minNode = current;
            
            current = current->left;
        }

        return minNode;
    }

    void removeHelper(kDTreeNode *node, const vector<int> &point, int treeLevel) {
        if (node == nullptr)
            return;
        
        if (search(point) == false)
            return;

        if (node->data == point) {
            int dimension = treeLevel % k;
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                count--;
                return;
            }

            if (node->right != nullptr) {
                kDTreeNode *minNode = findMin(node->right, dimension);
                node->data = minNode->data;
                return removeHelper(node->right, minNode->data, treeLevel + 1);
            }

            if (node->right == nullptr) {
                kDTreeNode *minNode = findMin(node->left, dimension);
                node->data = minNode->data;
                return removeHelper(node->left, minNode->data, treeLevel + 1);  
            }
        }

        if (point[treeLevel % k] < node->data[treeLevel % k])
            removeHelper(node->left, point, treeLevel + 1);
        else
            removeHelper(node->right, point, treeLevel + 1);
    }

    void remove(const vector<int> &point) {
        return removeHelper(root, point, 0);
    }

    void sort(vector<vector<int>> &pointList, int k, int dimension) {
        for (int i = 0; i < pointList.size(); i++) {
            for (int j = i + 1; j < pointList.size(); j++) {
                if (pointList[i][dimension] > pointList[j][dimension]) {
                    vector<int> temp = pointList[i];
                    pointList[i] = pointList[j];
                    pointList[j] = temp;
                }
            }
        }
    }

    int findFirstMedian (const vector<vector<int>> &pointList) {
        int ListMidpoint = pointList.size() / 2;
        for (int i = 0; i < ListMidpoint; i++) {
            if (pointList[i] == pointList[ListMidpoint])
                return i;
        }
    }
    
    kDTreeNode *buildTreeHelper(const vector<vector<int>> &pointList, int treeLevel) {
        vector<vector<int>> pointListCopy = pointList;
        if (pointList.size() == 0)
            return nullptr;
        
        int dimension = treeLevel % k;
        sort(pointListCopy, k, dimension);
        int ListMidpoint = findFirstMedian(pointListCopy);
        kDTreeNode *node = new kDTreeNode(pointListCopy[ListMidpoint]);

        node->left = buildTreeHelper(vector<vector<int>>(pointListCopy.begin(), pointListCopy.begin() + ListMidpoint), treeLevel + 1);
        node->right = buildTreeHelper(vector<vector<int>>(pointListCopy.begin() + ListMidpoint + 1, pointListCopy.end()), treeLevel + 1);

        return node;
    }

    void buildTree(const vector<vector<int>> &pointList) {
        this->root = buildTreeHelper(pointList, 0);
    }

    double calculateDistance(const vector<int> &point1, const vector<int> &point2) {
        double distance = 0;
        for (int i = 0; i < point1.size(); i++) {
            distance += pow(point1[i] - point2[i], 2);
        }
        return sqrt(distance);
    }

    // kDTreeNode *findBestNode(kDTreeNode *node, const vector<int> &target, int treeLevel) {
    //     if (node)
    //         return nullptr;
        
    //     if (target[treeLevel % k] < node->data[treeLevel % k]) 
    //         return findBestNode(node->left, target, treeLevel + 1);
    //     else
    //         return findBestNode(node->right, target, treeLevel + 1);
        
    //     return best;
    // }
    
    // void nearestNeighbourHelper(kDTreeNode *node, const vector<int> &target, kDTreeNode *best, int treeLevel) {
    //     kDTreeNode *bestNode = findBestNode(root, target, 0);
    // }

    // void nearestNeighbour(const vector<int> &target, kDTreeNode *best) {
    //     best = nullptr;
    //     return nearestNeighbourHelper(root, target, best, 0);
    // }

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
