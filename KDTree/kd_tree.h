/**
 * File: kd_tree.h
 * Author: (your name here)
 * ------------------------
 * An interface representing a kd-tree in some number of dimensions. The tree
 * can be constructed from a set of data and then queried for membership and
 * nearest neighbors.
 */

#ifndef KDTREE_INCLUDED
#define KDTREE_INCLUDED

#include <map>
#include "point.h"
#include "math.h"
#include "bounded_priority_queue.h"

// "using namespace" in a header file is conventionally frowned upon, but I'm
// including it here so that you may use things like size_t without having to
// type std::size_t every time.

template <size_t N, typename ElemType>
class KDTree {
public:
    struct Node {
        Point<N> point;
        ElemType element;
        Node* left_node;
        Node* right_node;
    };
    
    // Constructor: KDTree();
    // Usage: KDTree<3, int> myTree;
    // ----------------------------------------------------
    // Constructs an empty KDTree.
    KDTree();
    
    // Destructor: ~KDTree()
    // Usage: (implicit)
    // ----------------------------------------------------
    // Cleans up all resources used by the KDTree.
    ~KDTree();
    
    // KDTree(const KDTree& rhs);
    // KDTree& operator=(const KDTree& rhs);
    // Usage: KDTree<3, int> one = two;
    // Usage: one = two;
    // -----------------------------------------------------
    // Deep-copies the contents of another KDTree into this one.
    KDTree(const KDTree& rhs);
    KDTree& operator=(const KDTree& rhs);
    
    // size_t dimension() const;
    // Usage: size_t dim = kd.dimension();
    // ----------------------------------------------------
    // Returns the dimension of the points stored in this KDTree.
    size_t dimension() const;
    
    // size_t size() const;
    // bool empty() const;
    // Usage: if (kd.empty())
    // ----------------------------------------------------
    // Returns the number of elements in the kd-tree and whether the tree is
    // empty.
    size_t size() const;
    bool empty() const;
    
    // bool contains(const Point<N>& pt) const;
    // Usage: if (kd.contains(pt))
    // ----------------------------------------------------
    // Returns whether the specified point is contained in the KDTree.
    bool contains(const Point<N>& pt) const;
    
    // void insert(const Point<N>& pt, const ElemType& value);
    // Usage: kd.insert(v, "This value is associated with v.");
    // ----------------------------------------------------
    // Inserts the point pt into the KDTree, associating it with the specified
    // value. If the element already existed in the tree, the new value will
    // overwrite the existing one.
    void insert(const Point<N>& pt, const ElemType& value);
    
    // ElemType& operator[](const Point<N>& pt);
    // Usage: kd[v] = "Some Value";
    // ----------------------------------------------------
    // Returns a reference to the value associated with point pt in the KDTree.
    // If the point does not exist, then it is added to the KDTree using the
    // default value of ElemType as its key.
    ElemType& operator[](const Point<N>& pt);
    
    // ElemType& at(const Point<N>& pt);
    // const ElemType& at(const Point<N>& pt) const;
    // Usage: cout << kd.at(v) << endl;
    // ----------------------------------------------------
    // Returns a reference to the key associated with the point pt. If the point
    // is not in the tree, this function throws an out_of_range exception.
    ElemType& at(const Point<N>& pt);
    const ElemType& at(const Point<N>& pt) const;
    
    // ElemType kNNValue(const Point<N>& key, size_t k) const
    // Usage: cout << kd.kNNValue(v, 3) << endl;
    // ----------------------------------------------------
    // Given a point v and an integer k, finds the k points in the KDTree
    // nearest to v and returns the most common value associated with those
    // points. In the event of a tie, one of the most frequent value will be
    // chosen.
    ElemType kNNValue(const Point<N>& key, size_t k) const;

private:
    // TODO: Add implementation details here.

    size_t tree_size;
    Node* tree_head;

    void delete_tree(Node* node);
    Node* insert_node(Node*& head, const Point<N>& pt, const ElemType& value, size_t index);
    Node* find_node(Node* head, const Point<N>& pt, size_t index) const;
    void find_knn(BoundedPriorityQueue<ElemType>* bpq, Node* head, const Point<N>& pt, size_t index) const;
    Node* copy_tree(Node* source);
};

// private member

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::delete_tree(Node* node) {
    if(node == nullptr) return ;
    
    delete_tree(node->left_node);
    delete_tree(node->right_node);

    delete node;

    return ;
}


