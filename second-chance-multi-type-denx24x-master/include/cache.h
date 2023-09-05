#pragma once

#include <algorithm>
#include <cstddef>
#include <list>
#include <new>
#include <ostream>
#include <type_traits>

template <class Key, class KeyProvider, class Allocator>
class Cache
{
    static_assert(std::is_constructible_v<KeyProvider, const Key &>, "KeyProvider has to be constructible from Key");
    struct CacheElement
    {
        KeyProvider * element;
        bool flag = false;
        CacheElement(KeyProvider * val)
            : element(val)
        {
        }

        template <class T>
        T & get() const
        {
            return *static_cast<T *>(this->element);
        }
    };

public:
    template <class... AllocArgs>
    Cache(const std::size_t cache_size, AllocArgs &&... alloc_args)
        : m_max_size(cache_size)
        , m_alloc(std::forward<AllocArgs>(alloc_args)...)
    {
    }

    std::size_t size() const
    {
        return m_queue.size();
    }

    bool empty() const
    {
        return m_queue.empty();
    }

    template <class T>
    T & get(const Key & key);

    std::ostream & print(std::ostream & strm) const;

    friend std::ostream & operator<<(std::ostream & strm, const Cache & cache)
    {
        return cache.print(strm);
    }

private:
    const std::size_t m_max_size;
    std::list<CacheElement> m_queue;
    AllocatorWithPool m_alloc;
};

template <class Key, class KeyProvider, class Allocator>
template <class T>
inline T & Cache<Key, KeyProvider, Allocator>::get(const Key & key)
{
    static_assert(std::is_base_of_v<KeyProvider, T>, "Key has to be the base class of KeyProvider");
    auto it = std::find_if(m_queue.begin(), m_queue.end(), [&key](const CacheElement ptr) {
        return *ptr.element == key;
    });
    if (it != m_queue.end()) {
        it->flag = true;
        return it->template get<T>();
    }

    while (m_queue.size() == m_max_size) {
        CacheElement val = m_queue.back();
        if (val.flag) {
            val.flag = false;
            m_queue.push_front(val);
        }
        else {
            m_alloc.destroy<KeyProvider>(val.element);
        }
        m_queue.pop_back();
    }
    T * added = m_alloc.create<T>(key);
    m_queue.emplace_front(added);
    return *added;
}

template <class Key, class KeyProvider, class Allocator>
inline std::ostream & Cache<Key, KeyProvider, Allocator>::print(std::ostream & strm) const
{
    if (m_queue.size() > 0) {
        bool first = true;
        for (const auto ptr : m_queue) {
            if (!first) {
                strm << " ";
            }
            else {
                first = false;
            }
            strm << "(" << *ptr.element << " " << ptr.flag << ")";
        }
        return strm << "\n";
    }
    else {
        return strm << "<empty>\n";
    }
}
