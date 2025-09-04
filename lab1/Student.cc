#include "Student.h"
#include <string>
#include <sstream>
#include "Class.h"
#include <iostream>

using namespace std;
std::string Student::toString() const
{
    // TODO: uncomment the following code after implementing class Student.

    std::ostringstream oss;
    oss << "Student Information:"
        << "\n\tid: " << id
        << "\n\tname: " << name
        << "\n\tenrollment year: " << year
        << "\n\tdegree: " << (degree == graduate ? "graduate" : "undergraduate")
        << std::endl;
    return oss.str();
}

// TODO: implement functions which are declared in Student.h.
void Student::addClasses(Class* c) {
    classes.push_back(c);
}

double Undergraduate::getGpa() {
    double gpa = 0;
    int pointS=0;
    for (int i = 0; i<classes.size(); i++) {
        StudentWrapper& Stu = classes[i]->getStudentWrapper(this->id);
        double score0 = Stu.getScore();
        if(score0==-1)continue;
        gpa += (score0 /20) * (classes[i]->point);
        pointS+=classes[i]->point;
    }
    if(pointS==0)return 0;
    gpa=gpa/pointS;
    return gpa;
}
double Undergraduate::getAvgScore() {
    double avg = 0;
    int pointS=0;
    for (int i = 0; i < classes.size(); i++) {
        StudentWrapper& Stu = classes[i]->getStudentWrapper(this->id);
        double score0 = Stu.getScore();
        if(score0==-1)continue;
        avg +=score0 * (classes[i]->point);
        pointS+=classes[i]->point;
    }
    if(pointS==0)return 0;
    avg=avg/pointS;
    return avg;
}
double Graduate::getAvgScore() {
    double avg = 0;
    int pointS=0;
    for (int i = 0; i < classes.size(); i++) {
        StudentWrapper& Stu = classes[i]->getStudentWrapper(this->id);
        double score0 = Stu.getScore();
        if(score0==-1)continue;
        avg += score0 * (classes[i]->point);
        pointS+=classes[i]->point;
    }
    if(pointS==0)return 0;
    avg=avg/pointS;
    return avg;
}
double Graduate::getGpa() {
    double gpa = 0;
    int pointS=0;
    for (int i = 0; i < classes.size(); i++) {
        StudentWrapper& Stu = classes[i]->getStudentWrapper(this->id);
        double score0 = Stu.getScore();
        if(score0==-1)continue;
        double gpa1 = 0;
        switch (static_cast<int>(score0/10)) {
            case 10:
            case 9:gpa1 = 4.0; break;
            case 8:gpa1 = 3.5; break;
            case 7:gpa1 = 3.0; break;
            case 6:gpa1 = 2.5; break;
            default:gpa1 = 2.0;
        }
        gpa+=gpa1*classes[i]->point;
        pointS+=classes[i]->point;
    }
    if(pointS==0)return 0;
    gpa=gpa/pointS;
    return gpa;
}