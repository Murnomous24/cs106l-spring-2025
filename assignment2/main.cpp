/*
 * CS106L Assignment 2: Marriage Pact
 * Created by Haven Whitney with modifications by Fabio Ibanez & Jacob Roberts-Baca.
 *
 * Welcome to Assignment 2 of CS106L! Please complete each STUDENT TODO
 * in this file. You do not need to modify any other files.
 *
 */

#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>

std::string kYourName = "Bergling Murphy"; // Don't forget to change this!

/**
 * Takes in a file name and returns a set containing all of the applicant names as a set.
 *
 * @param filename  The name of the file to read.
 *                  Each line of the file will be a single applicant's name.
 * @returns         A set of all applicant names read from the file.
 *
 * @remark Feel free to change the return type of this function (and the function
 * below it) to use a `std::unordered_set` instead. If you do so, make sure
 * to also change the corresponding functions in `utils.h`.
 */
std::set<std::string> get_applicants(std::string filename) {
  // STUDENT TODO: Implement this function.
  std::set<std::string> student_name;

  std::ifstream ifs(filename);
  if(ifs.is_open()) {
    std::cout << "student's name file is ok" << std::endl;
  } else {
    std::cout << "student's name file is shit" << std::endl;
  }

  std::string buf;
  while(std::getline(ifs, buf)) {
    student_name.insert(buf);
  }

  ifs.close();

  // test if set works
  // std::cout << "1. set's size: " << student_name.size() << std::endl;
  // std::cout << "2. set's content: ";
  
  // int count = 5;
  // for(auto u : student_name) {
  //   std::cout << u << ";";
  //   count --;

  //   if(count == 0) break;
  // }

  return student_name;
}

std::pair<char, char> get_initals(std::string name) {
  char ch1 = name[0];

  int p = 1;
  while(name[p] != ' ') p ++;
  
  char ch2 = name[p + 1];

  return std::pair<char, char>(ch1, ch2);
}
 
/**
 * Takes in a set of student names by reference and returns a queue of names
 * that match the given student name.
 *
 * @param name      The returned queue of names should have the same initials as this name.
 * @param students  The set of student names.
 * @return          A queue containing pointers to each matching name.
 */
std::queue<const std::string*> find_matches(std::string name, std::set<std::string>& students) {
  // STUDENT TODO: Implement this function.

  // test get_initials works
  // std::pair<char, char> my_name = get_initals(name);
  // std::cout << "my: " << my_name.first << "/" << my_name.second << std::endl;

  std::queue<const std::string*> matches;

  std::pair<char, char> my_initials = get_initals(name);
  std::cout << "my: " << my_initials.first << "/" << my_initials.second << std::endl;

  for(auto &u : students) {
    // std::cout << "here! name: " << u << std::endl;
    std::pair<char, char> love_initalis = get_initals(u);
    // std::cout << "love: " << love_initalis.first << "/" << love_initalis.second << std::endl;

    if(my_initials.first == love_initalis.first && my_initials.second == love_initalis.second) {
      std::cout << "match! name: " << u << std::endl;
      matches.push(&u);
    }
  }

  if(matches.empty()) {
    std::cout << "no love" << std::endl;
  } else {
    std::cout << "match 1: " << *matches.front() << std::endl;
  }

  return matches;
}

/**
 * Takes in a queue of pointers to possible matches and determines the one true match!
 *
 * You can implement this function however you'd like, but try to do something a bit
 * more complicated than a simple `pop()`.
 *
 * @param matches The queue of possible matches.
 * @return        Your magical one true love.
 *                Will return "NO MATCHES FOUND." if `matches` is empty.
 */
std::string get_match(std::queue<const std::string*>& matches) {
  // STUDENT TODO: Implement this function.

  int choose_index = 114514 % matches.size();
  std::cout << "choose_index: " << choose_index << std::endl;
  
  std::string choose_one;
  int now_index = 0;
  while(now_index != choose_index) {
    matches.pop();
    now_index ++;
  }
  if(matches.empty()) {
    std::cout << "matches is null!" << std::endl;
  } else {
    choose_one = *matches.back();
  }

  std::cout << "choose one: " << choose_one << std::endl;

  return choose_one;
}

/* #### Please don't remove this line! #### */
#include "autograder/utils.hpp"
