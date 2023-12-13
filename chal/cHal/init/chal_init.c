#include "chal_init.h"

#if CHAL_EXPORT_EN

#else

void chal_drv_init(void)
{
void driver_uart_init(void);
void dirver_pin_init(void);
void led_init(void);
}
#endif
