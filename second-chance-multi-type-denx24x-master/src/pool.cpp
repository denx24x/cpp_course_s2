#include <pool.h>

PoolAllocator::PoolAllocator(const std::size_t block_size, std::initializer_list<std::size_t> sizes)
    : m_blocks_count(sizes.size())
    , m_block_size(block_size)
    , m_storage(block_size * sizes.size())
{
    m_aviable_memory.resize(m_blocks_count);
    m_sizes.insert(m_sizes.end(), sizes.begin(), sizes.end());
    std::sort(m_sizes.begin(), m_sizes.end());
    for (std::size_t i = 0; i < m_blocks_count; i++) {
        m_aviable_memory[i].resize(m_block_size / m_sizes[i], true);
    }
}

void * PoolAllocator::allocate(const std::size_t n)
{
    std::size_t start = std::lower_bound(m_sizes.begin(), m_sizes.end(), n) - m_sizes.begin();
    for (std::size_t i = start; i < m_blocks_count && m_sizes[i] == n; i++) {
        std::vector<bool> & current = m_aviable_memory[i];
        std::size_t pos = std::find(current.begin(), current.end(), true) - current.begin();
        if (pos != current.size()) {
            current[pos] = false;
            return &m_storage[i * m_block_size + pos * m_sizes[i]];
        }
    }
    throw std::bad_alloc{};
}

void PoolAllocator::deallocate(const void * ptr)
{
    auto b_ptr = static_cast<const std::byte *>(ptr);
    const auto begin = m_storage.data();
    std::less_equal<const std::byte *> cmp;
    if (cmp(b_ptr, &m_storage.back()) && cmp(begin, b_ptr)) {
        const std::size_t offset = b_ptr - begin;
        const std::size_t block = offset / m_block_size;
        m_aviable_memory[block][offset % m_block_size / m_sizes[block]] = true;
    }
}
