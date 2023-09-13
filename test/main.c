/*
 * main.c
 *
 *  Created on: 2.06.2023
 *      Author: chudnikov
 */

#include <stdio.h>
#include "../static_pool.h"

#define TEST_POOL_SIZE      10

typedef struct {
    uint16_t v1;
    uint16_t v2;
} test_type_t;

STATIC_POOL_DEFINE(test, TEST_POOL_SIZE, test_type_t, NULL, NULL);

int main(void)
{
    printf("--- Static Pool Test ---\r\n");

    printf("Init test: ");
    static_pool_init(pSTATIC_POOL(test));

    if ((static_pool_get_byte_size(pSTATIC_POOL(test)) == TEST_POOL_SIZE * sizeof(test_type_t))
            && (static_pool_get_blks_num(pSTATIC_POOL(test)) == TEST_POOL_SIZE))
    {
        printf("PASS\r\n");
    }
    else
    {
        printf("FAIL\r\n");
        return -1;
    }

    printf("Alloc test: ");
    test_type_t *t = static_pool_alloc(pSTATIC_POOL(test));

    if (t != NULL)
    {
        printf("PASS\r\n");
    }
    else
    {
        printf("FAIL\r\n");
        return -1;
    }

    printf("Free test: ");
    static_pool_free(pSTATIC_POOL(test), t);
    printf("PASS\r\n");

    /* Alloc all blocks */
    printf("Fill test: ");

    uint32_t allocated_blks = 0;
    test_type_t *ta[TEST_POOL_SIZE];

    do {
        if ((allocated_blks + static_pool_get_free_blks(pSTATIC_POOL(test)) != TEST_POOL_SIZE))
        {
            printf("FAIL on: Allocated blocks %d\r\n", allocated_blks);
            return -1;
        }

        ta[allocated_blks] = static_pool_alloc(pSTATIC_POOL(test));
        allocated_blks++;
    }
    while ((t != NULL) && (allocated_blks < TEST_POOL_SIZE));

    /* Check for NULL return */
    if (static_pool_alloc(pSTATIC_POOL(test)) == NULL)
    {
        printf("PASS\r\n");
    }
    else
    {
        printf("FAIL\r\n");
        return -1;
    }

    /* Free all and check free blocks */
    printf("Clean test: ");

    uint32_t i;
    for (i = 0; i < TEST_POOL_SIZE; i++)
    {
        static_pool_free(pSTATIC_POOL(test), ta[i]);
    }

    if (static_pool_get_blks_num(pSTATIC_POOL(test)) == TEST_POOL_SIZE)
    {
        printf("PASS\r\n");
    }
    else
    {
        printf("FAIL\r\n");
        return -1;
    }

    printf("All tests PASSED\r\n");

    return 0;
}
