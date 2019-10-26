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

#if !defined(CY_USBFS_USBTMC_pvt_H)
#define CY_USBFS_USBTMC_pvt_H

#include "USBTMC.h"
   
#ifdef USBTMC_ENABLE_AUDIO_CLASS
    #include "USBTMC_audio.h"
#endif /* USBTMC_ENABLE_AUDIO_CLASS */

#ifdef USBTMC_ENABLE_CDC_CLASS
    #include "USBTMC_cdc.h"
#endif /* USBTMC_ENABLE_CDC_CLASS */

#if (USBTMC_ENABLE_MIDI_CLASS)
    #include "USBTMC_midi.h"
#endif /* (USBTMC_ENABLE_MIDI_CLASS) */

#if (USBTMC_ENABLE_MSC_CLASS)
    #include "USBTMC_msc.h"
#endif /* (USBTMC_ENABLE_MSC_CLASS) */

#if (USBTMC_EP_MANAGEMENT_DMA)
    #if (CY_PSOC4)
        #include <CyDMA.h>
    #else
        #include <CyDmac.h>
        #if ((USBTMC_EP_MANAGEMENT_DMA_AUTO) && (USBTMC_EP_DMA_AUTO_OPT == 0u))
            #include "USBTMC_EP_DMA_Done_isr.h"
            #include "USBTMC_EP8_DMA_Done_SR.h"
            #include "USBTMC_EP17_DMA_Done_SR.h"
        #endif /* ((USBTMC_EP_MANAGEMENT_DMA_AUTO) && (USBTMC_EP_DMA_AUTO_OPT == 0u)) */
    #endif /* (CY_PSOC4) */
#endif /* (USBTMC_EP_MANAGEMENT_DMA) */

#if (USBTMC_DMA1_ACTIVE)
    #include "USBTMC_ep1_dma.h"
    #define USBTMC_EP1_DMA_CH     (USBTMC_ep1_dma_CHANNEL)
#endif /* (USBTMC_DMA1_ACTIVE) */

#if (USBTMC_DMA2_ACTIVE)
    #include "USBTMC_ep2_dma.h"
    #define USBTMC_EP2_DMA_CH     (USBTMC_ep2_dma_CHANNEL)
#endif /* (USBTMC_DMA2_ACTIVE) */

#if (USBTMC_DMA3_ACTIVE)
    #include "USBTMC_ep3_dma.h"
    #define USBTMC_EP3_DMA_CH     (USBTMC_ep3_dma_CHANNEL)
#endif /* (USBTMC_DMA3_ACTIVE) */

#if (USBTMC_DMA4_ACTIVE)
    #include "USBTMC_ep4_dma.h"
    #define USBTMC_EP4_DMA_CH     (USBTMC_ep4_dma_CHANNEL)
#endif /* (USBTMC_DMA4_ACTIVE) */

#if (USBTMC_DMA5_ACTIVE)
    #include "USBTMC_ep5_dma.h"
    #define USBTMC_EP5_DMA_CH     (USBTMC_ep5_dma_CHANNEL)
#endif /* (USBTMC_DMA5_ACTIVE) */

#if (USBTMC_DMA6_ACTIVE)
    #include "USBTMC_ep6_dma.h"
    #define USBTMC_EP6_DMA_CH     (USBTMC_ep6_dma_CHANNEL)
#endif /* (USBTMC_DMA6_ACTIVE) */

#if (USBTMC_DMA7_ACTIVE)
    #include "USBTMC_ep7_dma.h"
    #define USBTMC_EP7_DMA_CH     (USBTMC_ep7_dma_CHANNEL)
#endif /* (USBTMC_DMA7_ACTIVE) */

#if (USBTMC_DMA8_ACTIVE)
    #include "USBTMC_ep8_dma.h"
    #define USBTMC_EP8_DMA_CH     (USBTMC_ep8_dma_CHANNEL)
