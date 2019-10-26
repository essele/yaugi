/***************************************************************************//**
* \file USBTMC_cdc.c
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

#include "USBTMC_msc.h"
#include "USBTMC_pvt.h"
#include "cyapicallbacks.h"

#if (USBTMC_HANDLE_MSC_REQUESTS)

/***************************************
*          Internal variables
***************************************/

static uint8 USBTMC_lunCount = USBTMC_MSC_LUN_NUMBER;


/*******************************************************************************
* Function Name: USBTMC_DispatchMSCClassRqst
****************************************************************************//**
*   
*  \internal 
*  This routine dispatches MSC class requests.
*
* \return
*  Status of request processing: handled or not handled.
*
* \globalvars
*  USBTMC_lunCount - stores number of LUN (logical units).
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBTMC_DispatchMSCClassRqst(void) 
{
    uint8 requestHandled = USBTMC_FALSE;
    
    /* Get request data. */
    uint16 value  = USBTMC_GET_UINT16(USBTMC_wValueHiReg,  USBTMC_wValueLoReg);
    uint16 dataLength = USBTMC_GET_UINT16(USBTMC_wLengthHiReg, USBTMC_wLengthLoReg);
       
    /* Check request direction: D2H or H2D. */
    if (0u != (USBTMC_bmRequestTypeReg & USBTMC_RQST_DIR_D2H))
    {
        /* Handle direction from device to host. */
        
        if (USBTMC_MSC_GET_MAX_LUN == USBTMC_bRequestReg)
        {
            /* Check request fields. */
            if ((value  == USBTMC_MSC_GET_MAX_LUN_WVALUE) &&
                (dataLength == USBTMC_MSC_GET_MAX_LUN_WLENGTH))
            {
                /* Reply to Get Max LUN request: setup control read. */
                USBTMC_currentTD.pData = &USBTMC_lunCount;
                USBTMC_currentTD.count =  USBTMC_MSC_GET_MAX_LUN_WLENGTH;
                
                requestHandled  = USBTMC_InitControlRead();
            }
        }
    }
    else
    {
        /* Handle direction from host to device. */
        
        if (USBTMC_MSC_RESET == USBTMC_bRequestReg)
        {
            /* Check request fields. */
            if ((value  == USBTMC_MSC_RESET_WVALUE) &&
                (dataLength == USBTMC_MSC_RESET_WLENGTH))
            {
                /* Handle to Bulk-Only Reset request: no data control transfer. */
                USBTMC_currentTD.count = USBTMC_MSC_RESET_WLENGTH;
                
            #ifdef USBTMC_DISPATCH_MSC_CLASS_MSC_RESET_RQST_CALLBACK
                USBTMC_DispatchMSCClass_MSC_RESET_RQST_Callback();
            #endif /* (USBTMC_DISPATCH_MSC_CLASS_MSC_RESET_RQST_CALLBACK) */
                
                requestHandled = USBTMC_InitNoDataControlTransfer();
            }
        }
    }
    
    return (requestHandled);
}


/*******************************************************************************
* Function Name: USBTMC_MSC_SetLunCount
****************************************************************************//**
*
*  This function sets the number of logical units supported in the application. 
*  The default number of logical units is set in the component customizer.
*
*  \param lunCount: Count of the logical units. Valid range is between 1 and 16.
*
*
* \globalvars
*  USBTMC_lunCount - stores number of LUN (logical units).
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_MSC_SetLunCount(uint8 lunCount) 
{
    USBTMC_lunCount = (lunCount - 1u);
}


/*******************************************************************************
* Function Name: USBTMC_MSC_GetLunCount
****************************************************************************//**
*
*  This function returns the number of logical units.
*
* \return
*   Number of the logical units.
*
* \globalvars
*  USBTMC_lunCount - stores number of LUN (logical units).
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBTMC_MSC_GetLunCount(void) 
{
    return (USBTMC_lunCount + 1u);
}   

#endif /* (USBTMC_HANDLE_MSC_REQUESTS) */


/* [] END OF FILE */
