/***************************************************************************//**
* \file USBTMC_cls.c
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

#include "USBTMC_pvt.h"
#include "cyapicallbacks.h"

#if(USBTMC_EXTERN_CLS == USBTMC_FALSE)

/***************************************
* User Implemented Class Driver Declarations.
***************************************/
/* `#START USER_DEFINED_CLASS_DECLARATIONS` Place your declaration here */

/* `#END` */


/*******************************************************************************
* Function Name: USBTMC_DispatchClassRqst
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
uint8 USBTMC_DispatchClassRqst(void) 
{
    uint8 interfaceNumber;
    uint8 requestHandled = USBTMC_FALSE;

    /* Get interface to which request is intended. */
    switch (USBTMC_bmRequestTypeReg & USBTMC_RQST_RCPT_MASK)
    {
        case USBTMC_RQST_RCPT_IFC:
            /* Class-specific request directed to interface: wIndexLoReg 
            * contains interface number.
            */
            interfaceNumber = (uint8) USBTMC_wIndexLoReg;
            break;
        
        case USBTMC_RQST_RCPT_EP:
            /* Class-specific request directed to endpoint: wIndexLoReg contains 
            * endpoint number. Find interface related to endpoint. 
            */
            interfaceNumber = USBTMC_EP[USBTMC_wIndexLoReg & USBTMC_DIR_UNUSED].interface;
            break;
            
        default:
            /* Default interface is zero. */
            interfaceNumber = 0u;
            break;
    }
    
    /* Check that interface is within acceptable range */
    if (interfaceNumber <= USBTMC_MAX_INTERFACES_NUMBER)
    {
    #if (defined(USBTMC_ENABLE_HID_CLASS)   || \
         defined(USBTMC_ENABLE_AUDIO_CLASS) || \
         defined(USBTMC_ENABLE_CDC_CLASS)   || \
         USBTMC_ENABLE_MSC_CLASS)

        /* Handle class request depends on interface type. */
        switch (USBTMC_interfaceClass[interfaceNumber])
        {
        #if defined(USBTMC_ENABLE_HID_CLASS)
            case USBTMC_CLASS_HID:
                requestHandled = USBTMC_DispatchHIDClassRqst();
                break;
        #endif /* (USBTMC_ENABLE_HID_CLASS) */
                
        #if defined(USBTMC_ENABLE_AUDIO_CLASS)
            case USBTMC_CLASS_AUDIO:
                requestHandled = USBTMC_DispatchAUDIOClassRqst();
                break;
        #endif /* (USBTMC_CLASS_AUDIO) */
                
        #if defined(USBTMC_ENABLE_CDC_CLASS)
            case USBTMC_CLASS_CDC:
                requestHandled = USBTMC_DispatchCDCClassRqst();
                break;
        #endif /* (USBTMC_ENABLE_CDC_CLASS) */
            
        #if (USBTMC_ENABLE_MSC_CLASS)
            case USBTMC_CLASS_MSD:
            #if (USBTMC_HANDLE_MSC_REQUESTS)
                /* MSC requests are handled by the component. */
                requestHandled = USBTMC_DispatchMSCClassRqst();
            #elif defined(USBTMC_DISPATCH_MSC_CLASS_RQST_CALLBACK)
                /* MSC requests are handled by user defined callbcak. */
                requestHandled = USBTMC_DispatchMSCClassRqst_Callback();
            #else
                /* MSC requests are not handled. */
                requestHandled = USBTMC_FALSE;
            #endif /* (USBTMC_HANDLE_MSC_REQUESTS) */
                break;
        #endif /* (USBTMC_ENABLE_MSC_CLASS) */
            
            default:
                /* Request is not handled: unknown class request type. */
                requestHandled = USBTMC_FALSE;
                break;
        }
    #endif /* Class support is enabled */
    }
    
    /* `#START USER_DEFINED_CLASS_CODE` Place your Class request here */

    /* `#END` */

#ifdef USBTMC_DISPATCH_CLASS_RQST_CALLBACK
    if (USBTMC_FALSE == requestHandled)
    {
        requestHandled = USBTMC_DispatchClassRqst_Callback(interfaceNumber);
    }
#endif /* (USBTMC_DISPATCH_CLASS_RQST_CALLBACK) */

    return (requestHandled);
}


/*******************************************************************************
* Additional user functions supporting Class Specific Requests
********************************************************************************/

/* `#START CLASS_SPECIFIC_FUNCTIONS` Place any additional functions here */

/* `#END` */

#endif /* USBTMC_EXTERN_CLS */


/* [] END OF FILE */
