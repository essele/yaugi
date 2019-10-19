/***************************************************************************//**
* \file .h
* \version 3.20
*
* \brief
*  This file provides private function prototypes and constants for the 
*  USBFS component. It is not intended to be used in the user project.
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_USBFS_USBUART2_pvt_H)
#define CY_USBFS_USBUART2_pvt_H

#include "USBUART2.h"
   
#ifdef USBUART2_ENABLE_AUDIO_CLASS
    #include "USBUART2_audio.h"
#endif /* USBUART2_ENABLE_AUDIO_CLASS */

#ifdef USBUART2_ENABLE_CDC_CLASS
    #include "USBUART2_cdc.h"
#endif /* USBUART2_ENABLE_CDC_CLASS */

#if (USBUART2_ENABLE_MIDI_CLASS)
    #include "USBUART2_midi.h"
#endif /* (USBUART2_ENABLE_MIDI_CLASS) */

#if (USBUART2_ENABLE_MSC_CLASS)
    #include "USBUART2_msc.h"
#endif /* (USBUART2_ENABLE_MSC_CLASS) */

#if (USBUART2_EP_MANAGEMENT_DMA)
    #if (CY_PSOC4)
        #include <CyDMA.h>
    #else
        #include <CyDmac.h>
        #if ((USBUART2_EP_MANAGEMENT_DMA_AUTO) && (USBUART2_EP_DMA_AUTO_OPT == 0u))
            #include "USBUART2_EP_DMA_Done_isr.h"
            #include "USBUART2_EP8_DMA_Done_SR.h"
            #include "USBUART2_EP17_DMA_Done_SR.h"
        #endif /* ((USBUART2_EP_MANAGEMENT_DMA_AUTO) && (USBUART2_EP_DMA_AUTO_OPT == 0u)) */
    #endif /* (CY_PSOC4) */
#endif /* (USBUART2_EP_MANAGEMENT_DMA) */

#if (USBUART2_DMA1_ACTIVE)
    #include "USBUART2_ep1_dma.h"
    #define USBUART2_EP1_DMA_CH     (USBUART2_ep1_dma_CHANNEL)
#endif /* (USBUART2_DMA1_ACTIVE) */

#if (USBUART2_DMA2_ACTIVE)
    #include "USBUART2_ep2_dma.h"
    #define USBUART2_EP2_DMA_CH     (USBUART2_ep2_dma_CHANNEL)
#endif /* (USBUART2_DMA2_ACTIVE) */

#if (USBUART2_DMA3_ACTIVE)
    #include "USBUART2_ep3_dma.h"
    #define USBUART2_EP3_DMA_CH     (USBUART2_ep3_dma_CHANNEL)
#endif /* (USBUART2_DMA3_ACTIVE) */

#if (USBUART2_DMA4_ACTIVE)
    #include "USBUART2_ep4_dma.h"
    #define USBUART2_EP4_DMA_CH     (USBUART2_ep4_dma_CHANNEL)
#endif /* (USBUART2_DMA4_ACTIVE) */

#if (USBUART2_DMA5_ACTIVE)
    #include "USBUART2_ep5_dma.h"
    #define USBUART2_EP5_DMA_CH     (USBUART2_ep5_dma_CHANNEL)
#endif /* (USBUART2_DMA5_ACTIVE) */

#if (USBUART2_DMA6_ACTIVE)
    #include "USBUART2_ep6_dma.h"
    #define USBUART2_EP6_DMA_CH     (USBUART2_ep6_dma_CHANNEL)
#endif /* (USBUART2_DMA6_ACTIVE) */

#if (USBUART2_DMA7_ACTIVE)
    #include "USBUART2_ep7_dma.h"
    #define USBUART2_EP7_DMA_CH     (USBUART2_ep7_dma_CHANNEL)
#endif /* (USBUART2_DMA7_ACTIVE) */

#if (USBUART2_DMA8_ACTIVE)
    #include "USBUART2_ep8_dma.h"
    #define USBUART2_EP8_DMA_CH     (USBUART2_ep8_dma_CHANNEL)
