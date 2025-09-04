#ifndef QUEUE_IMPL_H
#define QUEUE_IMPL_H

template <typename T>
void Queue<T>::push(T t) {
    UniquePtr<Node<T>> tmp = MakeUnique<Node<T>>(std::move(t));
    if( head==nullptr ){
        head.reset(tmp.release());
        tail = head.get();
    }else{
        tail->next.reset(tmp.release());
        tail = tail->next.get();
    }
    sz += 1;
}

template <typename T>
void Queue<T>::pop() {
    if(!head) {
        throw std::out_of_range("Queue underflow: No elements to pop");
    }
    Node<T>* tmp2 = head.release();
    if(tmp2->next== nullptr){
        head.reset(nullptr);
        tail=nullptr;
    }else{
        head.reset(tmp2->next.get());
    }
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