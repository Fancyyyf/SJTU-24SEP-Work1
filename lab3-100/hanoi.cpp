#include <climits>
#include <iostream>
#include <string>
#include <sstream>

#include "board.h"

using namespace std;

int main() {
    while (true) {
        cout << "How many disks do you want? (1 ~ 5)" << endl;
        string input;
        getline(cin, input);
        if (input == "Q") {
            break;
        }
        if(input!="1"&&input!="2"&&input!="3"&&input!="4"&&input!="5") {
            continue; }
        Board board(stoi(input));
        board.draw();
        while(!board.win()){
            cout<<"Move a disk. Format: x y (or 'H' for a hint)"<<endl;
            string input0;
            getline(cin, input0);
            std::istringstream iss(input0);
            string s1, s2;
            iss >> s1 >> s2;
            try {
                if (s1 == "H") {
                    board.hint();
                    continue;
                }
                if (s1 == "Q") {
                    break;
                }
                if (s1 == "0" && s2 == "0") {
                    board.autoplay();
                    continue;
                }
                if (s1.size() == 1 && s2.size() == 1 &&
                    s1[0] >= '1' && s1[0] <= '3' &&
                    s2[0] >= '1' && s2[0] <= '3') {
                    int from = s1[0] - '1';
                    int to = s2[0] - '1';
                    board.move(from, to, true);
                    continue;
                }

                board.draw();  // 如果无效输入，重绘一下
            }catch(out_of_range& e){
                //cerr<<e.what()<<endl;
                board.draw();
                continue;
            }catch(logic_error& e){
                //cerr<<e.what()<<endl;
                board.draw();
                continue;
            }
        }
        cout<<" Congratulations! You win!"<<endl;
    }

    return 0;
}
