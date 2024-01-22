#include "SafeUnorderedMap.hpp"

namespace exng {

    template<typename K, typename V>
    SafeUnorderedMap<K, V>::SafeUnorderedMap() = default;

    template<typename K, typename V>
    SafeUnorderedMap<K, V>::~SafeUnorderedMap() = default;

    template<typename K, typename V>
    void SafeUnorderedMap<K, V>::insert(const K &key, const V &value)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_map.insert(std::make_pair(key, value));
        lock.unlock();
        m_condition.notify_one();
    }

    template<typename K, typename V>
    void SafeUnorderedMap<K, V>::erase(const K &key)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_map.erase(key);
        lock.unlock();
        m_condition.notify_one();
    }

    template<typename K, typename V>
    V &SafeUnorderedMap<K, V>::operator[](const K &key)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_map[key];
    }

    template<typename K, typename V>
    bool SafeUnorderedMap<K, V>::contains(const K &key)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_map.contains(key);
    }

    template<typename K, typename V>
    V &SafeUnorderedMap<K, V>::at(const K &key)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_map.at(key);
    }

    template<typename K, typename V>
    void SafeUnorderedMap<K, V>::clear()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_map.clear();
        lock.unlock();
        m_condition.notify_one();
    }

    template<typename K, typename V>
    bool SafeUnorderedMap<K, V>::empty()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_map.empty();
    }

    template<typename K, typename V>
    size_t SafeUnorderedMap<K, V>::size()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_map.size();
    }

    template<typename K, typename V>
    typename std::unordered_map<K, V>::iterator SafeUnorderedMap<K, V>::find(const K &key)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_map.find(key);
    }

    template<typename K, typename V>
    typename std::unordered_map<K, V>::iterator SafeUnorderedMap<K, V>::end()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_map.end();
    }
}