#include <iostream>
#include <string>
#include <vector>

struct Node {
    std::string element;
    int key;
    Node() : element(), key() {}
    Node( std::string e, int k ) : element( e ), key( k ) {}
};

class Heap {
  private:
    std::vector<Node> _heap;
    int find();
    int parent();

  public:
    Heap() {}
    Heap( int n ) { _heap.reserve( n ); }

    void max_insert( Node other ) {
        _heap.push_back( other );
        int i = _heap.size() - 1;
        while ( ( i != 0 ) && ( other.key > _heap[( i - 1 ) / 2].key ) ) {
            std::swap( _heap[i], _heap[( i - 1 ) / 2] );
            i = ( i - 1 ) / 2;    // (i-1)/2 is parent
        }
        //for ( const auto &s : _heap ) std::cout << s.key << " ";
        //std::cout << '\n';
    }

    inline void output() {
        if ( _heap.size() >= 3 ) {
            std::cout << "First three things to do:" << '\n';
            std::cout << _heap[0].element << '\n';

            if ( _heap[1].key > _heap[2].key ) {
                std::cout << _heap[1].element << '\n';
                std::cout << _heap[2].element << '\n';
            }
            else {
                std::cout << _heap[2].element << '\n';
                std::cout << _heap[1].element << '\n';
            }
        }
    }
};

int main() {
    int n, key;
    std::cin >> n;

    Heap heap( n );
    std::string str;
    for ( ; n != 0; --n ) {
        std::cin >> str >> key;
        heap.max_insert( Node( str, key ) );
    }
    heap.output();

    return 0;
}