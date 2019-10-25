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

#if !defined(CY_USBFS_USBCOMP_pvt_H)
#define CY_USBFS_USBCOMP_pvt_H

#include "USBCOMP.h"
   
#ifdef USBCOMP_ENABLE_AUDIO_CLASS
    #include "USBCOMP_audio.h"
#endif /* USBCOMP_ENABLE_AUDIO_CLASS */

#ifdef USBCOMP_ENABLE_CDC_CLASS
    #include "USBCOMP_cdc.h"
#endif /* USBCOMP_ENABLE_CDC_CLASS */

#if (USBCOMP_ENABLE_MIDI_CLASS)
    #include "USBCOMP_midi.h"
#endif /* (USBCOMP_ENABLE_MIDI_CLASS) */

#if (USBCOMP_ENABLE_MSC_CLASS)
    #include "USBCOMP_msc.h"
#endif /* (USBCOMP_ENABLE_MSC_CLASS) */

#if (USBCOMP_EP_MANAGEMENT_DMA)
    #if (CY_PSOC4)
        #include <CyDMA.h>
    #else
        #include <CyDmac.h>
        #if ((USBCOMP_EP_MANAGEMENT_DMA_AUTO) && (USBCOMP_EP_DMA_AUTO_OPT == 0u))
            #include "USBCOMP_EP_DMA_Done_isr.h"
            #include "USBCOMP_EP8_DMA_Done_SR.h"
            #include "USBCOMP_EP17_DMA_Done_SR.h"
        #endif /* ((USBCOMP_EP_MANAGEMENT_DMA_AUTO) && (USBCOMP_EP_DMA_AUTO_OPT == 0u)) */
    #endif /* (CY_PSOC4) */
#endif /* (USBCOMP_EP_MANAGEMENT_DMA) */

#if (USBCOMP_DMA1_ACTIVE)
    #include "USBCOMP_ep1_dma.h"
    #define USBCOMP_EP1_DMA_CH     (USBCOMP_ep1_dma_CHANNEL)
#endif /* (USBCOMP_DMA1_ACTIVE) */

#if (USBCOMP_DMA2_ACTIVE)
    #include "USBCOMP_ep2_dma.h"
    #define USBCOMP_EP2_DMA_CH     (USBCOMP_ep2_dma_CHANNEL)
#endif /* (USBCOMP_DMA2_ACTIVE) */

#if (USBCOMP_DMA3_ACTIVE)
    #include "USBCOMP_ep3_dma.h"
    #define USBCOMP_EP3_DMA_CH     (USBCOMP_ep3_dma_CHANNEL)
#endif /* (USBCOMP_DMA3_ACTIVE) */

#if (USBCOMP_DMA4_ACTIVE)
    #include "USBCOMP_ep4_dma.h"
    #define USBCOMP_EP4_DMA_CH     (USBCOMP_ep4_dma_CHANNEL)
#endif /* (USBCOMP_DMA4_ACTIVE) */

#if (USBCOMP_DMA5_ACTIVE)
    #include "USBCOMP_ep5_dma.h"
    #define USBCOMP_EP5_DMA_CH     (USBCOMP_ep5_dma_CHANNEL)
#endif /* (USBCOMP_DMA5_ACTIVE) */

#if (USBCOMP_DMA6_ACTIVE)
    #include "USBCOMP_ep6_dma.h"
    #define USBCOMP_EP6_DMA_CH     (USBCOMP_ep6_dma_CHANNEL)
#endif /* (USBCOMP_DMA6_ACTIVE) */

#if (USBCOMP_DMA7_ACTIVE)
    #include "USBCOMP_ep7_dma.h"
    #define USBCOMP_EP7_DMA_CH     (USBCOMP_ep7_dma_CHANNEL)
#endif /* (USBCOMP_DMA7_ACTIVE) */

#if (USBCOMP_DMA8_ACTIVE)
    #include "USBCOMP_ep8_dma.h"
    #define USBCOMP_EP8_DMA_CH     (USBCOMP_ep8_dma_CHANNEL)
