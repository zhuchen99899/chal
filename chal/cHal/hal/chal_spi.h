#ifndef __CHAL_SPI_
#define __CHAL_SPI_
#include "chal_device.h"

/* public macros -------------------------------------------------------------*/
enum CHal_Spi_Mode
{
    ELAB_SPI_MODE_0 = 0,                        /**< CPOL = 0, CPHA = 0 */
    ELAB_SPI_MODE_1,                            /**< CPOL = 0, CPHA = 1 */
    ELAB_SPI_MODE_2,                            /**< CPOL = 1, CPHA = 0 */
    ELAB_SPI_MODE_3,                            /**< CPOL = 1, CPHA = 1 */

    ELAB_SPI_MODE_MAX
};

/* Public types ------------------------------------------------------------- */
/**
 * SPI message structure
 */

typedef struct chal_spi_msg
{
    const void *buff_send;
    void *buff_recv;
    uint32_t size;
} chal_spi_msg_t;
/**
 * SPI configuration structure
 */
typedef struct chal_spi_config
{
    uint8_t mode;
    uint8_t data_width;
    uint32_t max_hz;
} chal_spi_config_t;

typedef struct chal_spi_bus
{
chal_dev_t *super;
uint8_t mode;
const struct chal_spi_bus_ops *ops;
#if CHAL_OS_EN
    osMutexId_t mutex;
    osSemaphoreId_t sem;
#else 
		uint8_t lock;
#endif
    chal_spi_config_t config_owner;
}chal_spi_bus_t;

typedef struct chal_spi_device
{
    chal_dev_t super;

    chal_spi_bus_t *bus;
    chal_dev_t *pin_cs;
    chal_spi_config_t config;
    void *user_data;
} chal_spi_t;

/**
 * SPI bus operators
 */
//typedef struct chal_spi_bus_ops
//{
//    elab_err_t (* config)(elab_spi_t *const me, elab_spi_config_t *config);
//    elab_err_t (* xfer)(elab_spi_t * const me, elab_spi_msg_t *message);
//} chal_spi_bus_ops_t;

#endif
