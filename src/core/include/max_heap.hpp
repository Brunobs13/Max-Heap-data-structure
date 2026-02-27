#ifndef MAX_HEAP_HPP
#define MAX_HEAP_HPP

#include <cstddef>
#include <optional>
#include <vector>

namespace maxheap {

class MaxHeap {
public:
    explicit MaxHeap(std::size_t capacity = 15);

    bool insert(int value);
    std::optional<int> delete_max();
    std::optional<int> peek_max() const;

    void clear();
    bool resize_capacity(std::size_t new_capacity);
    bool build_from(const std::vector<int>& values);

    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    const std::vector<int>& array() const noexcept;

private:
    void heapify_up(std::size_t index);
    void heapify_down(std::size_t index);

    std::vector<int> data_;
    std::size_t capacity_;
};

}  // namespace maxheap

#endif
