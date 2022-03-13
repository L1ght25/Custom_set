#include <stack>
#include <cstddef>


template<class ValueType>
class Set {
public:

    size_t size_of_tree = 0;

    inline long long max(long long a, long long b){
        return (a > b) ? a : b;
    }

    struct node{
        ValueType key;
        long long height;
        node* left;
        node* right;
        node *parent;
    };

    node *new_node(ValueType key, node *parent, long long height = 1, node *left = nullptr, node *right = nullptr){
        node *vertex = new node{key, height, left, right, parent};
        return vertex;
    }

    long long height_of_node(node *vertex){
        if(vertex != nullptr)
            return vertex->height;
        return 0;
    }

    long long balance_factor(node* vertex){
        return height_of_node(vertex->right) - height_of_node(vertex->left);
    }

    void fix_heights(node* vertex){
        if(vertex != nullptr)
            vertex->height = max(height_of_node(vertex->left), height_of_node(vertex->right)) + 1;
    }

    node *rotate_right(node *vertex){
        node *parent = vertex->parent;
        node *left_child = vertex->left;

        vertex->parent = left_child;
        left_child->parent = parent;
        if(left_child->right != nullptr)
            left_child->right->parent = vertex;

        vertex->left = left_child->right;
        left_child->right = vertex;
        fix_heights(vertex);
        fix_heights(left_child);
        return left_child;
    }

    node *rotate_left(node *vertex){
        node *parent = vertex->parent;
        node *right_child = vertex->right;

        vertex->parent = right_child;
        right_child->parent = parent;
        if(right_child->left != nullptr)
            right_child->left->parent = vertex;

        vertex->right = right_child->left;
        right_child->left = vertex;
        fix_heights(vertex);
        fix_heights(right_child);
        return right_child;
    }

    node *balance_tree(node *vertex){
        if(vertex == nullptr)
            return nullptr;
        fix_heights(vertex);
        if(balance_factor(vertex) == 2){
            if(balance_factor(vertex->right) < 0)
                vertex->right = rotate_right(vertex->right);
            return rotate_left(vertex);
        }
        if(balance_factor(vertex) == -2){
            if(balance_factor(vertex->left) > 0)
                vertex->left = rotate_left(vertex->left);
            return rotate_right(vertex);
        }
        return vertex;
    }

    node *insert_(node *vertex, ValueType key, node *parent){
        if(vertex == nullptr) {
            size_of_tree++;
            return new_node(key, parent);
        }
        if(key < vertex->key)
            vertex->left = insert_(vertex->left, key, vertex);
        else if(vertex->key < key)
            vertex->right = insert_(vertex->right, key, vertex);
        return balance_tree(vertex);
    }

    node *find_min_value_node(node *vertex) const{
        node *current_node = vertex;
        if(current_node == nullptr)
            return current_node;
        while (current_node->left != nullptr)
            current_node = current_node->left;
        return current_node;
    }

    node *find_max_value_node(node *vertex) const{
        node *current_node = vertex;
        if(current_node == nullptr)
            return current_node;
        while (current_node->right != nullptr)
            current_node = current_node->right;
        return current_node;
    }

    node *remove_min_value_node(node *vertex){
        if(vertex == nullptr)
            return nullptr;
        if(vertex->left == nullptr)
            return vertex->right;
        vertex->left = remove_min_value_node(vertex->left);
        if(vertex->left != nullptr)
            vertex->left->parent = vertex;
        return balance_tree(vertex);
    }

    node *remove_node(node *vertex, ValueType key, node *parent){
        if(vertex == nullptr)
            return nullptr;
        if(key < vertex->key) {
            vertex->left = remove_node(vertex->left, key, vertex);
            if(vertex->left != nullptr)
                vertex->left->parent  = vertex;
        }
        else if(vertex->key < key){
            vertex->right = remove_node(vertex->right, key, vertex);
            if(vertex->right != nullptr)
                vertex->right->parent = vertex;
        }
        else{
            size_of_tree--;
            node *left_child = vertex->left;
            node *right_child = vertex->right;
            node *parent_of_v = parent;
            delete vertex;
            if(right_child == nullptr) {
                if(left_child != nullptr)
                    left_child->parent = parent_of_v;
                return left_child;
            }
            node *min_node = find_min_value_node(right_child);
            min_node->parent = parent_of_v;
            min_node->right = remove_min_value_node(right_child);
            min_node->left = left_child;
            if(min_node->right != nullptr)
                min_node->right->parent = min_node;
            if(min_node->left != nullptr)
                min_node->left->parent = min_node;
            return balance_tree(min_node);
        }
        return balance_tree(vertex);
    }

    void destruct(node *&vertex){
        if(vertex != nullptr){
            if(vertex->right)
                destruct(vertex->right);
            if(vertex->left)
                destruct(vertex->left);
            delete vertex;
            vertex = nullptr;
        }
        size_of_tree = 0;
    }

