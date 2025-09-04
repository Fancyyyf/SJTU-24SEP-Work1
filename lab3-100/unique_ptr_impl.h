#ifndef UNIQUE_PTR_IMPL_H
#define UNIQUE_PTR_IMPL_H

// You need to implement follow functions, signatures are provided.
// NOTE: DON'T change the function definition

template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr &&other) noexcept : pointer (other.pointer ) {
    other.pointer = nullptr; }

template <typename T>
UniquePtr<T>::~UniquePtr() {
        delete pointer;
}

template <typename T>
UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr &&other) noexcept {

    if (this != &other) {
        delete pointer;
        pointer = other.pointer;
        other.pointer = nullptr;
    }
        //reset(other.release());
    return *this;
}

template <typename T>
UniquePtr<T> &UniquePtr<T>::operator=(std::nullptr_t) noexcept {
    delete pointer;
    pointer = nullptr;
    return *this;
}

template <typename T>
void UniquePtr<T>::reset(T *ptr) noexcept {
    /*if(ptr!= nullptr&&pointer->next!= nullptr&&pointer->next.get()==ptr){
        T* tmp = release();
        T* tmp2 =tmp->next.get();
        delete tmp;
        pointer = tmp2;
    }else if (pointer != ptr) {
        delete pointer;
        pointer = ptr;}*/
    delete pointer;
    pointer = ptr;
}

template <typename T>
T *UniquePtr<T>::release() noexcept {
    T* tmp = pointer;
    pointer = nullptr;
    return tmp;
}

#endif  // UNIQUE_PTR_IMPL_H