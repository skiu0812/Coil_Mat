/*
 * usb_mid.h
 *
 *  Created on: Sep 4, 2022
 *      Author: KSK
 */

#ifndef INC_USB_MID_H_
#define INC_USB_MID_H_


#include "hw_def.h"


#ifdef _USE_HW_USB


bool usbInit(void);


#endif



#ifdef _USE_HW_CDC


extern bool eFlagCDCBufFull;


uint32_t  usbCDCWrite(uint8_t *p_data, uint16_t length);
uint8_t   usbCDCSOF(struct _USBD_HandleTypeDef *pdev);


#endif


#endif /* INC_USB_MID_H_ */
