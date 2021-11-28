#include <atomic>
#include <iostream>
#include <memory>

template <typename T>
class Binary_Tree {
    struct Node {
        std::weak_ptr<Node> _parent = nullptr;
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
        T key = {};

        std::shared_ptr<Node> parent() {
            if ( auto spt = _parent.lock(); spt != nullptr )
                return spt;
        }

        Node( const T &_k, std::shared_ptr<Node> _p ) : key( _k ), _parent( _p ) {}
    };

    std::shared_ptr<Node> root = nullptr;

    auto _min( std::shared_ptr<Node> ) const -> decltype( root );
    auto _max( std::shared_ptr<Node> ) const -> decltype( root );
    auto _next( std::shared_ptr<Node> ) const -> decltype( root );
    auto _prev( std::shared_ptr<Node> ) const -> decltype( root );

    std::ostream &_inorder( std::shared_ptr<Node>, std::ostream & );
    std::ostream &_preorder( std::shared_ptr<Node>, std::ostream & );
    std::ostream &_postorder( std::shared_ptr<Node>, std::ostream & );

    size_t _size( std::shared_ptr<Node> ) const;

  public:
    Binary_Tree() = default;
    ~Binary_Tree() { destroy(); }

    // non - copyable, non - movable
    Binary_Tree( const Binary_Tree & ) = delete;
    Binary_Tree( const Binary_Tree && ) = delete;
    Binary_Tree &operator=( const Binary_Tree & ) = delete;
    Binary_Tree &operator=( const Binary_Tree && ) = delete;

    size_t size() const { return _size( root ); }
    bool isempty() { return ( _size( root ) == 0 ) ? true : false; }

    bool find( const T & ) const;
    void insert( const T & );
    void remove( const T & );
    void destroy();

    std::ostream &inorder( std::ostream & );
    std::ostream &preorder( std::ostream & );
    std::ostream &postorder( std::ostream & );
};

template <class T>
void Binary_Tree<T>::destroy() {
    root = nullptr;
}

template <class T>
auto Binary_Tree<T>::_min( std::shared_ptr<Node> _node ) const -> decltype( root ) {
    auto tmp = _node;
    while ( tmp->left != nullptr )
        tmp = tmp->left;
    return tmp;
}

template <class T>
auto Binary_Tree<T>::_max( std::shared_ptr<Node> _node ) const -> decltype( root ) {
    auto tmp = _node;
    while ( tmp->right != nullptr )
        tmp = tmp->right;
    return tmp;
}

template <class T>
auto Binary_Tree<T>::_next( std::shared_ptr<Node> _node ) const -> decltype( root ) {
    if ( _node->right != nullptr )
        return _min( _node->right );

    auto tmp = _node->parent();
    while ( tmp != nullptr && _node == tmp->right ) {
        _node = tmp;
        tmp = tmp->parent();
    }
    return tmp;
}

template <class T>
auto Binary_Tree<T>::_prev( std::shared_ptr<Node> _node ) const -> decltype( root ) {
    if ( _node->left != nullptr )
        return _max( _node->left );

    auto tmp = _node->parent();
    while ( tmp != nullptr && _node == tmp->left ) {
        _node = tmp;
        tmp = tmp->parent();
    }
    return tmp;
}

template <class T>
size_t Binary_Tree<T>::_size( std::shared_ptr<Node> _node ) const {
    if ( _node == nullptr )
        return 0;
    else
        return _size( _node->left ) + _size( _node->right ) + 1;
}

template <class T>
bool Binary_Tree<T>::find( const T &k ) const {
    auto tmp = root;
    while ( tmp != nullptr && ( k < tmp->key || tmp->key < k ) )
        if ( k < tmp->key )
            tmp = tmp->left;
        else
            tmp = tmp->right;
    if ( tmp == nullptr )
        return false;
    return true;
}

template <class T>
void Binary_Tree<T>::insert( const T &k ) {
    std::shared_ptr<Node> y = nullptr;
    auto x = root;
    // find the key position in the tree
    while ( x != nullptr && ( k < x->key || x->key < k ) ) {
        y = x;
        if ( k < x->key )
            x = x->left;
        else
            x = x->right;
    }
    // if node with key k exitst, do nothing
    if ( x != nullptr )
        return;
    // create new node with key k and parent y
    x = std::make_shared<Node>( k, y );
    // if x is first element, then set root to x
    if ( y == nullptr )
        root = x;
    else if ( x->key < y->key )
        y->left = x;
    else
        y->right = x;

    y = nullptr;
    x = nullptr;
}

template <class T>
void Binary_Tree<T>::remove( const T &k ) {
    // find the key position in the tree
    auto z = root;
    while ( z != nullptr && ( k < z->key || z->key < k ) )
        if ( k < z->key )
            z = z->left;
        else
            z = z->right;

    // if element with key k doesn't exsist, then return
    if ( z == nullptr )
        return;

    // remove the element from tree
    std::shared_ptr<Node> y = nullptr;
    if ( z->left == nullptr || z->right == nullptr )
        y = z;
    else
        y = _next( z );

    std::shared_ptr<Node> x = nullptr;
    if ( y->left != nullptr )
        x = y->left;
    else
        x = y->right;

    if ( x != nullptr )
        x->parent() = y->parent();
    if ( y->parent() == nullptr )
        root = x;
    else if ( !( y < y->parent()->left || y->parent()->left < y ) )
        y->parent()->left = x;
    else
        y->parent()->right = x;

    if ( y < z || z < y )
        z->key = y->key;

    y = x = nullptr;
}

template <class T>
std::ostream &Binary_Tree<T>::preorder( std::ostream &o ) {
    if ( isempty() ) {
        throw std::exception( "Missing element!\n" );
    }
    return _preorder( root, o );
}

template <class T>
std::ostream &Binary_Tree<T>::postorder( std::ostream &o ) {
    if ( isempty() ) {
        throw std::exception( "Missing element!\n" );
    }
    return _postorder( root, o );
}

template <class T>
std::ostream &Binary_Tree<T>::inorder( std::ostream &o ) {
    if ( isempty() ) {
        throw std::exception( "Missing element!\n" );
    }
    return _inorder( root, o );
}

template <class T>
std::ostream &Binary_Tree<T>::_preorder( std::shared_ptr<Node> i, std::ostream &o ) {
    o << i->key << ", ";
    if ( i->left != nullptr ) {
        _preorder( i->left, o );
    }
    if ( i->right != nullptr ) {
        _preorder( i->right, o );
    }
    return o;
}

template <class T>
std::ostream &Binary_Tree<T>::_postorder( std::shared_ptr<Node> i, std::ostream &o ) {
    if ( i->left != nullptr ) {
        _postorder( i->left, o );
    }
    if ( i->right != nullptr ) {
        _postorder( i->right, o );
    }
    o << i->key << ", ";
    return o;
}

template <class T>
std::ostream &Binary_Tree<T>::_inorder( std::shared_ptr<Node> i, std::ostream &o ) {
    if ( i->left != nullptr ) {
        _inorder( i->left, o );
    }
    o << i->key << ", ";
    if ( i->right != nullptr ) {
        _inorder( i->right, o );
    }
    return o;
}

int main() {
}