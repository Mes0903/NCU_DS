#include <iostream>
#include <memory>

template <typename T>
class BST {
    struct Node {
        std::weak_ptr<Node> _parent;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        T key;

        std::shared_ptr<Node> parent()
        {
            if (auto spt = _parent.lock(); spt != nullptr)
                return spt;
        }
        Node(const T &_k, std::shared_ptr<Node> _p = nullptr)
            : key(_k), _parent(_p) {}
        Node()
            : key(), _parent(nullptr), left(nullptr), right(nullptr) {}
    };

    std::shared_ptr<Node> root;

    auto _min(std::shared_ptr<Node> &_node) const -> decltype(root);    // 找最小值
    auto _max(std::shared_ptr<Node> &_node) const -> decltype(root);    // 找最大值
    auto _next(std::shared_ptr<Node> &_node) const -> decltype(root);    // 找下一個值
    auto _prev(std::shared_ptr<Node> &_node) const -> decltype(root);    // 找前一個值

    std::ostream &_inorder(std::shared_ptr<Node> &_node, std::ostream &);
    std::ostream &_preorder(std::shared_ptr<Node> &_node, std::ostream &);
    std::ostream &_postorder(std::shared_ptr<Node> &_node, std::ostream &);

    size_t _size(std::shared_ptr<Node> &_node) const;

  public:
    BST()
        : root(nullptr){};
    ~BST() { root = nullptr; };
    BST(const BST &other) = delete;
    BST(BST &&other) = delete;
    BST &operator=(const BST &other) = delete;
    BST &operator=(BST &&other) = delete;

    size_t size() const { return _size(root); };
    bool isempty() { return (_size(root) == 0) ? true : false; }

    bool find(const T &k) const;
    void insert(const T &k);
    void remove(const T &k);

    std::ostream &inorder();
    std::ostream &preorder();
    std::ostream &postorder();
};

template <typename T>
std::ostream &BST<T>::preorder()
{
    if (isempty()) {
        throw std::runtime_error("Missing element!\n");
    }
    return _preorder(root, std::cout);
}

template <typename T>
std::ostream &BST<T>::postorder()
{
    if (isempty()) {
        throw std::runtime_error("Missing element!\n");
    }
    return _postorder(root, std::cout);
}

template <typename T>
std::ostream &BST<T>::inorder()
{
    if (isempty()) {
        throw std::runtime_error("Missing element!\n");
    }
    return _inorder(root, std::cout);
}

template <typename T>
std::ostream &BST<T>::_preorder(std::shared_ptr<Node> &i, std::ostream &o)
{
    o << i->key << " ";
    if (i->left != nullptr) {
        _preorder(i->left, o);
    }
    if (i->right != nullptr) {
        _preorder(i->right, o);
    }
    return o;
}

template <typename T>
std::ostream &BST<T>::_postorder(std::shared_ptr<Node> &i, std::ostream &o)
{
    if (i->left != nullptr) {
        _postorder(i->left, o);
    }
    if (i->right != nullptr) {
        _postorder(i->right, o);
    }
    o << i->key << " ";
    return o;
}

template <typename T>
std::ostream &BST<T>::_inorder(std::shared_ptr<Node> &i, std::ostream &o)
{
    if (i->left != nullptr) {
        _inorder(i->left, o);
    }
    o << i->key << " ";
    if (i->right != nullptr) {
        _inorder(i->right, o);
    }
    return o;
}

template <typename T>
auto BST<T>::_min(std::shared_ptr<Node> &_node) const -> decltype(root)
{
    auto tmp = _node;
    while (tmp->left != nullptr)
        tmp = tmp->left;

    return tmp;
}

template <typename T>
auto BST<T>::_max(std::shared_ptr<Node> &_node) const -> decltype(root)
{
    auto tmp = _node;
    while (tmp->right != nullptr)
        tmp = tmp->right;

    return tmp;
}

template <typename T>
auto BST<T>::_next(std::shared_ptr<Node> &_node) const -> decltype(root)
{
    if (_node->right != nullptr)
        return _min(_node->right);

    auto tmp = _node->parent();
    while (tmp->parent() != nullptr) {
        if (tmp->parent()->left == tmp)
            return tmp->parent();

        tmp = tmp->parent();
    }

    return nullptr;
}

template <typename T>
auto BST<T>::_prev(std::shared_ptr<Node> &_node) const -> decltype(root)
{
    if (_node->left != nullptr)
        return _max(_node->left);

    auto tmp = _node->parent();
    while (tmp->parent() != nullptr) {
        if (tmp->parent()->right == tmp)
            return tmp->parent();

        tmp = tmp->parent();
    }

    return nullptr;
}

template <typename T>
size_t BST<T>::_size(std::shared_ptr<Node> &_node) const
{
    if (_node == nullptr)
        return 0;
    else
        return _size(_node->left) + _size(_node->right) + 1;
}

template <typename T>
bool BST<T>::find(const T &k) const
{
    auto tmp = root;
    while (tmp != nullptr) {
        if (k < tmp->key)
            tmp = tmp->left;
        else if (k > tmp->key)
            tmp = tmp->right;
        else
            return true;
    }

    return false;
}

template <typename T>
void BST<T>::insert(const T &k)
{
    if (root == nullptr) {
        root = std::make_shared<Node>(k);
        return;
    }

    auto tmp = root;    // tmp 為 parent
    while (tmp != nullptr) {
        if (k < tmp->key) {
            if (tmp->left != nullptr)
                tmp = tmp->left;
            else {
                tmp->left = std::make_shared<Node>(k, tmp);
                return;
            }
        }
        else if (k > tmp->key) {
            if (tmp->right != nullptr)
                tmp = tmp->right;
            else {
                tmp->right = std::make_shared<Node>(k, tmp);
                return;
            }
        }
        else
            return;
    }
}

/*
template <typename T>
void BST<T>::remove(const T &k)
{
    auto tmp = root;
    while (tmp != nullptr) {
        if (k < tmp->key)
            tmp = tmp->left;
        else if (k > tmp->key)
            tmp = tmp->right;
        else
            break;
    }

    if (tmp == nullptr)
        return;

    if (tmp->left == nullptr && tmp->right == nullptr)
        if (tmp->parent()->right == tmp)
            tmp->parent()->right.reset();
        else
            tmp->parent()->left.reset();
    else if (tmp->right != nullptr && tmp->left != nullptr)
        tmp = _next(tmp);
    else if (tmp->right != nullptr)
        if (tmp->parent()->right == tmp)
            tmp->parent()->right = std::move(tmp->left);
        else
            tmp->parent()->left = std::move(tmp->left);
    else if (tmp->left != nullptr)
        if (tmp->parent()->right == tmp)
            tmp->parent()->right = std::move(tmp->right);
        else
            tmp->parent()->left = std::move(tmp->right);
}
*/

int main()
{
    BST<int> tree;
    int cnt{}, num, target;
    std::cin >> cnt;

    for (int i{}; i < cnt; std::cin >> num, ++cnt)
        tree.insert(num);

    std::cin >> target;
    if (tree.find(target))
        puts("There exit at least one path in binary search tree.");
    else
        puts("There have no path in binary search tree.");

    return 0;
}