#endif /* (USBTMC_DMA8_ACTIVE) */


/***************************************
*     Private Variables
***************************************/

/* Generated external references for descriptors. */
extern const uint8 CYCODE USBTMC_DEVICE0_DESCR[18u];
extern const uint8 CYCODE USBTMC_DEVICE0_CONFIGURATION0_DESCR[39u];
extern const T_USBTMC_EP_SETTINGS_BLOCK CYCODE USBTMC_DEVICE0_CONFIGURATION0_EP_SETTINGS_TABLE[3u];
extern const uint8 CYCODE USBTMC_DEVICE0_CONFIGURATION0_INTERFACE_CLASS[1u];
extern const T_USBTMC_LUT CYCODE USBTMC_DEVICE0_CONFIGURATION0_TABLE[4u];
extern const T_USBTMC_LUT CYCODE USBTMC_DEVICE0_TABLE[3u];
extern const T_USBTMC_LUT CYCODE USBTMC_TABLE[1u];
extern const uint8 CYCODE USBTMC_SN_STRING_DESCRIPTOR[22];
extern const uint8 CYCODE USBTMC_STRING_DESCRIPTORS[27u];


extern const uint8 CYCODE USBTMC_MSOS_DESCRIPTOR[USBTMC_MSOS_DESCRIPTOR_LENGTH];
extern const uint8 CYCODE USBTMC_MSOS_CONFIGURATION_DESCR[USBTMC_MSOS_CONF_DESCR_LENGTH];
#if defined(USBTMC_ENABLE_IDSN_STRING)
    extern uint8 USBTMC_idSerialNumberStringDescriptor[USBTMC_IDSN_DESCR_LENGTH];
#endif /* (USBTMC_ENABLE_IDSN_STRING) */

extern volatile uint8 USBTMC_interfaceNumber;
extern volatile uint8 USBTMC_interfaceSetting[USBTMC_MAX_INTERFACES_NUMBER];
extern volatile uint8 USBTMC_interfaceSettingLast[USBTMC_MAX_INTERFACES_NUMBER];
extern volatile uint8 USBTMC_deviceAddress;
extern volatile uint8 USBTMC_interfaceStatus[USBTMC_MAX_INTERFACES_NUMBER];
extern const uint8 CYCODE *USBTMC_interfaceClass;

extern volatile T_USBTMC_EP_CTL_BLOCK USBTMC_EP[USBTMC_MAX_EP];
extern volatile T_USBTMC_TD USBTMC_currentTD;

#if (USBTMC_EP_MANAGEMENT_DMA)
    #if (CY_PSOC4)
        extern const uint8 USBTMC_DmaChan[USBTMC_MAX_EP];
    #else
        extern uint8 USBTMC_DmaChan[USBTMC_MAX_EP];
        extern uint8 USBTMC_DmaTd  [USBTMC_MAX_EP];
    #endif /* (CY_PSOC4) */
#endif /* (USBTMC_EP_MANAGEMENT_DMA) */

#if (USBTMC_EP_MANAGEMENT_DMA_AUTO)
#if (CY_PSOC4)
    extern uint8  USBTMC_DmaEpBurstCnt   [USBTMC_MAX_EP];
    extern uint8  USBTMC_DmaEpLastBurstEl[USBTMC_MAX_EP];

    extern uint8  USBTMC_DmaEpBurstCntBackup  [USBTMC_MAX_EP];
    extern uint32 USBTMC_DmaEpBufferAddrBackup[USBTMC_MAX_EP];
    
    extern const uint8 USBTMC_DmaReqOut     [USBTMC_MAX_EP];    
    extern const uint8 USBTMC_DmaBurstEndOut[USBTMC_MAX_EP];
