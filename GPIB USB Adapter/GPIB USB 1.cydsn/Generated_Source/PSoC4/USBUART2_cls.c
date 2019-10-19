/***************************************************************************//**
* \file USBUART2_cls.c
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

#include "USBUART2_pvt.h"
#include "cyapicallbacks.h"

#if(USBUART2_EXTERN_CLS == USBUART2_FALSE)

/***************************************
* User Implemented Class Driver Declarations.
***************************************/
/* `#START USER_DEFINED_CLASS_DECLARATIONS` Place your declaration here */

/* `#END` */


/*******************************************************************************
* Function Name: USBUART2_DispatchClassRqst
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
uint8 USBUART2_DispatchClassRqst(void) 
{
    uint8 interfaceNumber;
    uint8 requestHandled = USBUART2_FALSE;

    /* Get interface to which request is intended. */
    switch (USBUART2_bmRequestTypeReg & USBUART2_RQST_RCPT_MASK)
    {
        case USBUART2_RQST_RCPT_IFC:
            /* Class-specific request directed to interface: wIndexLoReg 
            * contains interface number.
            */
            interfaceNumber = (uint8) USBUART2_wIndexLoReg;
            break;
        
        case USBUART2_RQST_RCPT_EP:
            /* Class-specific request directed to endpoint: wIndexLoReg contains 
            * endpoint number. Find interface related to endpoint. 
            */
            interfaceNumber = USBUART2_EP[USBUART2_wIndexLoReg & USBUART2_DIR_UNUSED].interface;
            break;
            
        default:
            /* Default interface is zero. */
            interfaceNumber = 0u;
            break;
    }
    
    /* Check that interface is within acceptable range */
    if (interfaceNumber <= USBUART2_MAX_INTERFACES_NUMBER)
    {
    #if (defined(USBUART2_ENABLE_HID_CLASS)   || \
         defined(USBUART2_ENABLE_AUDIO_CLASS) || \
         defined(USBUART2_ENABLE_CDC_CLASS)   || \
         USBUART2_ENABLE_MSC_CLASS)

        /* Handle class request depends on interface type. */
        switch (USBUART2_interfaceClass[interfaceNumber])
        {
        #if defined(USBUART2_ENABLE_HID_CLASS)
            case USBUART2_CLASS_HID:
                requestHandled = USBUART2_DispatchHIDClassRqst();
                break;
        #endif /* (USBUART2_ENABLE_HID_CLASS) */
                
        #if defined(USBUART2_ENABLE_AUDIO_CLASS)
            case USBUART2_CLASS_AUDIO:
                requestHandled = USBUART2_DispatchAUDIOClassRqst();
                break;
        #endif /* (USBUART2_CLASS_AUDIO) */
                
        #if defined(USBUART2_ENABLE_CDC_CLASS)
            case USBUART2_CLASS_CDC:
                requestHandled = USBUART2_DispatchCDCClassRqst();
                break;
        #endif /* (USBUART2_ENABLE_CDC_CLASS) */
            
        #if (USBUART2_ENABLE_MSC_CLASS)
            case USBUART2_CLASS_MSD:
            #if (USBUART2_HANDLE_MSC_REQUESTS)
                /* MSC requests are handled by the component. */
                requestHandled = USBUART2_DispatchMSCClassRqst();
            #elif defined(USBUART2_DISPATCH_MSC_CLASS_RQST_CALLBACK)
                /* MSC requests are handled by user defined callbcak. */
                requestHandled = USBUART2_DispatchMSCClassRqst_Callback();
            #else
                /* MSC requests are not handled. */
                requestHandled = USBUART2_FALSE;
            #endif /* (USBUART2_HANDLE_MSC_REQUESTS) */
                break;
        #endif /* (USBUART2_ENABLE_MSC_CLASS) */
            
            default:
                /* Request is not handled: unknown class request type. */
                requestHandled = USBUART2_FALSE;
                break;
        }
    #endif /* Class support is enabled */
    }
    
    /* `#START USER_DEFINED_CLASS_CODE` Place your Class request here */

    /* `#END` */

#ifdef USBUART2_DISPATCH_CLASS_RQST_CALLBACK
    if (USBUART2_FALSE == requestHandled)
    {
        requestHandled = USBUART2_DispatchClassRqst_Callback(interfaceNumber);
    }
#endif /* (USBUART2_DISPATCH_CLASS_RQST_CALLBACK) */

    return (requestHandled);
}


/*******************************************************************************
* Additional user functions supporting Class Specific Requests
********************************************************************************/

/* `#START CLASS_SPECIFIC_FUNCTIONS` Place any additional functions here */

/* `#END` */

#endif /* USBUART2_EXTERN_CLS */


/* [] END OF FILE */