/*
Q1: why return value is 'typename'?
Q2: reference of pointer
*/
template <size_t N, typename ElemType>
typename KDTree<N, ElemType>::Node* KDTree<N, ElemType>::insert_node(Node*& head, const Point<N>& pt, const ElemType& value, size_t index) {
    if(head == nullptr) {
        head = new Node{pt, value, nullptr, nullptr};
        tree_size ++;

        return head;
    } 

    if(head->point == pt) {
        head->element = value;
        
        return head;
    }
    else if(pt[index] < head->point[index]) {
        return insert_node(head->left_node, pt, value, (index + 1) % N);
    } else {
        return insert_node(head->right_node, pt, value, (index + 1) % N);
    }
}

template <size_t N, typename ElemType>
typename KDTree<N, ElemType>::Node* KDTree<N, ElemType>::find_node(Node* head, const Point<N>& pt, size_t index) const {
    if(head == nullptr) {
        return nullptr;
    }

    if(head->point == pt) {
        return head;
    } else {
        if(head->point[index] > pt[index]) {
            return find_node(head->left_node, pt, (index + 1) % N);
        } else {
            return find_node(head->right_node, pt, (index + 1) % N);
        }
    }
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::find_knn(BoundedPriorityQueue<ElemType>* bpq, Node* head, const Point<N>& pt, size_t index) const {
    if(head == nullptr) return ;

    bpq->enqueue(head->element, Distance(head->point, pt));

    if(head->point[index] > pt[index]) {
        find_knn(bpq, head->left_node, pt, (index + 1) % N);
        if(bpq->size() < bpq->maxSize() || abs(head->point[index] - pt[index]) < bpq->worst()) {
            find_knn(bpq, head->right_node, pt, (index + 1) % N);
        }  
    } else {
        find_knn(bpq, head->right_node, pt, (index + 1) % N);
        if(bpq->size() < bpq->maxSize() || abs(head->point[index] - pt[index]) < bpq->worst()) {
            find_knn(bpq, head->left_node, pt, (index + 1) % N);
        }
    }

    return ;
}

template <size_t N, typename ElemType>
typename KDTree<N, ElemType>::Node* KDTree<N, ElemType>::copy_tree(Node* source) { 
    if(source == nullptr) {
        return nullptr;
    }

    Node* new_node = new Node;
    new_node->element = source->element;
    new_node->point = source->point;
    new_node->left_node = copy_tree(source->left_node); // NOTE: use recursive
    new_node->right_node = copy_tree(source->right_node);

    return new_node;
}

// public member

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree():
    tree_size(0),
    tree_head(nullptr) {
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::~KDTree() {
    delete_tree(tree_head);
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::dimension() const {
    return N;
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::size() const {
    return tree_size;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::empty() const {
    return tree_size == 0;
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::insert(const Point<N>& pt, const ElemType& value) {
    insert_node(tree_head, pt, value, 0);
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::contains(const Point<N>& pt) const {
    return find_node(tree_head, pt, 0) != nullptr;
}

template <size_t N, typename ElemType>
ElemType& KDTree<N, ElemType>::operator[](const Point<N>& pt) {
    Node* res = find_node(tree_head, pt, 0);
    if(res != nullptr) {
        return res->element; 
    } else {
        return insert_node(tree_head, pt, 0, 0)->element; // TODO: default element value?
    }
}

template <size_t N, typename ElemType>
ElemType& KDTree<N, ElemType>::at(const Point<N>& pt) {
    Node* res = find_node(tree_head, pt, 0);
    if(res != nullptr) {
        return res->element; 
    } else {
        throw std::out_of_range("at: out of range");
    }
}

template <size_t N, typename ElemType>
const ElemType& KDTree<N, ElemType>::at(const Point<N>& pt) const {
    return static_cast<const ElemType&>(const_cast<KDTree<N, ElemType> *>(this)->at(pt));
}

template <size_t N, typename ElemType>
ElemType KDTree<N, ElemType>::kNNValue(const Point<N>& key, size_t k) const {
    BoundedPriorityQueue<ElemType>* bpq = new BoundedPriorityQueue<ElemType>(k);

    find_knn(bpq, tree_head, key, 0);

    std::map<ElemType, size_t> value;
    while(!bpq->empty()) {
        auto pop_elem = bpq->dequeueMin();
        value[pop_elem] ++; // TODO: key's default value is zero?
    }

    ElemType ans;
    size_t ans_count = 0;
    for(auto [elem, count] : value) {
        if(count > ans_count) {
            ans = elem;
            ans_count = count;
        }
    }

    return ans;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree(const KDTree& rhs) {
    this->tree_head = copy_tree(rhs.tree_head);
    this->tree_size = rhs.tree_size;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>& KDTree<N, ElemType>::operator=(const KDTree& rhs) {
    if(this == &rhs) {
        return *this;
    }

    delete_tree(this->tree_head);

    this->tree_head = copy_tree(rhs.tree_head);
    this->tree_size = rhs.tree_size;

    return *this;
}

#endif // KDTREE_INCLUDED
