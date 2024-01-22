/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** Observer
*/

#ifndef EXNG_OBSERVER_HPP
#define EXNG_OBSERVER_HPP

#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

namespace exng {

    template<typename T>
    class Observer;

    template<typename T>
    class Subject {
    public:
        void addObserver(std::shared_ptr<Observer<T>> observer) {
            m_observers.push_back(observer);
        }

        void removeObserver(std::shared_ptr<Observer<T>> observer) {
            m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
        }

        void notify(T &event) {
            for (auto &observer : m_observers) {
                observer->onNotify(event);
            }
        }

    private:
        std::vector<std::shared_ptr<Observer<T>>> m_observers;
    };

    template<typename T>
    class Observer {
    public:
        virtual void onNotify(T &event) = 0;
    };

}

#endif // !EXNG_OBSERVER_HPP