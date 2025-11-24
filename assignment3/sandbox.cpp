/*
 * CS106L Assignment 3: Make a Class
 * Created by Fabio Ibanez with modifications by Jacob Roberts-Baca.
 */

#include "class.h"
#include <iostream>

void sandbox() {
  // STUDENT TODO: Construct an instance of your class!

  DMUCard card1;
  std::cout << "Card 1 - Name: " << card1.get_name() 
            << ", ID: " << card1.get_id() << std::endl;

  // 使用自定义构造函数
  DMUCard card2("Alice", 12345);
  std::cout << "Card 2 - Name: " << card2.get_name() 
            << ", ID: " << card2.get_id() << std::endl;

  // 测试 setter 函数
  card2.set_balance(100.50);
  card2.set_gpa(3, 4.5);  // 3学分课程，成绩4.5
  
  std::cout << "Card 2 - Balance: " << card2.get_balance() 
            << ", GPA: " << card2.get_gpa() 
            << ", Status: " << card2.get_status() << std::endl;

  // 使用 uniform initialization
  DMUCard card3{"Bob", 67890};
  std::string newStatus = "Active";
  card3.set_status(newStatus);
  std::cout << "Card 3 - Status: " << card3.get_status() << std::endl;
}