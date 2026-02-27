#include "max_heap.hpp"

#include <cassert>
#include <iostream>
#include <vector>

using maxheap::MaxHeap;

void test_insert_and_peek() {
    MaxHeap heap(5);
    assert(heap.insert(7));
    assert(heap.insert(22));
    assert(heap.insert(1));

    auto top = heap.peek_max();
    assert(top.has_value());
    assert(top.value() == 22);
}

void test_delete_max_order() {
    MaxHeap heap(10);
    for (int value : {3, 40, 15, 22, 5}) {
        assert(heap.insert(value));
    }

    std::vector<int> extracted;
    while (heap.size() > 0) {
        auto value = heap.delete_max();
        assert(value.has_value());
        extracted.push_back(value.value());
    }

    assert((extracted == std::vector<int>{40, 22, 15, 5, 3}));
}

void test_build_and_resize() {
    MaxHeap heap(4);
    assert(!heap.build_from({10, 8, 7, 6, 5}));
    assert(heap.build_from({10, 8, 7, 6}));
    assert(!heap.resize_capacity(2));
    assert(heap.resize_capacity(8));
    assert(heap.capacity() == 8);
}

int main() {
    test_insert_and_peek();
    test_delete_max_order();
    test_build_and_resize();

    std::cout << "all max heap tests passed\n";
    return 0;
}
