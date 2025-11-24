#ifndef DMUCARD_H
#define DMUCARD_H

#include <string>

class DMUCard {
    private:
        std::string name;
        int id;
        double balance;
        double gpa;
        enum statusType {Application, Enrollment, Active, Graduation};
        statusType currentStatus;
        double totalCredits;

        bool isValidGPA(double gpa) const;
        bool isValidID(int id) const;

        std::string statusToString() const;
        void updateStatus();

        bool recalculateGPA(int newCourseCredits, double newCourseGrade);

        void logChange(const std::string& msg);

    public:
        DMUCard();
        DMUCard(const std::string& name, int id);

        std::string get_name() const;
        int get_id() const;
        double get_balance() const;
        double get_gpa() const;
        std::string get_status() const;

        bool set_name(const std::string& name);
        bool set_id(int id);
        bool set_balance(double change);
        bool set_gpa(double courseCredits, double courseScore);
        bool set_status(const std::string& status);
};

#endif