/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** SafeUnorderedMap
*/

#ifndef EXNG_SAFEUNORDEREDMAP_HPP
#define EXNG_SAFEUNORDEREDMAP_HPP

#include <unordered_map>
#include <mutex>
#include <condition_variable>

namespace exng {

    template<typename K, typename V>
    class SafeUnorderedMap {
        public:
            SafeUnorderedMap();
            ~SafeUnorderedMap();

            void insert(const K &key, const V &value);
            void erase(const K &key);
            V &operator[](const K &key);
            bool contains(const K &key);
            V &at(const K &key);
            void clear();
            bool empty();
            size_t size();

            typename std::unordered_map<K, V>::iterator find(const K &key);
            typename std::unordered_map<K, V>::iterator end();

        private:
            std::unordered_map<K, V> m_map;
            std::mutex m_mutex;
            std::condition_variable m_condition;
    };

}

#include "SafeUnorderedMap.inl"

#endif //EXNG_SAFEUNORDEREDMAP_HPP