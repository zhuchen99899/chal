/**
 * @file chal_pin.c
 * @author zc (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-20
 * @note hal层 pin抽象层
 * @copyright Copyright (c) 2023
 * 
 */
#include "chal_pin.h"
#include "string.h"




/* private variables -------------------------------------------------------- */
static chal_dev_ops_t _dev_ops =
{
    .open = NULL,
    .close = NULL,
    .read = NULL,
    .write = NULL,
};

/* public functions --------------------------------------------------------- */
/**
  * @brief  chal_pin initialization.
  * @param  this     this pointer
  * @param  name    chal_pin's name.
  * @param  mode    chal_pin's mode.
  * @retval None
  */
void chal_pin_register( chal_pin_t * const this,
                        const char *name,
                        const chal_pin_ops_t *ops,
                        void *user_data)
{
    // assert(this != NULL);
    // assert(name != NULL);
    // assert(ops != NULL);
    chal_dev_attr_t attr =
    {
        .name =(char *)name, 
        .sole = true,
        .type = CHAL_DEVICE_PIN,
    };
    
    chal_dev_register(&this->super,&_dev_ops, &attr);
    this->super.user_data = user_data;  // driver data
    this->ops = ops;
    this->ops->init(this);
    this->status = this->ops->get_status(this);
}


/**
  * @brief  chal_pin's status getting function.
  * @param  this      this pointer
  * @retval The pin's status.
  */
void chal_pin_set_mode(chal_dev_t * const this, uint8_t mode)
{
    //assert(me != NULL);

    chal_pin_t *pin = (chal_pin_t *)this;
    if (pin->mode != mode)
    {
        pin->ops->set_mode(pin, mode);
        pin->mode = mode;
    }
}


/**
  * @brief  chal_pin's status getting function.
  * @param  this      this pointer
  * @retval The pin's status.
  */
bool chal_pin_get_status(chal_dev_t * const this)
{
    //assert(this != NULL);
    chal_pin_t *pin = (chal_pin_t *)this;

    pin->status = pin->ops->get_status(pin);

    return pin->status;
}

/**
  * @brief  chal_pin's status turning on function.
  * @param  this      this pointer
  * @param  status  the input pin status.
  * @retval None.
  */
void chal_pin_set_status(chal_dev_t * const this, bool status)
{
    //assert(this != NULL);

    chal_pin_t *pin = (chal_pin_t *)this;
    //assert(pin->mode == PIN_MODE_OUTPUT_PP ||
    //            pin->mode == PIN_MODE_OUTPUT_OD);
    
    if (status != pin->status)
    {
        pin->ops->set_status(pin, status);
        chal_pin_get_status(this);
        //assert(pin->status == status);
    }
}





/* ----------------------------- end of file -------------------------------- */



