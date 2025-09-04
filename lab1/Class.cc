#include "Class.h"
#include <string>
#include <vector>
#include "Student.h"
#include <sstream>
#include<fstream>
#include<iostream>

std::string Class::toString() const {
    std::ostringstream oss;
    oss << "Class Information:"
        << "\n\tname: " << name
        << "\n\tpoint: " << point
        << std::endl;
    return oss.str();
}

void Class::addStudent(const Student& st) {
    StudentWrapper sw(st.id, st);
    students.push_back(sw);
}

StudentWrapper& Class::getStudentWrapper(const std::string& studentId) {
    for (std::vector<StudentWrapper>::iterator it = students.begin();
         it != students.end();
         ++ it) {
        if (it->id == studentId)
            return *it;
    }
    throw "No Match Student!";
}

double Class::getHighestScore() {
    // TODO implement getHighestScore
    double high = -1;
    for (int i = 0; i < students.size(); i++) {
        double score0 = students[i].getScore();
        if(score0==-1)continue;
        if (score0 > high)high=score0;
    }
    return high;
}

double Class::getLowestScore() {
    // TODO implement getLowestScore
    double low = 101.0;
    for (int i = 0; i < students.size(); i++) {
        double score0 = students[i].getScore();
        if(score0==-1)continue;
        if (score0 < low)low = score0;
    }
    return low;
}

double Class::getAvgScore() {
    // TODO implement getAvgScore
    double sum = 0;
    int x=0;
    for (int i = 0; i < students.size(); i++) {
        double score0= students[i].getScore();
        if(score0==-1)continue;
        sum+=score0;
        x++;
    }
    if(x==0)return -1;
    return sum /x;
}

void Class::saveScore(const std::string& filename) {
    // TODO implement saveScore
    ofstream outFile(filename, std::ios::app);
    outFile << this->name << endl;
    for (vector<StudentWrapper>::iterator it = students.begin();
         it < students.end();
         it++) {
        double score0 = it->getScore();
        if(score0==-1)continue;
        outFile << it->id << ',' << score0 << endl;
    }
}