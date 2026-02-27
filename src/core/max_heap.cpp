#include "max_heap.hpp"

#include <algorithm>

namespace maxheap {

MaxHeap::MaxHeap(std::size_t capacity)
    : data_{},
      capacity_{capacity == 0 ? 1 : capacity} {
    data_.reserve(capacity_);
}

bool MaxHeap::insert(int value) {
    if (data_.size() >= capacity_) {
        return false;
    }

    data_.push_back(value);
    heapify_up(data_.size() - 1);
    return true;
}

std::optional<int> MaxHeap::delete_max() {
    if (data_.empty()) {
        return std::nullopt;
    }

    const int root = data_.front();

    if (data_.size() == 1) {
        data_.pop_back();
        return root;
    }

    data_.front() = data_.back();
    data_.pop_back();
    heapify_down(0);
    return root;
}

std::optional<int> MaxHeap::peek_max() const {
    if (data_.empty()) {
        return std::nullopt;
    }

    return data_.front();
}

void MaxHeap::clear() {
    data_.clear();
}

bool MaxHeap::resize_capacity(std::size_t new_capacity) {
    if (new_capacity == 0 || new_capacity < data_.size()) {
        return false;
    }

    capacity_ = new_capacity;
    data_.reserve(capacity_);
    return true;
}

bool MaxHeap::build_from(const std::vector<int>& values) {
    if (values.size() > capacity_) {
        return false;
    }

    data_ = values;
    if (data_.empty()) {
        return true;
    }

    for (std::size_t i = data_.size() / 2; i > 0; --i) {
        heapify_down(i - 1);
    }

    return true;
}

std::size_t MaxHeap::size() const noexcept {
    return data_.size();
}

std::size_t MaxHeap::capacity() const noexcept {
    return capacity_;
}

const std::vector<int>& MaxHeap::array() const noexcept {
    return data_;
}

void MaxHeap::heapify_up(std::size_t index) {
    while (index > 0) {
        const std::size_t parent = (index - 1) / 2;
        if (data_[index] <= data_[parent]) {
            break;
        }

        std::swap(data_[index], data_[parent]);
        index = parent;
    }
}

void MaxHeap::heapify_down(std::size_t index) {
    const std::size_t count = data_.size();

    while (true) {
        const std::size_t left = (2 * index) + 1;
        const std::size_t right = left + 1;
        std::size_t largest = index;

        if (left < count && data_[left] > data_[largest]) {
            largest = left;
        }

        if (right < count && data_[right] > data_[largest]) {
            largest = right;
        }

        if (largest == index) {
            break;
        }

        std::swap(data_[index], data_[largest]);
        index = largest;
    }
}

}  // namespace maxheap
