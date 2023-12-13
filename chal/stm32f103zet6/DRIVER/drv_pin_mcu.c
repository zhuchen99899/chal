/**
 * @file driver_pin.c
 * @author zc (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-29
 * @note stm32f407vgt6_driver
 * @copyright Copyright (c) 2023
 * 
 */


#include "chal_init.h"
#include "chal_pin.h"
#include "stm32f1xx_hal.h"

typedef struct driver_pin_data
{
    chal_pin_t *device;  /**<father class*/

    const char *name;
    GPIO_TypeDef *gpio_x;
    uint16_t pin;
    bool init_status;
} drv_pin_t;

/* private function prototype ----------------------------------------------- */
static void _init(chal_pin_t * const this);
static void _set_mode(chal_pin_t * const this, uint8_t mode);
static bool _get_status(chal_pin_t * const this);
static void _set_status(chal_pin_t * const this, bool status);


static chal_pin_t pin_test_b_5;
static chal_pin_t pin_test_e_5;


//driver data
static drv_pin_t chal_pin_driver_data[]=
{
{&pin_test_b_5,"pin_led1",GPIOB,GPIO_PIN_5,true},
{&pin_test_e_5,"pin_led2",GPIOE,GPIO_PIN_5,true}

};

static const chal_pin_ops_t pin_ops =
{
    .init = _init,
    .set_mode = _set_mode,
    .get_status = _get_status,
    .set_status = _set_status,
};



/* public functions --------------------------------------------------------- */
void dirver_pin_init(void)
{

          /* Enable the clock. */
	for (uint32_t i = 0;
            i < sizeof(chal_pin_driver_data) / sizeof(drv_pin_t); i ++)
    {
        if (chal_pin_driver_data[i].gpio_x == GPIOA)
        {
            __HAL_RCC_GPIOA_CLK_ENABLE();
        }
        else if (chal_pin_driver_data[i].gpio_x== GPIOB)
        {
            __HAL_RCC_GPIOB_CLK_ENABLE();
        }
        else if (chal_pin_driver_data[i].gpio_x == GPIOC)
        {
            __HAL_RCC_GPIOC_CLK_ENABLE();
        }
        else if (chal_pin_driver_data[i].gpio_x == GPIOD)
        {
            __HAL_RCC_GPIOD_CLK_ENABLE();
        }
        else if (chal_pin_driver_data[i].gpio_x == GPIOE)
        {
            __HAL_RCC_GPIOE_CLK_ENABLE();
        }
        else if (chal_pin_driver_data[i].gpio_x== GPIOF)
        {
            __HAL_RCC_GPIOF_CLK_ENABLE();
        }
        else if (chal_pin_driver_data[i].gpio_x == GPIOG)
        {
            __HAL_RCC_GPIOG_CLK_ENABLE();
        }
				
				
        /* Device registering. */
        chal_pin_register(chal_pin_driver_data[i].device,
                            chal_pin_driver_data[i].name,
                            &pin_ops,
                            &chal_pin_driver_data[i]);
    }
}
#if CHAL_EXPORT_EN
INIT_EXPORT(dirver_pin_init, DRVIVER_LEVEL_PIN);
#else
#endif

/* private functions -------------------------------------------------------- */
/**
  * @brief  The PIN driver initialization function.
  * @param  this  PIN device handle.
  * @retval None.
  */
static void _init(chal_pin_t * const this)
{
    //assert(this != NULL);
    drv_pin_t *driver_data = (drv_pin_t *)this->super.user_data;

    /* Configure GPIO pin. */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Pin = driver_data->pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(driver_data->gpio_x, &GPIO_InitStruct);

    HAL_GPIO_WritePin(driver_data->gpio_x,
                        driver_data->pin,
                        driver_data->init_status? GPIO_PIN_SET: GPIO_PIN_RESET);
}

/**
  * @brief  The PIN driver set_mode function.
  * @param  this  PIN device handle.
  * @retval None.
  */
static void _set_mode(chal_pin_t * const this, uint8_t mode)
{
    //assert(this != NULL);
    //assert(mode < PIN_MODE_MAX);

    drv_pin_t *driver_data = (drv_pin_t *)this->super.user_data;

    /* Configure GPIO pin. */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (mode == PIN_MODE_INPUT)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
    }
    else if (mode == PIN_MODE_INPUT_PULLDOWN)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    }
    else if (mode == PIN_MODE_OUTPUT_PP)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    }
    else if (mode == PIN_MODE_OUTPUT_OD)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
    }
    GPIO_InitStruct.Pin = driver_data->pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(driver_data->gpio_x, &GPIO_InitStruct);
}

/**
  * @brief  The PIN driver set_mode function.
  * @param  this  PIN device handle.
  * @retval GPIO status.
  */
static bool _get_status(chal_pin_t * const this)
{
    //assert(this != NULL);

    drv_pin_t *driver_data = (drv_pin_t *)this->super.user_data;

    GPIO_PinState status = HAL_GPIO_ReadPin(driver_data->gpio_x, driver_data->pin);

    return (status == GPIO_PIN_SET) ? true : false;
}

/**
  * @brief  The PIN driver set_status function.
  * @param  this      PIN device handle.
  * @param  status  GPIO status.
  * @retval None.
  */
static void _set_status(chal_pin_t * const this, bool status)
{
    //assert(this != NULL);
	
    drv_pin_t *driver_data = (drv_pin_t *)this->super.user_data;


    HAL_GPIO_WritePin(driver_data->gpio_x,
                        driver_data->pin,
                        status ? GPIO_PIN_SET : GPIO_PIN_RESET);

}



/* ----------------------------- end of file -------------------------------- */

