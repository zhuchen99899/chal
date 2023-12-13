#ifndef __CHAL_EXPORT_H_
#define __CHAL_EXPORT_H_



/* include ------------------------------------------------------------------ */
#include <stdint.h>
#include <stdbool.h>
#include "chal_def.h"
#include "chal_config.h"
#include "chal_init.h"
/* public define ------------------------------------------------------------ */

#if CHAL_EXPORT_EN
enum CHal_Export_Level
{
    EXPORT_POLL                         = -2,
    EXPORT_UNIT_TEST                    = -1,
    EXPORT_LEVEL_BSP                    = 0,
    EXPORT_LEVEL_HW_INDEPNEDENT         = 0,
    EXPORT_DRVIVER                      = 1,
    EXPORT_MIDWARE                      = 10,
    EXPORT_DEVICE                       = 11,
    EXPORT_APP                          = 12,
    EXPORT_USER                         = 13,
};

enum CHAL_Driver_Level
{
DRVIVER_LEVEL_PIN =2,

};

enum CHal_Midware_Level
{
MIDWARE_LEVEL_LED=5,

};
/* public typedef ----------------------------------------------------------- */


typedef struct chal_export
{
    uint32_t magic_head;
    const char *name;
    void *func;
    void *data;
    bool exit;
    int8_t level;
    uint8_t type;
    uint32_t magic_tail;
// uint32_t period_ms;
// #if defined(_WIN32)
//     uint32_t temp[9];
// #elif defined(__linux__)
//     uint32_t temp[4];
// #endif
} chal_export_t;

/* public export ------------------------------------------------------------ */
/**
  * @brief  Initialization function exporting macro.
  * @param  _func   The initialization function.
  * @param  _level  The export level, [0, 127].
  * @retval None.
  */

#define INIT_EXPORT(_func, _level)                                             \
    CHAL_USED const chal_export_t init_##_func CHAL_SECTION("chal_export") =   \
    {                                                                          \
        .name = #_func,                                                        \
        .func = (void *)&_func,                                                \
        .level = _level,                                                       \
        .exit = false,                                                         \
        .magic_head = EXPORT_ID_INIT,                                          \
        .magic_tail = EXPORT_ID_INIT,                                          \
    }


		
		
#else		
#endif

void chal_run(void);
		
#endif

/* ----------------------------- end of file -------------------------------- */


