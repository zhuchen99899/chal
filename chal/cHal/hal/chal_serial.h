#ifndef __CHAL_SERIAL_H_
#define __CHAL_SERIAL_H_

#include "chal_device.h"
#include "stdint.h"


/* public macros ------------------------------------------------------------ */
enum CHal_Serial_Data_Bits
{
    CHAL_SERIAL_DATA_BITS_5 = 5,
    CHAL_SERIAL_DATA_BITS_6,
    CHAL_SERIAL_DATA_BITS_7,
    CHAL_SERIAL_DATA_BITS_8,
};

enum CHal_Serail_Stop_Bits
{
    CHAL_SERIAL_STOP_BITS_1 = 0,
    CHAL_SERIAL_STOP_BITS_2,
};

enum CHal_Serial_Parity
{
    CHAL_SERIAL_PARITY_NONE = 0,
    CHAL_SERIAL_PARITY_ODD,
    CHAL_SERIAL_PARITY_EVEN,
};

enum CHAL_serail_mode
{
    CHAL_SERIAL_MODE_FULL_DUPLEX = 0,
    CHAL_SERIAL_MODE_HALF_DUPLEX,
};

/* Default config for chal_serial_config_t */
#define CHAL_SERIAL_ATTR_DEFAULT                                               \
{                                                                              \
    115200,                                         /* 115200 bits/s */        \
    CHAL_SERIAL_DATA_BITS_8,                        /* 8 databits */           \
    CHAL_SERIAL_STOP_BITS_1,                        /* 1 stopbit */            \
    CHAL_SERIAL_PARITY_NONE,                        /* No parity  */           \
    CHAL_SERIAL_MODE_FULL_DUPLEX,                   /* Full / half duplex */   \
    0,                                                                         \
    256,                                            /* rx buffer size */       \
}



/* public types ------------------------------------------------------------- */
typedef struct chal_serial_attr
{
    uint32_t baud_rate;
    uint32_t data_bits                  : 4;
    uint32_t stop_bits                  : 2;
    uint32_t parity                     : 2;
    uint32_t mode                       : 1;
    uint32_t reserved                   : 7;
    uint32_t rx_bufsz                   : 16;
}chal_serial_attr_t;

typedef struct chal_serial_config
{
    uint32_t baud_rate;
    uint32_t data_bits                  : 4;
    uint32_t stop_bits                  : 2;
    uint32_t parity                     : 2;
    uint32_t mode                       : 1;
    uint32_t reserved                   : 7;
} chal_serial_config_t;



typedef struct chal_serial
{
    chal_dev_t super; /**<father class */
    const struct chal_serial_ops *ops;
    chal_serial_attr_t attr;

}chal_serial_t;

typedef struct chal_serial_ops
{
    int32_t (* read)(chal_serial_t *chal_serial,void *buffer,uint32_t size);
    int32_t (* write)(chal_serial_t *chal_serial,const void *buffer,uint32_t size);
    void (* set_tx)(chal_serial_t *serial, bool status);
    CHal_Err_t (* config)(chal_serial_t *serial, chal_serial_config_t *config);
}chal_serial_ops_t;

/* public functions --------------------------------------------------------- */
void chal_serial_register(chal_serial_t *serial, const char *name,
                            chal_serial_ops_t *ops,
                            chal_serial_attr_t *attr,
                            void *user_data);

//cb_function
// void cb_chal_serial_tx_end(chal_serial_t *serial);
// void cb_chal_serial_isr_rx(chal_serial_t *serial, void *buffer, uint32_t size);

/* For high level program. */
int32_t chal_serial_write(chal_dev_t * const this, void *buff, uint32_t size);
int32_t chal_serial_read(chal_dev_t * const this, void *buff,uint32_t size);
void chal_serial_set_baudrate(chal_dev_t * const this, uint32_t baudrate);
void chal_serial_set_attr(chal_dev_t * const this, chal_serial_attr_t *attr);
chal_serial_attr_t chal_serial_get_attr(chal_dev_t * const me);

/* TODO Just for half duplex mode. */
int32_t chal_serial_xfer(chal_dev_t *me,
                            void *buff_tx, uint32_t size_tx,
                            void *buff_rx, uint32_t size_rx
                            );

#endif