    node *find_(node *vertex, ValueType key) const{
        if(vertex == nullptr)
            return vertex;
        if(key < vertex->key)
            return find_(vertex->left, key);
        if(vertex->key < key)
            return find_(vertex->right, key);
        return vertex;
    }

    static node *next_node(node *vertex){
        if(vertex == nullptr)
            return nullptr;
        if(vertex->right != nullptr){
            vertex = vertex->right;
            while(vertex->left != nullptr)
                vertex = vertex->left;
        } else{
            node *parent = vertex->parent;
            if(parent == nullptr)
                return nullptr;
            while(parent->right == vertex){
                if(parent->parent == nullptr)
                    return nullptr;
                vertex = parent;
                parent = parent->parent;
            }
            vertex = parent;
        }
        return vertex;
    }

    static node *prev_node(node *vertex){
        if(vertex->left != nullptr){
            node *node_ = vertex->left;
            while(node_->right != nullptr)
                node_ = node_->right;
            vertex = node_;
        } else{
            node *parent = vertex->parent;
            if(parent == nullptr)
                return nullptr;
            while(parent->left == vertex){
                if(parent->parent == nullptr)
                    return nullptr;
                vertex = parent;
                parent = parent->parent;
            }
            vertex = parent;
        }
        return vertex;
    }

    node *AVL_tree = nullptr;



    Set(){
        AVL_tree = nullptr;
        size_of_tree = 0;
    }

    template<typename Iterator>
    Set(Iterator first, Iterator second){
        AVL_tree = nullptr;
        size_of_tree = 0;
        for(auto it = first; it != second; ++it){
            AVL_tree = insert_(AVL_tree, *it, nullptr);
        }
    }

    Set(std::initializer_list<ValueType> elements){
        AVL_tree = nullptr;
        size_of_tree = 0;
        for(auto value : elements){
            AVL_tree = insert_(AVL_tree, value, nullptr);
        }
    }

    Set(const Set<ValueType> &another_tree){
        destruct(AVL_tree);
        for(ValueType & it : another_tree){
            AVL_tree = insert_(AVL_tree, it, nullptr);
        }
    }

    bool operator!=(const Set<ValueType>& another_tree) const{
        return (AVL_tree != another_tree.AVL_tree);
    }


    Set<ValueType> &operator=(const Set<ValueType>& another_tree){
        if(this != &another_tree){
            destruct(AVL_tree);
            for(ValueType & value : another_tree){
                AVL_tree = insert_(AVL_tree, value, nullptr);
            }
        }
        return *this;
    }

    ~Set() {
        destruct(AVL_tree);
    }

    size_t size() const{
        return size_of_tree;
    }

    bool empty() const {
        return (size_of_tree == 0);
    }

    void insert(ValueType key){
        AVL_tree = insert_(AVL_tree, key, nullptr);
    }

    void erase(ValueType key){
        AVL_tree = remove_node(AVL_tree, key, nullptr);
    }

    struct iterator{

        node *current_node;
        node *max_value_node;

        iterator(){
            current_node = nullptr;
            max_value_node = nullptr;
        }

        iterator(node *root, node *max_val_node){
            current_node = root;
            max_value_node = max_val_node;
        }

        iterator& operator++(){
            if(current_node == nullptr){
                this->current_node = nullptr;
            }
            this->current_node = next_node(current_node);
            return *this;
        }

        const iterator operator++(int){
            iterator temp = *this;
            ++*this;
            return temp;
        }

        iterator& operator--(){
            if(current_node == nullptr)
                this->current_node = max_value_node;
            else
                this->current_node = prev_node(current_node);
            return *this;
        }

        const iterator operator--(int){
            iterator temp = *this;
            --*this;
            return temp;
        }

        iterator &operator=(const iterator& another_iterator){
            if(this != &another_iterator){
                this->current_node = another_iterator.current_node;
                this->max_value_node = another_iterator.max_value_node;
            }
            return *this;
        }

        ValueType & operator*() const{
            return current_node->key;
        }

        ValueType* operator->() const{
            return &current_node->key;
        }

        bool operator==(iterator const & another_iterator) const{
            return this->current_node == another_iterator.current_node;
        }

        bool operator!=(iterator const & another_iterator) const{
            return this->current_node != another_iterator.current_node;
        }

    };

    iterator begin() const{
        return iterator(find_min_value_node(AVL_tree), find_max_value_node(AVL_tree));
    }

    iterator end() const{
        return iterator(nullptr, find_max_value_node(AVL_tree));
    }

    iterator find(const ValueType& element) const{
        return iterator(find_(AVL_tree, element), find_max_value_node(AVL_tree));
    }

    iterator lower_bound(const ValueType& element) const{
        node *result = nullptr;
        node *temp = AVL_tree;
        while (temp != nullptr){
            if(temp->key < element)
                temp = temp->right;
            else{
                result = temp;
                temp = temp->left;
            }
        }
        return iterator((result), find_max_value_node(AVL_tree));
    }
};
