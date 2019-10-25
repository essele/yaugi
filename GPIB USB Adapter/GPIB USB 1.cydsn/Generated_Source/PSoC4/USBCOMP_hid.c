/***************************************************************************//**
* \file USBCOMP_hid.c
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

#include "USBCOMP_hid.h"
#include "USBCOMP_pvt.h"
#include "cyapicallbacks.h"


#if defined(USBCOMP_ENABLE_HID_CLASS)

/***************************************
*    HID Variables
***************************************/
/** This variable is initialized in the USBFS_InitComponent() API to the 
 * PROTOCOL_REPORT value. It is controlled by the host using the 
 * HID_SET_PROTOCOL request. The value is returned to the user code by the 
 * USBFS_GetProtocol() API.*/
volatile uint8 USBCOMP_hidProtocol[USBCOMP_MAX_INTERFACES_NUMBER];

/** This variable controls the HID report rate. It is controlled by the host 
 * using the HID_SET_IDLE request and used by the USBFS_UpdateHIDTimer() API to 
 * reload timer.*/
volatile uint8 USBCOMP_hidIdleRate[USBCOMP_MAX_INTERFACES_NUMBER];

/** This variable contains the timer counter, which is decremented and reloaded 
 * by the USBFS_UpdateHIDTimer() API.*/
volatile uint8 USBCOMP_hidIdleTimer[USBCOMP_MAX_INTERFACES_NUMBER]; /* HID device idle rate value */


/***************************************
* Custom Declarations
***************************************/