#endif /* (USBCOMP_DMA8_ACTIVE) */


/***************************************
*     Private Variables
***************************************/

/* Generated external references for descriptors. */
extern const uint8 CYCODE USBCOMP_DEVICE0_DESCR[18u];
extern const uint8 CYCODE USBCOMP_DEVICE0_CONFIGURATION0_DESCR[39u];
extern const T_USBCOMP_EP_SETTINGS_BLOCK CYCODE USBCOMP_DEVICE0_CONFIGURATION0_EP_SETTINGS_TABLE[3u];
extern const uint8 CYCODE USBCOMP_DEVICE0_CONFIGURATION0_INTERFACE_CLASS[1u];
extern const T_USBCOMP_LUT CYCODE USBCOMP_DEVICE0_CONFIGURATION0_TABLE[4u];
extern const T_USBCOMP_LUT CYCODE USBCOMP_DEVICE0_TABLE[3u];
extern const T_USBCOMP_LUT CYCODE USBCOMP_TABLE[1u];
extern const uint8 CYCODE USBCOMP_SN_STRING_DESCRIPTOR[22];
extern const uint8 CYCODE USBCOMP_STRING_DESCRIPTORS[27u];


extern const uint8 CYCODE USBCOMP_MSOS_DESCRIPTOR[USBCOMP_MSOS_DESCRIPTOR_LENGTH];
extern const uint8 CYCODE USBCOMP_MSOS_CONFIGURATION_DESCR[USBCOMP_MSOS_CONF_DESCR_LENGTH];
#if defined(USBCOMP_ENABLE_IDSN_STRING)
    extern uint8 USBCOMP_idSerialNumberStringDescriptor[USBCOMP_IDSN_DESCR_LENGTH];
#endif /* (USBCOMP_ENABLE_IDSN_STRING) */

extern volatile uint8 USBCOMP_interfaceNumber;
extern volatile uint8 USBCOMP_interfaceSetting[USBCOMP_MAX_INTERFACES_NUMBER];
extern volatile uint8 USBCOMP_interfaceSettingLast[USBCOMP_MAX_INTERFACES_NUMBER];
extern volatile uint8 USBCOMP_deviceAddress;
extern volatile uint8 USBCOMP_interfaceStatus[USBCOMP_MAX_INTERFACES_NUMBER];
extern const uint8 CYCODE *USBCOMP_interfaceClass;

extern volatile T_USBCOMP_EP_CTL_BLOCK USBCOMP_EP[USBCOMP_MAX_EP];
extern volatile T_USBCOMP_TD USBCOMP_currentTD;

#if (USBCOMP_EP_MANAGEMENT_DMA)
    #if (CY_PSOC4)
        extern const uint8 USBCOMP_DmaChan[USBCOMP_MAX_EP];
    #else
        extern uint8 USBCOMP_DmaChan[USBCOMP_MAX_EP];
        extern uint8 USBCOMP_DmaTd  [USBCOMP_MAX_EP];
    #endif /* (CY_PSOC4) */
#endif /* (USBCOMP_EP_MANAGEMENT_DMA) */

#if (USBCOMP_EP_MANAGEMENT_DMA_AUTO)
#if (CY_PSOC4)
    extern uint8  USBCOMP_DmaEpBurstCnt   [USBCOMP_MAX_EP];
    extern uint8  USBCOMP_DmaEpLastBurstEl[USBCOMP_MAX_EP];

    extern uint8  USBCOMP_DmaEpBurstCntBackup  [USBCOMP_MAX_EP];
    extern uint32 USBCOMP_DmaEpBufferAddrBackup[USBCOMP_MAX_EP];
    
    extern const uint8 USBCOMP_DmaReqOut     [USBCOMP_MAX_EP];    
    extern const uint8 USBCOMP_DmaBurstEndOut[USBCOMP_MAX_EP];
