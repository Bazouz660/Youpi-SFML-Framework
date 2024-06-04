/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** Observer
*/

#ifndef YPI_Observer_HPP
#define YPI_Observer_HPP

#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

namespace ypi {

    template<typename T>
    class AObserver;

    template<typename T>
    class Subject {
    public:
        void addObserver(const std::shared_ptr<AObserver<T>>& observer) {
            m_observers.push_back(observer);
        }

        void addObserver(AObserver<T>* observer) {
            m_observers.push_back(std::shared_ptr<AObserver<T>>(observer, [](AObserver<T>*) {}));
        }

        void removeObserver(const std::shared_ptr<AObserver<T>>& observer) {
            m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
        }

        void removeObserver(AObserver<T>* observer) {
            m_observers.erase(std::remove_if(m_observers.begin(), m_observers.end(), [observer](const std::shared_ptr<AObserver<T>>& obs) {
                return obs.get() == observer;
            }), m_observers.end());
        }

        void notify(T event) {
            for (auto &observer : m_observers) {
                observer->onNotify(event);
            }
        }

    private:
        std::vector<std::shared_ptr<AObserver<T>>> m_observers;
    };

    template<typename T>
    class AObserver {
    public:
        virtual void onNotify(T event) = 0;
    };

    template<typename T>
    class LambdaObserver : public AObserver<T> {
    public:
        LambdaObserver(std::function<void(T)> lambda) : m_lambda(lambda) {}

        virtual void onNotify(T event) override {
            m_lambda(event);
        }

    private:
        std::function<void(T)> m_lambda = nullptr;
    };

}

#endif // !YPI_Observer_HPP