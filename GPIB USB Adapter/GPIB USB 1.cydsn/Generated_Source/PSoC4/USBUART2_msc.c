/***************************************************************************//**
* \file USBUART2_cdc.c
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

#include "USBUART2_msc.h"
#include "USBUART2_pvt.h"
#include "cyapicallbacks.h"

#if (USBUART2_HANDLE_MSC_REQUESTS)

/***************************************
*          Internal variables
***************************************/

static uint8 USBUART2_lunCount = USBUART2_MSC_LUN_NUMBER;


/*******************************************************************************
* Function Name: USBUART2_DispatchMSCClassRqst
****************************************************************************//**
*   
*  \internal 
*  This routine dispatches MSC class requests.
*
* \return
*  Status of request processing: handled or not handled.
*
* \globalvars
*  USBUART2_lunCount - stores number of LUN (logical units).
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBUART2_DispatchMSCClassRqst(void) 
{
    uint8 requestHandled = USBUART2_FALSE;
    
    /* Get request data. */
    uint16 value  = USBUART2_GET_UINT16(USBUART2_wValueHiReg,  USBUART2_wValueLoReg);
    uint16 dataLength = USBUART2_GET_UINT16(USBUART2_wLengthHiReg, USBUART2_wLengthLoReg);
       
    /* Check request direction: D2H or H2D. */
    if (0u != (USBUART2_bmRequestTypeReg & USBUART2_RQST_DIR_D2H))
    {
        /* Handle direction from device to host. */
        
        if (USBUART2_MSC_GET_MAX_LUN == USBUART2_bRequestReg)
        {
            /* Check request fields. */
            if ((value  == USBUART2_MSC_GET_MAX_LUN_WVALUE) &&
                (dataLength == USBUART2_MSC_GET_MAX_LUN_WLENGTH))
            {
                /* Reply to Get Max LUN request: setup control read. */
                USBUART2_currentTD.pData = &USBUART2_lunCount;
                USBUART2_currentTD.count =  USBUART2_MSC_GET_MAX_LUN_WLENGTH;
                
                requestHandled  = USBUART2_InitControlRead();
            }
        }
    }
    else
    {
        /* Handle direction from host to device. */
        
        if (USBUART2_MSC_RESET == USBUART2_bRequestReg)
        {
            /* Check request fields. */
            if ((value  == USBUART2_MSC_RESET_WVALUE) &&
                (dataLength == USBUART2_MSC_RESET_WLENGTH))
            {
                /* Handle to Bulk-Only Reset request: no data control transfer. */
                USBUART2_currentTD.count = USBUART2_MSC_RESET_WLENGTH;
                
            #ifdef USBUART2_DISPATCH_MSC_CLASS_MSC_RESET_RQST_CALLBACK
                USBUART2_DispatchMSCClass_MSC_RESET_RQST_Callback();
            #endif /* (USBUART2_DISPATCH_MSC_CLASS_MSC_RESET_RQST_CALLBACK) */
                
                requestHandled = USBUART2_InitNoDataControlTransfer();
            }
        }
    }
    
    return (requestHandled);
}


/*******************************************************************************
* Function Name: USBUART2_MSC_SetLunCount
****************************************************************************//**
*
*  This function sets the number of logical units supported in the application. 
*  The default number of logical units is set in the component customizer.
*
*  \param lunCount: Count of the logical units. Valid range is between 1 and 16.
*
*
* \globalvars
*  USBUART2_lunCount - stores number of LUN (logical units).
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBUART2_MSC_SetLunCount(uint8 lunCount) 
{
    USBUART2_lunCount = (lunCount - 1u);
}


/*******************************************************************************
* Function Name: USBUART2_MSC_GetLunCount
****************************************************************************//**
*
*  This function returns the number of logical units.
*
* \return
*   Number of the logical units.
*
* \globalvars
*  USBUART2_lunCount - stores number of LUN (logical units).
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBUART2_MSC_GetLunCount(void) 
{
    return (USBUART2_lunCount + 1u);
}   

#endif /* (USBUART2_HANDLE_MSC_REQUESTS) */


/* [] END OF FILE */