#else
    #if (USBCOMP_EP_DMA_AUTO_OPT == 0u)
        extern uint8 USBCOMP_DmaNextTd[USBCOMP_MAX_EP];
        extern volatile uint16 USBCOMP_inLength [USBCOMP_MAX_EP];
        extern volatile uint8  USBCOMP_inBufFull[USBCOMP_MAX_EP];
        extern const uint8 USBCOMP_epX_TD_TERMOUT_EN[USBCOMP_MAX_EP];
        extern const uint8 *USBCOMP_inDataPointer[USBCOMP_MAX_EP];
    #endif /* (USBCOMP_EP_DMA_AUTO_OPT == 0u) */
#endif /* CY_PSOC4 */
#endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO) */

extern volatile uint8 USBCOMP_ep0Toggle;
extern volatile uint8 USBCOMP_lastPacketSize;
extern volatile uint8 USBCOMP_ep0Mode;
extern volatile uint8 USBCOMP_ep0Count;
extern volatile uint16 USBCOMP_transferByteCount;


/***************************************
*     Private Function Prototypes
***************************************/
void  USBCOMP_ReInitComponent(void)            ;
void  USBCOMP_HandleSetup(void)                ;
void  USBCOMP_HandleIN(void)                   ;
void  USBCOMP_HandleOUT(void)                  ;
void  USBCOMP_LoadEP0(void)                    ;
uint8 USBCOMP_InitControlRead(void)            ;
uint8 USBCOMP_InitControlWrite(void)           ;
void  USBCOMP_ControlReadDataStage(void)       ;
void  USBCOMP_ControlReadStatusStage(void)     ;
void  USBCOMP_ControlReadPrematureStatus(void) ;
uint8 USBCOMP_InitControlWrite(void)           ;
uint8 USBCOMP_InitZeroLengthControlTransfer(void) ;
void  USBCOMP_ControlWriteDataStage(void)      ;
void  USBCOMP_ControlWriteStatusStage(void)    ;
void  USBCOMP_ControlWritePrematureStatus(void);
uint8 USBCOMP_InitNoDataControlTransfer(void)  ;
void  USBCOMP_NoDataControlStatusStage(void)   ;
void  USBCOMP_InitializeStatusBlock(void)      ;
void  USBCOMP_UpdateStatusBlock(uint8 completionCode) ;
uint8 USBCOMP_DispatchClassRqst(void)          ;

void USBCOMP_Config(uint8 clearAltSetting) ;
void USBCOMP_ConfigAltChanged(void)        ;
void USBCOMP_ConfigReg(void)               ;
void USBCOMP_EpStateInit(void)             ;


const T_USBCOMP_LUT CYCODE *USBCOMP_GetConfigTablePtr(uint8 confIndex);
const T_USBCOMP_LUT CYCODE *USBCOMP_GetDeviceTablePtr(void)           ;
#if (USBCOMP_BOS_ENABLE)
    const T_USBCOMP_LUT CYCODE *USBCOMP_GetBOSPtr(void)               ;
#endif /* (USBCOMP_BOS_ENABLE) */
const uint8 CYCODE *USBCOMP_GetInterfaceClassTablePtr(void)                    ;
uint8 USBCOMP_ClearEndpointHalt(void)                                          ;
uint8 USBCOMP_SetEndpointHalt(void)                                            ;
uint8 USBCOMP_ValidateAlternateSetting(void)                                   ;

void USBCOMP_SaveConfig(void)      ;
void USBCOMP_RestoreConfig(void)   ;

#if (CY_PSOC3 || CY_PSOC5LP)
    #if (USBCOMP_EP_MANAGEMENT_DMA_AUTO && (USBCOMP_EP_DMA_AUTO_OPT == 0u))
        void USBCOMP_LoadNextInEP(uint8 epNumber, uint8 mode)  ;
    #endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO && (USBCOMP_EP_DMA_AUTO_OPT == 0u)) */
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

#if defined(USBCOMP_ENABLE_IDSN_STRING)
    void USBCOMP_ReadDieID(uint8 descr[])  ;
#endif /* USBCOMP_ENABLE_IDSN_STRING */

