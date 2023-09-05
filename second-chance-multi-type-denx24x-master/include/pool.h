#pragma once

#include <functional> // std::less_equal
#include <initializer_list>
#include <vector>

class PoolAllocator
{
public:
    PoolAllocator(const std::size_t block_size, std::initializer_list<std::size_t> sizes);
    void * allocate(const std::size_t n);
    void deallocate(const void * ptr);

private:
    const std::size_t m_blocks_count;
    const std::size_t m_block_size;
    std::vector<std::size_t> m_sizes;
    std::vector<std::byte> m_storage;
    std::vector<std::vector<bool>> m_aviable_memory;
};