#else
    #if (USBTMC_EP_DMA_AUTO_OPT == 0u)
        extern uint8 USBTMC_DmaNextTd[USBTMC_MAX_EP];
        extern volatile uint16 USBTMC_inLength [USBTMC_MAX_EP];
        extern volatile uint8  USBTMC_inBufFull[USBTMC_MAX_EP];
        extern const uint8 USBTMC_epX_TD_TERMOUT_EN[USBTMC_MAX_EP];
        extern const uint8 *USBTMC_inDataPointer[USBTMC_MAX_EP];
    #endif /* (USBTMC_EP_DMA_AUTO_OPT == 0u) */
#endif /* CY_PSOC4 */
#endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */

extern volatile uint8 USBTMC_ep0Toggle;
extern volatile uint8 USBTMC_lastPacketSize;
extern volatile uint8 USBTMC_ep0Mode;
extern volatile uint8 USBTMC_ep0Count;
extern volatile uint16 USBTMC_transferByteCount;


/***************************************
*     Private Function Prototypes
***************************************/
void  USBTMC_ReInitComponent(void)            ;
void  USBTMC_HandleSetup(void)                ;
void  USBTMC_HandleIN(void)                   ;
void  USBTMC_HandleOUT(void)                  ;
void  USBTMC_LoadEP0(void)                    ;
uint8 USBTMC_InitControlRead(void)            ;
uint8 USBTMC_InitControlWrite(void)           ;
void  USBTMC_ControlReadDataStage(void)       ;
void  USBTMC_ControlReadStatusStage(void)     ;
void  USBTMC_ControlReadPrematureStatus(void) ;
uint8 USBTMC_InitControlWrite(void)           ;
uint8 USBTMC_InitZeroLengthControlTransfer(void) ;
void  USBTMC_ControlWriteDataStage(void)      ;
void  USBTMC_ControlWriteStatusStage(void)    ;
void  USBTMC_ControlWritePrematureStatus(void);
uint8 USBTMC_InitNoDataControlTransfer(void)  ;
void  USBTMC_NoDataControlStatusStage(void)   ;
void  USBTMC_InitializeStatusBlock(void)      ;
void  USBTMC_UpdateStatusBlock(uint8 completionCode) ;
uint8 USBTMC_DispatchClassRqst(void)          ;

void USBTMC_Config(uint8 clearAltSetting) ;
void USBTMC_ConfigAltChanged(void)        ;
void USBTMC_ConfigReg(void)               ;
void USBTMC_EpStateInit(void)             ;


const T_USBTMC_LUT CYCODE *USBTMC_GetConfigTablePtr(uint8 confIndex);
const T_USBTMC_LUT CYCODE *USBTMC_GetDeviceTablePtr(void)           ;
#if (USBTMC_BOS_ENABLE)
    const T_USBTMC_LUT CYCODE *USBTMC_GetBOSPtr(void)               ;
#endif /* (USBTMC_BOS_ENABLE) */
const uint8 CYCODE *USBTMC_GetInterfaceClassTablePtr(void)                    ;
uint8 USBTMC_ClearEndpointHalt(void)                                          ;
uint8 USBTMC_SetEndpointHalt(void)                                            ;
uint8 USBTMC_ValidateAlternateSetting(void)                                   ;

void USBTMC_SaveConfig(void)      ;
void USBTMC_RestoreConfig(void)   ;

#if (CY_PSOC3 || CY_PSOC5LP)
    #if (USBTMC_EP_MANAGEMENT_DMA_AUTO && (USBTMC_EP_DMA_AUTO_OPT == 0u))
        void USBTMC_LoadNextInEP(uint8 epNumber, uint8 mode)  ;
    #endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO && (USBTMC_EP_DMA_AUTO_OPT == 0u)) */
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

#if defined(USBTMC_ENABLE_IDSN_STRING)
    void USBTMC_ReadDieID(uint8 descr[])  ;
#endif /* USBTMC_ENABLE_IDSN_STRING */