#if defined(USBCOMP_ENABLE_HID_CLASS)
    uint8 USBCOMP_DispatchHIDClassRqst(void) ;
#endif /* (USBCOMP_ENABLE_HID_CLASS) */

#if defined(USBCOMP_ENABLE_AUDIO_CLASS)
    uint8 USBCOMP_DispatchAUDIOClassRqst(void) ;
#endif /* (USBCOMP_ENABLE_AUDIO_CLASS) */

#if defined(USBCOMP_ENABLE_CDC_CLASS)
    uint8 USBCOMP_DispatchCDCClassRqst(void) ;
#endif /* (USBCOMP_ENABLE_CDC_CLASS) */

#if (USBCOMP_ENABLE_MSC_CLASS)
    #if (USBCOMP_HANDLE_MSC_REQUESTS)
        uint8 USBCOMP_DispatchMSCClassRqst(void) ;
    #endif /* (USBCOMP_HANDLE_MSC_REQUESTS) */
#endif /* (USBCOMP_ENABLE_MSC_CLASS */

CY_ISR_PROTO(USBCOMP_EP_0_ISR);
CY_ISR_PROTO(USBCOMP_BUS_RESET_ISR);

#if (USBCOMP_SOF_ISR_ACTIVE)
    CY_ISR_PROTO(USBCOMP_SOF_ISR);
#endif /* (USBCOMP_SOF_ISR_ACTIVE) */

#if (USBCOMP_EP1_ISR_ACTIVE)
    CY_ISR_PROTO(USBCOMP_EP_1_ISR);
#endif /* (USBCOMP_EP1_ISR_ACTIVE) */

#if (USBCOMP_EP2_ISR_ACTIVE)
    CY_ISR_PROTO(USBCOMP_EP_2_ISR);
#endif /* (USBCOMP_EP2_ISR_ACTIVE) */

#if (USBCOMP_EP3_ISR_ACTIVE)
    CY_ISR_PROTO(USBCOMP_EP_3_ISR);
#endif /* (USBCOMP_EP3_ISR_ACTIVE) */

#if (USBCOMP_EP4_ISR_ACTIVE)
    CY_ISR_PROTO(USBCOMP_EP_4_ISR);
#endif /* (USBCOMP_EP4_ISR_ACTIVE) */

#if (USBCOMP_EP5_ISR_ACTIVE)
    CY_ISR_PROTO(USBCOMP_EP_5_ISR);
#endif /* (USBCOMP_EP5_ISR_ACTIVE) */

#if (USBCOMP_EP6_ISR_ACTIVE)
    CY_ISR_PROTO(USBCOMP_EP_6_ISR);
#endif /* (USBCOMP_EP6_ISR_ACTIVE) */

#if (USBCOMP_EP7_ISR_ACTIVE)
    CY_ISR_PROTO(USBCOMP_EP_7_ISR);
#endif /* (USBCOMP_EP7_ISR_ACTIVE) */

#if (USBCOMP_EP8_ISR_ACTIVE)
    CY_ISR_PROTO(USBCOMP_EP_8_ISR);
#endif /* (USBCOMP_EP8_ISR_ACTIVE) */

#if (USBCOMP_EP_MANAGEMENT_DMA)
    CY_ISR_PROTO(USBCOMP_ARB_ISR);
#endif /* (USBCOMP_EP_MANAGEMENT_DMA) */

#if (USBCOMP_DP_ISR_ACTIVE)
    CY_ISR_PROTO(USBCOMP_DP_ISR);
#endif /* (USBCOMP_DP_ISR_ACTIVE) */

#if (CY_PSOC4)
    CY_ISR_PROTO(USBCOMP_INTR_HI_ISR);
    CY_ISR_PROTO(USBCOMP_INTR_MED_ISR);
    CY_ISR_PROTO(USBCOMP_INTR_LO_ISR);
    #if (USBCOMP_LPM_ACTIVE)
        CY_ISR_PROTO(USBCOMP_LPM_ISR);
    #endif /* (USBCOMP_LPM_ACTIVE) */
