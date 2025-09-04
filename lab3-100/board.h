#ifndef BOARD_H
#define BOARD_H

#include "disk.h"
#include "rod.h"
#include "stack.h"
#include <unordered_set>


constexpr static size_t ROD_SIZE = 3;

class Board {
    const int num_disk;
    Rod rods[ROD_SIZE];
    Disk *disks;
    Stack<std::pair<int, int>> history;
    std::unordered_set<std::string> optimal_states;


public:
    explicit Board(int num_disk);
    ~Board();
    void draw();
    void move(int from, int to, bool log = true);
    [[nodiscard]] bool win() const;
    void autoplay();
    void hint();
    std::string toString();
};

#endif