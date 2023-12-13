/**
 * @file chal_led.c
 * @author zc (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-02
 * @note 基于 hal 层 pin抽象 的led设备层
 * @copyright Copyright (c) 2023
 * 
 */
#include "chal_led.h"
#include "chal_init.h"
/* Private variables ---------------------------------------------------------*/
static const chal_dev_ops_t _led_ops =
{
    .open  = NULL,
    .close = NULL,
    .read  = NULL,
    .write = NULL,

};


/* private variables -------------------------------------------------------- */
static chal_led_t led1;
static chal_led_t led2;

/**
 * @brief 
 * 
 * @param this 
 * @param name 
 * @param pin_name 
 * @param status_led_on 
 */
static void chal_led_register(chal_led_t *const this, const char *name,const char *pin_name,bool status_led_on)
{

    /* Set the data of the device. */
    this->super.user_data = NULL;
    this->status_led_on = status_led_on;
    /* Register to device manager. */
    this->pin = chal_dev_find(pin_name);
    
    chal_dev_attr_t attr_led =
    {
        .name =(char *)name, 
        .sole = true,
        .type = CHAL_DEVICE_UNKNOWN,
    };
    chal_dev_register(&this->super,&_led_ops, &attr_led);
}


void led_dev_init(void)
{
    /* PIN devices on MCU. */
    chal_led_register(&led2, "led2","pin_led2", false);
    chal_led_register(&led1, "led1","pin_led1", false);
}
#if CHAL_EXPORT_EN
INIT_EXPORT(led_dev_init, MIDWARE_LEVEL_LED);
#else
#endif
/**
 * @brief 
 * 
 * @param this 
 * @return true 
 * @return false 
 */
bool chal_led_get_status(chal_dev_t *const this)
{
chal_led_t *led = (chal_led_t * )this;

led->status=chal_pin_get_status(led->pin);

return led->status;
}

/**
 * @brief 
 * 
 * @param this 
 */
void chal_led_on(chal_dev_t *const this)
{
    chal_led_t *led = (chal_led_t * )this;

    chal_pin_set_status(led->pin,led->status_led_on? true :false);
    led->status=chal_pin_get_status(led->pin);
}

/**
 * @brief 
 * 
 * @param this 
 * @param status 
 */
void chal_led_off(chal_dev_t *const this)
{
    chal_led_t *led = (chal_led_t * )this;

    chal_pin_set_status(led->pin,led->status_led_on? false :true);
    led->status=chal_pin_get_status(led->pin);
}

/**
 * @brief 
 * 
 * @param this 
 */
void chal_led_toggle(chal_dev_t *const this)
{
    chal_led_t *led = (chal_led_t * )this;
    
    bool status=chal_pin_get_status(led->pin);

    chal_pin_set_status(led->pin,led->status? false :true);
    led->status=chal_pin_get_status(led->pin);
}


