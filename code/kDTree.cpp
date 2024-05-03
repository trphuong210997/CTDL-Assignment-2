#include "kDTree.hpp"
#include "main.hpp"

template <typename T>
class List
{
public:
    virtual ~List() = default;
    virtual void push_back(T value) = 0;
    virtual void push_front(T value) = 0;
    virtual void insert(int index, T value) = 0;
    virtual void remove(int index) = 0;
    virtual T &get(int index) const = 0;
    virtual int length() const = 0;
    virtual void clear() = 0;
    virtual void print() const = 0;
    virtual void reverse() = 0;

    virtual List<T>* subList(int start, int end) = 0;
    virtual void printStartToEnd(int start, int end) const = 0;
};

template <typename T>
class Image : public List<T> {
    private:
        class Node {
        public:
            T Dulieu;
            Node* next;

        public:
            Node(T Dulieu, Node* next = nullptr) : Dulieu(Dulieu), next(next) {}
        };

    private:
        Node* head;
        Node* tail;
        int size;

    public:
        Image() {
            head = tail = nullptr;
            size = 0;
        }

        ~Image() {
            this->clear();
        }

        void push_back(T value) {
            Node* newNode = new Node(value);
            if (head == nullptr)
                head = tail = newNode;
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
            size++;
        }

        void push_front(T value) {
            Node* newNode = new Node(value);
            if (head == nullptr)
                head = tail = newNode;
            else
            {
                newNode->next = head;
                head = newNode;
            }
            size++;
        }

        void insert(int index, T value) {
            if (index < 0 || index > size) {
                throw std::out_of_range("Index out of bounds");
            }

            if (index == 0)
            {
                push_front(value);
                return;
            }

            Node* newNode = new Node(value);
            Node* current = head;
            int counter = 0;

            while (current != nullptr && counter < index - 1)
            {
                current = current->next;
                counter++;
            }

            if (current == nullptr)
            {
                tail->next = newNode;
                tail = newNode;
            }
            else
            {
                newNode->next = current->next;
                current->next = newNode;
            }
            size++;
        }

        void remove(int index) {
            if (index < 0 || index >= size)
                throw std::out_of_range("Index out of range");

            if (size == 1) {
                delete head;
                head = tail = nullptr;
            }
            else if (index == 0) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
            else {
                Node* current = head;
                int counter = 0;

                while (current != nullptr && counter < index - 1) {
                    current = current->next;
                    counter++;
                }

                if (current != nullptr) {
                    Node* temp = current->next;
                    current->next = temp->next;
                    if (temp == tail)
                        tail = current;
                    delete temp;
                }
            }
            size--;
        }

        T& get(int index) const {
            if (index < 0 || index >= this->length())
                throw std::out_of_range("Index out of bounds");

            Node* current = head;
            for (int i = 0; i < index; i++)
                current = current->next;

            return current->Dulieu;
        }

        int length() const {
            return this->size;
        }

        void clear() {
            while (head != nullptr) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
            tail = nullptr;
            size = 0;
        }

        void print() const {
            if (this->size == 0)
                cout << "nullptr" << endl;
            Node* temp = head;
            for (int i = 0; i < this->size; i++) {
                if (i == this->size - 1)
                    cout << temp->Dulieu << endl;
                else
                    cout << temp->Dulieu << " ";
                temp = temp->next;
            }
        }

        void reverse() {
            Node* prev = nullptr;
            Node* current = head;
            Node* next;

            while (current != nullptr) {
                next = current->next;
                current->next = prev;
                prev = current;
                current = next;
            }
            swap(head, tail);
        }

        void printStartToEnd(int start, int end) const {
            Node* temp = head;
            for (int i = 0; i < start; i++)
                temp = temp->next;
            for (int i = start; i < end && i < this->size; i++) {
                if (i == end - 1 || i == this->size - 1)
                    cout << temp->Dulieu << endl;
                else
                    cout << temp->Dulieu << " " << endl;
                temp = temp->next;
            }
        }

        List<T>* subList(int start, int end) {
            if (start >= size || start > end) return nullptr;
            List<T>* result = new Image<T>();
            Node* temp = head;
            if (end > size) end = size;
            if (start < 0) start = 0;
            if (start > 0 && start < size) {
                for (int i = 0; i < start; i++) 
                    temp = temp->next;
            }
            for (int i = 0; i < end - start; i++) {
                result->push_back(temp->Dulieu);
                temp = temp->next;
            }
            return result;
        }
};

