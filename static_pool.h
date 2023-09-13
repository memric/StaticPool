/*
 * static_pool.h
 *
 *  Created on: 28.05.2023
 *      Author: Valeriy Chudnikov
 */

#ifndef STATIC_POOL_H_
#define STATIC_POOL_H_

#include <stdint.h>
#include <stddef.h>

#ifndef STATIC_POOL_SIZE_T
#define STATIC_POOL_SIZE_T              size_t
#endif /* STATIC_POOL_SIZE_T */

typedef STATIC_POOL_SIZE_T              spool_size_t;

/**
 * @brief   Static pool control block type
 *
 */
typedef struct {
    spool_size_t blks_num;      /*!< Number of blocks in pool */
    spool_size_t blk_size;      /*!< Block byte size */
    uint8_t *pState;            /*!< Pointer to state array contains each block state value */
    void *pPool;                /*!< Pointer to pool memory */
    void (* Lock)(void *arg);   /*!< Lock function pointer */
    void (* Unlock)(void *arg); /*!< Unlock function pointer */
} static_pool_cb;

/**
 * @brief   Macro for the pool handler definition.
 *
 */
#define STATIC_POOL_DEFINE(name, size, type, lock, unlock) \
static type pool_##name[size]; \
static uint8_t pool_state_##name[size]; \
static static_pool_cb pool_cb_##name = {\
        size, \
        sizeof(type), \
        pool_state_##name, \
        pool_##name, \
        lock, \
        unlock \
}

/**
 * @brief   Macro to get pointer to the pool control block by its name.
 *
 */
#define pSTATIC_POOL(name)                  (&pool_cb_##name)

/**
 * @brief   Macro for Lock function registration.
 *
 */
#define STATIC_POOL_SET_LOCK(name, func) \
        pool_cb_##name.Lock = func

/**
 * @brief   Macro for Unlock function registration.
 *
 */
#define STATIC_POOL_SET_UNLOCK(name, func) \
        pool_cb_##name.Unlock = func


#define static_pool_get_byte_size(pool)     (pool->blk_size * pool->blks_num)
#define static_pool_get_blks_num(pool)      pool->blks_num

#ifndef STATIC_POOL_ASSERT
#define STATIC_POOL_ASSERT(expr)            ((void) 0U)
#endif

void static_pool_init(static_pool_cb *pool);
void *static_pool_alloc(static_pool_cb *pool);
void static_pool_free(static_pool_cb *pool, void *p);
spool_size_t static_pool_get_free_blks(static_pool_cb *pool);

#endif /* STATIC_POOL_H_ */