#endif /* (CY_PSOC4) */

#if (USBCOMP_EP_MANAGEMENT_DMA_AUTO)
#if (CY_PSOC4)
    #if (USBCOMP_DMA1_ACTIVE)
        void USBCOMP_EP1_DMA_DONE_ISR(void);
    #endif /* (USBCOMP_DMA1_ACTIVE) */

    #if (USBCOMP_DMA2_ACTIVE)
        void USBCOMP_EP2_DMA_DONE_ISR(void);
    #endif /* (USBCOMP_DMA2_ACTIVE) */

    #if (USBCOMP_DMA3_ACTIVE)
        void USBCOMP_EP3_DMA_DONE_ISR(void);
    #endif /* (USBCOMP_DMA3_ACTIVE) */

    #if (USBCOMP_DMA4_ACTIVE)
        void USBCOMP_EP4_DMA_DONE_ISR(void);
    #endif /* (USBCOMP_DMA4_ACTIVE) */

    #if (USBCOMP_DMA5_ACTIVE)
        void USBCOMP_EP5_DMA_DONE_ISR(void);
    #endif /* (USBCOMP_DMA5_ACTIVE) */

    #if (USBCOMP_DMA6_ACTIVE)
        void USBCOMP_EP6_DMA_DONE_ISR(void);
    #endif /* (USBCOMP_DMA6_ACTIVE) */

    #if (USBCOMP_DMA7_ACTIVE)
        void USBCOMP_EP7_DMA_DONE_ISR(void);
    #endif /* (USBCOMP_DMA7_ACTIVE) */

    #if (USBCOMP_DMA8_ACTIVE)
        void USBCOMP_EP8_DMA_DONE_ISR(void);
    #endif /* (USBCOMP_DMA8_ACTIVE) */

#else
    #if (USBCOMP_EP_DMA_AUTO_OPT == 0u)
        CY_ISR_PROTO(USBCOMP_EP_DMA_DONE_ISR);
    #endif /* (USBCOMP_EP_DMA_AUTO_OPT == 0u) */
#endif /* (CY_PSOC4) */
#endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO) */


/***************************************
*         Request Handlers
***************************************/

uint8 USBCOMP_HandleStandardRqst(void) ;
uint8 USBCOMP_DispatchClassRqst(void)  ;
uint8 USBCOMP_HandleVendorRqst(void)   ;


/***************************************
*    HID Internal references
***************************************/

#if defined(USBCOMP_ENABLE_HID_CLASS)
    void USBCOMP_FindReport(void)            ;
    void USBCOMP_FindReportDescriptor(void)  ;
    void USBCOMP_FindHidClassDecriptor(void) ;
#endif /* USBCOMP_ENABLE_HID_CLASS */


/***************************************
*    MIDI Internal references
***************************************/

#if defined(USBCOMP_ENABLE_MIDI_STREAMING)
    void USBCOMP_MIDI_IN_EP_Service(void)  ;
#endif /* (USBCOMP_ENABLE_MIDI_STREAMING) */


/***************************************
*    CDC Internal references
***************************************/

#if defined(USBCOMP_ENABLE_CDC_CLASS)

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
    } t_USBCOMP_cdc_notification;

    uint8 USBCOMP_GetInterfaceComPort(uint8 interface) ;
    uint8 USBCOMP_Cdc_EpInit( const T_USBCOMP_EP_SETTINGS_BLOCK CYCODE *pEP, uint8 epNum, uint8 cdcComNums) ;

    extern volatile uint8  USBCOMP_cdc_dataInEpList[USBCOMP_MAX_MULTI_COM_NUM];
    extern volatile uint8  USBCOMP_cdc_dataOutEpList[USBCOMP_MAX_MULTI_COM_NUM];
    extern volatile uint8  USBCOMP_cdc_commInEpList[USBCOMP_MAX_MULTI_COM_NUM];
#endif /* (USBCOMP_ENABLE_CDC_CLASS) */


#endif /* CY_USBFS_USBCOMP_pvt_H */


/* [] END OF FILE */
