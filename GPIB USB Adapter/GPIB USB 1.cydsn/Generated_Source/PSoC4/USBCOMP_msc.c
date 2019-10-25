/***************************************************************************//**
* \file USBCOMP_cdc.c
* \version 3.20
*
* \brief
*  This file contains the USB MSC Class request handler and global API for MSC 
*  class.
*
* Related Document:
*  Universal Serial Bus Class Definitions for Communication Devices Version 1.1
*
********************************************************************************
* \copyright
* Copyright 2012-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USBCOMP_msc.h"
#include "USBCOMP_pvt.h"
#include "cyapicallbacks.h"

#if (USBCOMP_HANDLE_MSC_REQUESTS)

/***************************************
*          Internal variables
***************************************/

static uint8 USBCOMP_lunCount = USBCOMP_MSC_LUN_NUMBER;


/*******************************************************************************
* Function Name: USBCOMP_DispatchMSCClassRqst
****************************************************************************//**
*   
*  \internal 
*  This routine dispatches MSC class requests.
*
* \return
*  Status of request processing: handled or not handled.
*
* \globalvars
*  USBCOMP_lunCount - stores number of LUN (logical units).
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBCOMP_DispatchMSCClassRqst(void) 
{
    uint8 requestHandled = USBCOMP_FALSE;
    
    /* Get request data. */
    uint16 value  = USBCOMP_GET_UINT16(USBCOMP_wValueHiReg,  USBCOMP_wValueLoReg);
    uint16 dataLength = USBCOMP_GET_UINT16(USBCOMP_wLengthHiReg, USBCOMP_wLengthLoReg);
       
    /* Check request direction: D2H or H2D. */
    if (0u != (USBCOMP_bmRequestTypeReg & USBCOMP_RQST_DIR_D2H))
    {
        /* Handle direction from device to host. */
        
        if (USBCOMP_MSC_GET_MAX_LUN == USBCOMP_bRequestReg)
        {
            /* Check request fields. */
            if ((value  == USBCOMP_MSC_GET_MAX_LUN_WVALUE) &&
                (dataLength == USBCOMP_MSC_GET_MAX_LUN_WLENGTH))
            {
                /* Reply to Get Max LUN request: setup control read. */
                USBCOMP_currentTD.pData = &USBCOMP_lunCount;
                USBCOMP_currentTD.count =  USBCOMP_MSC_GET_MAX_LUN_WLENGTH;
                
                requestHandled  = USBCOMP_InitControlRead();
            }
        }
    }
    else
    {
        /* Handle direction from host to device. */
        
        if (USBCOMP_MSC_RESET == USBCOMP_bRequestReg)
        {
            /* Check request fields. */
            if ((value  == USBCOMP_MSC_RESET_WVALUE) &&
                (dataLength == USBCOMP_MSC_RESET_WLENGTH))
            {
                /* Handle to Bulk-Only Reset request: no data control transfer. */
                USBCOMP_currentTD.count = USBCOMP_MSC_RESET_WLENGTH;
                
            #ifdef USBCOMP_DISPATCH_MSC_CLASS_MSC_RESET_RQST_CALLBACK
                USBCOMP_DispatchMSCClass_MSC_RESET_RQST_Callback();
            #endif /* (USBCOMP_DISPATCH_MSC_CLASS_MSC_RESET_RQST_CALLBACK) */
                
                requestHandled = USBCOMP_InitNoDataControlTransfer();
            }
        }
    }
    
    return (requestHandled);
}


/*******************************************************************************
* Function Name: USBCOMP_MSC_SetLunCount
****************************************************************************//**
*
*  This function sets the number of logical units supported in the application. 
*  The default number of logical units is set in the component customizer.
*
*  \param lunCount: Count of the logical units. Valid range is between 1 and 16.
*
*
* \globalvars
*  USBCOMP_lunCount - stores number of LUN (logical units).
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_MSC_SetLunCount(uint8 lunCount) 
{
    USBCOMP_lunCount = (lunCount - 1u);
}


/*******************************************************************************
* Function Name: USBCOMP_MSC_GetLunCount
****************************************************************************//**
*
*  This function returns the number of logical units.
*
* \return
*   Number of the logical units.
*
* \globalvars
*  USBCOMP_lunCount - stores number of LUN (logical units).
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBCOMP_MSC_GetLunCount(void) 
{
    return (USBCOMP_lunCount + 1u);
}   

#endif /* (USBCOMP_HANDLE_MSC_REQUESTS) */


/* [] END OF FILE */