#endif /* (USBUART2_DMA8_ACTIVE) */


/***************************************
*     Private Variables
***************************************/

/* Generated external references for descriptors. */
extern const uint8 CYCODE USBUART2_DEVICE0_DESCR[18u];
extern const uint8 CYCODE USBUART2_DEVICE0_CONFIGURATION0_DESCR[75u];
extern const T_USBUART2_EP_SETTINGS_BLOCK CYCODE USBUART2_DEVICE0_CONFIGURATION0_EP_SETTINGS_TABLE[3u];
extern const uint8 CYCODE USBUART2_DEVICE0_CONFIGURATION0_INTERFACE_CLASS[2u];
extern const T_USBUART2_LUT CYCODE USBUART2_DEVICE0_CONFIGURATION0_TABLE[5u];
extern const T_USBUART2_LUT CYCODE USBUART2_DEVICE0_TABLE[3u];
extern const T_USBUART2_LUT CYCODE USBUART2_TABLE[1u];
extern const uint8 CYCODE USBUART2_SN_STRING_DESCRIPTOR[2];
extern const uint8 CYCODE USBUART2_STRING_DESCRIPTORS[175u];


extern const uint8 CYCODE USBUART2_MSOS_DESCRIPTOR[USBUART2_MSOS_DESCRIPTOR_LENGTH];
extern const uint8 CYCODE USBUART2_MSOS_CONFIGURATION_DESCR[USBUART2_MSOS_CONF_DESCR_LENGTH];
#if defined(USBUART2_ENABLE_IDSN_STRING)
    extern uint8 USBUART2_idSerialNumberStringDescriptor[USBUART2_IDSN_DESCR_LENGTH];
#endif /* (USBUART2_ENABLE_IDSN_STRING) */

extern volatile uint8 USBUART2_interfaceNumber;
extern volatile uint8 USBUART2_interfaceSetting[USBUART2_MAX_INTERFACES_NUMBER];
extern volatile uint8 USBUART2_interfaceSettingLast[USBUART2_MAX_INTERFACES_NUMBER];
extern volatile uint8 USBUART2_deviceAddress;
extern volatile uint8 USBUART2_interfaceStatus[USBUART2_MAX_INTERFACES_NUMBER];
extern const uint8 CYCODE *USBUART2_interfaceClass;

extern volatile T_USBUART2_EP_CTL_BLOCK USBUART2_EP[USBUART2_MAX_EP];
extern volatile T_USBUART2_TD USBUART2_currentTD;

#if (USBUART2_EP_MANAGEMENT_DMA)
    #if (CY_PSOC4)
        extern const uint8 USBUART2_DmaChan[USBUART2_MAX_EP];
    #else
        extern uint8 USBUART2_DmaChan[USBUART2_MAX_EP];
        extern uint8 USBUART2_DmaTd  [USBUART2_MAX_EP];
    #endif /* (CY_PSOC4) */
#endif /* (USBUART2_EP_MANAGEMENT_DMA) */

#if (USBUART2_EP_MANAGEMENT_DMA_AUTO)
#if (CY_PSOC4)
    extern uint8  USBUART2_DmaEpBurstCnt   [USBUART2_MAX_EP];
    extern uint8  USBUART2_DmaEpLastBurstEl[USBUART2_MAX_EP];

    extern uint8  USBUART2_DmaEpBurstCntBackup  [USBUART2_MAX_EP];
    extern uint32 USBUART2_DmaEpBufferAddrBackup[USBUART2_MAX_EP];
    
    extern const uint8 USBUART2_DmaReqOut     [USBUART2_MAX_EP];    
    extern const uint8 USBUART2_DmaBurstEndOut[USBUART2_MAX_EP];
