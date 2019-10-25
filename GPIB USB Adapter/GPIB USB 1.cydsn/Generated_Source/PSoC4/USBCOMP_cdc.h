/***************************************************************************//**
* \file USBCOMP_cdc.h
* \version 3.20
*
* \brief
*  This file provides function prototypes and constants for the USBFS component 
*  CDC class.
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

#if !defined(CY_USBFS_USBCOMP_cdc_H)
#define CY_USBFS_USBCOMP_cdc_H

#include "USBCOMP.h"


/*******************************************************************************
* Prototypes of the USBCOMP_cdc API.
*******************************************************************************/
/**
* \addtogroup group_cdc
* @{
*/
#if (USBCOMP_ENABLE_CDC_CLASS_API != 0u)
    uint8 USBCOMP_CDC_Init(void)            ;
    void USBCOMP_PutData(const uint8* pData, uint16 length) ;
    void USBCOMP_PutString(const char8 string[])            ;
    void USBCOMP_PutChar(char8 txDataByte) ;
    void USBCOMP_PutCRLF(void)             ;
    uint16 USBCOMP_GetCount(void)          ;
    uint8  USBCOMP_CDCIsReady(void)        ;
    uint8  USBCOMP_DataIsReady(void)       ;
    uint16 USBCOMP_GetData(uint8* pData, uint16 length)     ;
    uint16 USBCOMP_GetAll(uint8* pData)    ;
    uint8  USBCOMP_GetChar(void)           ;
    uint8  USBCOMP_IsLineChanged(void)     ;
    uint32 USBCOMP_GetDTERate(void)        ;
    uint8  USBCOMP_GetCharFormat(void)     ;
    uint8  USBCOMP_GetParityType(void)     ;
    uint8  USBCOMP_GetDataBits(void)       ;
    uint16 USBCOMP_GetLineControl(void)    ;
    void USBCOMP_SendSerialState (uint16 serialState) ;
    uint16 USBCOMP_GetSerialState (void)   ;
    void USBCOMP_SetComPort (uint8 comNumber) ;
    uint8 USBCOMP_GetComPort (void)        ;
    uint8 USBCOMP_NotificationIsReady(void) ;

#endif  /* (USBCOMP_ENABLE_CDC_CLASS_API) */
/** @} cdc */

/*******************************************************************************
*  Constants for USBCOMP_cdc API.
*******************************************************************************/

/* CDC Class-Specific Request Codes (CDC ver 1.2 Table 19) */
#define USBCOMP_CDC_SET_LINE_CODING        (0x20u)
#define USBCOMP_CDC_GET_LINE_CODING        (0x21u)
#define USBCOMP_CDC_SET_CONTROL_LINE_STATE (0x22u)

/*PSTN Subclass Specific Notifications (CDC ver 1.2 Table 30)*/
#define USBCOMP_SERIAL_STATE               (0x20u)

#define USBCOMP_LINE_CODING_CHANGED        (0x01u)
#define USBCOMP_LINE_CONTROL_CHANGED       (0x02u)

#define USBCOMP_1_STOPBIT                  (0x00u)
#define USBCOMP_1_5_STOPBITS               (0x01u)
#define USBCOMP_2_STOPBITS                 (0x02u)

#define USBCOMP_PARITY_NONE                (0x00u)
#define USBCOMP_PARITY_ODD                 (0x01u)
#define USBCOMP_PARITY_EVEN                (0x02u)
#define USBCOMP_PARITY_MARK                (0x03u)
#define USBCOMP_PARITY_SPACE               (0x04u)

#define USBCOMP_LINE_CODING_SIZE           (0x07u)
#define USBCOMP_LINE_CODING_RATE           (0x00u)
#define USBCOMP_LINE_CODING_STOP_BITS      (0x04u)
#define USBCOMP_LINE_CODING_PARITY         (0x05u)
#define USBCOMP_LINE_CODING_DATA_BITS      (0x06u)

#define USBCOMP_LINE_CONTROL_DTR           (0x01u)
#define USBCOMP_LINE_CONTROL_RTS           (0x02u)

#define USBCOMP_MAX_MULTI_COM_NUM          (2u) 

#define USBCOMP_COM_PORT1                  (0u) 
#define USBCOMP_COM_PORT2                  (1u) 

#define USBCOMP_SUCCESS                    (0u)
#define USBCOMP_FAILURE                    (1u)

#define USBCOMP_SERIAL_STATE_SIZE          (10u)

/* SerialState constants*/
#define USBCOMP_SERIAL_STATE_REQUEST_TYPE  (0xA1u)
#define USBCOMP_SERIAL_STATE_LENGTH        (0x2u)

/*******************************************************************************
* External data references
*******************************************************************************/
/**
* \addtogroup group_cdc
* @{
*/
extern volatile uint8  USBCOMP_linesCoding[USBCOMP_MAX_MULTI_COM_NUM][USBCOMP_LINE_CODING_SIZE];
extern volatile uint8  USBCOMP_linesChanged[USBCOMP_MAX_MULTI_COM_NUM];
extern volatile uint16 USBCOMP_linesControlBitmap[USBCOMP_MAX_MULTI_COM_NUM];
extern volatile uint16 USBCOMP_serialStateBitmap[USBCOMP_MAX_MULTI_COM_NUM];
extern volatile uint8  USBCOMP_cdcDataInEp[USBCOMP_MAX_MULTI_COM_NUM];
extern volatile uint8  USBCOMP_cdcDataOutEp[USBCOMP_MAX_MULTI_COM_NUM];
extern volatile uint8  USBCOMP_cdcCommInInterruptEp[USBCOMP_MAX_MULTI_COM_NUM];
/** @} cdc */

/*******************************************************************************
* The following code is DEPRECATED and
* must not be used.
*******************************************************************************/


#define USBCOMP_lineCoding             USBCOMP_linesCoding[0]
#define USBCOMP_lineChanged            USBCOMP_linesChanged[0]
#define USBCOMP_lineControlBitmap      USBCOMP_linesControlBitmap[0]
#define USBCOMP_cdc_data_in_ep         USBCOMP_cdcDataInEp[0]
#define USBCOMP_cdc_data_out_ep        USBCOMP_cdcDataOutEp[0]

#endif /* (CY_USBFS_USBCOMP_cdc_H) */


/* [] END OF FILE */
