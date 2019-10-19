/***************************************************************************//**
* \file USBUART2_vnd.c
* \version 3.20
*
* \brief
*  This file contains the  USB vendor request handler.
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

#if(USBUART2_EXTERN_VND == USBUART2_FALSE)

/***************************************
* Vendor Specific Declarations
***************************************/

/* `#START VENDOR_SPECIFIC_DECLARATIONS` Place your declaration here */

/* `#END` */


/*******************************************************************************
* Function Name: USBUART2_HandleVendorRqst
****************************************************************************//**
*
*  This routine provide users with a method to implement vendor specific
*  requests.
*
*  To implement vendor specific requests, add your code in this function to
*  decode and disposition the request.  If the request is handled, your code
*  must set the variable "requestHandled" to TRUE, indicating that the
*  request has been handled.
*
* \return
*  requestHandled.
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBUART2_HandleVendorRqst(void) 
{
    uint8 requestHandled = USBUART2_FALSE;

    /* Check request direction: D2H or H2D. */
    if (0u != (USBUART2_bmRequestTypeReg & USBUART2_RQST_DIR_D2H))
    {
        /* Handle direction from device to host. */
        
        switch (USBUART2_bRequestReg)
        {
            case USBUART2_GET_EXTENDED_CONFIG_DESCRIPTOR:
            #if defined(USBUART2_ENABLE_MSOS_STRING)
                USBUART2_currentTD.pData = (volatile uint8 *) &USBUART2_MSOS_CONFIGURATION_DESCR[0u];
                USBUART2_currentTD.count = USBUART2_MSOS_CONFIGURATION_DESCR[0u];
                requestHandled  = USBUART2_InitControlRead();
            #endif /* (USBUART2_ENABLE_MSOS_STRING) */
                break;
            
            default:
                break;
        }
    }

    /* `#START VENDOR_SPECIFIC_CODE` Place your vendor specific request here */

    /* `#END` */

#ifdef USBUART2_HANDLE_VENDOR_RQST_CALLBACK
    if (USBUART2_FALSE == requestHandled)
    {
        requestHandled = USBUART2_HandleVendorRqst_Callback();
    }
#endif /* (USBUART2_HANDLE_VENDOR_RQST_CALLBACK) */

    return (requestHandled);
}


/*******************************************************************************
* Additional user functions supporting Vendor Specific Requests
********************************************************************************/

/* `#START VENDOR_SPECIFIC_FUNCTIONS` Place any additional functions here */

/* `#END` */


#endif /* USBUART2_EXTERN_VND */


/* [] END OF FILE */
