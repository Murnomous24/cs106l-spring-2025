#include "class.h"
#include <iostream>

bool DMUCard::isValidGPA(double gpa) const {
    return gpa >= 0.0 && gpa <= 5.0;
}

bool DMUCard::isValidID(int id) const {
    return id > 0;
}

std::string DMUCard::statusToString() const {
    switch(DMUCard::currentStatus) {
        case Application : return "Application";
        case Enrollment : return "Enrollment";
        case Active : return "Active";
        case Graduation : return "Graduation";
        default : return "null";
    };
}

void DMUCard::updateStatus() {
    if (totalCredits >= 10 && balance >= 0) {
        currentStatus = Graduation;
    }
}

bool DMUCard::recalculateGPA(int newCourseCredits, double newCourseGrade) {
    if(!isValidGPA(newCourseGrade)) {
        return false;
    } else {
        gpa = (totalCredits * gpa + newCourseCredits * newCourseGrade) / (totalCredits + newCourseCredits);
        totalCredits += newCourseCredits; 

        return true;
    }
}

void DMUCard::logChange(const std::string& msg) {
    std::cout << "[ID " << id << "]: " << msg << std::endl;
}

DMUCard::DMUCard() : name("default"), id(0), balance(0.0), gpa(0.0), currentStatus(Application), totalCredits(0.0) {}

DMUCard::DMUCard(const std::string& name, int id) : name(name), id(id), balance(0.0), gpa(0.0), currentStatus(Application), totalCredits(0.0) {}

std::string DMUCard::get_name() const { return name; }
int DMUCard::get_id() const { return id; }
double DMUCard::get_balance() const { return balance; }
double DMUCard::get_gpa() const { return gpa; }
std::string DMUCard::get_status() const { return statusToString(); }

bool DMUCard::set_name(const std::string& name) {
    this->name = name;
    return true;
}

bool DMUCard::set_id(int id) {
    this->id = id;
    return true;
}

bool DMUCard::set_balance(double change) {
    this->balance += change;
    return true;
}

bool DMUCard::set_gpa(double courseCredits, double courseScore) {
    return recalculateGPA(courseCredits, courseScore);
}

bool DMUCard::set_status(const std::string& status) {
    if(status == "Application") {
        currentStatus = Application;
    } else if(status == "Enrollment") {
        currentStatus = Enrollment;
    } else if(status == "Active") {
        currentStatus = Active;
    } else if(status == "Graduation") {
        currentStatus = Graduation;
    } else {
        return false;
    }

    return true;
}