# Static Pool
Very simple static memory allocator for embedded systems. Can be used for dinamic fixed size blocks allocation.

## Usage
Define the static memory pool
```cpp
STATIC_POOL_DEFINE(test, TEST_POOL_SIZE, test_type_t, NULL, NULL);
```

Initialize pool. Use `pSTATIC_POOL` macro to pick the pool by it's name
```cpp
static_pool_init(pSTATIC_POOL(test));
```

Allocate fixed size block
```cpp
test_type_t *t = static_pool_alloc(pSTATIC_POOL(test));
```

Free allocated block
```cpp
static_pool_free(pSTATIC_POOL(test), t);
```

