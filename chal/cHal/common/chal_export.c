/**
 * @file chal_export.c
 * @author zc (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-30
 * @note export port
 * @copyright Copyright (c) 2023
 * 
 */
#include "chal_export.h"
#include "chal_init.h"
#include "string.h"
#if CHAL_EXPORT_EN

static int8_t export_level_max = INT8_MAX;
static uint32_t count_export_init = 0;
static void module_null_init(void);
static chal_export_t *export_init_table = NULL;

INIT_EXPORT(module_null_init, 0);

/**
  * @brief  Get the init export table.
  */
static void _get_init_export_table(void)
{
    chal_export_t *func_block = (chal_export_t *)&init_module_null_init;
    chal_pointer_t address_last;
    
    while (1)
    {
        address_last = ((chal_pointer_t)func_block - sizeof(chal_export_t));
        chal_export_t *table = (chal_export_t *)address_last;
        if (table->magic_head != EXPORT_ID_INIT ||
            table->magic_tail != EXPORT_ID_INIT)
        {
            break;
        }
        func_block = table;
    }
    export_init_table = func_block;

    uint32_t i = 0;
    while (1)
    {
        if (export_init_table[i].magic_head == EXPORT_ID_INIT &&
            export_init_table[i].magic_tail == EXPORT_ID_INIT)
        {
            if (export_init_table[i].level > export_level_max)
            {
                export_level_max = export_init_table[i].level;
            }
            i ++;
        }
        else
        {
            break;
        }
    }
    count_export_init = i;
}


/**
  * @brief  chal init exporting function executing.
  * @retval None
  */
static void _init_func_execute(int8_t level)
{
    /* Execute the poll function in the specific level. */
    for (uint32_t i = 0; i < count_export_init; i ++)
    {
        if (export_init_table[i].level == level)
        {
            if (!export_init_table[i].exit)
            {
                if (level != EXPORT_UNIT_TEST)
                {
                    //printf("Export init %s. /r/n" , export_init_table[i].name);
                }
                ((void (*)(void))export_init_table[i].func)();
            }
        }
    }
}

/**
  * @brief  chal null exporting function.
  * @retval None
  */
static void module_null_init(void)
{
    /* NULL */
}

#else //no export 
#endif
/**
 * @brief chal_run
 * @note chal_start
 */
void chal_run()
{
#if CHAL_EXPORT_EN
_get_init_export_table();
    /* Initialize all module in chal. */
    for (uint8_t level = 0; level < export_level_max; level ++)
    {
        _init_func_execute(level);
    }
#else
	chal_drv_init();	  
#endif
}









