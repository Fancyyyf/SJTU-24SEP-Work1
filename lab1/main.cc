#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Class.h"
#include "Student.h"
#include <string>
#include <limits>

using namespace std;

class AppX {
private:
    vector<Student *> studentVec;
    vector<Class *> classVec;

    void loadFiles();
    void inputScore();
    void printScoreStats();
    void printGrade();
    void saveScore();

public:
    AppX();
    ~AppX();
    int run();
};

AppX::~AppX()
{
    // You can use the traditional loop, which is more clear.
    for (vector<Class *>::iterator it = classVec.begin();
         it != classVec.end();
         ++it) {
        if (*it) delete (*it);
    }
    // You can use modern and simpler loops only if you know what it is doing.
    for (const auto &s: studentVec) {
        if (s) delete (s);
    }
}

AppX::AppX()
{
    loadFiles();
}

void AppX::loadFiles()
{
    string line;
    size_t pos1, pos2;
    vector<string> bufv;
    Student *st = nullptr;
    string clsname;
    int point;
    Class *cl = nullptr;

    // Open a file as an input stream.
    ifstream stfile("./Students.txt");
    if (!stfile) {
        cerr << "无法打开文件" << endl;
        return;
    }

    while (getline(stfile, line)) {
        if (line.empty()) // It's an empty line.
            continue;
        if (line[0] == '#') // It's a comment line.
            continue;

        // The bufv vector stores each column in the line.
        bufv.clear();
        // Split the line into columns.
        //   pos1: beginning of the column.
        //   pos2: end of the column.
        pos1 = 0;
        while (true) {
            pos2 = line.find(';', pos1 + 1);
            if (pos2 == string::npos) { // No more columns.
                // Save the last column (pos1 ~ eol).
                if (pos1 < line.size()) {
                    bufv.push_back(line.substr(pos1, string::npos));
                }
                break;
            } else {
                // Save the column (pos1 ~ pos2).
                bufv.push_back(line.substr(pos1, pos2 - pos1));
            }
            pos1 = pos2 + 1;
        }

        // TODO: uncomment next lines after implementing class Undergraduate and Graduate.

        if (bufv[3] == "U")
            st = new Undergraduate(bufv[0], bufv[1], bufv[2]);
        else
            st = new Graduate(bufv[0], bufv[1], bufv[2]);


        studentVec.push_back(st);
    }
    stfile.close();

    // TODO: load data from ./Classes.txt and push objects to the vector.
    // Hint: how is student information read?
    int A = -1;

    ifstream clfile("./Classes.txt");
    if (!clfile) {
        cerr << "无法打开文件" << endl;
        return;
    }

    while (getline(clfile, line)) {
        if (line.empty()) // It's an empty line.
            continue;
        if (line[0] == '#') // It's a comment line.
            continue;


        pos1 = 0;
        pos2 = line.find(':', pos1 + 1);
        if (pos2 != string::npos) {
            if (line[0] == 'C') {
                clsname = line.substr(pos2 + 1, string::npos);
            }
            else {
                point= line[pos2+1]-'0';
                cl = new Class(clsname, point);
                classVec.push_back(cl);
                A++;
            }
        }
        else{
                if (A < 0){
                    cerr << "No Match Class!" << endl;
                    continue;
                }

            string id0 = line.substr(pos1, string::npos);
            int p = -1;
            for (int i = 0; i < studentVec.size(); i++) {
                if (studentVec[i]->id == id0) { p = i; break; }
            }
            if (p == -1) {
                cerr << "No Match Student!" << endl; continue;
            }
            else { classVec[A]->addStudent(*studentVec[p]);
                   studentVec[p]->addClasses(classVec[A]);
            }
        }
    }
    clfile.close();
}

