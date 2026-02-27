#ifndef MAX_HEAP_C_API_H
#define MAX_HEAP_C_API_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

enum MhStatus {
    MH_OK = 0,
    MH_ERR_NULL = 1,
    MH_ERR_FULL = 2,
    MH_ERR_EMPTY = 3,
    MH_ERR_INVALID = 4,
    MH_ERR_CAPACITY = 5,
};

void* mh_create(size_t capacity);
void mh_destroy(void* handle);

int mh_insert(void* handle, int value);
int mh_delete_max(void* handle, int* out_value);
int mh_peek(void* handle, int* out_value);

size_t mh_size(void* handle);
size_t mh_capacity(void* handle);

int mh_resize(void* handle, size_t new_capacity);
void mh_clear(void* handle);
int mh_build(void* handle, const int* values, size_t count);

size_t mh_export(void* handle, int* out_values, size_t max_count);

#ifdef __cplusplus
}
#endif

#endif
