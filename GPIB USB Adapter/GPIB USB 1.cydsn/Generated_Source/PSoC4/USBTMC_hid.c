/***************************************************************************//**
* \file USBTMC_hid.c
* \version 3.20
*
* \brief
*  This file contains the USB HID Class request handler. 
*
* Related Document:
*  Device Class Definition for Human Interface Devices (HID) Version 1.11
*
********************************************************************************
* \copyright
* Copyright 2008-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USBTMC_hid.h"
#include "USBTMC_pvt.h"
#include "cyapicallbacks.h"


#if defined(USBTMC_ENABLE_HID_CLASS)

/***************************************
*    HID Variables
***************************************/
/** This variable is initialized in the USBFS_InitComponent() API to the 
 * PROTOCOL_REPORT value. It is controlled by the host using the 
 * HID_SET_PROTOCOL request. The value is returned to the user code by the 
 * USBFS_GetProtocol() API.*/
volatile uint8 USBTMC_hidProtocol[USBTMC_MAX_INTERFACES_NUMBER];

/** This variable controls the HID report rate. It is controlled by the host 
 * using the HID_SET_IDLE request and used by the USBFS_UpdateHIDTimer() API to 
 * reload timer.*/
volatile uint8 USBTMC_hidIdleRate[USBTMC_MAX_INTERFACES_NUMBER];

/** This variable contains the timer counter, which is decremented and reloaded 
 * by the USBFS_UpdateHIDTimer() API.*/
volatile uint8 USBTMC_hidIdleTimer[USBTMC_MAX_INTERFACES_NUMBER]; /* HID device idle rate value */


/***************************************
* Custom Declarations
***************************************/

/* `#START HID_CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


/*******************************************************************************
* Function Name: USBTMC_UpdateHIDTimer
****************************************************************************//**
*
*  This function updates the HID Report idle timer and returns the status and 
*  reloads the timer if it expires.
*
*  \param interface Contains the interface number.
*
* \return
*  Returns the state of the HID timer. Symbolic names and their associated values are given here:
*  Return Value               |Notes
*  ---------------------------|------------------------------------------------
*  USBFS_IDLE_TIMER_EXPIRED   | The timer expired.
*  USBFS_IDLE_TIMER_RUNNING   | The timer is running.
*  USBFS_IDLE_TIMER_IDEFINITE | The report is sent when data or state changes.
*
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBTMC_UpdateHIDTimer(uint8 interface) 
{
    uint8 stat = USBTMC_IDLE_TIMER_INDEFINITE;

    if(USBTMC_hidIdleRate[interface] != 0u)
    {
        if(USBTMC_hidIdleTimer[interface] > 0u)
        {
            USBTMC_hidIdleTimer[interface]--;
            stat = USBTMC_IDLE_TIMER_RUNNING;
        }
        else
        {
            USBTMC_hidIdleTimer[interface] = USBTMC_hidIdleRate[interface];
            stat = USBTMC_IDLE_TIMER_EXPIRED;
        }
    }

    return((uint8)stat);
}


/*******************************************************************************
* Function Name: USBTMC_GetProtocol
****************************************************************************//**
*
*  This function returns the HID protocol value for the selected interface.
*
*  \param interface:  Contains the interface number.
*
*  \return
*  Returns the protocol value. 
*
*******************************************************************************/
uint8 USBTMC_GetProtocol(uint8 interface) 
{
    return(USBTMC_hidProtocol[interface]);
}


