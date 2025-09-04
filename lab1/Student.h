#ifndef STUDENT_H_
#define STUDENT_H_

#include <string>
#include <vector>

using namespace std;

class Class;

enum Degree {
    undergraduate,
    graduate
};

class Student {
    // TODO: implement class Student.
private:
    string name;
    string year;
    Degree degree;

protected:
    vector<Class*> classes;

public:
    const std::string id;
    std::string toString() const;
    virtual double getGpa() = 0;
    virtual double getAvgScore() = 0;
    void addClasses(Class* c);
    Student(string& a,string& b,string& c,Degree d):id(a),name(b),year(c),degree(d){}
    virtual ~Student()=default ;
};

// TODO: implement class Graduate.
class Graduate :public Student
{public:
    double getGpa() override;
    double getAvgScore() override;
    Graduate(string a, string b, string c) :Student(a,b,c,graduate){}
};

// TODO: implement class Undergraduate.
class Undergraduate :public Student
{public:
    double getGpa () override;
    double getAvgScore () override;
    Undergraduate(string a, string b, string c) :Student(a, b, c, undergraduate) {};
};


class StudentWrapper {
private:
    const Student &student;
    double score = -1.0;
public:
    const std::string id;
    // TODO: fix error
    StudentWrapper(const std::string &id, const Student &student):id(id),student(student) {};
    ~StudentWrapper()=default;
    void setScore(double score0)
    {
        if (score0 < 0 || score0 > 100)
            throw "Wrong score!";
        this->score = score0;
    }

    double getScore() const
    {
        return this->score;
    }

    std::string toString() const
    {
        return student.toString();
    }
};

#endif // STUDENT_H_
