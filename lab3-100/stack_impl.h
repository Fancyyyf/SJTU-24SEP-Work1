#ifndef STACK_IMPL_H
#define STACK_IMPL_H
#include <cassert>

template <typename T>
void Stack<T>::push(T t) {
    UniquePtr<Node<T>>tmp=MakeUnique<Node<T>>(std::move(t));
    tmp->next.reset( head.release());
    head.reset(tmp.release());
    sz += 1;
}

template <typename T>
void Stack<T>::pop() {
    if(!head) {
            throw std::out_of_range("Stack underflow: No elements to pop");
        }
    Node<T>* tmp2 = head.release();
    head.reset(tmp2->next.get());
    sz-=1;
}

template <typename T>
T &Stack<T>::top() {
    if(!head) {
        throw std::out_of_range("Stack underflow: No valid elements");
    }
    return head->val;
}

template <typename T>
bool Stack<T>::empty() const {
    return head==nullptr;
}

template <typename T>
size_t Stack<T>::size() const {
    return sz;
}

#endif  // STACK_IMPL_H