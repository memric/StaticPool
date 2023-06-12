/*
 * static_pool.h
 *
 *  Created on: 28.05.2023
 *      Author: Valeriy Chudnikov
 */

#ifndef STATIC_POOL_H_
#define STATIC_POOL_H_

#include <stdint.h>
#include <string.h>

typedef struct {
    uint32_t blks_num;
    uint32_t blk_size;
    uint8_t *pState;
    void *pPool;
} static_pool_cb;

#define STATIC_POOL_DEFINE(name, size, type) \
static type pool_##name[size]; \
static uint8_t pool_state_##name[size]; \
static static_pool_cb pool_cb_##name = \
{size, sizeof(type), pool_state_##name, pool_##name}

#define static_pool_init(name) \
memset(pool_cb_##name.pPool, 0, pool_cb_##name.blks_num * pool_cb_##name.blk_size); \
memset(pool_cb_##name.pState, 0, pool_cb_##name.blks_num * sizeof(uint8_t))

#define pSTATIC_POOL(name)                  (&pool_cb_##name)

#define static_pool_get_byte_size(pool)     (pool->blk_size * pool->blks_num)
#define static_pool_get_blks_num(pool)      pool->blks_num

void *static_pool_alloc(static_pool_cb *pool);
void static_pool_free(static_pool_cb *pool, void *p);
uint32_t static_pool_get_free_blks(static_pool_cb *pool);

#endif /* STATIC_POOL_H_ */
