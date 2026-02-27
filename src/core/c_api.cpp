#include "max_heap.hpp"
#include "max_heap_c_api.h"

#include <new>
#include <vector>

using maxheap::MaxHeap;

extern "C" {

void* mh_create(size_t capacity) {
    if (capacity == 0) {
        return nullptr;
    }

    try {
        return new MaxHeap(capacity);
    } catch (const std::bad_alloc&) {
        return nullptr;
    }
}

void mh_destroy(void* handle) {
    if (handle == nullptr) {
        return;
    }

    delete static_cast<MaxHeap*>(handle);
}

int mh_insert(void* handle, int value) {
    if (handle == nullptr) {
        return MH_ERR_NULL;
    }

    MaxHeap* heap = static_cast<MaxHeap*>(handle);
    return heap->insert(value) ? MH_OK : MH_ERR_FULL;
}

int mh_delete_max(void* handle, int* out_value) {
    if (handle == nullptr || out_value == nullptr) {
        return MH_ERR_NULL;
    }

    MaxHeap* heap = static_cast<MaxHeap*>(handle);
    auto value = heap->delete_max();
    if (!value.has_value()) {
        return MH_ERR_EMPTY;
    }

    *out_value = value.value();
    return MH_OK;
}

int mh_peek(void* handle, int* out_value) {
    if (handle == nullptr || out_value == nullptr) {
        return MH_ERR_NULL;
    }

    MaxHeap* heap = static_cast<MaxHeap*>(handle);
    auto value = heap->peek_max();
    if (!value.has_value()) {
        return MH_ERR_EMPTY;
    }

    *out_value = value.value();
    return MH_OK;
}

size_t mh_size(void* handle) {
    if (handle == nullptr) {
        return 0;
    }

    return static_cast<MaxHeap*>(handle)->size();
}

size_t mh_capacity(void* handle) {
    if (handle == nullptr) {
        return 0;
    }

    return static_cast<MaxHeap*>(handle)->capacity();
}

int mh_resize(void* handle, size_t new_capacity) {
    if (handle == nullptr) {
        return MH_ERR_NULL;
    }

    if (new_capacity == 0) {
        return MH_ERR_INVALID;
    }

    MaxHeap* heap = static_cast<MaxHeap*>(handle);
    return heap->resize_capacity(new_capacity) ? MH_OK : MH_ERR_CAPACITY;
}

void mh_clear(void* handle) {
    if (handle == nullptr) {
        return;
    }

    static_cast<MaxHeap*>(handle)->clear();
}

int mh_build(void* handle, const int* values, size_t count) {
    if (handle == nullptr) {
        return MH_ERR_NULL;
    }

    if (count > 0 && values == nullptr) {
        return MH_ERR_INVALID;
    }

    MaxHeap* heap = static_cast<MaxHeap*>(handle);
    std::vector<int> items(values, values + count);
    return heap->build_from(items) ? MH_OK : MH_ERR_CAPACITY;
}

size_t mh_export(void* handle, int* out_values, size_t max_count) {
    if (handle == nullptr || out_values == nullptr || max_count == 0) {
        return 0;
    }

    MaxHeap* heap = static_cast<MaxHeap*>(handle);
    const auto& values = heap->array();
    const size_t copy_count = values.size() < max_count ? values.size() : max_count;

    for (size_t i = 0; i < copy_count; ++i) {
        out_values[i] = values[i];
    }

    return copy_count;
}

}  // extern "C"