/* `#START HID_CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


/*******************************************************************************
* Function Name: USBCOMP_UpdateHIDTimer
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
uint8 USBCOMP_UpdateHIDTimer(uint8 interface) 
{
    uint8 stat = USBCOMP_IDLE_TIMER_INDEFINITE;

    if(USBCOMP_hidIdleRate[interface] != 0u)
    {
        if(USBCOMP_hidIdleTimer[interface] > 0u)
        {
            USBCOMP_hidIdleTimer[interface]--;
            stat = USBCOMP_IDLE_TIMER_RUNNING;
        }
        else
        {
            USBCOMP_hidIdleTimer[interface] = USBCOMP_hidIdleRate[interface];
            stat = USBCOMP_IDLE_TIMER_EXPIRED;
        }
    }

    return((uint8)stat);
}


/*******************************************************************************
* Function Name: USBCOMP_GetProtocol
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
uint8 USBCOMP_GetProtocol(uint8 interface) 
{
    return(USBCOMP_hidProtocol[interface]);
}


/*******************************************************************************
* Function Name: USBCOMP_DispatchHIDClassRqst
****************************************************************************//**
*
*  This routine dispatches class requests
*
* \return
*  Results of HID Class request handling: 
*  - USBCOMP_TRUE  - request was handled without errors
*  - USBCOMP_FALSE - error occurs during handling of request  
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBCOMP_DispatchHIDClassRqst(void) 
{
    uint8 requestHandled = USBCOMP_FALSE;

    uint8 interfaceNumber = (uint8) USBCOMP_wIndexLoReg;
    
    /* Check request direction: D2H or H2D. */
    if (0u != (USBCOMP_bmRequestTypeReg & USBCOMP_RQST_DIR_D2H))
    {
        /* Handle direction from device to host. */
        
        switch (USBCOMP_bRequestReg)
        {
            case USBCOMP_GET_DESCRIPTOR:
                if (USBCOMP_wValueHiReg == USBCOMP_DESCR_HID_CLASS)
                {
                    USBCOMP_FindHidClassDecriptor();
                    if (USBCOMP_currentTD.count != 0u)
                    {
                        requestHandled = USBCOMP_InitControlRead();
                    }
                }
                else if (USBCOMP_wValueHiReg == USBCOMP_DESCR_HID_REPORT)
                {
                    USBCOMP_FindReportDescriptor();
                    if (USBCOMP_currentTD.count != 0u)
                    {
                        requestHandled = USBCOMP_InitControlRead();
                    }
                }
                else
                {   
                    /* Do not handle this request. */
                }
                break;
                
            case USBCOMP_HID_GET_REPORT:
                USBCOMP_FindReport();
                if (USBCOMP_currentTD.count != 0u)
                {
                    requestHandled = USBCOMP_InitControlRead();
                }
                break;

            case USBCOMP_HID_GET_IDLE:
                /* This function does not support multiple reports per interface*/
                /* Validate interfaceNumber and Report ID (should be 0): Do not support Idle per Report ID */
                if ((interfaceNumber < USBCOMP_MAX_INTERFACES_NUMBER) && (USBCOMP_wValueLoReg == 0u)) 
                {
                    USBCOMP_currentTD.count = 1u;
                    USBCOMP_currentTD.pData = &USBCOMP_hidIdleRate[interfaceNumber];
                    requestHandled  = USBCOMP_InitControlRead();
                }
                break;
                
            case USBCOMP_HID_GET_PROTOCOL:
                /* Validate interfaceNumber */
                if( interfaceNumber < USBCOMP_MAX_INTERFACES_NUMBER)
                {
                    USBCOMP_currentTD.count = 1u;
                    USBCOMP_currentTD.pData = &USBCOMP_hidProtocol[interfaceNumber];
                    requestHandled  = USBCOMP_InitControlRead();
                }
                break;
                
            default:    /* requestHandled is initialized as FALSE by default */
                break;
        }
    }
    else
    {   
        /* Handle direction from host to device. */
        
        switch (USBCOMP_bRequestReg)
        {
            case USBCOMP_HID_SET_REPORT:
                USBCOMP_FindReport();
                if (USBCOMP_currentTD.count != 0u)
                {
                    requestHandled = USBCOMP_InitControlWrite();
                }
                break;
                
            case USBCOMP_HID_SET_IDLE:
                /* This function does not support multiple reports per interface */
                /* Validate interfaceNumber and Report ID (should be 0): Do not support Idle per Report ID */
                if ((interfaceNumber < USBCOMP_MAX_INTERFACES_NUMBER) && (USBCOMP_wValueLoReg == 0u))
                {
                    USBCOMP_hidIdleRate[interfaceNumber] = (uint8)USBCOMP_wValueHiReg;
                    /* With regards to HID spec: "7.2.4 Set_Idle Request"
                    *  Latency. If the current period has gone past the
                    *  newly proscribed time duration, then a report
                    *  will be generated immediately.
                    */
                    if(USBCOMP_hidIdleRate[interfaceNumber] <
                       USBCOMP_hidIdleTimer[interfaceNumber])
                    {
                        /* Set the timer to zero and let the UpdateHIDTimer() API return IDLE_TIMER_EXPIRED status*/
                        USBCOMP_hidIdleTimer[interfaceNumber] = 0u;
                    }
                    /* If the new request is received within 4 milliseconds
                    *  (1 count) of the end of the current period, then the
                    *  new request will have no effect until after the report.
                    */
                    else if(USBCOMP_hidIdleTimer[interfaceNumber] <= 1u)
                    {
                        /* Do nothing.
                        *  Let the UpdateHIDTimer() API continue to work and
                        *  return IDLE_TIMER_EXPIRED status
                        */
                    }
                    else
                    {   /* Reload the timer*/
                        USBCOMP_hidIdleTimer[interfaceNumber] =
                        USBCOMP_hidIdleRate[interfaceNumber];
                    }
                    requestHandled = USBCOMP_InitNoDataControlTransfer();
                }
                break;

            case USBCOMP_HID_SET_PROTOCOL:
                /* Validate interfaceNumber and protocol (must be 0 or 1) */
                if ((interfaceNumber < USBCOMP_MAX_INTERFACES_NUMBER) && (USBCOMP_wValueLoReg <= 1u))
                {
                    USBCOMP_hidProtocol[interfaceNumber] = (uint8)USBCOMP_wValueLoReg;
                    requestHandled = USBCOMP_InitNoDataControlTransfer();
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
void USBCOMP_FindHidClassDecriptor(void) 
{
    const T_USBCOMP_LUT CYCODE *pTmp;
    volatile uint8 *pDescr;
    uint8 interfaceN;

    pTmp = USBCOMP_GetConfigTablePtr(USBCOMP_configuration - 1u);
    
    interfaceN = (uint8) USBCOMP_wIndexLoReg;
    /* Third entry in the LUT starts the Interface Table pointers */
    /* Now use the request interface number*/
    pTmp = &pTmp[interfaceN + 2u];
    
    /* USB_DEVICEx_CONFIGURATIONy_INTERFACEz_TABLE */
    pTmp = (const T_USBCOMP_LUT CYCODE *) pTmp->p_list;
    
    /* Now use Alternate setting number */
    pTmp = &pTmp[USBCOMP_interfaceSetting[interfaceN]];
    
    /* USB_DEVICEx_CONFIGURATIONy_INTERFACEz_ALTERNATEi_HID_TABLE */
    pTmp = (const T_USBCOMP_LUT CYCODE *) pTmp->p_list;
    
    /* Fifth entry in the LUT points to Hid Class Descriptor in Configuration Descriptor */
    pTmp = &pTmp[4u];
    pDescr = (volatile uint8 *)pTmp->p_list;
    
    /* The first byte contains the descriptor length */
    USBCOMP_currentTD.count = *pDescr;
    USBCOMP_currentTD.pData = pDescr;
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
void USBCOMP_FindReportDescriptor(void) 
{
    const T_USBCOMP_LUT CYCODE *pTmp;
    volatile uint8 *pDescr;
    uint8 interfaceN;

    pTmp = USBCOMP_GetConfigTablePtr(USBCOMP_configuration - 1u);
    interfaceN = (uint8) USBCOMP_wIndexLoReg;
    
    /* Third entry in the LUT starts the Interface Table pointers */
    /* Now use the request interface number */
    pTmp = &pTmp[interfaceN + 2u];
    
    /* USB_DEVICEx_CONFIGURATIONy_INTERFACEz_TABLE */
    pTmp = (const T_USBCOMP_LUT CYCODE *) pTmp->p_list;
    
    /* Now use Alternate setting number */
    pTmp = &pTmp[USBCOMP_interfaceSetting[interfaceN]];
    
    /* USB_DEVICEx_CONFIGURATIONy_INTERFACEz_ALTERNATEi_HID_TABLE */
    pTmp = (const T_USBCOMP_LUT CYCODE *) pTmp->p_list;
    
    /* Fourth entry in the LUT starts the Hid Report Descriptor */
    pTmp = &pTmp[3u];
    pDescr = (volatile uint8 *)pTmp->p_list;
    
    /* The 1st and 2nd bytes of descriptor contain its length. LSB is 1st. */
    USBCOMP_currentTD.count =  ((uint16)((uint16) pDescr[1u] << 8u) | pDescr[0u]);
    USBCOMP_currentTD.pData = &pDescr[2u];
}


/*******************************************************************************
* Function Name: USBCOMP_FindReport
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
void USBCOMP_FindReport(void) 
{
    const T_USBCOMP_LUT CYCODE *pTmp;
    T_USBCOMP_TD *pTD;
    uint8 reportType;
    uint8 interfaceN;
 
    /* `#START HID_FINDREPORT` Place custom handling here */

    /* `#END` */
    
#ifdef USBCOMP_FIND_REPORT_CALLBACK
    USBCOMP_FindReport_Callback();
#endif /* (USBCOMP_FIND_REPORT_CALLBACK) */
    
    USBCOMP_currentTD.count = 0u;   /* Init not supported condition */
    pTmp = USBCOMP_GetConfigTablePtr(USBCOMP_configuration - 1u);
    reportType = (uint8) USBCOMP_wValueHiReg;
    interfaceN = (uint8) USBCOMP_wIndexLoReg;
    
    /* Third entry in the LUT Configuration Table starts the Interface Table pointers */
    /* Now use the request interface number */
    pTmp = &pTmp[interfaceN + 2u];
    
    /* USB_DEVICEx_CONFIGURATIONy_INTERFACEz_TABLE */
    pTmp = (const T_USBCOMP_LUT CYCODE *) pTmp->p_list;
    if (interfaceN < USBCOMP_MAX_INTERFACES_NUMBER)
    {
        /* Now use Alternate setting number */
        pTmp = &pTmp[USBCOMP_interfaceSetting[interfaceN]];
        
        /* USB_DEVICEx_CONFIGURATIONy_INTERFACEz_ALTERNATEi_HID_TABLE */
        pTmp = (const T_USBCOMP_LUT CYCODE *) pTmp->p_list;
        
        /* Validate reportType to comply with "7.2.1 Get_Report Request" */
        if ((reportType >= USBCOMP_HID_GET_REPORT_INPUT) &&
            (reportType <= USBCOMP_HID_GET_REPORT_FEATURE))
        {
            /* Get the entry proper TD (IN, OUT or Feature Report Table)*/
            pTmp = &pTmp[reportType - 1u];
            
            /* Get reportID */
            reportType = (uint8) USBCOMP_wValueLoReg;
            
            /* Validate table support by the HID descriptor, compare table count with reportID */
            if (pTmp->c >= reportType)
            {
                pTD = (T_USBCOMP_TD *) pTmp->p_list;
                pTD = &pTD[reportType];                          /* select entry depend on report ID*/
                USBCOMP_currentTD.pData = pTD->pData;   /* Buffer pointer */
                USBCOMP_currentTD.count = pTD->count;   /* Buffer Size */
                USBCOMP_currentTD.pStatusBlock = pTD->pStatusBlock;
            }
        }
    }
}


/*******************************************************************************
* Additional user functions supporting HID Requests
********************************************************************************/

/* `#START HID_FUNCTIONS` Place any additional functions here */

/* `#END` */

#endif  /*  USBCOMP_ENABLE_HID_CLASS */


/* [] END OF FILE */
