/***************************************************************************//**
* \file USBTMC_cdc.h
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

#if !defined(CY_USBFS_USBTMC_cdc_H)
#define CY_USBFS_USBTMC_cdc_H

#include "USBTMC.h"


/*******************************************************************************
* Prototypes of the USBTMC_cdc API.
*******************************************************************************/
/**
* \addtogroup group_cdc
* @{
*/
#if (USBTMC_ENABLE_CDC_CLASS_API != 0u)
    uint8 USBTMC_CDC_Init(void)            ;
    void USBTMC_PutData(const uint8* pData, uint16 length) ;
    void USBTMC_PutString(const char8 string[])            ;
    void USBTMC_PutChar(char8 txDataByte) ;
    void USBTMC_PutCRLF(void)             ;
    uint16 USBTMC_GetCount(void)          ;
    uint8  USBTMC_CDCIsReady(void)        ;
    uint8  USBTMC_DataIsReady(void)       ;
    uint16 USBTMC_GetData(uint8* pData, uint16 length)     ;
    uint16 USBTMC_GetAll(uint8* pData)    ;
    uint8  USBTMC_GetChar(void)           ;
    uint8  USBTMC_IsLineChanged(void)     ;
    uint32 USBTMC_GetDTERate(void)        ;
    uint8  USBTMC_GetCharFormat(void)     ;
    uint8  USBTMC_GetParityType(void)     ;
    uint8  USBTMC_GetDataBits(void)       ;
    uint16 USBTMC_GetLineControl(void)    ;
    void USBTMC_SendSerialState (uint16 serialState) ;
    uint16 USBTMC_GetSerialState (void)   ;
    void USBTMC_SetComPort (uint8 comNumber) ;
    uint8 USBTMC_GetComPort (void)        ;
    uint8 USBTMC_NotificationIsReady(void) ;

#endif  /* (USBTMC_ENABLE_CDC_CLASS_API) */
/** @} cdc */

/*******************************************************************************
*  Constants for USBTMC_cdc API.
*******************************************************************************/

/* CDC Class-Specific Request Codes (CDC ver 1.2 Table 19) */
#define USBTMC_CDC_SET_LINE_CODING        (0x20u)
#define USBTMC_CDC_GET_LINE_CODING        (0x21u)
#define USBTMC_CDC_SET_CONTROL_LINE_STATE (0x22u)

/*PSTN Subclass Specific Notifications (CDC ver 1.2 Table 30)*/
#define USBTMC_SERIAL_STATE               (0x20u)

#define USBTMC_LINE_CODING_CHANGED        (0x01u)
#define USBTMC_LINE_CONTROL_CHANGED       (0x02u)

#define USBTMC_1_STOPBIT                  (0x00u)
#define USBTMC_1_5_STOPBITS               (0x01u)
#define USBTMC_2_STOPBITS                 (0x02u)

#define USBTMC_PARITY_NONE                (0x00u)
#define USBTMC_PARITY_ODD                 (0x01u)
#define USBTMC_PARITY_EVEN                (0x02u)
#define USBTMC_PARITY_MARK                (0x03u)
#define USBTMC_PARITY_SPACE               (0x04u)

#define USBTMC_LINE_CODING_SIZE           (0x07u)
#define USBTMC_LINE_CODING_RATE           (0x00u)
#define USBTMC_LINE_CODING_STOP_BITS      (0x04u)
#define USBTMC_LINE_CODING_PARITY         (0x05u)
#define USBTMC_LINE_CODING_DATA_BITS      (0x06u)

#define USBTMC_LINE_CONTROL_DTR           (0x01u)
#define USBTMC_LINE_CONTROL_RTS           (0x02u)

#define USBTMC_MAX_MULTI_COM_NUM          (2u) 

#define USBTMC_COM_PORT1                  (0u) 
#define USBTMC_COM_PORT2                  (1u) 

#define USBTMC_SUCCESS                    (0u)
#define USBTMC_FAILURE                    (1u)

#define USBTMC_SERIAL_STATE_SIZE          (10u)

/* SerialState constants*/
#define USBTMC_SERIAL_STATE_REQUEST_TYPE  (0xA1u)
#define USBTMC_SERIAL_STATE_LENGTH        (0x2u)

/*******************************************************************************
* External data references
*******************************************************************************/
/**
* \addtogroup group_cdc
* @{
*/
extern volatile uint8  USBTMC_linesCoding[USBTMC_MAX_MULTI_COM_NUM][USBTMC_LINE_CODING_SIZE];
extern volatile uint8  USBTMC_linesChanged[USBTMC_MAX_MULTI_COM_NUM];
extern volatile uint16 USBTMC_linesControlBitmap[USBTMC_MAX_MULTI_COM_NUM];
extern volatile uint16 USBTMC_serialStateBitmap[USBTMC_MAX_MULTI_COM_NUM];
extern volatile uint8  USBTMC_cdcDataInEp[USBTMC_MAX_MULTI_COM_NUM];
extern volatile uint8  USBTMC_cdcDataOutEp[USBTMC_MAX_MULTI_COM_NUM];
extern volatile uint8  USBTMC_cdcCommInInterruptEp[USBTMC_MAX_MULTI_COM_NUM];
/** @} cdc */

/*******************************************************************************
* The following code is DEPRECATED and
* must not be used.
*******************************************************************************/


#define USBTMC_lineCoding             USBTMC_linesCoding[0]
#define USBTMC_lineChanged            USBTMC_linesChanged[0]
#define USBTMC_lineControlBitmap      USBTMC_linesControlBitmap[0]
#define USBTMC_cdc_data_in_ep         USBTMC_cdcDataInEp[0]
#define USBTMC_cdc_data_out_ep        USBTMC_cdcDataOutEp[0]

#endif /* (CY_USBFS_USBTMC_cdc_H) */


/* [] END OF FILE */
