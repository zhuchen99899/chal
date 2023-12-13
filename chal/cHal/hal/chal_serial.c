/**
 * @file chal_serial.c
 * @author zc (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "chal_serial.h"

static const chal_dev_ops_t _device_ops =
{
.open  = NULL,
.close = NULL,
.read  = NULL,
.write = NULL,
};

/* public functions --------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param serial  Chal serial device handle
 * @param name    Serial name
 * @param ops     Serial device ops
 * @param attr    Serial device attribut
 * @param user_data  usr_data
 */
void chal_serial_register(chal_serial_t *serial, const char *name,
                            chal_serial_ops_t *ops,
                            chal_serial_attr_t *attr,
                            void *user_data)
{
/*init class data*/
    memset(serial, 0, sizeof(chal_serial_t));
    serial->ops = ops;
    if (attr == NULL)
    {
        serial->attr = (chal_serial_attr_t)CHAL_SERIAL_ATTR_DEFAULT;
    }
    else
    {
        memcpy(&serial->attr, attr, sizeof(chal_serial_attr_t));
    }

    /* The serial device initialization. */
    if (serial->ops->config != NULL)
    {
    chal_serial_attr_t attr = serial->attr;
    serial->ops->config(serial, (chal_serial_config_t *)&attr);
    }
    /* If RS485 mode, set to rx mode. */
    if (serial->attr.mode == CHAL_SERIAL_MODE_HALF_DUPLEX)
    {
        serial->ops->set_tx(serial, false);
    }
    
   /* The super class data */
    chal_dev_t *device = &(serial->super);
    
    device->user_data = user_data;

    /* register a character device */
    chal_dev_attr_t _dev_attr =
    {
        .name = name,
        .sole = true,
        .type = CHAL_DEVICE_UART,
    };
    if (serial->attr.mode == CHAL_SERIAL_MODE_HALF_DUPLEX)
    {
        _dev_attr.sole = false;
    }
    chal_dev_register(device, &_device_ops,&_dev_attr);
}

//cb_function


// void chal_serial_tx_end(chal_serial_t *serial);
// void chal_serial_isr_rx(chal_serial_t *serial, void *buffer, uint32_t size);

/* For high level program. */
/**
  * @brief  chal serial device write function.
  * @param  me      The chal device handle.
  * @param  buff    The pointer of buffer
  * @param  size    Expected write length
  * @retval Auctual write length or error ID.
  */
int32_t chal_serial_write(chal_dev_t * const this, void *buff, uint32_t size)
{
    int32_t ret=CHAL_OK;
    chal_serial_t *serial = (chal_serial_t *)this;
    /* If RS485 mode, set to tx mode. */
    if(serial->attr.mode == CHAL_SERIAL_MODE_HALF_DUPLEX)
    {
    serial->ops->set_tx(serial, true);
    }
    /* Send data by the low level serial write function in non-block mode. */
    ret = serial->ops->write(serial, buff, size);
    if (ret < 0)
    {
        goto exit;
    }

exit:
    /* If RS485 mode, set to rx mode. */
    if (serial->attr.mode == CHAL_SERIAL_MODE_HALF_DUPLEX)
    {
        serial->ops->set_tx(serial, false);
    }
    return ret;

}

/**
  * @brief  chal device read function
  * @param  me      The chal device handle.
  * @param  buffer  The pointer of buffer
  * @param  size    Expected read length
  * @retval  error ID.
  */
int32_t chal_serial_read(chal_dev_t * const this, void *buff,uint32_t size)
{
    
    chal_serial_t *serial = (chal_serial_t *)this;
    int32_t ret=serial->ops->read(serial,buff,size);
    return ret;
}

/**
  * @brief  Set the attribute of the chal serial device
  * @param  serial      chal serial device handle
  * @param  name        Serial name
  * @param  ops         Serial device ops
  * @param  user_data   The pointer of private data
  * @retval See elab_err_t
  */

 void chal_serial_set_attr(chal_dev_t * const this, chal_serial_attr_t *attr)
 {
    chal_serial_t *serial = (chal_serial_t *) this;
 /* Set the config data of serial device. */
    if (memcmp(&serial->attr, attr, sizeof(chal_serial_attr_t)) != 0)
    {
        CHal_Err_t ret = serial->ops->config(serial, (chal_serial_config_t *)attr);
        if (ret == CHAL_OK)
        {
            memcpy(&serial->attr, attr, sizeof(chal_serial_attr_t));
        }
    }
 }

/**
  * @brief  Register chal serial device to serial device
  * @param  serial      chal serial device handle
  * @param  name        Serial name
  * @param  ops         Serial device ops
  * @param  user_data   The pointer of private data
  * @retval See CHal_Err_t
  */
chal_serial_attr_t chal_serial_get_attr(chal_dev_t * const this)
{
    chal_serial_attr_t attr;
    chal_serial_t *serial = (chal_serial_t*)(this);


    memcpy(&attr, &serial->attr, sizeof(chal_serial_attr_t));

    return attr;

}

void chal_serial_set_baudrate(chal_dev_t * const this, uint32_t baudrate)
{



    chal_serial_attr_t attr = chal_serial_get_attr(this);
    if (attr.baud_rate != baudrate)
    {
        attr.baud_rate = baudrate;
        chal_serial_set_attr(this, &attr);
    }


}



/* TODO Just for half duplex mode. */
/**
  * @brief  chal serial device xfer function, just for half duplex mode.
  * @param  serial      chal serial device handle
  * @param  name        Serial name
  * @param  ops         Serial device ops
  * @param  user_data   The pointer of private data
  * @retval See CHal_Err_t
  */
// int32_t chal_serial_xfer(chal_dev_t *this,
//                             void *buff_tx, uint32_t size_tx,
//                             void *buff_rx, uint32_t size_rx)
// {
//     int32_t ret =CHAL_ERR_EMPTY;
//     chal_serial_t *serial= (chal_serial_t *) this;
//     if(serial->attr.mode ==CHAL_SERIAL_MODE_HALF_DUPLEX)
//     {
//         ret  = chal_serial_write(this, buff_tx, size_tx);
//         if(ret>0)
//         {
//         ret = chal_serial_read(this, buff_rx, size_rx);
//         }


//     }

// return ret;
// }

