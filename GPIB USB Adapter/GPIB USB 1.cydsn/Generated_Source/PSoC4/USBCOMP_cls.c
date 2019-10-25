/***************************************************************************//**
* \file USBCOMP_cls.c
* \version 3.20
*
* \brief
*  This file contains the USB Class request handler.
*
********************************************************************************
* \copyright
* Copyright 2008-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USBCOMP_pvt.h"
#include "cyapicallbacks.h"

#if(USBCOMP_EXTERN_CLS == USBCOMP_FALSE)

/***************************************
* User Implemented Class Driver Declarations.
***************************************/
/* `#START USER_DEFINED_CLASS_DECLARATIONS` Place your declaration here */

/* `#END` */


/*******************************************************************************
* Function Name: USBCOMP_DispatchClassRqst
****************************************************************************//**
*  This routine dispatches class specific requests depend on interface class.
*
* \return
*  requestHandled.
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBCOMP_DispatchClassRqst(void) 
{
    uint8 interfaceNumber;
    uint8 requestHandled = USBCOMP_FALSE;

    /* Get interface to which request is intended. */
    switch (USBCOMP_bmRequestTypeReg & USBCOMP_RQST_RCPT_MASK)
    {
        case USBCOMP_RQST_RCPT_IFC:
            /* Class-specific request directed to interface: wIndexLoReg 
            * contains interface number.
            */
            interfaceNumber = (uint8) USBCOMP_wIndexLoReg;
            break;
        
        case USBCOMP_RQST_RCPT_EP:
            /* Class-specific request directed to endpoint: wIndexLoReg contains 
            * endpoint number. Find interface related to endpoint. 
            */
            interfaceNumber = USBCOMP_EP[USBCOMP_wIndexLoReg & USBCOMP_DIR_UNUSED].interface;
            break;
            
        default:
            /* Default interface is zero. */
            interfaceNumber = 0u;
            break;
    }
    
    /* Check that interface is within acceptable range */
    if (interfaceNumber <= USBCOMP_MAX_INTERFACES_NUMBER)
    {
    #if (defined(USBCOMP_ENABLE_HID_CLASS)   || \
         defined(USBCOMP_ENABLE_AUDIO_CLASS) || \
         defined(USBCOMP_ENABLE_CDC_CLASS)   || \
         USBCOMP_ENABLE_MSC_CLASS)

        /* Handle class request depends on interface type. */
        switch (USBCOMP_interfaceClass[interfaceNumber])
        {
        #if defined(USBCOMP_ENABLE_HID_CLASS)
            case USBCOMP_CLASS_HID:
                requestHandled = USBCOMP_DispatchHIDClassRqst();
                break;
        #endif /* (USBCOMP_ENABLE_HID_CLASS) */
                
        #if defined(USBCOMP_ENABLE_AUDIO_CLASS)
            case USBCOMP_CLASS_AUDIO:
                requestHandled = USBCOMP_DispatchAUDIOClassRqst();
                break;
        #endif /* (USBCOMP_CLASS_AUDIO) */
                
        #if defined(USBCOMP_ENABLE_CDC_CLASS)
            case USBCOMP_CLASS_CDC:
                requestHandled = USBCOMP_DispatchCDCClassRqst();
                break;
        #endif /* (USBCOMP_ENABLE_CDC_CLASS) */
            
        #if (USBCOMP_ENABLE_MSC_CLASS)
            case USBCOMP_CLASS_MSD:
            #if (USBCOMP_HANDLE_MSC_REQUESTS)
                /* MSC requests are handled by the component. */
                requestHandled = USBCOMP_DispatchMSCClassRqst();
            #elif defined(USBCOMP_DISPATCH_MSC_CLASS_RQST_CALLBACK)
                /* MSC requests are handled by user defined callbcak. */
                requestHandled = USBCOMP_DispatchMSCClassRqst_Callback();
            #else
                /* MSC requests are not handled. */
                requestHandled = USBCOMP_FALSE;
            #endif /* (USBCOMP_HANDLE_MSC_REQUESTS) */
                break;
        #endif /* (USBCOMP_ENABLE_MSC_CLASS) */
            
            default:
                /* Request is not handled: unknown class request type. */
                requestHandled = USBCOMP_FALSE;
                break;
        }
    #endif /* Class support is enabled */
    }
    
    /* `#START USER_DEFINED_CLASS_CODE` Place your Class request here */

    /* `#END` */

#ifdef USBCOMP_DISPATCH_CLASS_RQST_CALLBACK
    if (USBCOMP_FALSE == requestHandled)
    {
        requestHandled = USBCOMP_DispatchClassRqst_Callback(interfaceNumber);
    }
#endif /* (USBCOMP_DISPATCH_CLASS_RQST_CALLBACK) */

    return (requestHandled);
}


/*******************************************************************************
* Additional user functions supporting Class Specific Requests
********************************************************************************/

/* `#START CLASS_SPECIFIC_FUNCTIONS` Place any additional functions here */

/* `#END` */

#endif /* USBCOMP_EXTERN_CLS */


/* [] END OF FILE */
