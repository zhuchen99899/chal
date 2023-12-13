/**
 * @file cHal_device.c
 * @author zc (387646983@qq.com)
 * @brief  device_base_class   
 * @version 0.1
 * @date 2023-11-29
 * @note 设备层，管理所有设备层设备和hal层设备
 * @copyright Copyright (c) 2023
 * 
 */
#include "chal_device.h"


/* private variables -------------------------------------------------------- */
static chal_dev_t *chal_dev_list = NULL;

#if CHAL_OS_EN
/**
 * The os global mutex attribute.
 * use to dev operate mutex
 */
static osMutexAttr_t _mutex_attr_chal =
{
    .name = "mutex_chal",
    .attr_bits = osMutexPrioInherit | osMutexRecursive,
    .cb_mem = NULL,
    .cb_size = 0, 
};
#endif


/* public functions --------------------------------------------------------- */
/**
  * @brief  chal_device registering function.
  * @param  this    this object pointer
  * @param  name    device object name.
  * @param  ops     device object operation interface.
  * @param  ops     device object attribute.
  * @retval None
  * @note this function registered by hal layer
  */
void chal_dev_register(chal_dev_t * const this,
                    const chal_dev_ops_t *ops,
                    chal_dev_attr_t *attr)
{
    // assert(this != NULL);
    // assert(name != NULL);
    // assert(ops != NULL);
    // assert(attr != NULL);

    if(this->status==DEV_REGISTED) return;

    this->status=DEV_REGISTING;

#if CHAL_OS_EN
    /*dev_registing_mutex*/
    //osStatus_t ret = osOK;
    osMutexId_t mutex = osMutexNew(&_mutex_attr_chal);
    osMutexAcquire(mutex, osWaitForever); //lock
    //assert(ret == osOK);
    this->mutex_attr=&_mutex_attr_chal;
#endif
    /*dev mutex lock create*/
    __CHAL_MUTEX_NEW(this);
    /* Set the device data. */
    this->ops = ops;// register function
    memcpy(&this->attr, attr, sizeof(chal_dev_attr_t));//register attr
    this->count_open = 0;
    this->count_lock = 0;

    this->status=DEV_REGISTED;
    //save list
    this->next = chal_dev_list;
    chal_dev_list = this;

#if CHAL_OS_EN
/*device operate mutex  unlocking*/
    osMutexRelease(mutex);
#else

#endif
    

}


/**
 * @brief This function unregisters a device with the device handle.
 * @param this   the pointer of device driver structure
 * @retval None.
 */
void chal_dev_unregister(chal_dev_t *this)
{
    // assert(this != NULL);
    // assert(!chal_device_is_enabled(this));
    
#if CHAL_OS_EN
    /* dev unregisting operate mutex locking. */
    //osStatus_t ret = osOK;
    osMutexId_t mutex = osMutexNew(&_mutex_attr_chal);
    osMutexAcquire(mutex, osWaitForever);
    //assert(ret == osOK);
#endif


    this->status=DEV_UNREGISTING; 
    /* dev  mutex or lock delete. */
    __CHAL_MUTEX_DELETE(this);


#if CHAL_OS_EN
    /* dev unregisting operate mutex unlocking. */
    osMutexRelease(mutex);
    //assert(ret == osOK);
#endif
/*release dev*/
    this->status=DEV_STATUS_NULL;  
    this=NULL;
}


/**
  * @brief  chal_device object handle getting function.
  * @param  name  device object's name.
  * @retval chal_device object handle.
  */
chal_dev_t *chal_dev_find(const char *name)
{
    // assert(name != NULL);

    chal_dev_t *obj = chal_dev_list;
    while (obj != NULL)
    {
        if (strcmp(obj->attr.name, name) == 0)
        {
            break;
        }
        obj = obj->next;
    }

    return obj;
}



/**
  * @brief  chal_device object handle opening function.
  * @param  this  device object handle.
  * @retval See CHal_Err_t.
  */
CHal_Err_t chal_dev_open(chal_dev_t * const this)
{
    //assert(this->ops->open != NULL);

    CHal_Err_t ret = CHAL_OK;

    if (this->attr.sole)
    {
        if (this->count_open != 0)
        {
            ret = CHAL_ERROR;
            goto exit;
        }
    }

    ret = this->ops->open(this);
    if (ret == CHAL_OK)
    {
        this->count_open ++;
    }

exit:
    return ret;
}


/**
  * @brief  chal_device object handle closing function.
  * @param  this  chal_device object handle.
  * @retval See CHal_Err_t.
  */
CHal_Err_t chal_dev_close(chal_dev_t * const this)
{
    //assert(this->ops->close != NULL);

    CHal_Err_t ret = CHAL_OK;

    if (this->count_open > 0)
    {
        ret = this->ops->close(this);
        if (ret == CHAL_OK)
        {
            this->count_open --;
            goto exit;
        }
    }
    else
    {
        ret = CHAL_ERROR;
    }

exit:
    return ret;
}



/**
  * @brief  chal_device object handle reading function.
  * @param  this      chal_device object handle.
  * @param  buffer  Reading buffer.
  * @param  size    The buffer size.
  * @retval If >= 0, the actual data bytes number; if < 0, see CHal_Err_t.
  */
int32_t chal_dev_read(chal_dev_t * const this, void *buffer, uint32_t size)
{
    // assert(this != NULL);
    // assert(buffer != NULL);
    // assert(size != 0);
    // assert(this->ops->read != NULL);

    int32_t ret = 0;

    if (this->count_open != 0)
    {
        ret = this->ops->read(this, buffer, size);
    }
    else
    {
        ret = (int32_t)CHAL_ERROR;
    }
    
    return ret;
}

/**
  * @brief  chal_device object handle writting function.
  * @param  this      chal_device object handle.
  * @param  buffer  Writting buffer.
  * @param  size    The buffer size.
  * @retval If >= 0, the actualThe che k。 data bytes number; if < 0, see CHal_Err_t.
  */
int32_t chal_dev_write(chal_dev_t * const this, const void *buffer, uint32_t size)
{
    //assert(this != NULL);
    //assert(buffer != NULL);
    //assert(size != 0);
    //assert(this->ops->write != NULL);

    int32_t ret = 0;

    if (this->count_open != 0)
    {
        ret = this->ops->write(this, buffer, size);
    }
    else
    {
        ret = (int32_t)CHAL_ERROR;
    }
    
    return ret;
}

/* ----------------------------- end of file -------------------------------- */

