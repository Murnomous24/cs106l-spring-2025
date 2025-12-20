#include "hashmap.h"

template<typename K, typename M, typename H>
HashMap<K, M, H>::HashMap() : _size(0), _hash_function(H()), _buckets_array(kDefaultBuckets, nullptr) {};

template<typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(size_t bucket_count, const H& hash):
    _size(0), 
    _hash_function(hash), 
    _buckets_array(bucket_count, nullptr) {};

template<typename K, typename M, typename H>
HashMap<K, M, H>::~HashMap() {
    clear();
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::clear() {
    for(auto& bucket : _buckets_array) {
        while(bucket != nullptr) {
            auto bucket_next = bucket->next;
            delete bucket;
            bucket = bucket_next;
        } 
    }

    _size = 0;
}

template<typename K, typename M, typename H>
inline size_t HashMap<K, M, H>::size() const { // should be const 
    return _size;
}

template<typename K, typename M, typename H>
inline bool HashMap<K, M, H>::empty() const{ // should be const 
    return _size == 0;
}

template<typename K, typename M, typename H>
inline float HashMap<K, M, H>::load_factor() const { // should be const 
    return ((float)_size / bucket_count());
}

template<typename K, typename M, typename H>
inline size_t HashMap<K, M, H>::bucket_count() const { // should be const 
    return _buckets_array.size();
}

template<typename K, typename M, typename H>
bool HashMap<K, M, H>::contains(const K& key) const { // should be const 
   auto [pre_node, cur_node] = find_node(key);

   return cur_node != nullptr;
}

template<typename K, typename M, typename H>
M& HashMap<K, M, H>::at(const K& key) {
    // TODO: The implementation of at should be modified
    // throw std::runtime_error("at() function not implemented yet");

    auto [pre_node, cur_node] = find_node(key);

    if(cur_node == nullptr) {
        throw std::out_of_range("HashMap<K, M, H>::at: key not found"); // msg from ref code
    } else {
        return cur_node->value.second;
    }
}

template<typename K, typename M, typename H>
const M& HashMap<K, M, H>::at(const K& key) const {
    return static_cast<const M&>(const_cast<HashMap<K,M,H> *>(this)->at(key));
}

template<typename K, typename M, typename H>
std::pair<typename HashMap<K, M, H>::iterator, bool> HashMap<K, M, H>::insert(const value_type& kv_pair) {
    auto [pre_node, cur_node] = find_node(kv_pair.first);

    if(cur_node != nullptr) {
        return {make_iterator(cur_node), false};
    } else {
        Node* ins_node = new Node{kv_pair, nullptr}; // curly not round
        if(pre_node == nullptr) {
            size_t index = _hash_function(kv_pair.first) % bucket_count();
            _buckets_array[index] = ins_node;
        } else {
            pre_node -> next = ins_node;
        }

        ++ _size; // NOTE
        return {make_iterator(ins_node), true};
    }
}

template<typename K, typename M, typename H>
bool HashMap<K, M, H>::erase(const K& key) {
    auto [pre_node, cur_node] = find_node(key);
    
    if(cur_node != nullptr) {
        if(pre_node != nullptr) {
            pre_node -> next = cur_node -> next;
            
            delete cur_node;
        } else {
            size_t index = _hash_function(key) % bucket_count();
            _buckets_array[index] = cur_node -> next;

            delete cur_node;
        }

        _size --;
        return true;
    } else {
        return false;
    }
}

template<typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::erase(const_iterator pos) { // TODO: pos never be nullptr ?
    auto cur_node = pos._node;
    auto temp = make_iterator(cur_node);
    auto next_iter = ++temp; // pre ++ TODO

    erase(cur_node->value.first);

    return next_iter;
}

template<typename K, typename M, typename H>
void HashMap<K, M, H>::rehash(size_t new_bucket) {
    // from cs106l's implementation
    if(new_bucket == 0) {
        throw std::out_of_range("HashMap<K,M,H>::rehash: Invalid Input Parameters");
    }

    bucket_array_type tmp_bucket_array = _buckets_array;

    _buckets_array.clear();
    _buckets_array.resize(new_bucket, nullptr);

    for(auto& cur_bucket : tmp_bucket_array) { // auto will point (Node) or (Node*) ?
        while(cur_bucket != nullptr) {
            Node* tmp_bucket = cur_bucket;
            cur_bucket = cur_bucket -> next;

            const auto& [tmp_key, tmp_mapped] = tmp_bucket -> value;
            size_t index = _hash_function(tmp_key) % bucket_count();

            tmp_bucket -> next = _buckets_array[index];
            _buckets_array[index] = tmp_bucket;
        }
    }
}

template<typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::begin() {
    size_t index = first_not_empty_bucket();
    if(index == bucket_count()) {
        return end(); // avoid out of bound
    }
    return make_iterator(_buckets_array[index]);
}

template<typename K, typename M, typename H>
typename HashMap<K, M, H>::const_iterator HashMap<K, M, H>::begin() const {
    return const_cast<HashMap<K, M, H> *>(this)->begin(); // from cs106l ref
}

template<typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::end() {
    return make_iterator(nullptr); // from cs106l ref
}

template<typename K, typename M, typename H>
typename HashMap<K, M, H>::const_iterator HashMap<K, M, H>::end() const {
    return const_cast<HashMap<K, M, H> *>(this)->end(); // from cs106l ref
}

template<typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::find(const K& key) {
    auto [pre_node, cur_node] = find_node(key);

    return make_iterator(cur_node);
}

// const version find
template<typename K, typename M, typename H>
typename HashMap<K, M, H>::const_iterator HashMap<K, M, H>::find(const K& key) const {
    return const_cast<HashMap<K, M, H> *>(this)->find(key);
}

template<typename K, typename M, typename H>
void HashMap<K, M, H>::debug() {
    // from cs106l ref
    std::cout << "HashMap Debug Info:" << std::endl;
    std::cout << "Bucket Count=" << bucket_count() <<" Size=" << size() << " Load Factor="<<load_factor()<< std::endl;
    for (size_t i = 0; i < _buckets_array.size(); i++) {
        std::cout << "Bucket-" << i << ": ";
        Node* temp = _buckets_array[i];
        while (temp != nullptr)
        {
            std::cout << temp->value.first << "-" << temp->value.second << " ";
            temp = temp->next;
        }
        std::cout << std::endl;
    }
}

template<typename K, typename M, typename H>
template<typename InputIter>
HashMap<K, M, H>::HashMap(InputIter begin, InputIter end, size_t bucket_count, const H& hash) : HashMap(kDefaultBuckets, hash) { // DO NOT WRITE DEFAULT VALUE
    // WARNING: w/o check validation of InputIter
    for(InputIter iter = begin; iter != end; ++ iter) {
        insert(*iter);
    }
}

template<typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(std::initializer_list<value_type> init, size_t bucket_count, const H& hash) : HashMap(init.begin(), init.end(), bucket_count, hash) {} // use initializer list 

template<typename K, typename M, typename H>
M& HashMap<K, M, H>::operator[](const K& key) {
    // from cs106l ref
    value_type default_val = {key, {}}; // note how to set default value use value_type
    auto [insert_iter, success] = insert(default_val);

    return insert_iter -> second;
}

template<typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(const HashMap<K, M, H>& map) : 
    _size(0), 
    _hash_function(map._hash_function), 
    _buckets_array(map.bucket_count(), nullptr) {
        // from cs106l ref, first to build up structure, and insert node one by one

        for(auto &kv_pair : map) { // no need to traverse bucket, just use iterator to get kv
            insert(kv_pair);
        }
}

template<typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(HashMap&& map) : 
    _size(map._size), 
    _hash_function(std::move(map._hash_function)), 
    _buckets_array(std::move(map._buckets_array)) {
        map._size = 0;
        // map._buckets_array.resize(map.bucket_count(), nullptr); // r-value's bucket array has been std::move
}

template<typename K, typename M, typename H>
HashMap<K, M, H>& HashMap<K, M, H>::operator=(const HashMap& map) { // why this name ? TODO
    // if(*this == map) return *this;
    if(this == &map) return *this; // compare point, not the object to accelerate

    clear();

    _size = 0;
    _hash_function = map._hash_function;

    if(_buckets_array.size() != map.bucket_count()) {
        _buckets_array.clear();
        _buckets_array.resize(map.bucket_count(), nullptr);
    } // resize the bucket array if needed

    for(auto &kv_pair : map) {
        insert(kv_pair);
    }

    return *this;
}

template<typename K, typename M, typename H>
HashMap<K, M, H>& HashMap<K, M, H>::operator=(HashMap&& map) {
    // if(*this == map) return *this;
    if(this == &map) return *this; // compare point, not the object to accelerate

    clear();

    _size = map._size;
    _hash_function = std::move(map._hash_function);
    _buckets_array = std::move(map._buckets_array);

    map._size = 0;
    // map._buckets_array.resize(map.bucket_count(), nullptr); // never needed

    return *this;
}

template<typename K, typename M, typename H>
std::ostream& operator<<(std::ostream& stream, const HashMap<K, M, H>& map) {
    std::stringstream ssm;

    for(const auto kv_pair : map) {
        ssm << kv_pair.first << ":" << kv_pair.second << ", ";
    }

    std::string str_tmp = ssm.str();
    if(str_tmp.size() != 0) str_tmp.erase(str_tmp.size() - 2, 2); // str_tmp not null equation ?

    stream << "{" << str_tmp << "}";


    return stream;
}

template<typename K, typename M, typename H>
bool operator==(const HashMap<K, M, H>& lhs, const HashMap<K, M, H>& rhs) {
    for(const auto& lhs_kv_pair : lhs) {
        if(!rhs.contains(lhs_kv_pair.first)) return false;
        if(rhs.at(lhs_kv_pair.first) != lhs_kv_pair.second) return false;
    }

    // return true;
    return lhs.size() == rhs.size(); // NOTE
}

template<typename K, typename M, typename H>
bool operator!=(const HashMap<K, M, H>& lhs, const HashMap<K, M, H>& rhs) {
    return !(lhs == rhs);
}

// ------------------------------
// private member
template<typename K, typename M, typename H>
typename HashMap<K, M, H>::node_pair HashMap<K, M, H>::find_node(const K& key) const {
    size_t index = _hash_function(key) % _buckets_array.size();

    Node* pre_node = nullptr;
    Node* cur_node = _buckets_array[index];

    while(cur_node != nullptr) {
        const auto& [cur_key, cur_value] = cur_node->value; // use auto to deduce the type
        
        if(cur_key == key) {
            return {pre_node, cur_node};
        } else {
            pre_node = cur_node;
            cur_node = cur_node -> next;
        }
    }

    // return {nullptr, nullptr}; // TODO: {pre_node, cur_node} or both nullptr ? answer: in `insert` method, we need use find_node to retrun last node get the position to insert.
    return {pre_node, cur_node};
} 

template<typename K, typename M, typename H>
size_t HashMap<K, M, H>::first_not_empty_bucket() const {
    // auto isNotNullptr = [](const Node* node) {
    //     return node != nullptr;
    // }; // the auto point to the lambda's type, not the return type
    //    // the _bucket_array store node*, not the node itself

    // auto found = std::find_if(_buckets_array.begin(), _buckets_array.end(), isNotNullptr);
    // return found - _buckets_array.begin();

    for(size_t i = 0; i < _buckets_array.size(); i ++) {
        if(_buckets_array[i] != nullptr) return i;
    }

    return _buckets_array.size() - 1;
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::make_iterator(Node* curr) {
    if(curr == nullptr) {
        // does not exist, return end
        return {&_buckets_array, curr, bucket_count()}; // why first use reference
    } else {
        size_t index = _hash_function(curr->value.first) % bucket_count();

        return {&_buckets_array, curr, index};
    }
}