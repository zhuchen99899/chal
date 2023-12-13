#ifndef __CHAL_LED_H_
#define __CHAL_LED_H_

#include "stdint.h"
#include "chal_device.h"
#include "chal_pin.h"
/* private typedef ---------------------------------------------------------- */
typedef struct chal_led
{
    chal_dev_t super;
    chal_dev_t *pin;
    bool status;
    bool status_led_on;
} chal_led_t;

/* public function ---------------------------------------------------------- */
/* LED class functions */
bool chal_get_status(chal_dev_t *const this);

void chal_led_on(chal_dev_t *const this);

void chal_led_off(chal_dev_t *const this);

void chal_led_toggle(chal_dev_t *const this);



#endif


