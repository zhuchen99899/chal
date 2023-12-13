#include "chal_device.h"
#include "chal_export.h"
#include "chal_led.h"
#include "app.h"
#include "chal_serial.h"
#include "stdio.h"
chal_dev_t *led2 = NULL;
chal_dev_t *led1 = NULL;
chal_dev_t *serial1= NULL;
uint8_t test[]={"HELLO"};


void led_init(void)
{

 led2 = chal_dev_find("led2");
 led1 = chal_dev_find("led1");
 serial1= chal_dev_find("usart1");
}

#if CHAL_EXPORT_EN
INIT_EXPORT(led_init, EXPORT_APP);
#else
#endif





void led_app(void)
{
    
chal_led_toggle(led2);
chal_led_toggle(led1);

printf("%s",test);

}


