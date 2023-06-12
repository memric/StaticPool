/*
 * static_pool.h
 *
 *  Created on: 28.05.2023
 *      Author: Valeriy Chudnikov
 */

#include "static_pool.h"

#ifdef STATIC_POOL_USER_LOCKS
extern void static_pool_lock(void);
extern void static_pool_unlock(void);
#else
#define static_pool_lock()     /* Critical section enter */
#define static_pool_unlock()   /* Critical section exit */
#endif /* STATIC_POOL_USER_LOCKS */

/**
 * @brief       Allocates one block from pool.
 *
 * @param pool  Pointer to pool control block. pSTATIC_POOL macro is useful.
 * @return      Pointer to successfully allocated block or NULL otherwise.
 */
void *static_pool_alloc(static_pool_cb *pool)
{
    static_pool_lock();

    uint32_t i;
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

    static_pool_unlock();

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

    static_pool_lock();

    for (i = 0; i < pool->blks_num; i++)
    {
        if (((pool->pPool + i * pool->blk_size) == p) && (pool->pState[i] != 0))
        {
            pool->pState[i] = 0;
            break;
        }
    }

    static_pool_unlock();
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

    static_pool_lock();

    for (i = 0; i < pool->blks_num; i++)
    {
        if (pool->pState[i] == 0)
        {
            num++;
        }
    }

    static_pool_unlock();

    return num;
}