#else
    #if (USBUART2_EP_DMA_AUTO_OPT == 0u)
        extern uint8 USBUART2_DmaNextTd[USBUART2_MAX_EP];
        extern volatile uint16 USBUART2_inLength [USBUART2_MAX_EP];
        extern volatile uint8  USBUART2_inBufFull[USBUART2_MAX_EP];
        extern const uint8 USBUART2_epX_TD_TERMOUT_EN[USBUART2_MAX_EP];
        extern const uint8 *USBUART2_inDataPointer[USBUART2_MAX_EP];
    #endif /* (USBUART2_EP_DMA_AUTO_OPT == 0u) */
#endif /* CY_PSOC4 */
#endif /* (USBUART2_EP_MANAGEMENT_DMA_AUTO) */

extern volatile uint8 USBUART2_ep0Toggle;
extern volatile uint8 USBUART2_lastPacketSize;
extern volatile uint8 USBUART2_ep0Mode;
extern volatile uint8 USBUART2_ep0Count;
extern volatile uint16 USBUART2_transferByteCount;


/***************************************
*     Private Function Prototypes
***************************************/
void  USBUART2_ReInitComponent(void)            ;
void  USBUART2_HandleSetup(void)                ;
void  USBUART2_HandleIN(void)                   ;
void  USBUART2_HandleOUT(void)                  ;
void  USBUART2_LoadEP0(void)                    ;
uint8 USBUART2_InitControlRead(void)            ;
uint8 USBUART2_InitControlWrite(void)           ;
void  USBUART2_ControlReadDataStage(void)       ;
void  USBUART2_ControlReadStatusStage(void)     ;
void  USBUART2_ControlReadPrematureStatus(void) ;
uint8 USBUART2_InitControlWrite(void)           ;
uint8 USBUART2_InitZeroLengthControlTransfer(void) ;
void  USBUART2_ControlWriteDataStage(void)      ;
void  USBUART2_ControlWriteStatusStage(void)    ;
void  USBUART2_ControlWritePrematureStatus(void);
uint8 USBUART2_InitNoDataControlTransfer(void)  ;
void  USBUART2_NoDataControlStatusStage(void)   ;
void  USBUART2_InitializeStatusBlock(void)      ;
void  USBUART2_UpdateStatusBlock(uint8 completionCode) ;
uint8 USBUART2_DispatchClassRqst(void)          ;

void USBUART2_Config(uint8 clearAltSetting) ;
void USBUART2_ConfigAltChanged(void)        ;
void USBUART2_ConfigReg(void)               ;
void USBUART2_EpStateInit(void)             ;


const T_USBUART2_LUT CYCODE *USBUART2_GetConfigTablePtr(uint8 confIndex);
const T_USBUART2_LUT CYCODE *USBUART2_GetDeviceTablePtr(void)           ;
#if (USBUART2_BOS_ENABLE)
    const T_USBUART2_LUT CYCODE *USBUART2_GetBOSPtr(void)               ;
#endif /* (USBUART2_BOS_ENABLE) */
const uint8 CYCODE *USBUART2_GetInterfaceClassTablePtr(void)                    ;
uint8 USBUART2_ClearEndpointHalt(void)                                          ;
uint8 USBUART2_SetEndpointHalt(void)                                            ;
uint8 USBUART2_ValidateAlternateSetting(void)                                   ;

void USBUART2_SaveConfig(void)      ;
void USBUART2_RestoreConfig(void)   ;

#if (CY_PSOC3 || CY_PSOC5LP)
    #if (USBUART2_EP_MANAGEMENT_DMA_AUTO && (USBUART2_EP_DMA_AUTO_OPT == 0u))
        void USBUART2_LoadNextInEP(uint8 epNumber, uint8 mode)  ;
    #endif /* (USBUART2_EP_MANAGEMENT_DMA_AUTO && (USBUART2_EP_DMA_AUTO_OPT == 0u)) */
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

#if defined(USBUART2_ENABLE_IDSN_STRING)
    void USBUART2_ReadDieID(uint8 descr[])  ;
#endif /* USBUART2_ENABLE_IDSN_STRING */

#if defined(USBUART2_ENABLE_HID_CLASS)
    uint8 USBUART2_DispatchHIDClassRqst(void) ;
#endif /* (USBUART2_ENABLE_HID_CLASS) */

