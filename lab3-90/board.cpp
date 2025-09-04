#include <iostream>

#include "board.h"
#include "queue.h"

void generateOptimalStates(Board& tmp, int n, int from, int to, int aux, std::unordered_set<std::string>& aid) {
    if (n == 0) return;

    // 第一步：将 n-1 个盘子从 from → aux
    generateOptimalStates(tmp, n - 1, from, aux, to, aid);

    // 插入递归左边结束后的状态
    aid.insert(tmp.toString());

    // 第二步：将第 n 个盘子从 from → to
    tmp.move(from, to, false); // 不记录历史，只改变状态
    aid.insert(tmp.toString()); // 插入移动大盘子之后的状态

    // 第三步：将 n-1 个盘子从 aux → to
    generateOptimalStates(tmp, n - 1, aux, to, from, aid);

    // 插入递归右边结束后的状态
    aid.insert(tmp.toString());
}


void solve(
        const int n,
        const int src,
        const int buf,
        const int dest,
        Queue<std::pair<int, int>> &solution) {

            if (n == 0) return;

            solve(n - 1, src, dest, buf, solution);
            solution.push({src, dest});
            solve(n - 1, buf, src, dest, solution);
}


Board::Board(const int num_disk) :num_disk(num_disk),
                                  rods{ Rod(num_disk, 0), Rod(num_disk, 1), Rod(num_disk, 2) },
                                  disks(new Disk[num_disk]) {
    for (int i = num_disk - 1; i >= 0; --i) {
        disks[i] = Disk(i, (i + 1) * 2 + 1);
        rods[0].push(disks[i]);
    }
}

Board::~Board() {
    delete[] disks;
}

void Board::draw() {
    Canvas canvas {};
    canvas.reset();
    for(int p=0;p<canvas.WIDTH;p++){
        canvas.buffer[10][p] = '-';
    }
    for(int i=0;i<3;i++){
        const int s_x = 5 + (i * 15) ;
        for (int j = 0; j < canvas.HEIGHT; j++) {
            canvas.buffer[j][s_x] = '|';
        }
    }
    for (Rod &rod : rods) {
        rod.draw(canvas);
    }
    canvas.draw();
}

void Board::move(int from, int to, const bool log) {
    if (from < 0 || from >= ROD_SIZE || to < 0 || to >= ROD_SIZE) {
        throw std::out_of_range("Rod index out of range.");
    }

    Rod &tmp1 = rods[from];
    Rod &tmp2 = rods[to];

    if (tmp1.empty()) {
        throw std::logic_error("Cannot move from an empty rod.");
    }

    const Disk &disk = tmp1.top();

    if (log && !tmp2.empty() && tmp2.top().val < disk.val) {
        throw std::logic_error("Cannot move a larger disk onto a smaller one.");
    }

    // 执行移动
    tmp1.pop();
    tmp2.push(disk);
    if (log) {
        history.push({from, to});
        draw();
    }
}

bool Board::win() const {
    return rods[1].size() == static_cast<size_t>(num_disk);
}


void Board::autoplay() {
    while(!history.empty()) {
        std::pair<int, int> move_pair = history.top();
        history.pop();
        auto [tmp1, tmp2] = move_pair;
        std::cout<<" Auto moving:"<<tmp2+1<<"->"<<tmp1+1<<std::endl;
        move(tmp2,tmp1,false);
        draw();
    }
    Queue<std::pair<int, int>> solution;
    solve(num_disk,0,2,1,solution);
    while(!solution.empty()){
        std::pair<int, int> move_pair = solution.front();
        solution.pop();
        auto [tmp1, tmp2] = move_pair;
        std::cout<<" Auto moving:"<<tmp1+1<<"->"<<tmp2+1<<std::endl;
        move(tmp1,tmp2);
    }
}

void Board::hint() {

    std::unordered_set<std::string> aid;
    aid.insert(toString()); // 初始状态

    Board tmp(num_disk);  // 初始棋盘
    generateOptimalStates(tmp, num_disk, 0, 2, 1, aid);

    optimal_states = std::move(aid);


    std::string tmp_str = toString();
    auto it =optimal_states.find(tmp_str);
    if (it != optimal_states.end()) {
        std::cout << " You are on the optimal path!" << std::endl;
    } else {
        std::cout << "Sorry. You are not on the optimal path." << std::endl;
    }
    draw();
}

std::string Board::toString() {
    std::string state;
    for (Rod &rod : rods) {
        state += rod.toString();
    }
    return state;
}