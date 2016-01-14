#include "pool.h"

template<typename T>
Pool<T>::Pool(std::function<T ()> f)
{
    this->f = f;
}

template<typename T>
T Pool<T>::obtain()
{
    if(objects.size() == 0) {
        objects.push(f());
    }
    return objects.pop();
}

template<typename T>
T Pool<T>::clone(T object)
{
    return object.clone();
}

template<typename T>
void Pool<T>::release(T object)
{
//    delete object;
//    if(!objects.contains(object)) {
//        objects.push(object);
//    }
}