#if defined(USBTMC_ENABLE_HID_CLASS)
    uint8 USBTMC_DispatchHIDClassRqst(void) ;
#endif /* (USBTMC_ENABLE_HID_CLASS) */

#if defined(USBTMC_ENABLE_AUDIO_CLASS)
    uint8 USBTMC_DispatchAUDIOClassRqst(void) ;
#endif /* (USBTMC_ENABLE_AUDIO_CLASS) */

#if defined(USBTMC_ENABLE_CDC_CLASS)
    uint8 USBTMC_DispatchCDCClassRqst(void) ;
#endif /* (USBTMC_ENABLE_CDC_CLASS) */

#if (USBTMC_ENABLE_MSC_CLASS)
    #if (USBTMC_HANDLE_MSC_REQUESTS)
        uint8 USBTMC_DispatchMSCClassRqst(void) ;
    #endif /* (USBTMC_HANDLE_MSC_REQUESTS) */
#endif /* (USBTMC_ENABLE_MSC_CLASS */

CY_ISR_PROTO(USBTMC_EP_0_ISR);
CY_ISR_PROTO(USBTMC_BUS_RESET_ISR);

#if (USBTMC_SOF_ISR_ACTIVE)
    CY_ISR_PROTO(USBTMC_SOF_ISR);
#endif /* (USBTMC_SOF_ISR_ACTIVE) */

#if (USBTMC_EP1_ISR_ACTIVE)
    CY_ISR_PROTO(USBTMC_EP_1_ISR);
#endif /* (USBTMC_EP1_ISR_ACTIVE) */

#if (USBTMC_EP2_ISR_ACTIVE)
    CY_ISR_PROTO(USBTMC_EP_2_ISR);
#endif /* (USBTMC_EP2_ISR_ACTIVE) */

#if (USBTMC_EP3_ISR_ACTIVE)
    CY_ISR_PROTO(USBTMC_EP_3_ISR);
#endif /* (USBTMC_EP3_ISR_ACTIVE) */

#if (USBTMC_EP4_ISR_ACTIVE)
    CY_ISR_PROTO(USBTMC_EP_4_ISR);
#endif /* (USBTMC_EP4_ISR_ACTIVE) */

#if (USBTMC_EP5_ISR_ACTIVE)
    CY_ISR_PROTO(USBTMC_EP_5_ISR);
#endif /* (USBTMC_EP5_ISR_ACTIVE) */

#if (USBTMC_EP6_ISR_ACTIVE)
    CY_ISR_PROTO(USBTMC_EP_6_ISR);
#endif /* (USBTMC_EP6_ISR_ACTIVE) */

#if (USBTMC_EP7_ISR_ACTIVE)
    CY_ISR_PROTO(USBTMC_EP_7_ISR);
#endif /* (USBTMC_EP7_ISR_ACTIVE) */

#if (USBTMC_EP8_ISR_ACTIVE)
    CY_ISR_PROTO(USBTMC_EP_8_ISR);
#endif /* (USBTMC_EP8_ISR_ACTIVE) */

#if (USBTMC_EP_MANAGEMENT_DMA)
    CY_ISR_PROTO(USBTMC_ARB_ISR);
#endif /* (USBTMC_EP_MANAGEMENT_DMA) */

#if (USBTMC_DP_ISR_ACTIVE)
    CY_ISR_PROTO(USBTMC_DP_ISR);
#endif /* (USBTMC_DP_ISR_ACTIVE) */

#if (CY_PSOC4)
    CY_ISR_PROTO(USBTMC_INTR_HI_ISR);
    CY_ISR_PROTO(USBTMC_INTR_MED_ISR);
    CY_ISR_PROTO(USBTMC_INTR_LO_ISR);
    #if (USBTMC_LPM_ACTIVE)
        CY_ISR_PROTO(USBTMC_LPM_ISR);
    #endif /* (USBTMC_LPM_ACTIVE) */
#endif /* (CY_PSOC4) */