#if defined(USBUART2_ENABLE_AUDIO_CLASS)
    uint8 USBUART2_DispatchAUDIOClassRqst(void) ;
#endif /* (USBUART2_ENABLE_AUDIO_CLASS) */

#if defined(USBUART2_ENABLE_CDC_CLASS)
    uint8 USBUART2_DispatchCDCClassRqst(void) ;
#endif /* (USBUART2_ENABLE_CDC_CLASS) */

#if (USBUART2_ENABLE_MSC_CLASS)
    #if (USBUART2_HANDLE_MSC_REQUESTS)
        uint8 USBUART2_DispatchMSCClassRqst(void) ;
    #endif /* (USBUART2_HANDLE_MSC_REQUESTS) */
#endif /* (USBUART2_ENABLE_MSC_CLASS */

CY_ISR_PROTO(USBUART2_EP_0_ISR);
CY_ISR_PROTO(USBUART2_BUS_RESET_ISR);

#if (USBUART2_SOF_ISR_ACTIVE)
    CY_ISR_PROTO(USBUART2_SOF_ISR);
#endif /* (USBUART2_SOF_ISR_ACTIVE) */

#if (USBUART2_EP1_ISR_ACTIVE)
    CY_ISR_PROTO(USBUART2_EP_1_ISR);
#endif /* (USBUART2_EP1_ISR_ACTIVE) */

#if (USBUART2_EP2_ISR_ACTIVE)
    CY_ISR_PROTO(USBUART2_EP_2_ISR);
#endif /* (USBUART2_EP2_ISR_ACTIVE) */

#if (USBUART2_EP3_ISR_ACTIVE)
    CY_ISR_PROTO(USBUART2_EP_3_ISR);
#endif /* (USBUART2_EP3_ISR_ACTIVE) */

#if (USBUART2_EP4_ISR_ACTIVE)
    CY_ISR_PROTO(USBUART2_EP_4_ISR);
#endif /* (USBUART2_EP4_ISR_ACTIVE) */

#if (USBUART2_EP5_ISR_ACTIVE)
    CY_ISR_PROTO(USBUART2_EP_5_ISR);
#endif /* (USBUART2_EP5_ISR_ACTIVE) */

#if (USBUART2_EP6_ISR_ACTIVE)
    CY_ISR_PROTO(USBUART2_EP_6_ISR);
#endif /* (USBUART2_EP6_ISR_ACTIVE) */

#if (USBUART2_EP7_ISR_ACTIVE)
    CY_ISR_PROTO(USBUART2_EP_7_ISR);
#endif /* (USBUART2_EP7_ISR_ACTIVE) */

#if (USBUART2_EP8_ISR_ACTIVE)
    CY_ISR_PROTO(USBUART2_EP_8_ISR);
#endif /* (USBUART2_EP8_ISR_ACTIVE) */

#if (USBUART2_EP_MANAGEMENT_DMA)
    CY_ISR_PROTO(USBUART2_ARB_ISR);
#endif /* (USBUART2_EP_MANAGEMENT_DMA) */

#if (USBUART2_DP_ISR_ACTIVE)
    CY_ISR_PROTO(USBUART2_DP_ISR);
#endif /* (USBUART2_DP_ISR_ACTIVE) */

#if (CY_PSOC4)
    CY_ISR_PROTO(USBUART2_INTR_HI_ISR);
    CY_ISR_PROTO(USBUART2_INTR_MED_ISR);
    CY_ISR_PROTO(USBUART2_INTR_LO_ISR);
    #if (USBUART2_LPM_ACTIVE)
        CY_ISR_PROTO(USBUART2_LPM_ISR);
    #endif /* (USBUART2_LPM_ACTIVE) */
#endif /* (CY_PSOC4) */

