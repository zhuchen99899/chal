#ifndef __CHAL_PIN_H_
#define __CHAL_PIN_H_

/* public define ------------------------------------------------------------ */
/**
 * @brief pin_mode enum
 * 
 */
#include "stdint.h"
#include "stdbool.h"
#include "chal_device.h"
enum Pin_Mode
{
    PIN_MODE_INPUT = 0,         /**< INPUT_mode   */
    PIN_MODE_INPUT_PULLUP,      /**< INPUT_PULLUP_mode   */
    PIN_MODE_INPUT_PULLDOWN,    /**< INPUT_PULLDOWN_mode   */
    PIN_MODE_OUTPUT_PP,         /**< OUTPUT_PP_mode   */
    PIN_MODE_OUTPUT_OD,         /**< OUTPUT_OD_mode   */
    PIN_MODE_MAX                /**< max_Pin_mode   */
};

/* public typedef ----------------------------------------------------------- */
/**
 * @brief chal_pin cls
 * 
 */
typedef struct chal_pin
{
    chal_dev_t super;              /**<father class */
    const struct chal_pin_ops *ops;/**< member_func   */
    uint8_t mode;                  /**< pin_mode   */
    bool status;                   /**< pin_status   */

} chal_pin_t;

/**
 * @brief chal_pin cls member_func
 * 
 */
typedef struct chal_pin_ops
{
    void (* init)(chal_pin_t * const this);
    void (* set_mode)(chal_pin_t * const this, uint8_t mode);
    bool (* get_status)(chal_pin_t * const this);
    void (* set_status)(chal_pin_t * const this, bool status);

} chal_pin_ops_t;


/* public functions --------------------------------------------------------- */
/* For low-level driver. */
void chal_pin_register(chal_pin_t * const this,
                        const char *name,
                        const chal_pin_ops_t *ops,
                        void *user_data);

/* For high-level code. */
void chal_pin_set_mode(chal_dev_t * const this, uint8_t mode);
bool chal_pin_get_status(chal_dev_t * const this);
void chal_pin_set_status(chal_dev_t * const this, bool status);




#endif

