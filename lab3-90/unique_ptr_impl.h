#ifndef UNIQUE_PTR_IMPL_H
#define UNIQUE_PTR_IMPL_H

// You need to implement follow functions, signatures are provided.
// NOTE: DON'T change the function definition

template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr &&other) noexcept : pointer (other.pointer ) {
    other.pointer = nullptr; }

template <typename T>
UniquePtr<T>::~UniquePtr() {
    if (pointer != nullptr) {
        delete pointer;
        pointer = nullptr;
    }
}

template <typename T>
UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr &&other) noexcept {
    if (this != &other) {
        //delete pointer;
        pointer = other.pointer;
        other.pointer = nullptr;
        }
    //std::swap(pointer, other.pointer);
    //other.pointer= nullptr;
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
    if (pointer != ptr) {
        if(pointer!=nullptr){
            delete pointer;
        }
    pointer = ptr;}
}

template <typename T>
T *UniquePtr<T>::release() noexcept {
    T* tmp = pointer;
    pointer = nullptr;
    return tmp;
}

#endif  // UNIQUE_PTR_IMPL_H