#if (USBUART2_EP_MANAGEMENT_DMA_AUTO)
#if (CY_PSOC4)
    #if (USBUART2_DMA1_ACTIVE)
        void USBUART2_EP1_DMA_DONE_ISR(void);
    #endif /* (USBUART2_DMA1_ACTIVE) */

    #if (USBUART2_DMA2_ACTIVE)
        void USBUART2_EP2_DMA_DONE_ISR(void);
    #endif /* (USBUART2_DMA2_ACTIVE) */

    #if (USBUART2_DMA3_ACTIVE)
        void USBUART2_EP3_DMA_DONE_ISR(void);
    #endif /* (USBUART2_DMA3_ACTIVE) */

    #if (USBUART2_DMA4_ACTIVE)
        void USBUART2_EP4_DMA_DONE_ISR(void);
    #endif /* (USBUART2_DMA4_ACTIVE) */

    #if (USBUART2_DMA5_ACTIVE)
        void USBUART2_EP5_DMA_DONE_ISR(void);
    #endif /* (USBUART2_DMA5_ACTIVE) */

    #if (USBUART2_DMA6_ACTIVE)
        void USBUART2_EP6_DMA_DONE_ISR(void);
    #endif /* (USBUART2_DMA6_ACTIVE) */

    #if (USBUART2_DMA7_ACTIVE)
        void USBUART2_EP7_DMA_DONE_ISR(void);
    #endif /* (USBUART2_DMA7_ACTIVE) */

    #if (USBUART2_DMA8_ACTIVE)
        void USBUART2_EP8_DMA_DONE_ISR(void);
    #endif /* (USBUART2_DMA8_ACTIVE) */

#else
    #if (USBUART2_EP_DMA_AUTO_OPT == 0u)
        CY_ISR_PROTO(USBUART2_EP_DMA_DONE_ISR);
    #endif /* (USBUART2_EP_DMA_AUTO_OPT == 0u) */
#endif /* (CY_PSOC4) */
#endif /* (USBUART2_EP_MANAGEMENT_DMA_AUTO) */


/***************************************
*         Request Handlers
***************************************/

uint8 USBUART2_HandleStandardRqst(void) ;
uint8 USBUART2_DispatchClassRqst(void)  ;
uint8 USBUART2_HandleVendorRqst(void)   ;


/***************************************
*    HID Internal references
***************************************/

#if defined(USBUART2_ENABLE_HID_CLASS)
    void USBUART2_FindReport(void)            ;
    void USBUART2_FindReportDescriptor(void)  ;
    void USBUART2_FindHidClassDecriptor(void) ;
#endif /* USBUART2_ENABLE_HID_CLASS */


/***************************************
*    MIDI Internal references
***************************************/

#if defined(USBUART2_ENABLE_MIDI_STREAMING)
    void USBUART2_MIDI_IN_EP_Service(void)  ;
#endif /* (USBUART2_ENABLE_MIDI_STREAMING) */


/***************************************
*    CDC Internal references
***************************************/

#if defined(USBUART2_ENABLE_CDC_CLASS)

    typedef struct
    {
        uint8  bRequestType;
        uint8  bNotification;
        uint8  wValue;
        uint8  wValueMSB;
        uint8  wIndex;
        uint8  wIndexMSB;
        uint8  wLength;
        uint8  wLengthMSB;
        uint8  wSerialState;
        uint8  wSerialStateMSB;
    } t_USBUART2_cdc_notification;

    uint8 USBUART2_GetInterfaceComPort(uint8 interface) ;
    uint8 USBUART2_Cdc_EpInit( const T_USBUART2_EP_SETTINGS_BLOCK CYCODE *pEP, uint8 epNum, uint8 cdcComNums) ;

    extern volatile uint8  USBUART2_cdc_dataInEpList[USBUART2_MAX_MULTI_COM_NUM];
    extern volatile uint8  USBUART2_cdc_dataOutEpList[USBUART2_MAX_MULTI_COM_NUM];
    extern volatile uint8  USBUART2_cdc_commInEpList[USBUART2_MAX_MULTI_COM_NUM];
#endif /* (USBUART2_ENABLE_CDC_CLASS) */


#endif /* CY_USBFS_USBUART2_pvt_H */


/* [] END OF FILE */
