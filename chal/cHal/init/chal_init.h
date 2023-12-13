#ifndef __CHAL_INIT_H_
#define __CHAL_INIT_H_

#include "chal_export.h"

	#if CHAL_EXPORT_EN

	#else
	void dirver_pin_init(void);
	void driver_uart_init(void);
	void led_dev_init(void);
	void chal_drv_init(void);
	#endif


#endif