/*******************************************************************************
* Function Name: USBTMC_DispatchHIDClassRqst
****************************************************************************//**
*
*  This routine dispatches class requests
*
* \return
*  Results of HID Class request handling: 
*  - USBTMC_TRUE  - request was handled without errors
*  - USBTMC_FALSE - error occurs during handling of request  
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBTMC_DispatchHIDClassRqst(void) 
{
    uint8 requestHandled = USBTMC_FALSE;

    uint8 interfaceNumber = (uint8) USBTMC_wIndexLoReg;
    
    /* Check request direction: D2H or H2D. */
    if (0u != (USBTMC_bmRequestTypeReg & USBTMC_RQST_DIR_D2H))
    {
        /* Handle direction from device to host. */
        
        switch (USBTMC_bRequestReg)
        {
            case USBTMC_GET_DESCRIPTOR:
                if (USBTMC_wValueHiReg == USBTMC_DESCR_HID_CLASS)
                {
                    USBTMC_FindHidClassDecriptor();
                    if (USBTMC_currentTD.count != 0u)
                    {
                        requestHandled = USBTMC_InitControlRead();
                    }
                }
                else if (USBTMC_wValueHiReg == USBTMC_DESCR_HID_REPORT)
                {
                    USBTMC_FindReportDescriptor();
                    if (USBTMC_currentTD.count != 0u)
                    {
                        requestHandled = USBTMC_InitControlRead();
                    }
                }
                else
                {   
                    /* Do not handle this request. */
                }
                break;
                
            case USBTMC_HID_GET_REPORT:
                USBTMC_FindReport();
                if (USBTMC_currentTD.count != 0u)
                {
                    requestHandled = USBTMC_InitControlRead();
                }
                break;

            case USBTMC_HID_GET_IDLE:
                /* This function does not support multiple reports per interface*/
                /* Validate interfaceNumber and Report ID (should be 0): Do not support Idle per Report ID */
                if ((interfaceNumber < USBTMC_MAX_INTERFACES_NUMBER) && (USBTMC_wValueLoReg == 0u)) 
                {
                    USBTMC_currentTD.count = 1u;
                    USBTMC_currentTD.pData = &USBTMC_hidIdleRate[interfaceNumber];
                    requestHandled  = USBTMC_InitControlRead();
                }
                break;
                
            case USBTMC_HID_GET_PROTOCOL:
                /* Validate interfaceNumber */
                if( interfaceNumber < USBTMC_MAX_INTERFACES_NUMBER)
                {
                    USBTMC_currentTD.count = 1u;
                    USBTMC_currentTD.pData = &USBTMC_hidProtocol[interfaceNumber];
                    requestHandled  = USBTMC_InitControlRead();
                }
                break;
                
            default:    /* requestHandled is initialized as FALSE by default */
                break;
        }
    }
    else
    {   
        /* Handle direction from host to device. */
        
        switch (USBTMC_bRequestReg)
        {
            case USBTMC_HID_SET_REPORT:
                USBTMC_FindReport();
                if (USBTMC_currentTD.count != 0u)
                {
                    requestHandled = USBTMC_InitControlWrite();
                }
                break;
                
            case USBTMC_HID_SET_IDLE:
                /* This function does not support multiple reports per interface */
                /* Validate interfaceNumber and Report ID (should be 0): Do not support Idle per Report ID */
                if ((interfaceNumber < USBTMC_MAX_INTERFACES_NUMBER) && (USBTMC_wValueLoReg == 0u))
                {
                    USBTMC_hidIdleRate[interfaceNumber] = (uint8)USBTMC_wValueHiReg;
                    /* With regards to HID spec: "7.2.4 Set_Idle Request"
                    *  Latency. If the current period has gone past the
                    *  newly proscribed time duration, then a report
                    *  will be generated immediately.
                    */
                    if(USBTMC_hidIdleRate[interfaceNumber] <
                       USBTMC_hidIdleTimer[interfaceNumber])
                    {
                        /* Set the timer to zero and let the UpdateHIDTimer() API return IDLE_TIMER_EXPIRED status*/
                        USBTMC_hidIdleTimer[interfaceNumber] = 0u;
                    }
                    /* If the new request is received within 4 milliseconds
                    *  (1 count) of the end of the current period, then the
                    *  new request will have no effect until after the report.
                    */
                    else if(USBTMC_hidIdleTimer[interfaceNumber] <= 1u)
                    {
                        /* Do nothing.
                        *  Let the UpdateHIDTimer() API continue to work and
                        *  return IDLE_TIMER_EXPIRED status
                        */
                    }
                    else
                    {   /* Reload the timer*/
                        USBTMC_hidIdleTimer[interfaceNumber] =
                        USBTMC_hidIdleRate[interfaceNumber];
                    }
                    requestHandled = USBTMC_InitNoDataControlTransfer();
                }
                break;

            case USBTMC_HID_SET_PROTOCOL:
                /* Validate interfaceNumber and protocol (must be 0 or 1) */
                if ((interfaceNumber < USBTMC_MAX_INTERFACES_NUMBER) && (USBTMC_wValueLoReg <= 1u))
                {
                    USBTMC_hidProtocol[interfaceNumber] = (uint8)USBTMC_wValueLoReg;
                    requestHandled = USBTMC_InitNoDataControlTransfer();
                }
                break;
            
            default:    
                /* Unknown class request is not handled. */
                break;
        }
    }

    return (requestHandled);
}