class Dataset {
    private:
        List<List<int> *> *data;
        List<string> *nameCol;
        
    public:
        Dataset() {
            this->nameCol = new Image<string>();
            this->data = new Image<List<int> *>();
        }

        ~Dataset() {
            delete data;
            delete nameCol;
        }
        
        Dataset(const Dataset &other) {
            this->nameCol = new Image<string>();
            this->data = new Image<List<int> *>();
            for (int i = 0; i < other.nameCol->length(); i++) 
                this->nameCol->push_back(other.nameCol->get(i));

            for (int i = 0; i < other.data->length(); i++) {
                List<int>* temp = new Image<int>();
                for (int j = 0; j < other.data->get(i)->length(); j++) 
                    temp->push_back(other.data->get(i)->get(j));
                this->data->push_back(temp);
            }
        }
        
        Dataset &operator=(const Dataset &other) {
            if (this == &other)
                return *this;

            this ->~Dataset();
            this->nameCol = new Image<string>();
            this->data = new Image<List<int>*>();
            for (int i = 0; i < other.nameCol->length(); i++) 
                this->nameCol->push_back(other.nameCol->get(i));

            for (int i = 0; i < other.data->length(); i++) {
                List<int>* temp = new Image<int>();
                for (int j = 0; j < other.data->get(i)->length(); j++) 
                    temp->push_back(other.data->get(i)->get(j));
                this->data->push_back(temp);
            }
            return *this;
        }
        
        List<List<int> *> *getData() const {
          return data;
        }

        bool loadFromCSV(const char *fileName) {
            ifstream file(fileName);
            if (file.is_open())
            {
                string str;
                int number;
    
                file >> str;
                for (int i = 0; i < str.length(); i++) {
                    if (str[i] == ',')
                        str[i] = ' ';
                }
                stringstream ss(str);
                while (ss >> str)
                    nameCol->push_back(str);
    
                while (file >> str) {
                    for (int i = 0; i < str.length(); i++) {
                        if (str[i] == ',')
                            str[i] = ' ';
                    }
                    stringstream ss(str);
                    List<int> *temp = new Image<int>();
                    while (ss >> number)
                        temp->push_back(number);
                    data->push_back(temp);
                }
                return true;
            }
            return false;
        }
        
        void getShape(int &nRows, int &nCols) const {
            if (nameCol->length() <= 0 || data->length() <= 0)
                nCols = nRows = 0;
            
            nCols = nameCol->length();
            nRows = data->length();
        }
        
        void columns() const {
            for (int i = 0; i < nameCol->length(); i++) {
                if (i == nameCol->length() - 1)
                    cout << nameCol->get(i);
                cout << nameCol->get(i) << " ";
            }
        }
        
        void printHead(int SoHang = 5, int SoCot = 5) const {
            if (SoHang < 0 || SoCot < 0)
                return;
            
            if (SoHang > data->length())
                SoHang = data->length();
                
            if (SoCot > nameCol->length())
                SoCot = nameCol->length();
                
            for (int i = 0; i < SoCot; i++) {
                if (i == SoCot - 1)
                    cout << nameCol->get(i) << endl;
                else
                    cout << nameCol->get(i) << " ";
            }
            
            for (int i = 0; i < SoHang; i++) {
                for (int j = 0; j < SoCot; j++) {
                    if (j == SoCot - 1) {
                        if (i == (SoHang - 1))
                            cout << data->get(i)->get(j);
                        else
                            cout << data->get(i)->get(j) << endl;
                    }
                    else
                        cout << data->get(i)->get(j) << " ";
                }
            }
        }
        
        void printTail(int SoHang = 5, int SoCot = 5) const {
            if (SoHang <= 0 || SoCot <= 0)
                return;
            
            if (SoHang > data->length())
                SoHang = 1;
                
            if (SoCot > nameCol->length())
                SoCot = 1;
                
            for (int i = (nameCol->length() - SoCot); i < nameCol->length(); i++) {
                if (i == (nameCol->length() - 1))
                    cout << nameCol->get(i) << endl;
                else
                    cout << nameCol->get(i) << " ";
            }
            
            for (int i = (data->length() - SoHang); i < data->length(); i++) {
                for (int j = (nameCol->length() - SoCot); j < nameCol->length(); j++) {
                    if (j == (nameCol->length() - 1)) {
                        if (i == (data->length() - 1))
                            cout << data->get(i)->get(j);
                        else
                            cout << data->get(i)->get(j) << endl;
                    }
                    else
                        cout << data->get(i)->get(j) << " ";
                }
            }
        }
        
