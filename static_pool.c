/*
 * static_pool.h
 *
 *  Created on: 28.05.2023
 *      Author: Valeriy Chudnikov
 */

#include "static_pool.h"
#include <string.h>

/**
 * @brief       Static pool initialization.
 *
 * @param pool  Pointer to pool control block. pSTATIC_POOL macro is useful.
 */
void static_pool_init(static_pool_cb *pool)
{
    STATIC_POOL_ASSERT(pool != NULL);
    STATIC_POOL_ASSERT(pool->pPool != NULL);
    STATIC_POOL_ASSERT(pool->pState != NULL);
    STATIC_POOL_ASSERT(pool->blks_num > 0);
    STATIC_POOL_ASSERT(pool->blk_size > 0);

    memset(pool->pPool, 0, pool->blks_num * pool->blk_size);
    memset(pool->pState, 0, pool->blks_num * sizeof(uint8_t));
}

/**
 * @brief       Allocates one block from pool.
 *
 * @param pool  Pointer to pool control block. pSTATIC_POOL macro is useful.
 * @return      Pointer to successfully allocated block or NULL otherwise.
 */
void *static_pool_alloc(static_pool_cb *pool)
{
    STATIC_POOL_ASSERT(pool != NULL);

    void *lock_arg = NULL;

    if (pool->Lock != NULL)
    {
        pool->Lock(lock_arg);
    }

    spool_size_t i;
    void *pRet = NULL;

    for (i = 0; i < pool->blks_num; i++)
    {
        if (pool->pState[i] == 0)
        {
            pRet = pool->pPool + i * pool->blk_size;
            pool->pState[i] = 1;
            break;
        }
    }

    if (pool->Unlock != NULL)
    {
        pool->Unlock(lock_arg);
    }

    return pRet;
}

/**
 * @brief       Deallocates one block in pool.
 *
 * @param pool  Pointer to pool control block. pSTATIC_POOL macro is useful.
 * @param p     Pointer to previously allocated block.
 */
void static_pool_free(static_pool_cb *pool, void *p)
{
    STATIC_POOL_ASSERT(pool != NULL);
    STATIC_POOL_ASSERT(p != NULL);

    spool_size_t i;
    void *lock_arg = NULL;

    if (pool->Lock != NULL)
    {
        pool->Lock(lock_arg);
    }

    for (i = 0; i < pool->blks_num; i++)
    {
        if (((pool->pPool + i * pool->blk_size) == p) && (pool->pState[i] != 0))
        {
            pool->pState[i] = 0;
            break;
        }
    }

    if (pool->Unlock != NULL)
    {
        pool->Unlock(lock_arg);
    }
}

/**
 * @brief       Returns number of free blocks in pool.
 *
 * @param pool  Pointer to pool control block. pSTATIC_POOL macro is useful.
 * @return      Number of free blocks
 */
spool_size_t static_pool_get_free_blks(static_pool_cb *pool)
{
    STATIC_POOL_ASSERT(pool != NULL);

    spool_size_t i;
    spool_size_t num = 0;
    void *lock_arg = NULL;

    if (pool->Lock != NULL)
    {
        pool->Lock(lock_arg);
    }

    for (i = 0; i < pool->blks_num; i++)
    {
        if (pool->pState[i] == 0)
        {
            num++;
        }
    }

    if (pool->Unlock != NULL)
    {
        pool->Unlock(lock_arg);
    }

    return num;
}