#if (USBTMC_EP_MANAGEMENT_DMA_AUTO)
#if (CY_PSOC4)
    #if (USBTMC_DMA1_ACTIVE)
        void USBTMC_EP1_DMA_DONE_ISR(void);
    #endif /* (USBTMC_DMA1_ACTIVE) */

    #if (USBTMC_DMA2_ACTIVE)
        void USBTMC_EP2_DMA_DONE_ISR(void);
    #endif /* (USBTMC_DMA2_ACTIVE) */

    #if (USBTMC_DMA3_ACTIVE)
        void USBTMC_EP3_DMA_DONE_ISR(void);
    #endif /* (USBTMC_DMA3_ACTIVE) */

    #if (USBTMC_DMA4_ACTIVE)
        void USBTMC_EP4_DMA_DONE_ISR(void);
    #endif /* (USBTMC_DMA4_ACTIVE) */

    #if (USBTMC_DMA5_ACTIVE)
        void USBTMC_EP5_DMA_DONE_ISR(void);
    #endif /* (USBTMC_DMA5_ACTIVE) */

    #if (USBTMC_DMA6_ACTIVE)
        void USBTMC_EP6_DMA_DONE_ISR(void);
    #endif /* (USBTMC_DMA6_ACTIVE) */

    #if (USBTMC_DMA7_ACTIVE)
        void USBTMC_EP7_DMA_DONE_ISR(void);
    #endif /* (USBTMC_DMA7_ACTIVE) */

    #if (USBTMC_DMA8_ACTIVE)
        void USBTMC_EP8_DMA_DONE_ISR(void);
    #endif /* (USBTMC_DMA8_ACTIVE) */

#else
    #if (USBTMC_EP_DMA_AUTO_OPT == 0u)
        CY_ISR_PROTO(USBTMC_EP_DMA_DONE_ISR);
    #endif /* (USBTMC_EP_DMA_AUTO_OPT == 0u) */
#endif /* (CY_PSOC4) */
#endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */


/***************************************
*         Request Handlers
***************************************/

uint8 USBTMC_HandleStandardRqst(void) ;
uint8 USBTMC_DispatchClassRqst(void)  ;
uint8 USBTMC_HandleVendorRqst(void)   ;


/***************************************
*    HID Internal references
***************************************/

#if defined(USBTMC_ENABLE_HID_CLASS)
    void USBTMC_FindReport(void)            ;
    void USBTMC_FindReportDescriptor(void)  ;
    void USBTMC_FindHidClassDecriptor(void) ;
#endif /* USBTMC_ENABLE_HID_CLASS */


/***************************************
*    MIDI Internal references
***************************************/

#if defined(USBTMC_ENABLE_MIDI_STREAMING)
    void USBTMC_MIDI_IN_EP_Service(void)  ;
#endif /* (USBTMC_ENABLE_MIDI_STREAMING) */


/***************************************
*    CDC Internal references
***************************************/

#if defined(USBTMC_ENABLE_CDC_CLASS)

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
    } t_USBTMC_cdc_notification;

    uint8 USBTMC_GetInterfaceComPort(uint8 interface) ;
    uint8 USBTMC_Cdc_EpInit( const T_USBTMC_EP_SETTINGS_BLOCK CYCODE *pEP, uint8 epNum, uint8 cdcComNums) ;

    extern volatile uint8  USBTMC_cdc_dataInEpList[USBTMC_MAX_MULTI_COM_NUM];
    extern volatile uint8  USBTMC_cdc_dataOutEpList[USBTMC_MAX_MULTI_COM_NUM];
    extern volatile uint8  USBTMC_cdc_commInEpList[USBTMC_MAX_MULTI_COM_NUM];
#endif /* (USBTMC_ENABLE_CDC_CLASS) */


#endif /* CY_USBFS_USBTMC_pvt_H */


/* [] END OF FILE */