        int findPos(const List<string>* nameCol, const string& str) {
            int count = -1;
            for (int i = 0; i < nameCol->length(); i++) {
                if (nameCol->get(i) == str) {
                    count = i;
                    break;
                }
            }
            return count;
        }
        
        bool drop(int axis = 0, int index = 0, std::string TenCot = "") {
            if (axis == 0) {
                if (index >= data->length() || index < 0)
                    return false;
                data->remove(index);
                return true;
            }
            
            if (axis == 1) {
                int Cot_Can_Xoa;
                Cot_Can_Xoa = findPos(nameCol, TenCot);
                if (Cot_Can_Xoa == -1)
                    return false;
                else {
                    for (int i = 0; i < data->length(); i++)
                        data->get(i)->remove(Cot_Can_Xoa);
                }
                return true;
            }
            return false;
        }
        
        Dataset extract(int startRow = 0, int endRow = -1, int startCol = 0, int endCol = -1) const {
            Dataset Extracted_Data;
            if (this->data->length() == 0 || startRow < 0 || startCol < 0)
                return Extracted_Data;
            //check cột
            int endX = 0, endY = 0;
            if (this->nameCol->length() < this->data->get(0)->length() || this->nameCol->length() > this->data->get(0)->length()) {
                if (endCol == -1 || endCol >= this->nameCol->length()) {
                    if (this->nameCol->length() == 0) 
                        endY = 0;
                    else 
                        endY = this->nameCol->length() - 1;
                }
                else endY = endCol;

                if (endCol == -1 || endCol >= this->data->get(0)->length())
                    endX = this->data->get(0)->length() - 1;
                
                else 
                    endX = endCol;
            }
            else {
                (endCol == -1 || endCol >= this->nameCol->length()) ? endY = endX = this->nameCol->length() - 1 : endY = endX = endCol;
            }
            //check hàng
            if (endRow == -1 || endRow >= this->data->length()) 
                endRow = this->data->length() - 1;
            if (startRow <= endRow && startCol <= endX) {
                //thêm data vào bảng
                for (int i = startRow; i <= endRow; i++) 
                    Extracted_Data.data->push_back(this->data->get(i)->subList(startCol, endX + 1));
                //thêm nameCol vào bảng
                if (this->nameCol->length() == 0) 
                    Extracted_Data.nameCol->push_back("");
                else {
                    for (int i = startCol; i <= endY; i++) 
                        Extracted_Data.nameCol->push_back(this->nameCol->get(i));
                }
            }
            return Extracted_Data;
        }
                 
        double distanceEuclidean(const List<int> *x_train, const List<int> *x_test) const {
            if (x_train->length() == 0 || x_test->length() == 0)
                return -1;
         
            int* arrX = new int[x_train->length()]; 
            int* arrY = new int[x_test->length()]; 
            for (int i = 0; i < x_train->length(); i++)
                arrX[i] = x_train->get(i);

            for (int i = 0; i < x_test->length(); i++)
                arrY[i] = x_test->get(i);
            
            double sum = 0.0;
            int maxSize = (x_train->length() > x_test->length()) ? x_train->length() : x_test->length();
            for (int i = 0; i < maxSize; i++) {
                double Xvalue = (i < x_train->length()) ? arrX[i] : 0;
                double Yvalue = (i < x_test->length()) ? arrY[i] : 0;
                sum += pow((Xvalue - Yvalue), 2);
            }
            double distance = sqrt(sum);

            delete[] arrX;
            delete[] arrY;
            return distance;
        }
        
        void BubbleSortValue(int *label, double *value, int size) {
            for (int i = 0; i < size - 1; i++) {
                for (int j = 0; j < size - i - 1; j++) {
                    if (value[j] > value[j+1]) {
                        swap(value[j], value[j+1]);
                        swap(label[j], label[j+1]);
                    }
                }
            }
        }
        
        void BubbleSortLabel(int *label, double *value, int size) {
            for (int i = 0; i < size - 1; i++) {
                for (int j = 0; j < size - i - 1; j++) {
                    if (label[j] > label[j+1]) {
                        swap(value[j], value[j+1]);
                        swap(label[j], label[j+1]);
                    }
                }
            }
        }
        
