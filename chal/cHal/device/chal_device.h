#ifndef __CHAL_DEVICE_H_
#define __CHAL_DEVICE_H_

#include "chal_def.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"
#include "chal_config.h"
#include "chal_def.h"
/* public define ------------------------------------------------------------ */
#if CHAL_OS_EN
#include "cmsis_os.h"
#else
#endif

enum CHal_Dev_Type
{
    CHAL_DEVICE_NULL = 0,
    CHAL_DEVICE_PIN,
    CHAL_DEVICE_PWM,
    CHAL_DEVICE_ADC,
    CHAL_DEVICE_DAC,
    CHAL_DEVICE_UART,
    CHAL_DEVICE_I2C_BUS,
    CHAL_DEVICE_I2C,
    CHAL_DEVICE_SPI_BUS,
    CHAL_DEVICE_SPI,
    CHAL_DEVICE_CAN,
    CHAL_DEVICE_WATCHDOG,
    CHAL_DEVICE_RTC,
    CHAL_DEVICE_UNKNOWN,

    CHAL_DEVICE_NORMAL_MAX,
};


typedef enum CHal_Device_Status
{
DEV_STATUS_NULL=0,
//IDLE
DEV_REGISTED,
//USING
DEV_REGISTING,
DEV_UNREGISTING,
DEV_USING
}Dev_Status_t;


/**
 * @brief chal_dev_attr
 * 
 */
typedef struct chal_dev_attr
{
    const char *name;/**< chal_device name */
    bool sole;       /**< standlone device  */
    uint8_t type;    /**< chal_device type */
} chal_dev_attr_t;

/**
 * @brief chal_dev_t  chal_device class
 * 
 */
typedef struct chal_dev
{
    struct chal_dev *next;       /**< next chal_device  obj   (listdata)*/
    void *user_data;                /**<user_data send to son obj (driver layer)   */
    const struct chal_dev_ops *ops; /**<chal_device ops   */
    chal_dev_attr_t attr;           /**<chal_device attr   */
    uint16_t count_open;            /**<chal_device open count   */
    uint16_t count_lock;            /**<chal_device lock count   */
    uint8_t status;
    #if CHAL_OS_EN
    osMutexId_t mutex;
    osMutexAttr_t *mutex_attr;
    #else
    uint8_t lock;
    #endif

} chal_dev_t;

typedef struct chal_dev_ops
{
    CHal_Err_t (* open)(chal_dev_t * const this);
    CHal_Err_t (* close)(chal_dev_t * const this);
    int32_t (* read)(chal_dev_t * const this, void *buffer, uint32_t size);
    int32_t (* write)(chal_dev_t * const this, const void *buffer, uint32_t size);
} chal_dev_ops_t;

/* public functions --------------------------------------------------------- */
/* For low-level driver. */
void chal_dev_register(chal_dev_t * const this, 
                    const chal_dev_ops_t *ops,
                    chal_dev_attr_t *attr);

/* For high-level code. */
chal_dev_t *chal_dev_find(const char *name);
CHal_Err_t chal_dev_open(chal_dev_t * const this);
CHal_Err_t chal_dev_close(chal_dev_t * const this);
int32_t chal_dev_read(chal_dev_t * const this, void *buffer, uint32_t size);
int32_t chal_dev_write(chal_dev_t * const this, const void *buffer, uint32_t size);


#endif

