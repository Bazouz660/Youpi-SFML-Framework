/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** SafeDequeue
*/

#include "SafeDequeue.hpp"

template<typename T>
ypi::SafeDequeue<T>::SafeDequeue() = default;

template<typename T>
ypi::SafeDequeue<T>::~SafeDequeue() = default;

template<typename T>
void ypi::SafeDequeue<T>::push(T value)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_deque.push_back(value);
    lock.unlock();
    m_condition.notify_one();
}

template<typename T>
T ypi::SafeDequeue<T>::pop()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_condition.wait(lock, [this] { return !m_deque.empty(); });
    T value = m_deque.front();
    m_deque.pop_front();
    return value;
}

template<typename T>
bool ypi::SafeDequeue<T>::empty()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_deque.empty();
}

template<typename T>
T ypi::SafeDequeue<T>::front()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_deque.front();
}

template<typename T>
T ypi::SafeDequeue<T>::back()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_deque.back();
}

template<typename T>
typename std::deque<T>::iterator ypi::SafeDequeue<T>::begin()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_deque.begin();
}

template<typename T>
typename std::deque<T>::iterator ypi::SafeDequeue<T>::end()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_deque.end();
}