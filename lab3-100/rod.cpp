#include "rod.h"
#include <cstddef>

Rod::Rod(const int capacity, const int id) :capacity(capacity),id(id){ }

bool Rod::push(const Disk d) {
    if(stack.size()>=capacity){
        throw std::out_of_range("Full Rod");
    }
    stack.push(d);
    return true;
}

const Disk &Rod::top() {
    if(stack.empty()) throw std::out_of_range("Empty Rod");
    return stack.top();
}

void Rod::pop() {
    if(stack.empty()) throw std::out_of_range("Empty Rod");
    stack.pop();
}

size_t Rod::size() const {
    return stack.size();
}

bool Rod::empty() const {
    return stack.empty();
}

bool Rod::full() const {
    return stack.size()==capacity;
}

void Rod::draw(Canvas &canvas) {
    Stack<const Disk> aid;
    int level = static_cast<int>(stack.size()) - 1;
    while (!stack.empty()) {
        const Disk tmp = stack.top();
        tmp.draw(canvas, level, id);
        aid.push(tmp);
        stack.pop();
        level--;
    }
    while(!aid.empty()){
        stack.push(aid.top());
        aid.pop();

    }
}

std::string Rod::toString() {
    std::string result = "Rod " + std::to_string(id) + ": [";
    Stack<const Disk> aid;
    while (!stack.empty()) {
        const Disk tmp = stack.top();
        result += "Disk(id=" + std::to_string(tmp.id)
                + ", val=" + std::to_string(tmp.val) + ")";
        aid.push(tmp);
        stack.pop();
        if (!stack.empty()) result += ", ";
    }
    while(!aid.empty()){
        const Disk tmp = aid.top();
        aid.pop();
        stack.push(tmp);
    }
    result += "]";
    return result;
}