void AppX::inputScore()
{
    // TODO: implement inputScore.
    // Hint: Take a look at printScoreStats().

    string sbuf;
    string sbuf2;
    string id0;
    Class* cl;
    double score0;
    size_t pos1,pos2;


    while (true) {
        cout << "Please input the class name (or input q to quit): ";
        cin >> sbuf2;
        if (sbuf2 == "q")
            break;
        cl = nullptr;
        for (vector<Class*>::iterator it = classVec.begin();
             it != classVec.end();
             ++it) {
            if ((*it)->name == sbuf2) {
                cl = *it;
                break;
            }
        }
        if (cl == nullptr) {
            cerr << "No Match Class!" << endl;
            continue;
        }
        while (true) {
            cout << "Please input the id and score (or input q to quit): "<<endl;
            cin >> sbuf;
            if (sbuf == "q")break;
            pos1=0;
            pos2=sbuf.find(',',pos1+1);
            if(pos2==string::npos)cerr<<"Wrong Input"<<endl;
            id0=sbuf.substr(pos1,pos2);
            try{
                StudentWrapper& st = cl->getStudentWrapper(id0);
                string str0=sbuf.substr(pos2+1,string::npos);
                score0=stod(str0);
                if (score0 < 0.0 || score0 > 100.0) {  // 检查分数范围
                        throw "Wrong Score";
                    }
                st.setScore(score0);

            }catch (string& f) {
                cerr<<f<<endl;
                continue;
            }catch (char const* e){
                cerr << e << endl;
                continue;
            } catch (...) {
                cerr << "Unknown exception caught" << endl;
                continue;
            }
        }
    }
}

void AppX::printScoreStats()
{
    string sbuf;
    Class *cl;
    double highest, lowest, avg;

    while (true) {
        cout << "Please input the class name (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q")
            break;

        cl = nullptr;
        for (vector<Class*>::iterator it = classVec.begin();
             it != classVec.end();
             ++ it) {
            if ((*it)->name == sbuf) {
                cl = *it;
                break;
            }
        }
        if (cl == nullptr) {
            cerr << "No Match Class!" << endl;
            continue;
        }

        try {
            highest = cl->getHighestScore();
            if (highest == -1) throw "No Valid Score!";
            lowest = cl->getLowestScore();
            if (lowest == 101) throw "No Valid Score!";
            avg = cl->getAvgScore();
            if (avg==-1) throw "No Valid Score!";

            cout << cl->toString() << endl;
            cout << setiosflags(ios::fixed) << setprecision(2)<< "Highest,Lowest,Avg = " << highest << "," << lowest << "," << avg << endl;
        } catch (const string& e) {
            cerr << e << endl;
        } catch (char const* e){
            cerr << e << endl;
        } catch (...) {
            cerr << "Unknown exception caught" << endl;
        }
    }
}

void AppX::printGrade()
{
    // TODO: implement printGrade.
    // Hint: Take a look at printScoreStats().
    string sbuf;
    double GPA=0.0, AVG = 0.0;
    while (true) {
        cout << "Please input the id (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q")
            break;

        Student* st = nullptr;
        for (vector<Student*>::iterator it = studentVec.begin();
             it != studentVec.end();
             ++it) {
            if ((*it)->id == sbuf) {
                st = *it;
                break;
            }
        }
        if (st == nullptr) {
            cerr << "No Match Student!" << endl;
            continue;
        }
        try {
            GPA = st->getGpa();
            AVG = st->getAvgScore();

            cout << st->toString() << endl;
            cout << setiosflags(ios::fixed) << setprecision(2) << "GPA,AVG = " << GPA << "," << AVG << endl;
        }
        catch (string& e) {
            cerr << e << endl;
        }
        catch (char const* e) {
            cerr << e << endl;
        }
        catch (...) {
            cerr << "Unknown exception caught" << endl;
        }
    }
}

void AppX::saveScore() {
    // TODO: implement saveScore.
    std::ofstream outFile("./scores.txt");

    if (!outFile) {
        std::cerr << "无法打开文件！" << std::endl;
        return;
    }

    // 写入数组内容
    for (int i = 0; i < classVec.size(); i++) {
        classVec[i]->saveScore("./scores.txt");
    }

    // 关闭文件
    outFile.close();
}

int AppX::run()
{
    char cmd;
    while (true) {
        cout << "Command menu:\n"
             << "\ti: Input score\n"
             << "\ta: Compute score statistics of a class\n"
             << "\tg: Compute grade of a student\n"
             << "\tq: Quit\n"
             << "Please input the command: ";
        cin >> cmd;
        if (cmd == 'i') {
            inputScore();
        } else if (cmd == 'a') {
            printScoreStats();
        } else if (cmd == 'g') {
            printGrade();
        } else if (cmd == 'q') {
            saveScore();
            break;
        } else {
            cout << "Invalid command!\n" << endl;
        }
    }
    return 0;
}

int main()
{
    AppX app;
    return app.run();
}