#ifndef QUEUE_IMPL_H
#define QUEUE_IMPL_H

template <typename T>
void Queue<T>::push(T t) {
    UniquePtr<Node<T>> tmp = MakeUnique<Node<T>>(std::move(t));
    if( head==nullptr ){
        head = std::move(tmp);
        tail = head.get();
    }else{
        Node<T>* raw = tmp.get();
        tail->next = std::move(tmp);
        tail = raw;
    }
    sz += 1;
}

template <typename T>
void Queue<T>::pop() {
    if(!head) {
        throw std::out_of_range("Queue underflow: No elements to pop");
    }
    head = std::move(head->next);
    if(head.get()==nullptr) tail=nullptr;
    sz-=1;
}

template <typename T>
T &Queue<T>::front() {
    if(!head) {
        throw std::out_of_range("Queue underflow: No valid elements");
    }
    return head->val;
}

template <typename T>
bool Queue<T>::empty() const {
    return sz==0;
}

template <typename T>
size_t Queue<T>::size() const {
    return sz;
}
#endif  // QUEUE_IMPL_H