/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** SafeDequeue
*/

#ifndef EXNG_SAFEDEQUEUE_HPP
#define EXNG_SAFEDEQUEUE_HPP

#include <deque>
#include <mutex>
#include <condition_variable>

namespace exng {

    template<typename T>
    class SafeDequeue {
    public:
        SafeDequeue();
        ~SafeDequeue();

        // front is the oldest element, back is the newest
        T front();
        T back();
        void push(T value);
        T pop();
        bool empty();

        typename std::deque<T>::iterator begin();
        typename std::deque<T>::iterator end();


    private:
        std::deque<T> m_deque;
        std::mutex m_mutex;
        std::condition_variable m_condition;
    };

}

#include "SafeDequeue.inl"

#endif //EXNG_SAFEQUEUE_HPP