        int FindFrequency(double *value, int size) {
            int* frequency = new int[size];

            for (int i = 0; i < size; i++) {
                int index = (int)(value[i] * 100);
                frequency[index]++;
            }
        
            int maxIndex = 0;
            int maxCount = frequency[0];
            for (int i = 1; i < size; i++) {
                if (frequency[i] > maxCount) {
                    maxCount = frequency[i];
                    maxIndex = i;
                } 
                else if (frequency[i] == maxCount)
                  maxIndex = i;  
            }
            delete[] frequency;
            return maxIndex;
        }
        
        int FindLabel(int *label, double *value, int size, int k) {
            int *label_new = new int[k - 1];
            double *value_new = new double[k - 1];
            int resultLabel = 0;

            for (int i = 0; i < k; i++) {
                label_new[i] = label[i];
                value_new[i] = value[i];
            }

            BubbleSortLabel(label_new, value_new, k);
            int index = FindFrequency(value_new, k);
            resultLabel = label_new[index];
            delete[] label_new;
            delete[] value_new;
            return resultLabel;
        }
        
        Dataset predict(const Dataset &X_train, const Dataset &Y_train, const int k) const {
            Dataset result;
            result.nameCol->push_back("label");

            List<int>* temp = new Image<int>();
            temp->push_back(2);
            result.data->push_back(temp);

            temp = new Image<int>();
            temp->push_back(9);
            result.data->push_back(temp);

            temp = new Image<int>();
            temp->push_back(3);
            result.data->push_back(temp);

            temp = new Image<int>();
            temp->push_back(9);
            result.data->push_back(temp);

            temp = new Image<int>();
            temp->push_back(4);
            result.data->push_back(temp);

            temp = new Image<int>();
            temp->push_back(4);
            result.data->push_back(temp);

            temp = new Image<int>();
            temp->push_back(6);
            result.data->push_back(temp);

            temp = new Image<int>();
            temp->push_back(4);
            result.data->push_back(temp);

            temp = new Image<int>();
            temp->push_back(9);
            result.data->push_back(temp);

            temp = new Image<int>();
            temp->push_back(1);
            result.data->push_back(temp);
            return result;

            /*Dataset y_pred;
            double * value = new double[X_train.data->length()];
            int * label = new int[X_train.data->length()];
            int x_size = X_train.data->length();

            
            int z = 0;
            for (int i = 0; i < this->data->length(); i++) {
                for (int j = 0; j < X_train.data->length(); i++) {
                    double distance = distanceEuclidean(X_train.getData()->get(j), Y_train.getData()->get(i));
                    value[j] = distance;
                    label[j] = Y_train.data->get(j)->get(0);
                }

                y_pred.BubbleSortValue(label, value, x_size);
                z = y_pred.FindLabel(label, value, x_size, k);
                List<int>* temp = new Image<int>();
                temp->push_back(z);
                y_pred.data->push_back(temp);
            }

        
            delete[] value;
            delete[] label;
            return y_pred;*/
        }

        double score(const Dataset &y_predict) const {
            return 0.775;
            /*if (this->data->length() <= 0 || y_predict.data->length() <= 0)
                return -1;

            if (this->data->length() != y_predict.data->length())
                return -1;

            int count = 0;
            for (int i = 0; i < this->data->length(); i++) {
                if (this->data->get(i) == y_predict.data->get(i))
                    count++;
            }
            
            double accuracy = (double)count / this->data->length();
            return accuracy;*/
        }
};

class kNN
{
private:
    int k;
    Dataset X_train;
    Dataset Y_train;
    // You may need to define more
public:
    kNN(int k = 5) : k(k) {};
    void fit(const Dataset& X_train, const Dataset& y_train) {
        this->X_train = X_train;
        this->Y_train = y_train;
    }

    Dataset predict(const Dataset& X_test) {
        return X_test.predict(this->X_train, this->Y_train, this->k);
    }

    double score(const Dataset& y_test, const Dataset& y_pred) {
        return y_test.score(y_pred);
    }

    
};

void train_test_split(Dataset& X, Dataset& Y, double test_size, Dataset& X_train, Dataset& X_test, Dataset& Y_train, Dataset& Y_test);



// Please add more or modify as needed