/*******************************************************************************
* Function Name: USB_FindHidClassDescriptor
****************************************************************************//**
*
*  This routine find Hid Class Descriptor pointer based on the Interface number
*  and Alternate setting then loads the currentTD structure with the address of
*  the buffer and the size.
*  The HID Class Descriptor resides inside the config descriptor.
*
* \return
*  currentTD
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_FindHidClassDecriptor(void) 
{
    const T_USBTMC_LUT CYCODE *pTmp;
    volatile uint8 *pDescr;
    uint8 interfaceN;

    pTmp = USBTMC_GetConfigTablePtr(USBTMC_configuration - 1u);
    
    interfaceN = (uint8) USBTMC_wIndexLoReg;
    /* Third entry in the LUT starts the Interface Table pointers */
    /* Now use the request interface number*/
    pTmp = &pTmp[interfaceN + 2u];
    
    /* USB_DEVICEx_CONFIGURATIONy_INTERFACEz_TABLE */
    pTmp = (const T_USBTMC_LUT CYCODE *) pTmp->p_list;
    
    /* Now use Alternate setting number */
    pTmp = &pTmp[USBTMC_interfaceSetting[interfaceN]];
    
    /* USB_DEVICEx_CONFIGURATIONy_INTERFACEz_ALTERNATEi_HID_TABLE */
    pTmp = (const T_USBTMC_LUT CYCODE *) pTmp->p_list;
    
    /* Fifth entry in the LUT points to Hid Class Descriptor in Configuration Descriptor */
    pTmp = &pTmp[4u];
    pDescr = (volatile uint8 *)pTmp->p_list;
    
    /* The first byte contains the descriptor length */
    USBTMC_currentTD.count = *pDescr;
    USBTMC_currentTD.pData = pDescr;
}


/*******************************************************************************
* Function Name: USB_FindReportDescriptor
****************************************************************************//**
*
*  This routine find Hid Report Descriptor pointer based on the Interface
*  number, then loads the currentTD structure with the address of the buffer
*  and the size.
*  Hid Report Descriptor is located after IN/OUT/FEATURE reports.
*
* \return
*  currentTD
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_FindReportDescriptor(void) 
{
    const T_USBTMC_LUT CYCODE *pTmp;
    volatile uint8 *pDescr;
    uint8 interfaceN;

    pTmp = USBTMC_GetConfigTablePtr(USBTMC_configuration - 1u);
    interfaceN = (uint8) USBTMC_wIndexLoReg;
    
    /* Third entry in the LUT starts the Interface Table pointers */
    /* Now use the request interface number */
    pTmp = &pTmp[interfaceN + 2u];
    
    /* USB_DEVICEx_CONFIGURATIONy_INTERFACEz_TABLE */
    pTmp = (const T_USBTMC_LUT CYCODE *) pTmp->p_list;
    
    /* Now use Alternate setting number */
    pTmp = &pTmp[USBTMC_interfaceSetting[interfaceN]];
    
    /* USB_DEVICEx_CONFIGURATIONy_INTERFACEz_ALTERNATEi_HID_TABLE */
    pTmp = (const T_USBTMC_LUT CYCODE *) pTmp->p_list;
    
    /* Fourth entry in the LUT starts the Hid Report Descriptor */
    pTmp = &pTmp[3u];
    pDescr = (volatile uint8 *)pTmp->p_list;
    
    /* The 1st and 2nd bytes of descriptor contain its length. LSB is 1st. */
    USBTMC_currentTD.count =  ((uint16)((uint16) pDescr[1u] << 8u) | pDescr[0u]);
    USBTMC_currentTD.pData = &pDescr[2u];
}


