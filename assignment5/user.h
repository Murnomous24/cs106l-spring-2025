/*
 * CS106L Assignment 5: TreeBook
 * Created by Fabio Ibanez with modifications by Jacob Roberts-Baca.
 */

#include <iostream>
#include <string>

class User
{
public:
  User(const std::string& name);
  void add_friend(const std::string& name);
  std::string get_name() const;
  size_t size() const;
  void set_friend(size_t index, const std::string& name);

  /** 
   * STUDENT TODO:
   * Your custom operators and special member functions will go here!
   */

  friend std::ostream& operator<<(std::ostream& os, const User& user);
  
  ~User() {
    delete[] _friends;
  }

  User(const User& user) : _name(user._name), _size(user._size), _capacity(user._capacity) {
    _friends = new std::string[_size];
    
    for(int i = 0 ; i < _size; i ++) {
      *(_friends + i) = *(user._friends + i); 
    }
  }

  User& operator=(const User& user) {
    if(this == &user) return *this;

    delete[] _friends;

    _name = user._name;
    _size = user._size;
    _capacity = user._capacity;
    if(user._friends) {
      _friends = new std::string[_size];
      
      for(int i = 0 ; i < _size; i ++) {
        *(_friends + i) = *(user._friends + i); 
      }
    } else {
      _friends = nullptr;
    }

    return *this;
  }

  User(User&& user) noexcept = delete;

  User& operator=(User&& user) = delete;

  User& operator+=(User& rhs);

  bool operator<(const User& rhs) const;

private:
  std::string _name;
  std::string* _friends;
  size_t _size;
  size_t _capacity;
};