/*
 * static_pool.h
 *
 *  Created on: 28.05.2023
 *      Author: Valeriy Chudnikov
 */

#include "static_pool.h"

#ifndef STATIC_POOL_LOCK()
#define STATIC_POOL_LOCK()     /* Critical section enter */
#endif

#ifndef STATIC_POOL_UNLOCK()
#define STATIC_POOL_UNLOCK()   /* Critical section exit */
#endif

/**
 * @brief       Allocates one block from pool.
 *
 * @param pool  Pointer to pool control block. pSTATIC_POOL macro is useful.
 * @return      Pointer to successfully allocated block or NULL otherwise.
 */
void *static_pool_alloc(static_pool_cb *pool)
{
    STATIC_POOL_LOCK();

    uint32_t i;
    void *pRet = NULL;

    for (i = 0; i < pool->blks_num; i++)
    {
        if (pool->pState[i] == 0)
        {
            pRet = &pool->pPool[i];
            pool->pState[i] = 1;
            break;
        }
    }

    STATIC_POOL_UNLOCK();

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
    uint32_t i;

    STATIC_POOL_LOCK();

    for (i = 0; i < pool->blks_num; i++)
    {
        if ((&pool->pPool[i] == p) && (pool->pState[i] != 0))
        {
            pool->pState[i] = 0;
            break;
        }
    }

    STATIC_POOL_UNLOCK();
}

/**
 * @brief       Returns number of free blocks in pool.
 *
 * @param pool  Pointer to pool control block. pSTATIC_POOL macro is useful.
 * @return      Number of free blocks
 */
uint32_t static_pool_get_free_blks(static_pool_cb *pool)
{
    uint32_t i;
    uint32_t num = 0;

    STATIC_POOL_LOCK();

    for (i = 0; i < pool->blks_num; i++)
    {
        if (pool->pState[i] == 0)
        {
            num++;
        }
    }

    STATIC_POOL_UNLOCK();

    return num;
}