/*******************************************************************************
* Function Name: USBTMC_FindReport
****************************************************************************//**
*
*  This routine sets up a transfer based on the Interface number, Report Type
*  and Report ID, then loads the currentTD structure with the address of the
*  buffer and the size.  The caller has to decide if it is a control read or
*  control write.
*
* \return
*  currentTD
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_FindReport(void) 
{
    const T_USBTMC_LUT CYCODE *pTmp;
    T_USBTMC_TD *pTD;
    uint8 reportType;
    uint8 interfaceN;
 
    /* `#START HID_FINDREPORT` Place custom handling here */

    /* `#END` */
    
#ifdef USBTMC_FIND_REPORT_CALLBACK
    USBTMC_FindReport_Callback();
#endif /* (USBTMC_FIND_REPORT_CALLBACK) */
    
    USBTMC_currentTD.count = 0u;   /* Init not supported condition */
    pTmp = USBTMC_GetConfigTablePtr(USBTMC_configuration - 1u);
    reportType = (uint8) USBTMC_wValueHiReg;
    interfaceN = (uint8) USBTMC_wIndexLoReg;
    
    /* Third entry in the LUT Configuration Table starts the Interface Table pointers */
    /* Now use the request interface number */
    pTmp = &pTmp[interfaceN + 2u];
    
    /* USB_DEVICEx_CONFIGURATIONy_INTERFACEz_TABLE */
    pTmp = (const T_USBTMC_LUT CYCODE *) pTmp->p_list;
    if (interfaceN < USBTMC_MAX_INTERFACES_NUMBER)
    {
        /* Now use Alternate setting number */
        pTmp = &pTmp[USBTMC_interfaceSetting[interfaceN]];
        
        /* USB_DEVICEx_CONFIGURATIONy_INTERFACEz_ALTERNATEi_HID_TABLE */
        pTmp = (const T_USBTMC_LUT CYCODE *) pTmp->p_list;
        
        /* Validate reportType to comply with "7.2.1 Get_Report Request" */
        if ((reportType >= USBTMC_HID_GET_REPORT_INPUT) &&
            (reportType <= USBTMC_HID_GET_REPORT_FEATURE))
        {
            /* Get the entry proper TD (IN, OUT or Feature Report Table)*/
            pTmp = &pTmp[reportType - 1u];
            
            /* Get reportID */
            reportType = (uint8) USBTMC_wValueLoReg;
            
            /* Validate table support by the HID descriptor, compare table count with reportID */
            if (pTmp->c >= reportType)
            {
                pTD = (T_USBTMC_TD *) pTmp->p_list;
                pTD = &pTD[reportType];                          /* select entry depend on report ID*/
                USBTMC_currentTD.pData = pTD->pData;   /* Buffer pointer */
                USBTMC_currentTD.count = pTD->count;   /* Buffer Size */
                USBTMC_currentTD.pStatusBlock = pTD->pStatusBlock;
            }
        }
    }
}


/*******************************************************************************
* Additional user functions supporting HID Requests
********************************************************************************/

/* `#START HID_FUNCTIONS` Place any additional functions here */

/* `#END` */

#endif  /*  USBTMC_ENABLE_HID_CLASS */


/* [] END OF FILE */
