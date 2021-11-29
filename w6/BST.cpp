#include <iostream>
#include <memory>

template <typename T>
class BST {
    struct Node {
        std::weak_ptr<Node> _parent;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        T key;

        std::shared_ptr<Node> parent() {
            if ( auto spt = _parent.lock(); spt != nullptr )
                return spt;
        }
        Node( const T &_k, std::shared_ptr<Node> _p = nullptr ) : key( _k ), _parent( _p ) {}
        Node() : key(), _parent( nullptr ), left( nullptr ), right( nullptr ) {}
    };

    std::shared_ptr<Node> root;

    auto _min( std::shared_ptr<Node> &_node ) const;    // 找最小值
    auto _max( std::shared_ptr<Node> &_node ) const;    // 找最大值
    auto _next( std::shared_ptr<Node> &_node ) const;    // 找下一個值
    auto _prev( std::shared_ptr<Node> &_node ) const;    // 找前一個值

    std::ostream &_inorder( std::shared_ptr<Node> &_node, std::ostream & ) const;
    std::ostream &_preorder( std::shared_ptr<Node> &_node, std::ostream & ) const;
    std::ostream &_postorder( std::shared_ptr<Node> &_node, std::ostream & ) const;

    size_t _size( std::shared_ptr<Node> &_node ) const;

  public:
    BST() : root( nullptr ){};
    ~BST() { root = nullptr; };
    BST( const BST &other ) = delete;
    BST( BST &&other ) = delete;
    BST &operator=( const BST &other ) = delete;
    BST &operator=( BST &&other ) = delete;

    size_t size() const { return _size( root ); };
    bool isempty() { return ( _size( root ) == 0 ) ? true : false; }

    bool find( const T &k ) const;
    void insert( const T &k );
    void remove( const T &k );

    std::ostream &inorder() const;
    std::ostream &preorder() const;
    std::ostream &postorder() const;
};

template <typename T>
auto BST<T>::_min( std::shared_ptr<Node> &_node ) const {
    auto tmp = _node;
    while ( tmp->left != nullptr )
        tmp = tmp->left;

    return tmp;
}

template <typename T>
auto BST<T>::_max( std::shared_ptr<Node> &_node ) const {
    auto tmp = _node;
    while ( tmp->right != nullptr )
        tmp = tmp->right;

    return tmp;
}

template <typename T>
auto BST<T>::_next( std::shared_ptr<Node> &_node ) const {
    if ( _node->right != nullptr )
        return _min( _node->right );

    auto tmp = _node->parent();
    while ( tmp->parent() != nullptr ) {

        if ( tmp->parent()->left == tmp )
            return tmp->parent();

        tmp = tmp->parent();
    }

    return nullptr;
}

template <typename T>
auto BST<T>::_prev( std::shared_ptr<Node> &_node ) const {
    if ( _node->left != nullptr )
        return _max( _node->left );

    auto tmp = node->parent();
    while ( tmp->parent() != nullptr ) {

        if ( tmp->parent()->right == tmp )
            return tmp->parent();

        tmp = tmp->parent();
    }

    return nullptr;
}

template <typename T>
size_t BST<T>::_size( std::shared_ptr<Node> &_node ) const {
    if ( _node == nullptr )
        return 0;
    else
        return _size( _node->left ) + _size( _node->right ) + 1;
}

template <typename T>
bool BST<T>::find( const T &k ) const {
    auto tmp = root;
    while ( tmp != nullptr ) {
        if ( k < tmp->key )
            tmp = tmp->left;
        else if ( k > tmp->key )
            tmp = tmp->right;
        else
            return true;
    }

    return false;
}

template <typename T>
void BST<T>::insert( const T &k ) {
    if ( root == nullptr ) {
        root = std::make_shared<Node>( k );
        return;
    }

    auto tmp = root;    // tmp 為 parent
    while ( tmp != nullptr ) {
        if ( k < tmp->key ) {
            if ( tmp->left != nullptr )
                tmp = tmp->left;
            else {
                tmp->left = std::make_shared<Node>( k, tmp );
                return;
            }
        }
        else if ( k > tmp->key ) {
            if ( tmp->right != nullptr )
                tmp = tmp->right;
            else {
                tmp->right = std::make_shared<Node>( k, tmp );
                return;
            }
        }
        else
            return;
    }
}

template <typename T>
void BST<T>::remove( const T &k ) {
    if ( root == nullptr )
        return;

    auto tmp = root;
    while ( tmp != nullptr ) {
        if ( k < tmp->key ) {
            if ( tmp->left != nullptr )
                tmp = tmp->left;
        }
        else if ( k > tmp->key ) {
            if ( tmp->right != nullptr )
                tmp = tmp->right;
        }
        else
            break;
    }

    if ( tmp->right == nullptr && tmp->left == nullptr )
        tmp =
}

int main() {
}