/***************************************************************************//**
* \file  USBTMC.h
* \version 3.20
*
* \brief
*  This file provides function prototypes and constants for the USBFS component. 
*
********************************************************************************
* \copyright
* Copyright 2008-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_USBFS_USBTMC_H)
#define CY_USBFS_USBTMC_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h"
#include "cyapicallbacks.h"

/*  User supplied definitions. */
/* `#START USER_DEFINITIONS` Place your declaration here */

/* `#END` */

/***************************************
* Enumerated Types and Parameters
***************************************/

/* USB IP memory management options. */
#define USBTMC__EP_MANUAL    (0u)
#define USBTMC__EP_DMAMANUAL (1u)
#define USBTMC__EP_DMAAUTO   (2u)

/* USB IP memory allocation options. */
#define USBTMC__MA_STATIC    (0u)
#define USBTMC__MA_DYNAMIC   (1u)


/***************************************
*    Initial Parameter Constants
***************************************/

#define USBTMC_NUM_DEVICES                     (1u)
#define USBTMC_ENABLE_MIDI_CLASS               (0u)
#define USBTMC_ENABLE_MSC_CLASS                (0u)
#define USBTMC_BOS_ENABLE                      (0u)
#define USBTMC_INTR_LVL_SEL                    (0x00150018u)
#define USBTMC_ENABLE_DESCRIPTOR_STRINGS       
#define USBTMC_ENABLE_SN_STRING                
#define USBTMC_ENABLE_STRINGS                  
#define USBTMC_MAX_REPORTID_NUMBER             (0u)

#define USBTMC_MON_VBUS               (0u)
#define USBTMC_EXTERN_VBUS            (1u)
#define USBTMC_POWER_PAD_VBUS         (0u)
#define USBTMC_EXTERN_VND             (0u)
#define USBTMC_EXTERN_CLS             (1u)
#define USBTMC_MAX_INTERFACES_NUMBER  (1u)
#define USBTMC_EP_MM                  (0u)
#define USBTMC_EP_MA                  (0u)
#define USBTMC_ENABLE_BATT_CHARG_DET  (0u)
#define USBTMC_GEN_16BITS_EP_ACCESS   (0u)

/* Enable Class APIs: MIDI, CDC, MSC. */         
#define USBTMC_ENABLE_CDC_CLASS_API   (0u != (1u))

/* General parameters */
#define USBTMC_EP_ALLOC_STATIC            (USBTMC_EP_MA == USBTMC__MA_STATIC)
#define USBTMC_EP_ALLOC_DYNAMIC           (USBTMC_EP_MA == USBTMC__MA_DYNAMIC)
#define USBTMC_EP_MANAGEMENT_MANUAL       (USBTMC_EP_MM == USBTMC__EP_MANUAL)
#define USBTMC_EP_MANAGEMENT_DMA          (USBTMC_EP_MM != USBTMC__EP_MANUAL)
#define USBTMC_EP_MANAGEMENT_DMA_MANUAL   (USBTMC_EP_MM == USBTMC__EP_DMAMANUAL)
#define USBTMC_EP_MANAGEMENT_DMA_AUTO     (USBTMC_EP_MM == USBTMC__EP_DMAAUTO)
#define USBTMC_BATT_CHARG_DET_ENABLE      (CY_PSOC4 && (0u != USBTMC_ENABLE_BATT_CHARG_DET))
#define USBTMC_16BITS_EP_ACCESS_ENABLE    (CY_PSOC4 && (0u != USBTMC_GEN_16BITS_EP_ACCESS))
#define USBTMC_VBUS_MONITORING_ENABLE     (0u != USBTMC_MON_VBUS)
#define USBTMC_VBUS_MONITORING_INTERNAL   (0u == USBTMC_EXTERN_VBUS)
#define USBTMC_VBUS_POWER_PAD_ENABLE      (0u != USBTMC_POWER_PAD_VBUS)

/* Control endpoints availability */
#define USBTMC_SOF_ISR_REMOVE       (1u)
#define USBTMC_BUS_RESET_ISR_REMOVE (0u)
#define USBTMC_EP0_ISR_REMOVE       (0u)
#define USBTMC_ARB_ISR_REMOVE       (0u)
#define USBTMC_DP_ISR_REMOVE        (0u)
#define USBTMC_LPM_REMOVE           (1u)
#define USBTMC_SOF_ISR_ACTIVE       ((0u == USBTMC_SOF_ISR_REMOVE) ? 1u: 0u)
#define USBTMC_BUS_RESET_ISR_ACTIVE ((0u == USBTMC_BUS_RESET_ISR_REMOVE) ? 1u: 0u)
#define USBTMC_EP0_ISR_ACTIVE       ((0u == USBTMC_EP0_ISR_REMOVE) ? 1u: 0u)
#define USBTMC_ARB_ISR_ACTIVE       ((0u == USBTMC_ARB_ISR_REMOVE) ? 1u: 0u)
#define USBTMC_DP_ISR_ACTIVE        ((0u == USBTMC_DP_ISR_REMOVE) ? 1u: 0u)
#define USBTMC_LPM_ACTIVE           ((CY_PSOC4 && (0u == USBTMC_LPM_REMOVE)) ? 1u: 0u)

/* Data endpoints availability */
#define USBTMC_EP1_ISR_REMOVE     (0u)
#define USBTMC_EP2_ISR_REMOVE     (0u)
#define USBTMC_EP3_ISR_REMOVE     (0u)
#define USBTMC_EP4_ISR_REMOVE     (1u)
#define USBTMC_EP5_ISR_REMOVE     (1u)
#define USBTMC_EP6_ISR_REMOVE     (1u)
#define USBTMC_EP7_ISR_REMOVE     (1u)
#define USBTMC_EP8_ISR_REMOVE     (1u)
#define USBTMC_EP1_ISR_ACTIVE     ((0u == USBTMC_EP1_ISR_REMOVE) ? 1u: 0u)
#define USBTMC_EP2_ISR_ACTIVE     ((0u == USBTMC_EP2_ISR_REMOVE) ? 1u: 0u)
#define USBTMC_EP3_ISR_ACTIVE     ((0u == USBTMC_EP3_ISR_REMOVE) ? 1u: 0u)
#define USBTMC_EP4_ISR_ACTIVE     ((0u == USBTMC_EP4_ISR_REMOVE) ? 1u: 0u)
#define USBTMC_EP5_ISR_ACTIVE     ((0u == USBTMC_EP5_ISR_REMOVE) ? 1u: 0u)
#define USBTMC_EP6_ISR_ACTIVE     ((0u == USBTMC_EP6_ISR_REMOVE) ? 1u: 0u)
#define USBTMC_EP7_ISR_ACTIVE     ((0u == USBTMC_EP7_ISR_REMOVE) ? 1u: 0u)
#define USBTMC_EP8_ISR_ACTIVE     ((0u == USBTMC_EP8_ISR_REMOVE) ? 1u: 0u)

#define USBTMC_EP_DMA_AUTO_OPT    ((CY_PSOC4) ? (1u) : (0u))
#define USBTMC_DMA1_REMOVE        (1u)
#define USBTMC_DMA2_REMOVE        (1u)
#define USBTMC_DMA3_REMOVE        (1u)
#define USBTMC_DMA4_REMOVE        (1u)
#define USBTMC_DMA5_REMOVE        (1u)
#define USBTMC_DMA6_REMOVE        (1u)
#define USBTMC_DMA7_REMOVE        (1u)
#define USBTMC_DMA8_REMOVE        (1u)
#define USBTMC_DMA1_ACTIVE        ((0u == USBTMC_DMA1_REMOVE) ? 1u: 0u)
#define USBTMC_DMA2_ACTIVE        ((0u == USBTMC_DMA2_REMOVE) ? 1u: 0u)
#define USBTMC_DMA3_ACTIVE        ((0u == USBTMC_DMA3_REMOVE) ? 1u: 0u)
#define USBTMC_DMA4_ACTIVE        ((0u == USBTMC_DMA4_REMOVE) ? 1u: 0u)
#define USBTMC_DMA5_ACTIVE        ((0u == USBTMC_DMA5_REMOVE) ? 1u: 0u)
#define USBTMC_DMA6_ACTIVE        ((0u == USBTMC_DMA6_REMOVE) ? 1u: 0u)
#define USBTMC_DMA7_ACTIVE        ((0u == USBTMC_DMA7_REMOVE) ? 1u: 0u)
#define USBTMC_DMA8_ACTIVE        ((0u == USBTMC_DMA8_REMOVE) ? 1u: 0u)


/***************************************
*    Data Structures Definition
***************************************/

typedef struct
{
    uint8  attrib;
    uint8  apiEpState;
    uint8  hwEpState;
    uint8  epToggle;
    uint8  addr;
    uint8  epMode;
    uint16 buffOffset;
    uint16 bufferSize;
    uint8  interface;
} T_USBTMC_EP_CTL_BLOCK;

typedef struct
{
    uint8  interface;
    uint8  altSetting;
    uint8  addr;
    uint8  attributes;
    uint16 bufferSize;
    uint8  bMisc;
} T_USBTMC_EP_SETTINGS_BLOCK;

typedef struct
{
    uint8  status;
    uint16 length;
} T_USBTMC_XFER_STATUS_BLOCK;

typedef struct
{
    uint16  count;
    volatile uint8 *pData;
    T_USBTMC_XFER_STATUS_BLOCK *pStatusBlock;
} T_USBTMC_TD;

typedef struct
{
    uint8   c;
    const void *p_list;
} T_USBTMC_LUT;

/* Resume/Suspend API Support */
typedef struct
{
    uint8 enableState;
    uint8 mode;
#if (CY_PSOC4)
    uint8 intrSeiMask;
#endif /* (CY_PSOC4) */
} USBTMC_BACKUP_STRUCT;

/* Number of endpoint 0 data registers. */
#define USBTMC_EP0_DR_MAPPED_REG_CNT  (8u)

/* Structure to access data registers for EP0. */
typedef struct
{
    uint32 epData[USBTMC_EP0_DR_MAPPED_REG_CNT];
} USBTMC_ep0_data_struct;

/* Number of SIE endpoint registers group. */
#define USBTMC_SIE_EP_REG_SIZE   (USBTMC_cy_m0s8_usb__SIE_EP1_CR0 - \
                                            USBTMC_cy_m0s8_usb__SIE_EP1_CNT0)

/* Size of gap between SIE endpoint registers groups. */
#define USBTMC_SIE_GAP_CNT        (((USBTMC_cy_m0s8_usb__SIE_EP2_CNT0 - \
                                             (USBTMC_cy_m0s8_usb__SIE_EP1_CNT0 + \
                                              USBTMC_SIE_EP_REG_SIZE)) / sizeof(reg32)) - 1u)

/* Structure to access to SIE registers for endpoint. */
typedef struct
{
    uint32 epCnt0;
    uint32 epCnt1;
    uint32 epCr0;
    uint32 gap[USBTMC_SIE_GAP_CNT];
} USBTMC_sie_ep_struct;

/* Number of ARB endpoint registers group. */
#define USBTMC_ARB_EP_REG_SIZE    (USBTMC_cy_m0s8_usb__ARB_RW1_DR - \
                                             USBTMC_cy_m0s8_usb__ARB_EP1_CFG)

/* Size of gap between ARB endpoint registers groups. */
#define USBTMC_ARB_GAP_CNT        (((USBTMC_cy_m0s8_usb__ARB_EP2_CFG - \
                                             (USBTMC_cy_m0s8_usb__ARB_EP1_CFG + \
                                              USBTMC_ARB_EP_REG_SIZE)) / sizeof(reg32)) - 1u)

/* Structure to access to ARB registers for endpoint. */
typedef struct
{
    uint32 epCfg;
    uint32 epIntEn;
    uint32 epSr;
    uint32 reserved;
    uint32 rwWa;
    uint32 rwWaMsb;
    uint32 rwRa;
    uint32 rwRaMsb;
    uint32 rwDr;
    uint32 gap[USBTMC_ARB_GAP_CNT];
} USBTMC_arb_ep_struct;

#if (CY_PSOC4)
    /* Number of ARB endpoint registers group (16-bits access). */
    #define USBTMC_ARB_EP_REG16_SIZE      (USBTMC_cy_m0s8_usb__ARB_RW1_DR16 - \
                                                     USBTMC_cy_m0s8_usb__ARB_RW1_WA16)

    /* Size of gap between ARB endpoint registers groups (16-bits access). */
    #define USBTMC_ARB_EP_REG16_GAP_CNT   (((USBTMC_cy_m0s8_usb__ARB_RW2_WA16 - \
                                                     (USBTMC_cy_m0s8_usb__ARB_RW1_WA16 + \
                                                      USBTMC_ARB_EP_REG16_SIZE)) / sizeof(reg32)) - 1u)

    /* Structure to access to ARB registers for endpoint (16-bits access). */
    typedef struct
    {
        uint32 rwWa16;
        uint32 reserved0;
        uint32 rwRa16;
        uint32 reserved1;
        uint32 rwDr16;
        uint32 gap[USBTMC_ARB_EP_REG16_GAP_CNT];
    } USBTMC_arb_ep_reg16_struct;
#endif /* (CY_PSOC4) */

/* Number of endpoint (takes to account that endpoints numbers are 1-8). */
#define USBTMC_NUMBER_EP  (9u)

/* Consoled SIE register groups for endpoints 1-8. */
typedef struct
{
    USBTMC_sie_ep_struct sieEp[USBTMC_NUMBER_EP];
} USBTMC_sie_eps_struct;

/* Consolidate ARB register groups for endpoints 1-8.*/
typedef struct
{
    USBTMC_arb_ep_struct arbEp[USBTMC_NUMBER_EP];
} USBTMC_arb_eps_struct;

#if (CY_PSOC4)
    /* Consolidate ARB register groups for endpoints 1-8 (16-bits access). */
    typedef struct
    {
        USBTMC_arb_ep_reg16_struct arbEp[USBTMC_NUMBER_EP];
    } USBTMC_arb_eps_reg16_struct;
#endif /* (CY_PSOC4) */


/***************************************
*       Function Prototypes
***************************************/
/**
* \addtogroup group_general
* @{
*/
void   USBTMC_InitComponent(uint8 device, uint8 mode) ;
void   USBTMC_Start(uint8 device, uint8 mode)         ;
void   USBTMC_Init(void)                              ;
void   USBTMC_Stop(void)                              ;
uint8  USBTMC_GetConfiguration(void)                  ;
uint8  USBTMC_IsConfigurationChanged(void)            ;
uint8  USBTMC_GetInterfaceSetting(uint8 interfaceNumber) ;
uint8  USBTMC_GetEPState(uint8 epNumber)              ;
uint16 USBTMC_GetEPCount(uint8 epNumber)              ;
void   USBTMC_LoadInEP(uint8 epNumber, const uint8 pData[], uint16 length)
                                                                ;
uint16 USBTMC_ReadOutEP(uint8 epNumber, uint8 pData[], uint16 length)
                                                                ;
void   USBTMC_EnableOutEP(uint8 epNumber)             ;
void   USBTMC_DisableOutEP(uint8 epNumber)            ;
void   USBTMC_Force(uint8 bState)                     ;
uint8  USBTMC_GetEPAckState(uint8 epNumber)           ;
void   USBTMC_SetPowerStatus(uint8 powerStatus)       ;
void   USBTMC_TerminateEP(uint8 epNumber)             ;

uint8 USBTMC_GetDeviceAddress(void) ;

void USBTMC_EnableSofInt(void)  ;
void USBTMC_DisableSofInt(void) ;


#if defined(USBTMC_ENABLE_FWSN_STRING)
    void   USBTMC_SerialNumString(uint8 snString[]) ;
#endif  /* USBTMC_ENABLE_FWSN_STRING */

#if (USBTMC_VBUS_MONITORING_ENABLE)
    uint8  USBTMC_VBusPresent(void) ;
#endif /*  (USBTMC_VBUS_MONITORING_ENABLE) */

#if (USBTMC_16BITS_EP_ACCESS_ENABLE)
    /* PSoC4 specific functions for 16-bit data register access. */
    void   USBTMC_LoadInEP16 (uint8 epNumber, const uint8 pData[], uint16 length);
    uint16 USBTMC_ReadOutEP16(uint8 epNumber,       uint8 pData[], uint16 length);
#endif /* (USBTMC_16BITS_EP_ACCESS_ENABLE) */

#if (USBTMC_BATT_CHARG_DET_ENABLE)
        uint8 USBTMC_Bcd_DetectPortType(void);
#endif /* (USBTMC_BATT_CHARG_DET_ENABLE) */

#if (USBTMC_EP_MANAGEMENT_DMA)
    void USBTMC_InitEP_DMA(uint8 epNumber, const uint8 *pData) ;   
    void USBTMC_Stop_DMA(uint8 epNumber) ;
/** @} general */ 
#endif /* (USBTMC_EP_MANAGEMENT_DMA) */

/**
* \addtogroup group_power
* @{
*/
uint8  USBTMC_CheckActivity(void) ;
void   USBTMC_Suspend(void)       ;
void   USBTMC_Resume(void)        ;
uint8  USBTMC_RWUEnabled(void)    ;

#if (USBTMC_LPM_ACTIVE)
    uint32 USBTMC_Lpm_GetBeslValue(void);
    uint32 USBTMC_Lpm_RemoteWakeUpAllowed(void);
    void   USBTMC_Lpm_SetResponse(uint32 response);
    uint32 USBTMC_Lpm_GetResponse(void);
#endif /* (USBTMC_LPM_ACTIVE) */

/** @} power */


#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_USBTMC) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
/**
* \addtogroup group_bootloader
* @{
*/
    void USBTMC_CyBtldrCommStart(void)        ;
    void USBTMC_CyBtldrCommStop(void)         ;
    void USBTMC_CyBtldrCommReset(void)        ;
    cystatus USBTMC_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 *count, uint8 timeOut) CYSMALL
                                                        ;
    cystatus USBTMC_CyBtldrCommRead       (uint8 pData[], uint16 size, uint16 *count, uint8 timeOut) CYSMALL
                                                        ;
/** @} bootloader */

    #define USBTMC_BTLDR_OUT_EP   (0x01u)
    #define USBTMC_BTLDR_IN_EP    (0x02u)

    #define USBTMC_BTLDR_SIZEOF_WRITE_BUFFER  (64u)   /* Endpoint 1 (OUT) buffer size. */
    #define USBTMC_BTLDR_SIZEOF_READ_BUFFER   (64u)   /* Endpoint 2 (IN)  buffer size. */
    #define USBTMC_BTLDR_MAX_PACKET_SIZE      USBTMC_BTLDR_SIZEOF_WRITE_BUFFER

    #define USBTMC_BTLDR_WAIT_1_MS            (1u)    /* Time Out quantity equal 1mS */

    /* Map-specific USB bootloader communication functions to common bootloader functions */
    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_USBTMC)
        #define CyBtldrCommStart        USBTMC_CyBtldrCommStart
        #define CyBtldrCommStop         USBTMC_CyBtldrCommStop
        #define CyBtldrCommReset        USBTMC_CyBtldrCommReset
        #define CyBtldrCommWrite        USBTMC_CyBtldrCommWrite
        #define CyBtldrCommRead         USBTMC_CyBtldrCommRead
    #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_USBTMC) */
#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/

#define USBTMC_EP0                        (0u)
#define USBTMC_EP1                        (1u)
#define USBTMC_EP2                        (2u)
#define USBTMC_EP3                        (3u)
#define USBTMC_EP4                        (4u)
#define USBTMC_EP5                        (5u)
#define USBTMC_EP6                        (6u)
#define USBTMC_EP7                        (7u)
#define USBTMC_EP8                        (8u)
#define USBTMC_MAX_EP                     (9u)

#define USBTMC_TRUE                       (1u)
#define USBTMC_FALSE                      (0u)

#define USBTMC_NO_EVENT_ALLOWED           (2u)
#define USBTMC_EVENT_PENDING              (1u)
#define USBTMC_NO_EVENT_PENDING           (0u)

#define USBTMC_IN_BUFFER_FULL             USBTMC_NO_EVENT_PENDING
#define USBTMC_IN_BUFFER_EMPTY            USBTMC_EVENT_PENDING
#define USBTMC_OUT_BUFFER_FULL            USBTMC_EVENT_PENDING
#define USBTMC_OUT_BUFFER_EMPTY           USBTMC_NO_EVENT_PENDING

#define USBTMC_FORCE_J                    (0xA0u)
#define USBTMC_FORCE_K                    (0x80u)
#define USBTMC_FORCE_SE0                  (0xC0u)
#define USBTMC_FORCE_NONE                 (0x00u)

#define USBTMC_IDLE_TIMER_RUNNING         (0x02u)
#define USBTMC_IDLE_TIMER_EXPIRED         (0x01u)
#define USBTMC_IDLE_TIMER_INDEFINITE      (0x00u)

#define USBTMC_DEVICE_STATUS_BUS_POWERED  (0x00u)
#define USBTMC_DEVICE_STATUS_SELF_POWERED (0x01u)

#define USBTMC_3V_OPERATION               (0x00u)
#define USBTMC_5V_OPERATION               (0x01u)
#define USBTMC_DWR_POWER_OPERATION        (0x02u)

#define USBTMC_MODE_DISABLE               (0x00u)
#define USBTMC_MODE_NAK_IN_OUT            (0x01u)
#define USBTMC_MODE_STATUS_OUT_ONLY       (0x02u)
#define USBTMC_MODE_STALL_IN_OUT          (0x03u)
#define USBTMC_MODE_RESERVED_0100         (0x04u)
#define USBTMC_MODE_ISO_OUT               (0x05u)
#define USBTMC_MODE_STATUS_IN_ONLY        (0x06u)
#define USBTMC_MODE_ISO_IN                (0x07u)
#define USBTMC_MODE_NAK_OUT               (0x08u)
#define USBTMC_MODE_ACK_OUT               (0x09u)
#define USBTMC_MODE_RESERVED_1010         (0x0Au)
#define USBTMC_MODE_ACK_OUT_STATUS_IN     (0x0Bu)
#define USBTMC_MODE_NAK_IN                (0x0Cu)
#define USBTMC_MODE_ACK_IN                (0x0Du)
#define USBTMC_MODE_RESERVED_1110         (0x0Eu)
#define USBTMC_MODE_ACK_IN_STATUS_OUT     (0x0Fu)
#define USBTMC_MODE_MASK                  (0x0Fu)
#define USBTMC_MODE_STALL_DATA_EP         (0x80u)

#define USBTMC_MODE_ACKD                  (0x10u)
#define USBTMC_MODE_OUT_RCVD              (0x20u)
#define USBTMC_MODE_IN_RCVD               (0x40u)
#define USBTMC_MODE_SETUP_RCVD            (0x80u)

#define USBTMC_RQST_TYPE_MASK             (0x60u)
#define USBTMC_RQST_TYPE_STD              (0x00u)
#define USBTMC_RQST_TYPE_CLS              (0x20u)
#define USBTMC_RQST_TYPE_VND              (0x40u)
#define USBTMC_RQST_DIR_MASK              (0x80u)
#define USBTMC_RQST_DIR_D2H               (0x80u)
#define USBTMC_RQST_DIR_H2D               (0x00u)
#define USBTMC_RQST_RCPT_MASK             (0x03u)
#define USBTMC_RQST_RCPT_DEV              (0x00u)
#define USBTMC_RQST_RCPT_IFC              (0x01u)
#define USBTMC_RQST_RCPT_EP               (0x02u)
#define USBTMC_RQST_RCPT_OTHER            (0x03u)

#if (USBTMC_LPM_ACTIVE)
    #define USBTMC_LPM_REQ_ACK            (0x01u << USBTMC_LPM_CTRL_LPM_ACK_RESP_POS)
    #define USBTMC_LPM_REQ_NACK           (0x00u)
    #define USBTMC_LPM_REQ_NYET           (0x01u << USBTMC_LPM_CTRL_NYET_EN_POS)
#endif /*(USBTMC_LPM_ACTIVE)*/

/* USB Class Codes */
#define USBTMC_CLASS_DEVICE               (0x00u)     /* Use class code info from Interface Descriptors */
#define USBTMC_CLASS_AUDIO                (0x01u)     /* Audio device */
#define USBTMC_CLASS_CDC                  (0x02u)     /* Communication device class */
#define USBTMC_CLASS_HID                  (0x03u)     /* Human Interface Device */
#define USBTMC_CLASS_PDC                  (0x05u)     /* Physical device class */
#define USBTMC_CLASS_IMAGE                (0x06u)     /* Still Imaging device */
#define USBTMC_CLASS_PRINTER              (0x07u)     /* Printer device  */
#define USBTMC_CLASS_MSD                  (0x08u)     /* Mass Storage device  */
#define USBTMC_CLASS_HUB                  (0x09u)     /* Full/Hi speed Hub */
#define USBTMC_CLASS_CDC_DATA             (0x0Au)     /* CDC data device */
#define USBTMC_CLASS_SMART_CARD           (0x0Bu)     /* Smart Card device */
#define USBTMC_CLASS_CSD                  (0x0Du)     /* Content Security device */
#define USBTMC_CLASS_VIDEO                (0x0Eu)     /* Video device */
#define USBTMC_CLASS_PHD                  (0x0Fu)     /* Personal Health care device */
#define USBTMC_CLASS_WIRELESSD            (0xDCu)     /* Wireless Controller */
#define USBTMC_CLASS_MIS                  (0xE0u)     /* Miscellaneous */
#define USBTMC_CLASS_APP                  (0xEFu)     /* Application Specific */
#define USBTMC_CLASS_VENDOR               (0xFFu)     /* Vendor specific */

/* Standard Request Types (Table 9-4) */
#define USBTMC_GET_STATUS                 (0x00u)
#define USBTMC_CLEAR_FEATURE              (0x01u)
#define USBTMC_SET_FEATURE                (0x03u)
#define USBTMC_SET_ADDRESS                (0x05u)
#define USBTMC_GET_DESCRIPTOR             (0x06u)
#define USBTMC_SET_DESCRIPTOR             (0x07u)
#define USBTMC_GET_CONFIGURATION          (0x08u)
#define USBTMC_SET_CONFIGURATION          (0x09u)
#define USBTMC_GET_INTERFACE              (0x0Au)
#define USBTMC_SET_INTERFACE              (0x0Bu)
#define USBTMC_SYNCH_FRAME                (0x0Cu)

/* Vendor Specific Request Types */
/* Request for Microsoft OS String Descriptor */
#define USBTMC_GET_EXTENDED_CONFIG_DESCRIPTOR (0x01u)

/* Descriptor Types (Table 9-5) */
#define USBTMC_DESCR_DEVICE                   (1u)
#define USBTMC_DESCR_CONFIG                   (2u)
#define USBTMC_DESCR_STRING                   (3u)
#define USBTMC_DESCR_INTERFACE                (4u)
#define USBTMC_DESCR_ENDPOINT                 (5u)
#define USBTMC_DESCR_DEVICE_QUALIFIER         (6u)
#define USBTMC_DESCR_OTHER_SPEED              (7u)
#define USBTMC_DESCR_INTERFACE_POWER          (8u)
#if (USBTMC_BOS_ENABLE)
    #define USBTMC_DESCR_BOS                  (15u)
#endif /* (USBTMC_BOS_ENABLE) */
/* Device Descriptor Defines */
#define USBTMC_DEVICE_DESCR_LENGTH            (18u)
#define USBTMC_DEVICE_DESCR_SN_SHIFT          (16u)

/* Config Descriptor Shifts and Masks */
#define USBTMC_CONFIG_DESCR_LENGTH                (0u)
#define USBTMC_CONFIG_DESCR_TYPE                  (1u)
#define USBTMC_CONFIG_DESCR_TOTAL_LENGTH_LOW      (2u)
#define USBTMC_CONFIG_DESCR_TOTAL_LENGTH_HI       (3u)
#define USBTMC_CONFIG_DESCR_NUM_INTERFACES        (4u)
#define USBTMC_CONFIG_DESCR_CONFIG_VALUE          (5u)
#define USBTMC_CONFIG_DESCR_CONFIGURATION         (6u)
#define USBTMC_CONFIG_DESCR_ATTRIB                (7u)
#define USBTMC_CONFIG_DESCR_ATTRIB_SELF_POWERED   (0x40u)
#define USBTMC_CONFIG_DESCR_ATTRIB_RWU_EN         (0x20u)

#if (USBTMC_BOS_ENABLE)
    /* Config Descriptor BOS */
    #define USBTMC_BOS_DESCR_LENGTH               (0u)
    #define USBTMC_BOS_DESCR_TYPE                 (1u)
    #define USBTMC_BOS_DESCR_TOTAL_LENGTH_LOW     (2u)
    #define USBTMC_BOS_DESCR_TOTAL_LENGTH_HI      (3u)
    #define USBTMC_BOS_DESCR_NUM_DEV_CAPS         (4u)
#endif /* (USBTMC_BOS_ENABLE) */

/* Feature Selectors (Table 9-6) */
#define USBTMC_DEVICE_REMOTE_WAKEUP           (0x01u)
#define USBTMC_ENDPOINT_HALT                  (0x00u)
#define USBTMC_TEST_MODE                      (0x02u)

/* USB Device Status (Figure 9-4) */
#define USBTMC_DEVICE_STATUS_BUS_POWERED      (0x00u)
#define USBTMC_DEVICE_STATUS_SELF_POWERED     (0x01u)
#define USBTMC_DEVICE_STATUS_REMOTE_WAKEUP    (0x02u)

/* USB Endpoint Status (Figure 9-4) */
#define USBTMC_ENDPOINT_STATUS_HALT           (0x01u)

/* USB Endpoint Directions */
#define USBTMC_DIR_IN                         (0x80u)
#define USBTMC_DIR_OUT                        (0x00u)
#define USBTMC_DIR_UNUSED                     (0x7Fu)

/* USB Endpoint Attributes */
#define USBTMC_EP_TYPE_CTRL                   (0x00u)
#define USBTMC_EP_TYPE_ISOC                   (0x01u)
#define USBTMC_EP_TYPE_BULK                   (0x02u)
#define USBTMC_EP_TYPE_INT                    (0x03u)
#define USBTMC_EP_TYPE_MASK                   (0x03u)

#define USBTMC_EP_SYNC_TYPE_NO_SYNC           (0x00u)
#define USBTMC_EP_SYNC_TYPE_ASYNC             (0x04u)
#define USBTMC_EP_SYNC_TYPE_ADAPTIVE          (0x08u)
#define USBTMC_EP_SYNC_TYPE_SYNCHRONOUS       (0x0Cu)
#define USBTMC_EP_SYNC_TYPE_MASK              (0x0Cu)

#define USBTMC_EP_USAGE_TYPE_DATA             (0x00u)
#define USBTMC_EP_USAGE_TYPE_FEEDBACK         (0x10u)
#define USBTMC_EP_USAGE_TYPE_IMPLICIT         (0x20u)
#define USBTMC_EP_USAGE_TYPE_RESERVED         (0x30u)
#define USBTMC_EP_USAGE_TYPE_MASK             (0x30u)

/* Point Status defines */
#define USBTMC_EP_STATUS_LENGTH               (0x02u)

/* Point Device defines */
#define USBTMC_DEVICE_STATUS_LENGTH           (0x02u)

#define USBTMC_STATUS_LENGTH_MAX \
                 ((USBTMC_EP_STATUS_LENGTH > USBTMC_DEVICE_STATUS_LENGTH) ? \
                        USBTMC_EP_STATUS_LENGTH : USBTMC_DEVICE_STATUS_LENGTH)

/* Transfer Completion Notification */
#define USBTMC_XFER_IDLE                      (0x00u)
#define USBTMC_XFER_STATUS_ACK                (0x01u)
#define USBTMC_XFER_PREMATURE                 (0x02u)
#define USBTMC_XFER_ERROR                     (0x03u)

/* Driver State defines */
#define USBTMC_TRANS_STATE_IDLE               (0x00u)
#define USBTMC_TRANS_STATE_CONTROL_READ       (0x02u)
#define USBTMC_TRANS_STATE_CONTROL_WRITE      (0x04u)
#define USBTMC_TRANS_STATE_NO_DATA_CONTROL    (0x06u)

/* String Descriptor defines */
#define USBTMC_STRING_MSOS                    (0xEEu)
#define USBTMC_MSOS_DESCRIPTOR_LENGTH         (18u)
#define USBTMC_MSOS_CONF_DESCR_LENGTH         (40u)

/* Return values */
#define USBTMC_BCD_PORT_SDP       (1u) /* Standard downstream port detected */
#define USBTMC_BCD_PORT_CDP       (2u) /* Charging downstream port detected */
#define USBTMC_BCD_PORT_DCP       (3u) /* Dedicated charging port detected */
#define USBTMC_BCD_PORT_UNKNOWN   (0u) /* Unable to detect charging port */
#define USBTMC_BCD_PORT_ERR       (4u) /* Error condition in detection process*/


/* Timeouts for BCD */
#define USBTMC_BCD_TIMEOUT                (400u)  /* Copied from PBK#163 TIMEOUT (300 ms) */
#define USBTMC_BCD_TDCD_DBNC              (10u)  /*BCD v1.2: DCD debounce time 10 ms*/
#define USBTMC_BCD_PRIMARY_WAIT           (40u)   /* Copied from PBK#163 TIMEOUT (40 ms)  */
#define USBTMC_BCD_SECONDARY_WAIT         (47u)   /* Copied from PBK#163 TIMEOUT (40 ms)  */
#define USBTMC_BCD_SUSPEND_DISABLE_WAIT   (2u)    /* Copied from PBK#163 TIMEOUT (2 us)   */

/* Wait cycles required before clearing SUSPEND_DEL in POWER_CTRL: 2us */
#define USBTMC_WAIT_SUSPEND_DEL_DISABLE   (2u)

/* Wait cycles required for USB regulator stabilization after it is enabled : 50ns */
#define USBTMC_WAIT_VREF_STABILITY        (0u)

#if (CY_PSOC3 || CY_PSOC5LP)
/* Wait cycles required for USB reference restore: 40us */
#define USBTMC_WAIT_VREF_RESTORE          (40u)

/* Wait cycles required for stabilization after register is written : 50ns */
#define USBTMC_WAIT_REG_STABILITY_50NS    (0u)
#define USBTMC_WAIT_REG_STABILITY_1US     (1u)

/* Wait cycles required after CR0 register write: 1 cycle */
#define USBTMC_WAIT_CR0_REG_STABILITY     (1u)

/* Wait cycles required after PD_PULLUP_N bit is set in PM_USB_CR0: 2us */
#define USBTMC_WAIT_PD_PULLUP_N_ENABLE    (2u)
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

#if (CY_PSOC4)
    #if (USBTMC_EP_MANAGEMENT_DMA)
        #define USBTMC_DMA_DESCR0         (0u)
        #define USBTMC_DMA_DESCR1         (1u)
    #endif /* (USBTMC_EP_MANAGEMENT_DMA) */

    #if (USBTMC_EP_MANAGEMENT_DMA_AUTO)
        /* BUF_SIZE-BYTES_PER_BURST examples: 0x55 - 32 bytes, 0x44 - 16 bytes, 0x33 - 8 bytes, etc. */
        #define USBTMC_DMA_BUF_SIZE             (0x55u)
        #define USBTMC_DMA_BYTES_PER_BURST      (32u)
        #define USBTMC_DMA_HALFWORDS_PER_BURST  (16u)
        #define USBTMC_DMA_BURST_BYTES_MASK     (USBTMC_DMA_BYTES_PER_BURST - 1u)

        #define USBTMC_DMA_DESCR0_MASK        (0x00u)
        #define USBTMC_DMA_DESCR1_MASK        (0x80u)
        #define USBTMC_DMA_DESCR_REVERT       (0x40u)
        #define USBTMC_DMA_DESCR_16BITS       (0x20u)
        #define USBTMC_DMA_DESCR_SHIFT        (7u)

        #define USBTMC_DMA_GET_DESCR_NUM(desrc)
        #define USBTMC_DMA_GET_BURST_CNT(dmaBurstCnt) \
                    (((dmaBurstCnt) > 2u)? ((dmaBurstCnt) - 2u) : 0u)

        #define USBTMC_DMA_GET_MAX_ELEM_PER_BURST(dmaLastBurstEl) \
                    ((0u != ((dmaLastBurstEl) & USBTMC_DMA_DESCR_16BITS)) ? \
                                (USBTMC_DMA_HALFWORDS_PER_BURST - 1u) : (USBTMC_DMA_BYTES_PER_BURST - 1u))
    #endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */
#else
    #if (USBTMC_EP_MANAGEMENT_DMA_MANUAL)
        #define USBTMC_DMA_BYTES_PER_BURST    (0u)
        #define USBTMC_DMA_REQUEST_PER_BURST  (0u)
    #endif /* (USBTMC_EP_MANAGEMENT_DMA_MANUAL) */

    #if (USBTMC_EP_MANAGEMENT_DMA_AUTO)
        #define USBTMC_DMA_BYTES_PER_BURST    (32u)
        #define USBTMC_DMA_BYTES_REPEAT       (2u)

        /* BUF_SIZE-BYTES_PER_BURST examples: 0x55 - 32 bytes, 0x44 - 16 bytes, 0x33 - 8 bytes, etc. */
        #define USBTMC_DMA_BUF_SIZE           (0x55u)
        #define USBTMC_DMA_REQUEST_PER_BURST  (1u)

        #define USBTMC_EP17_SR_MASK           (0x7Fu)
        #define USBTMC_EP8_SR_MASK            (0x03u)
    #endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */
#endif /* (CY_PSOC4) */

/* DIE ID string descriptor defines */
#if defined(USBTMC_ENABLE_IDSN_STRING)
    #define USBTMC_IDSN_DESCR_LENGTH      (0x22u)
#endif /* (USBTMC_ENABLE_IDSN_STRING) */


/***************************************
*     Vars with External Linkage
***************************************/

/**
* \addtogroup group_globals
* @{
*/
extern uint8 USBTMC_initVar;
extern volatile uint8 USBTMC_device;
extern volatile uint8 USBTMC_transferState;
extern volatile uint8 USBTMC_configuration;
extern volatile uint8 USBTMC_configurationChanged;
extern volatile uint8 USBTMC_deviceStatus;
/** @} globals */

/**
* \addtogroup group_hid
* @{
*/
/* HID Variables */
#if defined(USBTMC_ENABLE_HID_CLASS)
    extern volatile uint8 USBTMC_hidProtocol [USBTMC_MAX_INTERFACES_NUMBER]; 
    extern volatile uint8 USBTMC_hidIdleRate [USBTMC_MAX_INTERFACES_NUMBER];
    extern volatile uint8 USBTMC_hidIdleTimer[USBTMC_MAX_INTERFACES_NUMBER];
#endif /* (USBTMC_ENABLE_HID_CLASS) */
/** @} hid */


/***************************************
*              Registers
***************************************/

/* Common registers for all PSoCs: 3/4/5LP */
#define USBTMC_ARB_CFG_PTR        ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_CFG)
#define USBTMC_ARB_CFG_REG        (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_CFG)

#define USBTMC_ARB_EP1_CFG_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP1_CFG)
#define USBTMC_ARB_EP1_CFG_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP1_CFG)
#define USBTMC_ARB_EP1_INT_EN_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP1_INT_EN)
#define USBTMC_ARB_EP1_INT_EN_REG (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP1_INT_EN)
#define USBTMC_ARB_EP1_SR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP1_SR)
#define USBTMC_ARB_EP1_SR_REG     (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP1_SR)
#define USBTMC_ARB_EP1_CFG_IND    USBTMC_cy_m0s8_usb__ARB_EP1_CFG
#define USBTMC_ARB_EP1_INT_EN_IND USBTMC_cy_m0s8_usb__ARB_EP1_INT_EN
#define USBTMC_ARB_EP1_SR_IND     USBTMC_cy_m0s8_usb__ARB_EP1_SR
#define USBTMC_ARB_EP_BASE        (*(volatile USBTMC_arb_eps_struct CYXDATA *) \
                                            (USBTMC_cy_m0s8_usb__ARB_EP1_CFG - sizeof(USBTMC_arb_ep_struct)))

#define USBTMC_ARB_EP2_CFG_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP2_CFG)
#define USBTMC_ARB_EP2_CFG_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP2_CFG)
#define USBTMC_ARB_EP2_INT_EN_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP2_INT_EN)
#define USBTMC_ARB_EP2_INT_EN_REG (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP2_INT_EN)
#define USBTMC_ARB_EP2_SR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP2_SR)
#define USBTMC_ARB_EP2_SR_REG     (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP2_SR)

#define USBTMC_ARB_EP3_CFG_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP3_CFG)
#define USBTMC_ARB_EP3_CFG_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP3_CFG)
#define USBTMC_ARB_EP3_INT_EN_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP3_INT_EN)
#define USBTMC_ARB_EP3_INT_EN_REG (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP3_INT_EN)
#define USBTMC_ARB_EP3_SR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP3_SR)
#define USBTMC_ARB_EP3_SR_REG     (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP3_SR)

#define USBTMC_ARB_EP4_CFG_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP4_CFG)
#define USBTMC_ARB_EP4_CFG_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP4_CFG)
#define USBTMC_ARB_EP4_INT_EN_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP4_INT_EN)
#define USBTMC_ARB_EP4_INT_EN_REG (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP4_INT_EN)
#define USBTMC_ARB_EP4_SR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP4_SR)
#define USBTMC_ARB_EP4_SR_REG     (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP4_SR)

#define USBTMC_ARB_EP5_CFG_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP5_CFG)
#define USBTMC_ARB_EP5_CFG_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP5_CFG)
#define USBTMC_ARB_EP5_INT_EN_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP5_INT_EN)
#define USBTMC_ARB_EP5_INT_EN_REG (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP5_INT_EN)
#define USBTMC_ARB_EP5_SR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP5_SR)
#define USBTMC_ARB_EP5_SR_REG     (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP5_SR)

#define USBTMC_ARB_EP6_CFG_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP6_CFG)
#define USBTMC_ARB_EP6_CFG_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP6_CFG)
#define USBTMC_ARB_EP6_INT_EN_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP6_INT_EN)
#define USBTMC_ARB_EP6_INT_EN_REG (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP6_INT_EN)
#define USBTMC_ARB_EP6_SR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP6_SR)
#define USBTMC_ARB_EP6_SR_REG     (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP6_SR)

#define USBTMC_ARB_EP7_CFG_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP7_CFG)
#define USBTMC_ARB_EP7_CFG_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP7_CFG)
#define USBTMC_ARB_EP7_INT_EN_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP7_INT_EN)
#define USBTMC_ARB_EP7_INT_EN_REG (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP7_INT_EN)
#define USBTMC_ARB_EP7_SR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP7_SR)
#define USBTMC_ARB_EP7_SR_REG     (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP7_SR)

#define USBTMC_ARB_EP8_CFG_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP8_CFG)
#define USBTMC_ARB_EP8_CFG_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP8_CFG)
#define USBTMC_ARB_EP8_INT_EN_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP8_INT_EN)
#define USBTMC_ARB_EP8_INT_EN_REG (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP8_INT_EN)
#define USBTMC_ARB_EP8_SR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_EP8_SR)
#define USBTMC_ARB_EP8_SR_REG     (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_EP8_SR)

#define USBTMC_ARB_INT_EN_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_INT_EN)
#define USBTMC_ARB_INT_EN_REG     (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_INT_EN)
#define USBTMC_ARB_INT_SR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_INT_SR)
#define USBTMC_ARB_INT_SR_REG     (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_INT_SR)

#define USBTMC_ARB_RW1_DR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW1_DR)
#define USBTMC_ARB_RW1_RA_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW1_RA)

#define USBTMC_ARB_RW1_RA_MSB_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW1_RA_MSB)
#define USBTMC_ARB_RW1_WA_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW1_WA)
#define USBTMC_ARB_RW1_WA_MSB_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW1_WA_MSB)
#define USBTMC_ARB_RW1_DR_IND     USBTMC_cy_m0s8_usb__ARB_RW1_DR
#define USBTMC_ARB_RW1_RA_IND     USBTMC_cy_m0s8_usb__ARB_RW1_RA
#define USBTMC_ARB_RW1_RA_MSB_IND USBTMC_cy_m0s8_usb__ARB_RW1_RA_MSB
#define USBTMC_ARB_RW1_WA_IND     USBTMC_cy_m0s8_usb__ARB_RW1_WA
#define USBTMC_ARB_RW1_WA_MSB_IND USBTMC_cy_m0s8_usb__ARB_RW1_WA_MSB

#define USBTMC_ARB_RW2_DR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW2_DR)
#define USBTMC_ARB_RW2_RA_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW2_RA)
#define USBTMC_ARB_RW2_RA_MSB_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW2_RA_MSB)
#define USBTMC_ARB_RW2_WA_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW2_WA)
#define USBTMC_ARB_RW2_WA_MSB_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW2_WA_MSB)

#define USBTMC_ARB_RW3_DR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW3_DR)
#define USBTMC_ARB_RW3_RA_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW3_RA)
#define USBTMC_ARB_RW3_RA_MSB_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW3_RA_MSB)
#define USBTMC_ARB_RW3_WA_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW3_WA)
#define USBTMC_ARB_RW3_WA_MSB_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW3_WA_MSB)

#define USBTMC_ARB_RW4_DR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW4_DR)
#define USBTMC_ARB_RW4_RA_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW4_RA)
#define USBTMC_ARB_RW4_RA_MSB_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW4_RA_MSB)
#define USBTMC_ARB_RW4_WA_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW4_WA)
#define USBTMC_ARB_RW4_WA_MSB_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW4_WA_MSB)

#define USBTMC_ARB_RW5_DR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW5_DR)
#define USBTMC_ARB_RW5_RA_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW5_RA)
#define USBTMC_ARB_RW5_RA_MSB_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW5_RA_MSB)
#define USBTMC_ARB_RW5_WA_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW5_WA)
#define USBTMC_ARB_RW5_WA_MSB_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW5_WA_MSB)

#define USBTMC_ARB_RW6_DR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW6_DR)
#define USBTMC_ARB_RW6_RA_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW6_RA)
#define USBTMC_ARB_RW6_RA_MSB_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW6_RA_MSB)
#define USBTMC_ARB_RW6_WA_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW6_WA)
#define USBTMC_ARB_RW6_WA_MSB_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW6_WA_MSB)

#define USBTMC_ARB_RW7_DR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW7_DR)
#define USBTMC_ARB_RW7_RA_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW7_RA)
#define USBTMC_ARB_RW7_RA_MSB_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW7_RA_MSB)
#define USBTMC_ARB_RW7_WA_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW7_WA)
#define USBTMC_ARB_RW7_WA_MSB_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW7_WA_MSB)

#define USBTMC_ARB_RW8_DR_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW8_DR)
#define USBTMC_ARB_RW8_RA_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW8_RA)
#define USBTMC_ARB_RW8_RA_MSB_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW8_RA_MSB)
#define USBTMC_ARB_RW8_WA_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW8_WA)
#define USBTMC_ARB_RW8_WA_MSB_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW8_WA_MSB)

#define USBTMC_BUF_SIZE_PTR       ( (reg32 *) USBTMC_cy_m0s8_usb__BUF_SIZE)
#define USBTMC_BUF_SIZE_REG       (*(reg32 *) USBTMC_cy_m0s8_usb__BUF_SIZE)
#define USBTMC_BUS_RST_CNT_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__BUS_RST_CNT)
#define USBTMC_BUS_RST_CNT_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__BUS_RST_CNT)
#define USBTMC_CWA_PTR            ( (reg32 *) USBTMC_cy_m0s8_usb__CWA)
#define USBTMC_CWA_REG            (*(reg32 *) USBTMC_cy_m0s8_usb__CWA)
#define USBTMC_CWA_MSB_PTR        ( (reg32 *) USBTMC_cy_m0s8_usb__CWA_MSB)
#define USBTMC_CWA_MSB_REG        (*(reg32 *) USBTMC_cy_m0s8_usb__CWA_MSB)
#define USBTMC_CR0_PTR            ( (reg32 *) USBTMC_cy_m0s8_usb__CR0)
#define USBTMC_CR0_REG            (*(reg32 *) USBTMC_cy_m0s8_usb__CR0)
#define USBTMC_CR1_PTR            ( (reg32 *) USBTMC_cy_m0s8_usb__CR1)
#define USBTMC_CR1_REG            (*(reg32 *) USBTMC_cy_m0s8_usb__CR1)

#define USBTMC_DMA_THRES_PTR      ( (reg32 *) USBTMC_cy_m0s8_usb__DMA_THRES)
#define USBTMC_DMA_THRES_REG      (*(reg32 *) USBTMC_cy_m0s8_usb__DMA_THRES)
#define USBTMC_DMA_THRES_MSB_PTR  ( (reg32 *) USBTMC_cy_m0s8_usb__DMA_THRES_MSB)
#define USBTMC_DMA_THRES_MSB_REG  (*(reg32 *) USBTMC_cy_m0s8_usb__DMA_THRES_MSB)

#define USBTMC_EP_ACTIVE_PTR      ( (reg32 *) USBTMC_cy_m0s8_usb__EP_ACTIVE)
#define USBTMC_EP_ACTIVE_REG      (*(reg32 *) USBTMC_cy_m0s8_usb__EP_ACTIVE)
#define USBTMC_EP_TYPE_PTR        ( (reg32 *) USBTMC_cy_m0s8_usb__EP_TYPE)
#define USBTMC_EP_TYPE_REG        (*(reg32 *) USBTMC_cy_m0s8_usb__EP_TYPE)

#define USBTMC_EP0_CNT_PTR        ( (reg32 *) USBTMC_cy_m0s8_usb__EP0_CNT)
#define USBTMC_EP0_CNT_REG        (*(reg32 *) USBTMC_cy_m0s8_usb__EP0_CNT)
#define USBTMC_EP0_CR_PTR         ( (reg32 *) USBTMC_cy_m0s8_usb__EP0_CR)
#define USBTMC_EP0_CR_REG         (*(reg32 *) USBTMC_cy_m0s8_usb__EP0_CR)
#define USBTMC_EP0_DR0_PTR        ( (reg32 *) USBTMC_cy_m0s8_usb__EP0_DR0)
#define USBTMC_EP0_DR0_REG        (*(reg32 *) USBTMC_cy_m0s8_usb__EP0_DR0)
#define USBTMC_EP0_DR1_PTR        ( (reg32 *) USBTMC_cy_m0s8_usb__EP0_DR1)
#define USBTMC_EP0_DR1_REG        (*(reg32 *) USBTMC_cy_m0s8_usb__EP0_DR1)
#define USBTMC_EP0_DR2_PTR        ( (reg32 *) USBTMC_cy_m0s8_usb__EP0_DR2)
#define USBTMC_EP0_DR2_REG        (*(reg32 *) USBTMC_cy_m0s8_usb__EP0_DR2)
#define USBTMC_EP0_DR3_PTR        ( (reg32 *) USBTMC_cy_m0s8_usb__EP0_DR3)
#define USBTMC_EP0_DR3_REG        (*(reg32 *) USBTMC_cy_m0s8_usb__EP0_DR3)
#define USBTMC_EP0_DR4_PTR        ( (reg32 *) USBTMC_cy_m0s8_usb__EP0_DR4)
#define USBTMC_EP0_DR4_REG        (*(reg32 *) USBTMC_cy_m0s8_usb__EP0_DR4)
#define USBTMC_EP0_DR5_PTR        ( (reg32 *) USBTMC_cy_m0s8_usb__EP0_DR5)
#define USBTMC_EP0_DR5_REG        (*(reg32 *) USBTMC_cy_m0s8_usb__EP0_DR5)
#define USBTMC_EP0_DR6_PTR        ( (reg32 *) USBTMC_cy_m0s8_usb__EP0_DR6)
#define USBTMC_EP0_DR6_REG        (*(reg32 *) USBTMC_cy_m0s8_usb__EP0_DR6)
#define USBTMC_EP0_DR7_PTR        ( (reg32 *) USBTMC_cy_m0s8_usb__EP0_DR7)
#define USBTMC_EP0_DR7_REG        (*(reg32 *) USBTMC_cy_m0s8_usb__EP0_DR7)
#define USBTMC_EP0_DR0_IND        USBTMC_cy_m0s8_usb__EP0_DR0
#define USBTMC_EP0_DR_BASE        (*(volatile USBTMC_ep0_data_struct CYXDATA *) USBTMC_cy_m0s8_usb__EP0_DR0)

#define USBTMC_OSCLK_DR0_PTR      ( (reg32 *) USBTMC_cy_m0s8_usb__OSCLK_DR0)
#define USBTMC_OSCLK_DR0_REG      (*(reg32 *) USBTMC_cy_m0s8_usb__OSCLK_DR0)
#define USBTMC_OSCLK_DR1_PTR      ( (reg32 *) USBTMC_cy_m0s8_usb__OSCLK_DR1)
#define USBTMC_OSCLK_DR1_REG      (*(reg32 *) USBTMC_cy_m0s8_usb__OSCLK_DR1)

#define USBTMC_SIE_EP_INT_EN_PTR  ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP_INT_EN)
#define USBTMC_SIE_EP_INT_EN_REG  (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP_INT_EN)
#define USBTMC_SIE_EP_INT_SR_PTR  ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP_INT_SR)
#define USBTMC_SIE_EP_INT_SR_REG  (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP_INT_SR)

#define USBTMC_SIE_EP1_CNT0_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP1_CNT0)
#define USBTMC_SIE_EP1_CNT0_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP1_CNT0)
#define USBTMC_SIE_EP1_CNT1_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP1_CNT1)
#define USBTMC_SIE_EP1_CNT1_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP1_CNT1)
#define USBTMC_SIE_EP1_CR0_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP1_CR0)
#define USBTMC_SIE_EP1_CR0_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP1_CR0)
#define USBTMC_SIE_EP1_CNT1_IND   USBTMC_cy_m0s8_usb__SIE_EP1_CNT1
#define USBTMC_SIE_EP1_CNT0_IND   USBTMC_cy_m0s8_usb__SIE_EP1_CNT0
#define USBTMC_SIE_EP1_CR0_IND    USBTMC_cy_m0s8_usb__SIE_EP1_CR0
#define USBTMC_SIE_EP_BASE        (*(volatile USBTMC_sie_eps_struct CYXDATA *) \
                                            (USBTMC_cy_m0s8_usb__SIE_EP1_CNT0 - sizeof(USBTMC_sie_ep_struct)))

#define USBTMC_SIE_EP2_CNT0_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP2_CNT0)
#define USBTMC_SIE_EP2_CNT0_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP2_CNT0)
#define USBTMC_SIE_EP2_CNT1_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP2_CNT1)
#define USBTMC_SIE_EP2_CNT1_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP2_CNT1)
#define USBTMC_SIE_EP2_CR0_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP2_CR0)
#define USBTMC_SIE_EP2_CR0_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP2_CR0)

#define USBTMC_SIE_EP3_CNT0_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP3_CNT0)
#define USBTMC_SIE_EP3_CNT0_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP3_CNT0)
#define USBTMC_SIE_EP3_CNT1_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP3_CNT1)
#define USBTMC_SIE_EP3_CNT1_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP3_CNT1)
#define USBTMC_SIE_EP3_CR0_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP3_CR0)
#define USBTMC_SIE_EP3_CR0_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP3_CR0)

#define USBTMC_SIE_EP4_CNT0_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP4_CNT0)
#define USBTMC_SIE_EP4_CNT0_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP4_CNT0)
#define USBTMC_SIE_EP4_CNT1_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP4_CNT1)
#define USBTMC_SIE_EP4_CNT1_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP4_CNT1)
#define USBTMC_SIE_EP4_CR0_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP4_CR0)
#define USBTMC_SIE_EP4_CR0_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP4_CR0)

#define USBTMC_SIE_EP5_CNT0_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP5_CNT0)
#define USBTMC_SIE_EP5_CNT0_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP5_CNT0)
#define USBTMC_SIE_EP5_CNT1_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP5_CNT1)
#define USBTMC_SIE_EP5_CNT1_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP5_CNT1)
#define USBTMC_SIE_EP5_CR0_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP5_CR0)
#define USBTMC_SIE_EP5_CR0_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP5_CR0)

#define USBTMC_SIE_EP6_CNT0_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP6_CNT0)
#define USBTMC_SIE_EP6_CNT0_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP6_CNT0)
#define USBTMC_SIE_EP6_CNT1_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP6_CNT1)
#define USBTMC_SIE_EP6_CNT1_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP6_CNT1)
#define USBTMC_SIE_EP6_CR0_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP6_CR0)
#define USBTMC_SIE_EP6_CR0_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP6_CR0)

#define USBTMC_SIE_EP7_CNT0_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP7_CNT0)
#define USBTMC_SIE_EP7_CNT0_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP7_CNT0)
#define USBTMC_SIE_EP7_CNT1_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP7_CNT1)
#define USBTMC_SIE_EP7_CNT1_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP7_CNT1)
#define USBTMC_SIE_EP7_CR0_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP7_CR0)
#define USBTMC_SIE_EP7_CR0_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP7_CR0)

#define USBTMC_SIE_EP8_CNT0_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP8_CNT0)
#define USBTMC_SIE_EP8_CNT0_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP8_CNT0)
#define USBTMC_SIE_EP8_CNT1_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP8_CNT1)
#define USBTMC_SIE_EP8_CNT1_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP8_CNT1)
#define USBTMC_SIE_EP8_CR0_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__SIE_EP8_CR0)
#define USBTMC_SIE_EP8_CR0_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__SIE_EP8_CR0)

#define USBTMC_SOF0_PTR           ( (reg32 *) USBTMC_cy_m0s8_usb__SOF0)
#define USBTMC_SOF0_REG           (*(reg32 *) USBTMC_cy_m0s8_usb__SOF0)
#define USBTMC_SOF1_PTR           ( (reg32 *) USBTMC_cy_m0s8_usb__SOF1)
#define USBTMC_SOF1_REG           (*(reg32 *) USBTMC_cy_m0s8_usb__SOF1)

#define USBTMC_USB_CLK_EN_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__USB_CLK_EN)
#define USBTMC_USB_CLK_EN_REG     (*(reg32 *) USBTMC_cy_m0s8_usb__USB_CLK_EN)

#define USBTMC_USBIO_CR0_PTR      ( (reg32 *) USBTMC_cy_m0s8_usb__USBIO_CR0)
#define USBTMC_USBIO_CR0_REG      (*(reg32 *) USBTMC_cy_m0s8_usb__USBIO_CR0)
#define USBTMC_USBIO_CR1_PTR      ( (reg32 *) USBTMC_cy_m0s8_usb__USBIO_CR1)
#define USBTMC_USBIO_CR1_REG      (*(reg32 *) USBTMC_cy_m0s8_usb__USBIO_CR1)

#define USBTMC_DYN_RECONFIG_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__DYN_RECONFIG)
#define USBTMC_DYN_RECONFIG_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__DYN_RECONFIG)

#if (CY_PSOC4)
    #define USBTMC_ARB_RW1_RA16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW1_RA16)
    #define USBTMC_ARB_RW1_RA16_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_RW1_RA16)
    #define USBTMC_ARB_RW1_WA16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW1_WA16)
    #define USBTMC_ARB_RW1_WA16_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__ARB_RW1_WA16)
    #define USBTMC_ARB_RW1_DR16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW1_DR16)
    #define USBTMC_ARB_RW1_DR16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW1_DR16)
    #define USBTMC_ARB_EP16_BASE      (*(volatile USBTMC_arb_eps_reg16_struct CYXDATA *) \
                                                (USBTMC_cy_m0s8_usb__ARB_RW1_WA16 - sizeof(USBTMC_arb_ep_reg16_struct)))

    #define USBTMC_ARB_RW2_DR16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW2_DR16)
    #define USBTMC_ARB_RW2_RA16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW2_RA16)
    #define USBTMC_ARB_RW2_WA16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW2_WA16)

    #define USBTMC_ARB_RW3_DR16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW3_DR16)
    #define USBTMC_ARB_RW3_RA16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW3_RA16)
    #define USBTMC_ARB_RW3_WA16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW3_WA16)

    #define USBTMC_ARB_RW4_DR16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW4_DR16)
    #define USBTMC_ARB_RW4_RA16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW4_RA16)
    #define USBTMC_ARB_RW4_WA16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW4_WA16)

    #define USBTMC_ARB_RW5_DR16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW5_DR16)
    #define USBTMC_ARB_RW5_RA16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW5_RA16)
    #define USBTMC_ARB_RW5_WA16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW5_WA16)

    #define USBTMC_ARB_RW6_DR16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW6_DR16)
    #define USBTMC_ARB_RW6_RA16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW6_RA16)
    #define USBTMC_ARB_RW6_WA16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW6_WA16)

    #define USBTMC_ARB_RW7_DR16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW7_DR16)
    #define USBTMC_ARB_RW7_RA16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW7_RA16)
    #define USBTMC_ARB_RW7_WA16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW7_WA16)

    #define USBTMC_ARB_RW8_DR16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW8_DR16)
    #define USBTMC_ARB_RW8_RA16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW8_RA16)
    #define USBTMC_ARB_RW8_WA16_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__ARB_RW8_WA16)

    #define USBTMC_OSCLK_DR16_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__OSCLK_DR16)
    #define USBTMC_OSCLK_DR16_REG     (*(reg32 *) USBTMC_cy_m0s8_usb__OSCLK_DR16)

    #define USBTMC_SOF16_PTR          ( (reg32 *) USBTMC_cy_m0s8_usb__SOF16)
    #define USBTMC_SOF16_REG          (*(reg32 *) USBTMC_cy_m0s8_usb__SOF16)
    
    #define USBTMC_CWA16_PTR          ( (reg32 *) USBTMC_cy_m0s8_usb__CWA16)
    #define USBTMC_CWA16_REG          (*(reg32 *) USBTMC_cy_m0s8_usb__CWA16)

    #define USBTMC_DMA_THRES16_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__DMA_THRES16)
    #define USBTMC_DMA_THRES16_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__DMA_THRES16)

    #define USBTMC_USB_CLK_EN_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__USB_CLK_EN)
    #define USBTMC_USB_CLK_EN_REG     (*(reg32 *) USBTMC_cy_m0s8_usb__USB_CLK_EN)

    #define USBTMC_USBIO_CR2_PTR      ( (reg32 *) USBTMC_cy_m0s8_usb__USBIO_CR2)
    #define USBTMC_USBIO_CR2_REG      (*(reg32 *) USBTMC_cy_m0s8_usb__USBIO_CR2)

    #define USBTMC_USB_MEM            ( (reg32 *) USBTMC_cy_m0s8_usb__MEM_DATA0)

    #define USBTMC_POWER_CTRL_REG      (*(reg32 *) USBTMC_cy_m0s8_usb__USB_POWER_CTRL)
    #define USBTMC_POWER_CTRL_PTR      ( (reg32 *) USBTMC_cy_m0s8_usb__USB_POWER_CTRL)

    #define USBTMC_CHGDET_CTRL_REG     (*(reg32 *) USBTMC_cy_m0s8_usb__USB_CHGDET_CTRL)
    #define USBTMC_CHGDET_CTRL_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__USB_CHGDET_CTRL)

    #define USBTMC_USBIO_CTRL_REG      (*(reg32 *) USBTMC_cy_m0s8_usb__USB_USBIO_CTRL)
    #define USBTMC_USBIO_CTRL_PTR      ( (reg32 *) USBTMC_cy_m0s8_usb__USB_USBIO_CTRL)

    #define USBTMC_FLOW_CTRL_REG       (*(reg32 *) USBTMC_cy_m0s8_usb__USB_FLOW_CTRL)
    #define USBTMC_FLOW_CTRL_PTR       ( (reg32 *) USBTMC_cy_m0s8_usb__USB_FLOW_CTRL)

    #define USBTMC_LPM_CTRL_REG        (*(reg32 *) USBTMC_cy_m0s8_usb__USB_LPM_CTRL)
    #define USBTMC_LPM_CTRL_PTR        ( (reg32 *) USBTMC_cy_m0s8_usb__USB_LPM_CTRL)

    #define USBTMC_LPM_STAT_REG        (*(reg32 *) USBTMC_cy_m0s8_usb__USB_LPM_STAT)
    #define USBTMC_LPM_STAT_PTR        ( (reg32 *) USBTMC_cy_m0s8_usb__USB_LPM_STAT)

    #define USBTMC_PHY_CONTROL_REG     (*(reg32 *) USBTMC_cy_m0s8_usb__USB_PHY_CONTROL)
    #define USBTMC_PHY_CONTROL_PTR     ( (reg32 *) USBTMC_cy_m0s8_usb__USB_PHY_CONTROL)

    #define USBTMC_INTR_SIE_REG        (*(reg32 *) USBTMC_cy_m0s8_usb__USB_INTR_SIE)
    #define USBTMC_INTR_SIE_PTR        ( (reg32 *) USBTMC_cy_m0s8_usb__USB_INTR_SIE)

    #define USBTMC_INTR_SIE_SET_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__USB_INTR_SIE_SET)
    #define USBTMC_INTR_SIE_SET_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__USB_INTR_SIE_SET)

    #define USBTMC_INTR_SIE_MASK_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__USB_INTR_SIE_MASK)
    #define USBTMC_INTR_SIE_MASK_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__USB_INTR_SIE_MASK)

    #define USBTMC_INTR_SIE_MASKED_REG (*(reg32 *) USBTMC_cy_m0s8_usb__USB_INTR_SIE_MASKED)
    #define USBTMC_INTR_SIE_MASKED_PTR ( (reg32 *) USBTMC_cy_m0s8_usb__USB_INTR_SIE_MASKED)

    #define USBTMC_INTR_LVL_SEL_REG    (*(reg32 *) USBTMC_cy_m0s8_usb__USB_INTR_LVL_SEL)
    #define USBTMC_INTR_LVL_SEL_PTR    ( (reg32 *) USBTMC_cy_m0s8_usb__USB_INTR_LVL_SEL)

    #define USBTMC_INTR_CAUSE_HI_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__USB_INTR_CAUSE_HI)
    #define USBTMC_INTR_CAUSE_HI_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__USB_INTR_CAUSE_HI)

    #define USBTMC_INTR_CAUSE_LO_REG   (*(reg32 *) USBTMC_cy_m0s8_usb__USB_INTR_CAUSE_LO)
    #define USBTMC_INTR_CAUSE_LO_PTR   ( (reg32 *) USBTMC_cy_m0s8_usb__USB_INTR_CAUSE_LO)

    #define USBTMC_INTR_CAUSE_MED_REG  (*(reg32 *) USBTMC_cy_m0s8_usb__USB_INTR_CAUSE_MED)
    #define USBTMC_INTR_CAUSE_MED_PTR  ( (reg32 *) USBTMC_cy_m0s8_usb__USB_INTR_CAUSE_MED)

    #define USBTMC_DFT_CTRL_REG        (*(reg32 *) USBTMC_cy_m0s8_usb__USB_DFT_CTRL)
    #define USBTMC_DFT_CTRL_PTR        ( (reg32 *) USBTMC_cy_m0s8_usb__USB_DFT_CTRL)

    #if (USBTMC_VBUS_MONITORING_ENABLE)
        #if (USBTMC_VBUS_POWER_PAD_ENABLE)
            /* Vbus power pad pin is hard wired to P13[2] */
            #define USBTMC_VBUS_STATUS_REG    (*(reg32 *) CYREG_GPIO_PRT13_PS)
            #define USBTMC_VBUS_STATUS_PTR    ( (reg32 *) CYREG_GPIO_PRT13_PS)
            #define USBTMC_VBUS_VALID         (0x04u)
        #else
            /* Vbus valid pin is hard wired to P0[0] */
            #define USBTMC_VBUS_STATUS_REG    (*(reg32 *) CYREG_GPIO_PRT0_PS)
            #define USBTMC_VBUS_STATUS_PTR    ( (reg32 *) CYREG_GPIO_PRT0_PS)
            #define USBTMC_VBUS_VALID         (0x01u)
        #endif
    #endif /*(USBTMC_VBUS_MONITORING_ENABLE) */

    #define USBTMC_BURSTEND_0_TR_OUTPUT    (USBTMC_cy_m0s8_usb__BURSTEND0_TR_OUTPUT)
    #define USBTMC_BURSTEND_1_TR_OUTPUT    (USBTMC_cy_m0s8_usb__BURSTEND1_TR_OUTPUT)
    #define USBTMC_BURSTEND_2_TR_OUTPUT    (USBTMC_cy_m0s8_usb__BURSTEND2_TR_OUTPUT)
    #define USBTMC_BURSTEND_3_TR_OUTPUT    (USBTMC_cy_m0s8_usb__BURSTEND3_TR_OUTPUT)
    #define USBTMC_BURSTEND_4_TR_OUTPUT    (USBTMC_cy_m0s8_usb__BURSTEND4_TR_OUTPUT)
    #define USBTMC_BURSTEND_5_TR_OUTPUT    (USBTMC_cy_m0s8_usb__BURSTEND5_TR_OUTPUT)
    #define USBTMC_BURSTEND_6_TR_OUTPUT    (USBTMC_cy_m0s8_usb__BURSTEND6_TR_OUTPUT)
    #define USBTMC_BURSTEND_7_TR_OUTPUT    (USBTMC_cy_m0s8_usb__BURSTEND7_TR_OUTPUT)
    
#else /* (CY_PSOC3 || CY_PSOC5LP) */

    /* USBTMC_PM_USB_CR0 */
    #define USBTMC_PM_USB_CR0_PTR     ( (reg8 *) CYREG_PM_USB_CR0)
    #define USBTMC_PM_USB_CR0_REG     (*(reg8 *) CYREG_PM_USB_CR0)

    /* USBTMC_PM_ACT/STBY_CFG */
    #define USBTMC_PM_ACT_CFG_PTR     ( (reg8 *) USBTMC_USB__PM_ACT_CFG)
    #define USBTMC_PM_ACT_CFG_REG     (*(reg8 *) USBTMC_USB__PM_ACT_CFG)
    #define USBTMC_PM_STBY_CFG_PTR    ( (reg8 *) USBTMC_USB__PM_STBY_CFG)
    #define USBTMC_PM_STBY_CFG_REG    (*(reg8 *) USBTMC_USB__PM_STBY_CFG)

    #if (!CY_PSOC5LP)
        #define USBTMC_USBIO_CR2_PTR  (  (reg8 *) USBTMC_USB__USBIO_CR2)
        #define USBTMC_USBIO_CR2_REG  (* (reg8 *) USBTMC_USB__USBIO_CR2)
    #endif /* (!CY_PSOC5LP) */

    /* USBTMC_USB_MEM - USB IP memory buffer */
    #define USBTMC_USB_MEM            ((reg8 *) CYDEV_USB_MEM_BASE)

    #if (USBTMC_VBUS_MONITORING_ENABLE)
        #if (USBTMC_VBUS_MONITORING_INTERNAL)
            #define USBTMC_VBUS_STATUS_REG    (*(reg8 *) USBTMC_VBUS__PS)
            #define USBTMC_VBUS_STATUS_PTR    ( (reg8 *) USBTMC_VBUS__PS)
            #define USBTMC_VBUS_VALID         (USBTMC_VBUS__MASK)
        #else
            #define USBTMC_VBUS_STATUS_REG    (*(reg8 *) USBTMC_Vbus_ps_sts_sts_reg__STATUS_REG)
            #define USBTMC_VBUS_STATUS_PTR    ( (reg8 *) USBTMC_Vbus_ps_sts_sts_reg__STATUS_REG)
            #define USBTMC_VBUS_VALID         (USBTMC_Vbus_ps_sts_sts_reg__MASK)
        #endif /* (USBTMC_VBUS_MONITORING_INTERNAL) */
    #endif /*(USBTMC_VBUS_MONITORING_ENABLE) */
#endif /* (CY_PSOC4) */


/***************************************
*       Interrupt source constants
***************************************/

#define USBTMC_DP_INTC_PRIORITY       USBTMC_dp_int__INTC_PRIOR_NUM
#define USBTMC_DP_INTC_VECT_NUM       USBTMC_dp_int__INTC_NUMBER

#if (CY_PSOC4)
    #define USBTMC_DMA_AUTO_INTR_PRIO (0u)
    
    #define USBTMC_INTR_HI_PRIORITY   USBTMC_high_int__INTC_PRIOR_NUM
    #define USBTMC_INTR_HI_VECT_NUM   USBTMC_high_int__INTC_NUMBER

    #define USBTMC_INTR_MED_PRIORITY  USBTMC_med_int__INTC_PRIOR_NUM
    #define USBTMC_INTR_MED_VECT_NUM  USBTMC_med_int__INTC_NUMBER

    #define USBTMC_INTR_LO_PRIORITY   USBTMC_lo_int__INTC_PRIOR_NUM
    #define USBTMC_INTR_LO_VECT_NUM   USBTMC_lo_int__INTC_NUMBER

    /* Interrupt sources in USBTMC_isrCallbacks[] table */
    #define USBTMC_SOF_INTR_NUM       (0u)
    #define USBTMC_BUS_RESET_INT_NUM  (1u)
    #define USBTMC_EP0_INTR_NUM       (2u)
    #define USBTMC_LPM_INTR_NUM       (3u)
    #define USBTMC_ARB_EP_INTR_NUM    (4u)
    #define USBTMC_EP1_INTR_NUM       (5u)
    #define USBTMC_EP2_INTR_NUM       (6u)
    #define USBTMC_EP3_INTR_NUM       (7u)
    #define USBTMC_EP4_INTR_NUM       (8u)
    #define USBTMC_EP5_INTR_NUM       (9u)
    #define USBTMC_EP6_INTR_NUM       (10u)
    #define USBTMC_EP7_INTR_NUM       (11u)
    #define USBTMC_EP8_INTR_NUM       (12u)

#else
    #define USBTMC_BUS_RESET_PRIOR    USBTMC_bus_reset__INTC_PRIOR_NUM
    #define USBTMC_BUS_RESET_MASK     USBTMC_bus_reset__INTC_MASK
    #define USBTMC_BUS_RESET_VECT_NUM USBTMC_bus_reset__INTC_NUMBER

    #define USBTMC_SOF_PRIOR          USBTMC_sof_int__INTC_PRIOR_NUM
    #define USBTMC_SOF_MASK           USBTMC_sof_int__INTC_MASK
    #define USBTMC_SOF_VECT_NUM       USBTMC_sof_int__INTC_NUMBER

    #define USBTMC_EP_0_PRIOR         USBTMC_ep_0__INTC_PRIOR_NUM
    #define USBTMC_EP_0_MASK          USBTMC_ep_0__INTC_MASK
    #define USBTMC_EP_0_VECT_NUM      USBTMC_ep_0__INTC_NUMBER

    #define USBTMC_EP_1_PRIOR         USBTMC_ep_1__INTC_PRIOR_NUM
    #define USBTMC_EP_1_MASK          USBTMC_ep_1__INTC_MASK
    #define USBTMC_EP_1_VECT_NUM      USBTMC_ep_1__INTC_NUMBER

    #define USBTMC_EP_2_PRIOR         USBTMC_ep_2__INTC_PRIOR_NUM
    #define USBTMC_EP_2_MASK          USBTMC_ep_2__INTC_MASK
    #define USBTMC_EP_2_VECT_NUM      USBTMC_ep_2__INTC_NUMBER

    #define USBTMC_EP_3_PRIOR         USBTMC_ep_3__INTC_PRIOR_NUM
    #define USBTMC_EP_3_MASK          USBTMC_ep_3__INTC_MASK
    #define USBTMC_EP_3_VECT_NUM      USBTMC_ep_3__INTC_NUMBER

    #define USBTMC_EP_4_PRIOR         USBTMC_ep_4__INTC_PRIOR_NUM
    #define USBTMC_EP_4_MASK          USBTMC_ep_4__INTC_MASK
    #define USBTMC_EP_4_VECT_NUM      USBTMC_ep_4__INTC_NUMBER

    #define USBTMC_EP_5_PRIOR         USBTMC_ep_5__INTC_PRIOR_NUM
    #define USBTMC_EP_5_MASK          USBTMC_ep_5__INTC_MASK
    #define USBTMC_EP_5_VECT_NUM      USBTMC_ep_5__INTC_NUMBER

    #define USBTMC_EP_6_PRIOR         USBTMC_ep_6__INTC_PRIOR_NUM
    #define USBTMC_EP_6_MASK          USBTMC_ep_6__INTC_MASK
    #define USBTMC_EP_6_VECT_NUM      USBTMC_ep_6__INTC_NUMBER

    #define USBTMC_EP_7_PRIOR         USBTMC_ep_7__INTC_PRIOR_NUM
    #define USBTMC_EP_7_MASK          USBTMC_ep_7__INTC_MASK
    #define USBTMC_EP_7_VECT_NUM      USBTMC_ep_7__INTC_NUMBER

    #define USBTMC_EP_8_PRIOR         USBTMC_ep_8__INTC_PRIOR_NUM
    #define USBTMC_EP_8_MASK          USBTMC_ep_8__INTC_MASK
    #define USBTMC_EP_8_VECT_NUM      USBTMC_ep_8__INTC_NUMBER

    /* Set ARB ISR priority 0 to be highest for all EPX ISRs. */
    #define USBTMC_ARB_PRIOR          (0u)
    #define USBTMC_ARB_MASK           USBTMC_arb_int__INTC_MASK
    #define USBTMC_ARB_VECT_NUM       USBTMC_arb_int__INTC_NUMBER
#endif /* (CY_PSOC4) */


/***************************************
*       Endpoint 0 offsets (Table 9-2)
***************************************/
#define USBTMC_bmRequestTypeReg      USBTMC_EP0_DR_BASE.epData[0u]
#define USBTMC_bRequestReg           USBTMC_EP0_DR_BASE.epData[1u]
#define USBTMC_wValueLoReg           USBTMC_EP0_DR_BASE.epData[2u]
#define USBTMC_wValueHiReg           USBTMC_EP0_DR_BASE.epData[3u]
#define USBTMC_wIndexLoReg           USBTMC_EP0_DR_BASE.epData[4u]
#define USBTMC_wIndexHiReg           USBTMC_EP0_DR_BASE.epData[5u]
#define USBTMC_wLengthLoReg          USBTMC_EP0_DR_BASE.epData[6u]
#define USBTMC_wLengthHiReg          USBTMC_EP0_DR_BASE.epData[7u]

/* Compatibility defines */
#define USBTMC_lengthLoReg           USBTMC_EP0_DR_BASE.epData[6u]
#define USBTMC_lengthHiReg           USBTMC_EP0_DR_BASE.epData[7u]


/***************************************
*       Register Constants
***************************************/

#define USBTMC_3500MV     (3500u)
#if (CY_PSOC4)
    #define USBTMC_VDDD_MV    (CYDEV_VBUS_MV)
#else
    #define USBTMC_VDDD_MV    (CYDEV_VDDD_MV)
#endif /* (CY_PSOC4) */


/* USBTMC_USB_CLK */
#define USBTMC_USB_CLK_CSR_CLK_EN_POS (0u)
#define USBTMC_USB_CLK_CSR_CLK_EN     ((uint32) ((uint32) 0x1u << USBTMC_USB_CLK_CSR_CLK_EN_POS))
#define USBTMC_USB_CLK_ENABLE         (USBTMC_USB_CLK_CSR_CLK_EN)

/* USBTMC_CR0 */
#define USBTMC_CR0_DEVICE_ADDRESS_POS     (0u)
#define USBTMC_CR0_ENABLE_POS             (7u)
#define USBTMC_CR0_DEVICE_ADDRESS_MASK    ((uint32) ((uint32) 0x7Fu << USBTMC_CR0_DEVICE_ADDRESS_POS))
#define USBTMC_CR0_ENABLE                 ((uint32) ((uint32) 0x01u << USBTMC_CR0_ENABLE_POS))


/* USBTMC_CR1 */
#define USBTMC_CR1_REG_ENABLE_POS         (0u)
#define USBTMC_CR1_ENABLE_LOCK_POS        (1u)
#define USBTMC_CR1_BUS_ACTIVITY_POS       (2u)
#define USBTMC_CR1_TRIM_OFFSET_MSB_POS    (3u)
#define USBTMC_CR1_REG_ENABLE             ((uint32) ((uint32) 0x1u << USBTMC_CR1_REG_ENABLE_POS))
#define USBTMC_CR1_ENABLE_LOCK            ((uint32) ((uint32) 0x1u << USBTMC_CR1_ENABLE_LOCK_POS))
#define USBTMC_CR1_BUS_ACTIVITY           ((uint32) ((uint32) 0x1u << USBTMC_CR1_BUS_ACTIVITY_POS))
#define USBTMC_CR1_TRIM_OFFSET_MSB        ((uint32) ((uint32) 0x1u << USBTMC_CR1_TRIM_OFFSET_MSB_POS))

/* USBTMC_EPX_CNT */
#define USBTMC_EP0_CNT_DATA_TOGGLE        (0x80u)
#define USBTMC_EPX_CNT_DATA_TOGGLE        (0x80u)
#define USBTMC_EPX_CNT0_MASK              (0x0Fu)
#define USBTMC_EPX_CNTX_MSB_MASK          (0x07u)
#define USBTMC_EPX_CNTX_ADDR_SHIFT        (0x04u)
#define USBTMC_EPX_CNTX_ADDR_OFFSET       (0x10u)
#define USBTMC_EPX_CNTX_CRC_COUNT         (0x02u)
#define USBTMC_EPX_DATA_BUF_MAX           (512u)

/* USBTMC_USBIO_CR0 */

#define USBTMC_USBIO_CR0_TEN              (0x80u)
#define USBTMC_USBIO_CR0_TSE0             (0x40u)
#define USBTMC_USBIO_CR0_TD               (0x20u)
#define USBTMC_USBIO_CR0_RD               (0x01u)

/* USBTMC_USBIO_CR1 */
#define USBTMC_USBIO_CR1_DM0_POS      (0u)
#define USBTMC_USBIO_CR1_DP0_POS      (1u)
#define USBTMC_USBIO_CR1_USBPUEN_POS  (2u)
#define USBTMC_USBIO_CR1_IOMODE_POS   (5u)
#define USBTMC_USBIO_CR1_DM0          ((uint32) ((uint32) 0x1u << USBTMC_USBIO_CR1_DM0_POS))
#define USBTMC_USBIO_CR1_DP0          ((uint32) ((uint32) 0x1u << USBTMC_USBIO_CR1_DP0_POS))
#define USBTMC_USBIO_CR1_USBPUEN      ((uint32) ((uint32) 0x1u << USBTMC_USBIO_CR1_USBPUEN_POS))
#define USBTMC_USBIO_CR1_IOMODE       ((uint32) ((uint32) 0x1u << USBTMC_USBIO_CR1_IOMODE_POS))

/* USBTMC_FASTCLK_IMO_CR */
#define USBTMC_FASTCLK_IMO_CR_USBCLK_ON   (0x40u)
#define USBTMC_FASTCLK_IMO_CR_XCLKEN      (0x20u)
#define USBTMC_FASTCLK_IMO_CR_FX2ON       (0x10u)

/* USBTMC_ARB_EPX_CFG */
#define USBTMC_ARB_EPX_CFG_IN_DATA_RDY_POS    (0u)
#define USBTMC_ARB_EPX_CFG_DMA_REQ_POS        (1u)
#define USBTMC_ARB_EPX_CFG_CRC_BYPASS_POS     (2u)
#define USBTMC_ARB_EPX_CFG_RESET_POS          (3u)
#define USBTMC_ARB_EPX_CFG_IN_DATA_RDY        ((uint32) ((uint32) 0x1u << USBTMC_ARB_EPX_CFG_IN_DATA_RDY_POS))
#define USBTMC_ARB_EPX_CFG_DMA_REQ            ((uint32) ((uint32) 0x1u << USBTMC_ARB_EPX_CFG_DMA_REQ_POS))
#define USBTMC_ARB_EPX_CFG_CRC_BYPASS         ((uint32) ((uint32) 0x1u << USBTMC_ARB_EPX_CFG_CRC_BYPASS_POS))
#define USBTMC_ARB_EPX_CFG_RESET              ((uint32) ((uint32) 0x1u << USBTMC_ARB_EPX_CFG_RESET_POS))

/* USBTMC_ARB_EPX_INT / SR */
#define USBTMC_ARB_EPX_INT_IN_BUF_FULL_POS    (0u)
#define USBTMC_ARB_EPX_INT_DMA_GNT_POS        (1u)
#define USBTMC_ARB_EPX_INT_BUF_OVER_POS       (2u)
#define USBTMC_ARB_EPX_INT_BUF_UNDER_POS      (3u)
#define USBTMC_ARB_EPX_INT_ERR_INT_POS        (4u)
#define USBTMC_ARB_EPX_INT_IN_BUF_FULL        ((uint32) ((uint32) 0x1u << USBTMC_ARB_EPX_INT_IN_BUF_FULL_POS))
#define USBTMC_ARB_EPX_INT_DMA_GNT            ((uint32) ((uint32) 0x1u << USBTMC_ARB_EPX_INT_DMA_GNT_POS))
#define USBTMC_ARB_EPX_INT_BUF_OVER           ((uint32) ((uint32) 0x1u << USBTMC_ARB_EPX_INT_BUF_OVER_POS))
#define USBTMC_ARB_EPX_INT_BUF_UNDER          ((uint32) ((uint32) 0x1u << USBTMC_ARB_EPX_INT_BUF_UNDER_POS))
#define USBTMC_ARB_EPX_INT_ERR_INT            ((uint32) ((uint32) 0x1u << USBTMC_ARB_EPX_INT_ERR_INT_POS))

#if (CY_PSOC4)
#define USBTMC_ARB_EPX_INT_DMA_TERMIN_POS     (5u)
#define USBTMC_ARB_EPX_INT_DMA_TERMIN         ((uint32) ((uint32) 0x1u << USBTMC_ARB_EPX_INT_DMA_TERMIN_POS))
#endif /* (CY_PSOC4) */

/* Common arbiter interrupt sources for all PSoC devices. */
#define USBTMC_ARB_EPX_INT_COMMON    (USBTMC_ARB_EPX_INT_IN_BUF_FULL | \
                                                USBTMC_ARB_EPX_INT_DMA_GNT     | \
                                                USBTMC_ARB_EPX_INT_BUF_OVER    | \
                                                USBTMC_ARB_EPX_INT_BUF_UNDER   | \
                                                USBTMC_ARB_EPX_INT_ERR_INT)

#if (CY_PSOC4)
    #define USBTMC_ARB_EPX_INT_ALL    (USBTMC_ARB_EPX_INT_COMMON | USBTMC_ARB_EPX_INT_DMA_TERMIN)
#else
    #define USBTMC_ARB_EPX_INT_ALL    (USBTMC_ARB_EPX_INT_COMMON)
#endif /* (CY_PSOC4) */

/* USBTMC_ARB_CFG */
#define USBTMC_ARB_CFG_AUTO_MEM_POS   (4u)
#define USBTMC_ARB_CFG_DMA_CFG_POS    (5u)
#define USBTMC_ARB_CFG_CFG_CMP_POS    (7u)
#define USBTMC_ARB_CFG_AUTO_MEM       ((uint32) ((uint32) 0x1u << USBTMC_ARB_CFG_AUTO_MEM_POS))
#define USBTMC_ARB_CFG_DMA_CFG_MASK   ((uint32) ((uint32) 0x3u << USBTMC_ARB_CFG_DMA_CFG_POS))
#define USBTMC_ARB_CFG_DMA_CFG_NONE   ((uint32) ((uint32) 0x0u << USBTMC_ARB_CFG_DMA_CFG_POS))
#define USBTMC_ARB_CFG_DMA_CFG_MANUAL ((uint32) ((uint32) 0x1u << USBTMC_ARB_CFG_DMA_CFG_POS))
#define USBTMC_ARB_CFG_DMA_CFG_AUTO   ((uint32) ((uint32) 0x2u << USBTMC_ARB_CFG_DMA_CFG_POS))
#define USBTMC_ARB_CFG_CFG_CMP        ((uint32) ((uint32) 0x1u << USBTMC_ARB_CFG_CFG_CMP_POS))

/* USBTMC_DYN_RECONFIG */
#define USBTMC_DYN_RECONFIG_EP_SHIFT      (1u)
#define USBTMC_DYN_RECONFIG_ENABLE_POS    (0u)
#define USBTMC_DYN_RECONFIG_EPNO_POS      (1u)
#define USBTMC_DYN_RECONFIG_RDY_STS_POS   (4u)
#define USBTMC_DYN_RECONFIG_ENABLE        ((uint32) ((uint32) 0x1u << USBTMC_DYN_RECONFIG_ENABLE_POS))
#define USBTMC_DYN_RECONFIG_EPNO_MASK     ((uint32) ((uint32) 0x7u << USBTMC_DYN_RECONFIG_EPNO_POS))
#define USBTMC_DYN_RECONFIG_RDY_STS       ((uint32) ((uint32) 0x1u << USBTMC_DYN_RECONFIG_RDY_STS_POS))

/* USBTMC_ARB_INT */
#define USBTMC_ARB_INT_EP1_INTR_POS          (0u) /* [0] Interrupt for USB EP1 */
#define USBTMC_ARB_INT_EP2_INTR_POS          (1u) /* [1] Interrupt for USB EP2 */
#define USBTMC_ARB_INT_EP3_INTR_POS          (2u) /* [2] Interrupt for USB EP3 */
#define USBTMC_ARB_INT_EP4_INTR_POS          (3u) /* [3] Interrupt for USB EP4 */
#define USBTMC_ARB_INT_EP5_INTR_POS          (4u) /* [4] Interrupt for USB EP5 */
#define USBTMC_ARB_INT_EP6_INTR_POS          (5u) /* [5] Interrupt for USB EP6 */
#define USBTMC_ARB_INT_EP7_INTR_POS          (6u) /* [6] Interrupt for USB EP7 */
#define USBTMC_ARB_INT_EP8_INTR_POS          (7u) /* [7] Interrupt for USB EP8 */
#define USBTMC_ARB_INT_EP1_INTR              ((uint32) ((uint32) 0x1u << USBTMC_ARB_INT_EP1_INTR_POS))
#define USBTMC_ARB_INT_EP2_INTR              ((uint32) ((uint32) 0x1u << USBTMC_ARB_INT_EP2_INTR_POS))
#define USBTMC_ARB_INT_EP3_INTR              ((uint32) ((uint32) 0x1u << USBTMC_ARB_INT_EP3_INTR_POS))
#define USBTMC_ARB_INT_EP4_INTR              ((uint32) ((uint32) 0x1u << USBTMC_ARB_INT_EP4_INTR_POS))
#define USBTMC_ARB_INT_EP5_INTR              ((uint32) ((uint32) 0x1u << USBTMC_ARB_INT_EP5_INTR_POS))
#define USBTMC_ARB_INT_EP6_INTR              ((uint32) ((uint32) 0x1u << USBTMC_ARB_INT_EP6_INTR_POS))
#define USBTMC_ARB_INT_EP7_INTR              ((uint32) ((uint32) 0x1u << USBTMC_ARB_INT_EP7_INTR_POS))
#define USBTMC_ARB_INT_EP8_INTR              ((uint32) ((uint32) 0x1u << USBTMC_ARB_INT_EP8_INTR_POS))

/* USBTMC_SIE_INT */
#define USBTMC_SIE_INT_EP1_INTR_POS          (0u) /* [0] Interrupt for USB EP1 */
#define USBTMC_SIE_INT_EP2_INTR_POS          (1u) /* [1] Interrupt for USB EP2 */
#define USBTMC_SIE_INT_EP3_INTR_POS          (2u) /* [2] Interrupt for USB EP3 */
#define USBTMC_SIE_INT_EP4_INTR_POS          (3u) /* [3] Interrupt for USB EP4 */
#define USBTMC_SIE_INT_EP5_INTR_POS          (4u) /* [4] Interrupt for USB EP5 */
#define USBTMC_SIE_INT_EP6_INTR_POS          (5u) /* [5] Interrupt for USB EP6 */
#define USBTMC_SIE_INT_EP7_INTR_POS          (6u) /* [6] Interrupt for USB EP7 */
#define USBTMC_SIE_INT_EP8_INTR_POS          (7u) /* [7] Interrupt for USB EP8 */
#define USBTMC_SIE_INT_EP1_INTR              ((uint32) ((uint32) 0x01u << USBTMC_SIE_INT_EP1_INTR_POS))
#define USBTMC_SIE_INT_EP2_INTR              ((uint32) ((uint32) 0x01u << USBTMC_SIE_INT_EP2_INTR_POS))
#define USBTMC_SIE_INT_EP3_INTR              ((uint32) ((uint32) 0x01u << USBTMC_SIE_INT_EP3_INTR_POS))
#define USBTMC_SIE_INT_EP4_INTR              ((uint32) ((uint32) 0x01u << USBTMC_SIE_INT_EP4_INTR_POS))
#define USBTMC_SIE_INT_EP5_INTR              ((uint32) ((uint32) 0x01u << USBTMC_SIE_INT_EP5_INTR_POS))
#define USBTMC_SIE_INT_EP6_INTR              ((uint32) ((uint32) 0x01u << USBTMC_SIE_INT_EP6_INTR_POS))
#define USBTMC_SIE_INT_EP7_INTR              ((uint32) ((uint32) 0x01u << USBTMC_SIE_INT_EP7_INTR_POS))
#define USBTMC_SIE_INT_EP8_INTR              ((uint32) ((uint32) 0x01u << USBTMC_SIE_INT_EP8_INTR_POS))

#if (CY_PSOC4)
    /* USBTMC_POWER_CTRL_REG */
    #define USBTMC_POWER_CTRL_VBUS_VALID_OVR_POS       (0u)  /* [0] */
    #define USBTMC_POWER_CTRL_SUSPEND_POS              (2u)  /* [1] */
    #define USBTMC_POWER_CTRL_SUSPEND_DEL_POS          (3u)  /* [3] */
    #define USBTMC_POWER_CTRL_ISOLATE_POS              (4u)  /* [4] */
    #define USBTMC_POWER_CTRL_CHDET_PWR_CTL_POS        (5u)  /* [5] */
    #define USBTMC_POWER_CTRL_ENABLE_DM_PULLDOWN_POS   (25u) /* [25] */
    #define USBTMC_POWER_CTRL_ENABLE_VBUS_PULLDOWN_POS (26u) /* [26] */
    #define USBTMC_POWER_CTRL_ENABLE_RCVR_POS          (27u) /* [27] */
    #define USBTMC_POWER_CTRL_ENABLE_DPO_POS           (28u) /* [28] */
    #define USBTMC_POWER_CTRL_ENABLE_DMO_POS           (29u) /* [29] */
    #define USBTMC_POWER_CTRL_ENABLE_CHGDET_POS        (30u) /* [30] */
    #define USBTMC_POWER_CTRL_ENABLE_POS               (31u) /* [31] */
    #define USBTMC_POWER_CTRL_VBUS_VALID_OVR_MASK      ((uint32) 0x03u << USBTMC_POWER_CTRL_VBUS_VALID_OVR_POS)
    #define USBTMC_POWER_CTRL_VBUS_VALID_OVR_0         ((uint32) 0x00u << USBTMC_POWER_CTRL_VBUS_VALID_OVR_POS)
    #define USBTMC_POWER_CTRL_VBUS_VALID_OVR_1         ((uint32) 0x01u << USBTMC_POWER_CTRL_VBUS_VALID_OVR_POS)
    #define USBTMC_POWER_CTRL_VBUS_VALID_OVR_GPIO      ((uint32) 0x02u << USBTMC_POWER_CTRL_VBUS_VALID_OVR_POS)
    #define USBTMC_POWER_CTRL_VBUS_VALID_OVR_PHY       ((uint32) 0x03u << USBTMC_POWER_CTRL_VBUS_VALID_OVR_POS)
    #define USBTMC_POWER_CTRL_SUSPEND                  ((uint32) 0x01u << USBTMC_POWER_CTRL_SUSPEND_POS)
    #define USBTMC_POWER_CTRL_SUSPEND_DEL              ((uint32) 0x01u << USBTMC_POWER_CTRL_SUSPEND_DEL_POS)
    #define USBTMC_POWER_CTRL_ISOLATE                  ((uint32) 0x01u << USBTMC_POWER_CTRL_ISOLATE_POS)
    #define USBTMC_POWER_CTRL_CHDET_PWR_CTL_MASK       ((uint32) 0x03u << USBTMC_POWER_CTRL_CHDET_PWR_CTL_POS)
    #define USBTMC_POWER_CTRL_ENABLE_DM_PULLDOWN       ((uint32) 0x01u << USBTMC_POWER_CTRL_ENABLE_DM_PULLDOWN_POS)
    #define USBTMC_POWER_CTRL_ENABLE_VBUS_PULLDOWN     ((uint32) 0x01u << USBTMC_POWER_CTRL_ENABLE_VBUS_PULLDOWN_POS)
    #define USBTMC_POWER_CTRL_ENABLE_RCVR              ((uint32) 0x01u << USBTMC_POWER_CTRL_ENABLE_RCVR_POS)
    #define USBTMC_POWER_CTRL_ENABLE_DPO               ((uint32) 0x01u << USBTMC_POWER_CTRL_ENABLE_DPO_POS)
    #define USBTMC_POWER_CTRL_ENABLE_DMO               ((uint32) 0x01u << USBTMC_POWER_CTRL_ENABLE_DMO_POS)
    #define USBTMC_POWER_CTRL_ENABLE_CHGDET            ((uint32) 0x01u << USBTMC_POWER_CTRL_ENABLE_CHGDET_POS)
    #define USBTMC_POWER_CTRL_ENABLE                   ((uint32) 0x01u << USBTMC_POWER_CTRL_ENABLE_POS)

    /* USBTMC_CHGDET_CTRL_REG */
    #define USBTMC_CHGDET_CTRL_COMP_DP_POS        (0u)  /* [0] */
    #define USBTMC_CHGDET_CTRL_COMP_DM_POS        (1u)  /* [1] */
    #define USBTMC_CHGDET_CTRL_COMP_EN_POS        (2u)  /* [2] */
    #define USBTMC_CHGDET_CTRL_REF_DP_POS         (3u)  /* [3] */
    #define USBTMC_CHGDET_CTRL_REF_DM_POS         (4u)  /* [4] */
    #define USBTMC_CHGDET_CTRL_REF_EN_POS         (5u)  /* [5] */
    #define USBTMC_CHGDET_CTRL_DCD_SRC_EN_POS     (6u)  /* [6] */
    #define USBTMC_CHGDET_CTRL_ADFT_CTRL_POS      (12u) /* [12] */
    #define USBTMC_CHGDET_CTRL_COMP_OUT_POS       (31u) /* [31] */
    #define USBTMC_CHGDET_CTRL_COMP_DP            ((uint32) 0x01u << USBTMC_CHGDET_CTRL_COMP_DP_POS)
    #define USBTMC_CHGDET_CTRL_COMP_DM            ((uint32) 0x01u << USBTMC_CHGDET_CTRL_COMP_DM_POS)
    #define USBTMC_CHGDET_CTRL_COMP_EN            ((uint32) 0x01u << USBTMC_CHGDET_CTRL_COMP_EN_POS)
    #define USBTMC_CHGDET_CTRL_REF_DP             ((uint32) 0x01u << USBTMC_CHGDET_CTRL_REF_DP_POS)
    #define USBTMC_CHGDET_CTRL_REF_DM             ((uint32) 0x01u << USBTMC_CHGDET_CTRL_REF_DM_POS)
    #define USBTMC_CHGDET_CTRL_REF_EN             ((uint32) 0x01u << USBTMC_CHGDET_CTRL_REF_EN_POS)
    #define USBTMC_CHGDET_CTRL_DCD_SRC_EN         ((uint32) 0x01u << USBTMC_CHGDET_CTRL_DCD_SRC_EN_POS)
    #define USBTMC_CHGDET_CTRL_ADFT_CTRL_MASK     ((uint32) 0x03u << USBTMC_CHGDET_CTRL_ADFT_CTRL_POS)
    #define USBTMC_CHGDET_CTRL_ADFT_CTRL_NORMAL   ((uint32) 0x00u << USBTMC_CHGDET_CTRL_ADFT_CTRL_POS)
    #define USBTMC_CHGDET_CTRL_ADFT_CTRL_VBG      ((uint32) 0x01u << USBTMC_CHGDET_CTRL_ADFT_CTRL_POS)
    #define USBTMC_CHGDET_CTRL_ADFT_CTRL_DONTUSE  ((uint32) 0x02u << USBTMC_CHGDET_CTRL_ADFT_CTRL_POS)
    #define USBTMC_CHGDET_CTRL_ADFT_CTRL_ADFTIN   ((uint32) 0x03u << USBTMC_CHGDET_CTRL_ADFT_CTRL_POS)
    #define USBTMC_CHGDET_CTRL_COMP_OUT           ((uint32) 0x01u << USBTMC_CHGDET_CTRL_COMP_OUT_POS)

    /* USBTMC_LPM_CTRL */
    #define USBTMC_LPM_CTRL_LPM_EN_POS        (0u)
    #define USBTMC_LPM_CTRL_LPM_ACK_RESP_POS  (1u)
    #define USBTMC_LPM_CTRL_NYET_EN_POS       (2u)
    #define USBTMC_LPM_CTRL_SUB_RESP_POS      (4u)
    #define USBTMC_LPM_CTRL_LPM_EN            ((uint32) 0x01u << USBTMC_LPM_CTRL_LPM_EN_POS)
    #define USBTMC_LPM_CTRL_LPM_ACK_RESP      ((uint32) 0x01u << USBTMC_LPM_CTRL_LPM_ACK_RESP_POS)
    #define USBTMC_LPM_CTRL_NYET_EN           ((uint32) 0x01u << USBTMC_LPM_CTRL_NYET_EN_POS)
    #define USBTMC_LPM_CTRL_ACK_NYET_MASK     ((uint32) 0x03u << USBTMC_LPM_CTRL_LPM_ACK_RESP_POS)
    #define USBTMC_LPM_CTRL_SUB_RESP          ((uint32) 0x01u << USBTMC_LPM_CTRL_SUB_RESP_POS)

    #define USBTMC_LPM_STAT_LPM_BESL_POS          (0u)
    #define USBTMC_LPM_STAT_LPM_REMOTE_WAKE_POS   (4u)
    #define USBTMC_LPM_STAT_LPM_BESL_MASK         ((uint32) 0x0Fu << USBTMC_LPM_STAT_LPM_BESL_POS)
    #define USBTMC_LPM_STAT_LPM_REMOTE_WAKE       ((uint32) 0x01u << USBTMC_LPM_STAT_LPM_REMOTE_WAKE_POS)

    /* USBTMC_INTR_SIE */
    #define USBTMC_INTR_SIE_SOF_INTR_POS          (0u) /* [0] Interrupt for USB SOF   */
    #define USBTMC_INTR_SIE_BUS_RESET_INTR_POS    (1u) /* [1] Interrupt for BUS RESET */
    #define USBTMC_INTR_SIE_EP0_INTR_POS          (2u) /* [2] Interrupt for EP0       */
    #define USBTMC_INTR_SIE_LPM_INTR_POS          (3u) /* [3] Interrupt for LPM       */
    #define USBTMC_INTR_SIE_RESUME_INTR_POS       (4u) /* [4] Interrupt for RESUME (not used by component) */
    #define USBTMC_INTR_SIE_SOF_INTR              ((uint32) 0x01u << USBTMC_INTR_SIE_SOF_INTR_POS)
    #define USBTMC_INTR_SIE_BUS_RESET_INTR        ((uint32) 0x01u << USBTMC_INTR_SIE_BUS_RESET_INTR_POS)
    #define USBTMC_INTR_SIE_EP0_INTR              ((uint32) 0x01u << USBTMC_INTR_SIE_EP0_INTR_POS)
    #define USBTMC_INTR_SIE_LPM_INTR              ((uint32) 0x01u << USBTMC_INTR_SIE_LPM_INTR_POS)
    #define USBTMC_INTR_SIE_RESUME_INTR           ((uint32) 0x01u << USBTMC_INTR_SIE_RESUME_INTR_POS)

    /* USBTMC_INTR_CAUSE_LO, MED and HI */
    #define USBTMC_INTR_CAUSE_SOF_INTR_POS       (0u)  /* [0] Interrupt status for USB SOF    */
    #define USBTMC_INTR_CAUSE_BUS_RESET_INTR_POS (1u)  /* [1] Interrupt status for USB BUS RSET */
    #define USBTMC_INTR_CAUSE_EP0_INTR_POS       (2u)  /* [2] Interrupt status for USB EP0    */
    #define USBTMC_INTR_CAUSE_LPM_INTR_POS       (3u)  /* [3] Interrupt status for USB LPM    */
    #define USBTMC_INTR_CAUSE_RESUME_INTR_POS    (4u)  /* [4] Interrupt status for USB RESUME */
    #define USBTMC_INTR_CAUSE_ARB_INTR_POS       (7u)  /* [7] Interrupt status for USB ARB    */
    #define USBTMC_INTR_CAUSE_EP1_INTR_POS       (8u)  /* [8] Interrupt status for USB EP1    */
    #define USBTMC_INTR_CAUSE_EP2_INTR_POS       (9u)  /* [9] Interrupt status for USB EP2    */
    #define USBTMC_INTR_CAUSE_EP3_INTR_POS       (10u) /* [10] Interrupt status for USB EP3   */
    #define USBTMC_INTR_CAUSE_EP4_INTR_POS       (11u) /* [11] Interrupt status for USB EP4   */
    #define USBTMC_INTR_CAUSE_EP5_INTR_POS       (12u) /* [12] Interrupt status for USB EP5   */
    #define USBTMC_INTR_CAUSE_EP6_INTR_POS       (13u) /* [13] Interrupt status for USB EP6   */
    #define USBTMC_INTR_CAUSE_EP7_INTR_POS       (14u) /* [14] Interrupt status for USB EP7   */
    #define USBTMC_INTR_CAUSE_EP8_INTR_POS       (15u) /* [15] Interrupt status for USB EP8   */
    #define USBTMC_INTR_CAUSE_SOF_INTR           ((uint32) 0x01u << USBTMC_INTR_CAUSE_SOF_INTR_POS)
    #define USBTMC_INTR_CAUSE_BUS_RESET_INTR     ((uint32) 0x01u << USBTMC_INTR_CAUSE_BUS_RESET_INTR_POS)
    #define USBTMC_INTR_CAUSE_EP0_INTR           ((uint32) 0x01u << USBTMC_INTR_CAUSE_EP0_INTR_POS)
    #define USBTMC_INTR_CAUSE_LPM_INTR           ((uint32) 0x01u << USBTMC_INTR_CAUSE_LPM_INTR_POS)
    #define USBTMC_INTR_CAUSE_RESUME_INTR        ((uint32) 0x01u << USBTMC_INTR_CAUSE_RESUME_INTR_POS)
    #define USBTMC_INTR_CAUSE_ARB_INTR           ((uint32) 0x01u << USBTMC_INTR_CAUSE_ARB_INTR_POS)
    #define USBTMC_INTR_CAUSE_EP1_INTR           ((uint32) 0x01u << USBTMC_INTR_CAUSE_EP1_INTR_POS)
    #define USBTMC_INTR_CAUSE_EP2_INTR           ((uint32) 0x01u << USBTMC_INTR_CAUSE_EP2_INTR_POS)
    #define USBTMC_INTR_CAUSE_EP3_INTR           ((uint32) 0x01u << USBTMC_INTR_CAUSE_EP3_INTR_POS)
    #define USBTMC_INTR_CAUSE_EP4_INTR           ((uint32) 0x01u << USBTMC_INTR_CAUSE_EP4_INTR_POS)
    #define USBTMC_INTR_CAUSE_EP5_INTR           ((uint32) 0x01u << USBTMC_INTR_CAUSE_EP5_INTR_POS)
    #define USBTMC_INTR_CAUSE_EP6_INTR           ((uint32) 0x01u << USBTMC_INTR_CAUSE_EP6_INTR_POS)
    #define USBTMC_INTR_CAUSE_EP7_INTR           ((uint32) 0x01u << USBTMC_INTR_CAUSE_EP7_INTR_POS)
    #define USBTMC_INTR_CAUSE_EP8_INTR           ((uint32) 0x01u << USBTMC_INTR_CAUSE_EP8_INTR_POS)

    #define USBTMC_INTR_CAUSE_CTRL_INTR_MASK     (USBTMC_INTR_CAUSE_SOF_INTR       | \
                                                            USBTMC_INTR_CAUSE_BUS_RESET_INTR | \
                                                            USBTMC_INTR_CAUSE_EP0_INTR       | \
                                                            USBTMC_INTR_CAUSE_LPM_INTR)

    #define USBTMC_INTR_CAUSE_EP1_8_INTR_MASK    (USBTMC_INTR_CAUSE_EP1_INTR       | \
                                                            USBTMC_INTR_CAUSE_EP2_INTR       | \
                                                            USBTMC_INTR_CAUSE_EP3_INTR       | \
                                                            USBTMC_INTR_CAUSE_EP4_INTR       | \
                                                            USBTMC_INTR_CAUSE_EP5_INTR       | \
                                                            USBTMC_INTR_CAUSE_EP6_INTR       | \
                                                            USBTMC_INTR_CAUSE_EP7_INTR       | \
                                                            USBTMC_INTR_CAUSE_EP8_INTR)

    #define USBTMC_INTR_CAUSE_EP_INTR_SHIFT      (USBTMC_INTR_CAUSE_ARB_INTR_POS - \
                                                           (USBTMC_INTR_CAUSE_LPM_INTR_POS + 1u))
    #define USBTMC_INTR_CAUSE_SRC_COUNT          (13u)

    #define USBTMC_CHGDET_CTRL_PRIMARY    (USBTMC_CHGDET_CTRL_COMP_EN | \
                                                     USBTMC_CHGDET_CTRL_COMP_DM | \
                                                     USBTMC_CHGDET_CTRL_REF_EN  | \
                                                     USBTMC_CHGDET_CTRL_REF_DP)

    #define USBTMC_CHGDET_CTRL_SECONDARY  (USBTMC_CHGDET_CTRL_COMP_EN | \
                                                     USBTMC_CHGDET_CTRL_COMP_DP | \
                                                     USBTMC_CHGDET_CTRL_REF_EN  | \
                                                     USBTMC_CHGDET_CTRL_REF_DM)

    #define USBTMC_CHGDET_CTRL_DEFAULT    (0x00000900u)


#else /* (CY_PSOC3 || CY_PSOC5LP) */
    #define USBTMC_PM_ACT_EN_FSUSB            USBTMC_USB__PM_ACT_MSK
    #define USBTMC_PM_STBY_EN_FSUSB           USBTMC_USB__PM_STBY_MSK
    #define USBTMC_PM_AVAIL_EN_FSUSBIO        (0x10u)

    #define USBTMC_PM_USB_CR0_REF_EN          (0x01u)
    #define USBTMC_PM_USB_CR0_PD_N            (0x02u)
    #define USBTMC_PM_USB_CR0_PD_PULLUP_N     (0x04u)
#endif /* (CY_PSOC4) */


/***************************************
*       Macros Definitions
***************************************/

#if (CY_PSOC4)
    #define USBTMC_ClearSieInterruptSource(intMask) \
                do{ \
                    USBTMC_INTR_SIE_REG = (uint32) (intMask); \
                }while(0)
#else
    #define USBTMC_ClearSieInterruptSource(intMask) \
                do{ /* Does nothing. */ }while(0)
#endif /* (CY_PSOC4) */

#define USBTMC_ClearSieEpInterruptSource(intMask) \
            do{ \
                USBTMC_SIE_EP_INT_SR_REG = (uint32) (intMask); \
            }while(0)

#define USBTMC_GET_ACTIVE_IN_EP_CR0_MODE(epType)  (((epType) == USBTMC_EP_TYPE_ISOC) ? \
                                                                (USBTMC_MODE_ISO_IN) : (USBTMC_MODE_ACK_IN))

#define USBTMC_GET_ACTIVE_OUT_EP_CR0_MODE(epType) (((epType) == USBTMC_EP_TYPE_ISOC) ? \
                                                                (USBTMC_MODE_ISO_OUT) : (USBTMC_MODE_ACK_OUT))

#define USBTMC_GET_EP_TYPE(epNumber)  (USBTMC_EP[epNumber].attrib & USBTMC_EP_TYPE_MASK)

#define USBTMC_GET_UINT16(hi, low)    (((uint16) ((uint16) (hi) << 8u)) | ((uint16) (low) & 0xFFu))


/***************************************
*    Initialization Register Settings
***************************************/

/* Clear device address and enable USB IP respond to USB traffic. */
#define USBTMC_DEFUALT_CR0    (USBTMC_CR0_ENABLE)

/* Arbiter configuration depends on memory management mode. */
#define USBTMC_DEFAULT_ARB_CFG    ((USBTMC_EP_MANAGEMENT_MANUAL) ? (USBTMC_ARB_CFG_DMA_CFG_NONE) : \
                                                ((USBTMC_EP_MANAGEMENT_DMA_MANUAL) ? \
                                                    (USBTMC_ARB_CFG_DMA_CFG_MANUAL) : \
                                                        (USBTMC_ARB_CFG_AUTO_MEM | USBTMC_ARB_CFG_DMA_CFG_AUTO)))

/* Enable arbiter interrupt for active endpoints only */
#define USBTMC_DEFAULT_ARB_INT_EN \
        ((uint32) ((uint32) USBTMC_DMA1_ACTIVE << USBTMC_ARB_INT_EP1_INTR_POS) | \
         (uint32) ((uint32) USBTMC_DMA2_ACTIVE << USBTMC_ARB_INT_EP2_INTR_POS) | \
         (uint32) ((uint32) USBTMC_DMA3_ACTIVE << USBTMC_ARB_INT_EP3_INTR_POS) | \
         (uint32) ((uint32) USBTMC_DMA4_ACTIVE << USBTMC_ARB_INT_EP4_INTR_POS) | \
         (uint32) ((uint32) USBTMC_DMA5_ACTIVE << USBTMC_ARB_INT_EP5_INTR_POS) | \
         (uint32) ((uint32) USBTMC_DMA6_ACTIVE << USBTMC_ARB_INT_EP6_INTR_POS) | \
         (uint32) ((uint32) USBTMC_DMA7_ACTIVE << USBTMC_ARB_INT_EP7_INTR_POS) | \
         (uint32) ((uint32) USBTMC_DMA8_ACTIVE << USBTMC_ARB_INT_EP8_INTR_POS))

/* Enable all SIE endpoints interrupts */
#define USBTMC_DEFAULT_SIE_EP_INT_EN  (USBTMC_SIE_INT_EP1_INTR | \
                                                 USBTMC_SIE_INT_EP2_INTR | \
                                                 USBTMC_SIE_INT_EP3_INTR | \
                                                 USBTMC_SIE_INT_EP4_INTR | \
                                                 USBTMC_SIE_INT_EP5_INTR | \
                                                 USBTMC_SIE_INT_EP6_INTR | \
                                                 USBTMC_SIE_INT_EP7_INTR | \
                                                 USBTMC_SIE_INT_EP8_INTR)

#define USBTMC_ARB_EPX_CFG_DEFAULT    (USBTMC_ARB_EPX_CFG_RESET | \
                                                 USBTMC_ARB_EPX_CFG_CRC_BYPASS)

/* Default EP arbiter interrupt source register */
#define USBTMC_ARB_EPX_INT_COMMON_MASK   (USBTMC_ARB_EPX_INT_IN_BUF_FULL | \
                                                    USBTMC_ARB_EPX_INT_BUF_OVER    | \
                                                    USBTMC_ARB_EPX_INT_BUF_UNDER   | \
                                                    USBTMC_ARB_EPX_INT_ERR_INT     | \
                                                    (USBTMC_EP_MANAGEMENT_DMA_MANUAL ? USBTMC_ARB_EPX_INT_DMA_GNT : 0u))

#define USBTMC_CLEAR_REG      (0u)

#if (CY_PSOC4)
    /* Set USB lock option when IMO is locked to USB traffic. */
    #define USBTMC_DEFUALT_CR1    ((0u != CySysClkImoGetUsbLock()) ? (USBTMC_CR1_ENABLE_LOCK) : (0u))

    /* Recommended value is increased from 3 to 10 due to suppress glitch on  
     * RSE0 with USB2.0 hubs (LF CLK = 32kHz equal to 350us). */
    #define USBTMC_DEFUALT_BUS_RST_CNT  (10u)

    /* Select VBUS sources as: valid, PHY of GPIO, and clears isolate bit. */
    /* Application level must ensure that VBUS is valid valid to use. */
    #define USBTMC_DEFAULT_POWER_CTRL_VBUS    (USBTMC_POWER_CTRL_ENABLE_VBUS_PULLDOWN | \
                                                         ((!USBTMC_VBUS_MONITORING_ENABLE) ? \
                                                            (USBTMC_POWER_CTRL_VBUS_VALID_OVR_1) : \
                                                                (USBTMC_VBUS_POWER_PAD_ENABLE ? \
                                                                    (USBTMC_POWER_CTRL_VBUS_VALID_OVR_PHY) : \
                                                                    (USBTMC_POWER_CTRL_VBUS_VALID_OVR_GPIO))))
    /* Enable USB IP. */
    #define USBTMC_DEFAULT_POWER_CTRL_PHY (USBTMC_POWER_CTRL_SUSPEND     | \
                                                     USBTMC_POWER_CTRL_SUSPEND_DEL | \
                                                     USBTMC_POWER_CTRL_ENABLE_RCVR | \
                                                     USBTMC_POWER_CTRL_ENABLE_DPO  | \
                                                     USBTMC_POWER_CTRL_ENABLE_DMO  | \
                                                     USBTMC_POWER_CTRL_ENABLE)

    /* Assign interrupt between levels lo, med, hi. */
    #define USBTMC_DEFAULT_INTR_LVL_SEL   ((uint32) (USBTMC_INTR_LVL_SEL))

    /* Enable interrupt source in the INTR_SIE. The SOF is always disabled and EP0 is enabled. */
    #define USBTMC_DEFAULT_INTR_SIE_MASK \
                ((uint32) ((uint32) USBTMC_BUS_RESET_ISR_ACTIVE << USBTMC_INTR_SIE_BUS_RESET_INTR_POS) | \
                 (uint32) ((uint32) USBTMC_SOF_ISR_ACTIVE       << USBTMC_INTR_SIE_SOF_INTR_POS)       | \
                 (uint32) ((uint32) USBTMC_LPM_ACTIVE           << USBTMC_INTR_SIE_LPM_INTR_POS)       | \
                 (uint32) ((uint32) USBTMC_INTR_SIE_EP0_INTR))

    /* Arbiter interrupt sources */
    #define USBTMC_ARB_EPX_INT_MASK   (USBTMC_ARB_EPX_INT_COMMON_MASK | \
                                                (USBTMC_EP_MANAGEMENT_DMA_AUTO ? USBTMC_ARB_EPX_INT_DMA_TERMIN : 0u))

    /* Common DMA configuration */
    #define USBTMC_DMA_COMMON_CFG     (CYDMA_PULSE | CYDMA_ENTIRE_DESCRIPTOR | \
                                                 CYDMA_NON_PREEMPTABLE)


#else
    #define USBTMC_ARB_EPX_INT_MASK   (USBTMC_ARB_EPX_INT_COMMON_MASK)

    #define USBTMC_DEFUALT_CR1        (USBTMC_CR1_ENABLE_LOCK)

    /* Recommended value is 3 for LF CLK = 100kHz equal to 100us. */
    #define USBTMC_DEFUALT_BUS_RST_CNT    (10u)
#endif /* (CY_PSOC4) */

/*
* \addtogroup group_deprecated
* @{
*/

/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Renamed type definitions */
#define USBTMC_CODE CYCODE
#define USBTMC_FAR CYFAR
#if defined(__C51__) || defined(__CX51__)
    #define USBTMC_DATA data
    #define USBTMC_XDATA xdata
#else
    #define USBTMC_DATA
    #define USBTMC_XDATA
#endif /*  __C51__ */
#define USBTMC_NULL       NULL
/** @} deprecated */
/* Renamed structure fields */
#define wBuffOffset         buffOffset
#define wBufferSize         bufferSize
#define bStatus             status
#define wLength             length
#define wCount              count

/* Renamed global variable */
#define CurrentTD           USBTMC_currentTD
#define USBTMC_interfaceSetting_last       USBTMC_interfaceSettingLast

/* Renamed global constants */
#define USBTMC_DWR_VDDD_OPERATION         (USBTMC_DWR_POWER_OPERATION)

/* Renamed functions */
#define USBTMC_bCheckActivity             USBTMC_CheckActivity
#define USBTMC_bGetConfiguration          USBTMC_GetConfiguration
#define USBTMC_bGetInterfaceSetting       USBTMC_GetInterfaceSetting
#define USBTMC_bGetEPState                USBTMC_GetEPState
#define USBTMC_wGetEPCount                USBTMC_GetEPCount
#define USBTMC_bGetEPAckState             USBTMC_GetEPAckState
#define USBTMC_bRWUEnabled                USBTMC_RWUEnabled
#define USBTMC_bVBusPresent               USBTMC_VBusPresent

#define USBTMC_bConfiguration             USBTMC_configuration
#define USBTMC_bInterfaceSetting          USBTMC_interfaceSetting
#define USBTMC_bDeviceAddress             USBTMC_deviceAddress
#define USBTMC_bDeviceStatus              USBTMC_deviceStatus
#define USBTMC_bDevice                    USBTMC_device
#define USBTMC_bTransferState             USBTMC_transferState
#define USBTMC_bLastPacketSize            USBTMC_lastPacketSize

#define USBTMC_LoadEP                     USBTMC_LoadInEP
#define USBTMC_LoadInISOCEP               USBTMC_LoadInEP
#define USBTMC_EnableOutISOCEP            USBTMC_EnableOutEP

#define USBTMC_SetVector                  CyIntSetVector
#define USBTMC_SetPriority                CyIntSetPriority
#define USBTMC_EnableInt                  CyIntEnable

/* Replace with register access. */
#define USBTMC_bmRequestType      USBTMC_EP0_DR0_PTR
#define USBTMC_bRequest           USBTMC_EP0_DR1_PTR
#define USBTMC_wValue             USBTMC_EP0_DR2_PTR
#define USBTMC_wValueHi           USBTMC_EP0_DR3_PTR
#define USBTMC_wValueLo           USBTMC_EP0_DR2_PTR
#define USBTMC_wIndex             USBTMC_EP0_DR4_PTR
#define USBTMC_wIndexHi           USBTMC_EP0_DR5_PTR
#define USBTMC_wIndexLo           USBTMC_EP0_DR4_PTR
#define USBTMC_length             USBTMC_EP0_DR6_PTR
#define USBTMC_lengthHi           USBTMC_EP0_DR7_PTR
#define USBTMC_lengthLo           USBTMC_EP0_DR6_PTR

/* Rename VBUS monitoring registers. */
#if (CY_PSOC3 || CY_PSOC5LP)
    #if (USBTMC_VBUS_MONITORING_ENABLE)
        #if (USBTMC_VBUS_MONITORING_INTERNAL)
            #define USBTMC_VBUS_DR_PTR    ( (reg8 *) USBTMC_VBUS__DR)
            #define USBTMC_VBUS_DR_REG    (*(reg8 *) USBTMC_VBUS__DR)
            #define USBTMC_VBUS_PS_PTR    ( (reg8 *) USBTMC_VBUS__PS)
            #define USBTMC_VBUS_PS_REG    (*(reg8 *) USBTMC_VBUS__PS)
            #define USBTMC_VBUS_MASK          USBTMC_VBUS__MASK
        #else
            #define USBTMC_VBUS_PS_PTR    ( (reg8 *) USBTMC_Vbus_ps_sts_sts_reg__STATUS_REG)
            #define USBTMC_VBUS_MASK      (0x01u)
        #endif /* (USBTMC_VBUS_MONITORING_INTERNAL) */
    #endif /*(USBTMC_VBUS_MONITORING_ENABLE) */
        
    /* Pointer DIE structure in flash (8 bytes): Y and X location, wafer, lot msb, lot lsb, 
    *  work week, fab/year, minor. */
    #define USBTMC_DIE_ID             CYDEV_FLSHID_CUST_TABLES_BASE

     #if (USBTMC_EP_MANAGEMENT_DMA_AUTO)
        #if (USBTMC_DMA1_ACTIVE)
            #define USBTMC_ep1_TD_TERMOUT_EN  (USBTMC_ep1__TD_TERMOUT_EN)
        #else
            #define USBTMC_ep1_TD_TERMOUT_EN  (0u)
        #endif /* (USBTMC_DMA1_ACTIVE) */

        #if (USBTMC_DMA2_ACTIVE)
            #define USBTMC_ep2_TD_TERMOUT_EN  (USBTMC_ep2__TD_TERMOUT_EN)
        #else
            #define USBTMC_ep2_TD_TERMOUT_EN  (0u)
        #endif /* (USBTMC_DMA2_ACTIVE) */

        #if (USBTMC_DMA3_ACTIVE)
            #define USBTMC_ep3_TD_TERMOUT_EN  (USBTMC_ep3__TD_TERMOUT_EN)
        #else
            #define USBTMC_ep3_TD_TERMOUT_EN  (0u)
        #endif /* (USBTMC_DMA3_ACTIVE) */

        #if (USBTMC_DMA4_ACTIVE)
            #define USBTMC_ep4_TD_TERMOUT_EN  (USBTMC_ep4__TD_TERMOUT_EN)
        #else
            #define USBTMC_ep4_TD_TERMOUT_EN  (0u)
        #endif /* (USBTMC_DMA4_ACTIVE) */

        #if (USBTMC_DMA5_ACTIVE)
            #define USBTMC_ep5_TD_TERMOUT_EN  (USBTMC_ep5__TD_TERMOUT_EN)
        #else
            #define USBTMC_ep5_TD_TERMOUT_EN  (0u)
        #endif /* (USBTMC_DMA5_ACTIVE) */

        #if (USBTMC_DMA6_ACTIVE)
            #define USBTMC_ep6_TD_TERMOUT_EN  (USBTMC_ep6__TD_TERMOUT_EN)
        #else
            #define USBTMC_ep6_TD_TERMOUT_EN  (0u)
        #endif /* (USBTMC_DMA6_ACTIVE) */

        #if (USBTMC_DMA7_ACTIVE)
            #define USBTMC_ep7_TD_TERMOUT_EN  (USBTMC_ep7__TD_TERMOUT_EN)
        #else
            #define USBTMC_ep7_TD_TERMOUT_EN  (0u)
        #endif /* (USBTMC_DMA7_ACTIVE) */

        #if (USBTMC_DMA8_ACTIVE)
            #define USBTMC_ep8_TD_TERMOUT_EN  (USBTMC_ep8__TD_TERMOUT_EN)
        #else
            #define USBTMC_ep8_TD_TERMOUT_EN  (0u)
        #endif /* (USBTMC_DMA8_ACTIVE) */
    #endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */   
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

/* Rename USB IP registers. */
#define USBTMC_ARB_CFG        USBTMC_ARB_CFG_PTR

#define USBTMC_ARB_EP1_CFG    USBTMC_ARB_EP1_CFG_PTR
#define USBTMC_ARB_EP1_INT_EN USBTMC_ARB_EP1_INT_EN_PTR
#define USBTMC_ARB_EP1_SR     USBTMC_ARB_EP1_SR_PTR

#define USBTMC_ARB_EP2_CFG    USBTMC_ARB_EP2_CFG_PTR
#define USBTMC_ARB_EP2_INT_EN USBTMC_ARB_EP2_INT_EN_PTR
#define USBTMC_ARB_EP2_SR     USBTMC_ARB_EP2_SR_PTR

#define USBTMC_ARB_EP3_CFG    USBTMC_ARB_EP3_CFG_PTR
#define USBTMC_ARB_EP3_INT_EN USBTMC_ARB_EP3_INT_EN_PTR
#define USBTMC_ARB_EP3_SR     USBTMC_ARB_EP3_SR_PTR

#define USBTMC_ARB_EP4_CFG    USBTMC_ARB_EP4_CFG_PTR
#define USBTMC_ARB_EP4_INT_EN USBTMC_ARB_EP4_INT_EN_PTR
#define USBTMC_ARB_EP4_SR     USBTMC_ARB_EP4_SR_PTR

#define USBTMC_ARB_EP5_CFG    USBTMC_ARB_EP5_CFG_PTR
#define USBTMC_ARB_EP5_INT_EN USBTMC_ARB_EP5_INT_EN_PTR
#define USBTMC_ARB_EP5_SR     USBTMC_ARB_EP5_SR_PTR

#define USBTMC_ARB_EP6_CFG    USBTMC_ARB_EP6_CFG_PTR
#define USBTMC_ARB_EP6_INT_EN USBTMC_ARB_EP6_INT_EN_PTR
#define USBTMC_ARB_EP6_SR     USBTMC_ARB_EP6_SR_PTR

#define USBTMC_ARB_EP7_CFG    USBTMC_ARB_EP7_CFG_PTR
#define USBTMC_ARB_EP7_INT_EN USBTMC_ARB_EP7_INT_EN_PTR
#define USBTMC_ARB_EP7_SR     USBTMC_ARB_EP7_SR_PTR

#define USBTMC_ARB_EP8_CFG    USBTMC_ARB_EP8_CFG_PTR
#define USBTMC_ARB_EP8_INT_EN USBTMC_ARB_EP8_INT_EN_PTR
#define USBTMC_ARB_EP8_SR     USBTMC_ARB_EP8_SR_PTR

#define USBTMC_ARB_INT_EN     USBTMC_ARB_INT_EN_PTR
#define USBTMC_ARB_INT_SR     USBTMC_ARB_INT_SR_PTR

#define USBTMC_ARB_RW1_DR     USBTMC_ARB_RW1_DR_PTR
#define USBTMC_ARB_RW1_RA     USBTMC_ARB_RW1_RA_PTR
#define USBTMC_ARB_RW1_RA_MSB USBTMC_ARB_RW1_RA_MSB_PTR
#define USBTMC_ARB_RW1_WA     USBTMC_ARB_RW1_WA_PTR
#define USBTMC_ARB_RW1_WA_MSB USBTMC_ARB_RW1_WA_MSB_PTR

#define USBTMC_ARB_RW2_DR     USBTMC_ARB_RW2_DR_PTR
#define USBTMC_ARB_RW2_RA     USBTMC_ARB_RW2_RA_PTR
#define USBTMC_ARB_RW2_RA_MSB USBTMC_ARB_RW2_RA_MSB_PTR
#define USBTMC_ARB_RW2_WA     USBTMC_ARB_RW2_WA_PTR
#define USBTMC_ARB_RW2_WA_MSB USBTMC_ARB_RW2_WA_MSB_PTR

#define USBTMC_ARB_RW3_DR     USBTMC_ARB_RW3_DR_PTR
#define USBTMC_ARB_RW3_RA     USBTMC_ARB_RW3_RA_PTR
#define USBTMC_ARB_RW3_RA_MSB USBTMC_ARB_RW3_RA_MSB_PTR
#define USBTMC_ARB_RW3_WA     USBTMC_ARB_RW3_WA_PTR
#define USBTMC_ARB_RW3_WA_MSB USBTMC_ARB_RW3_WA_MSB_PTR

#define USBTMC_ARB_RW4_DR     USBTMC_ARB_RW4_DR_PTR
#define USBTMC_ARB_RW4_RA     USBTMC_ARB_RW4_RA_PTR
#define USBTMC_ARB_RW4_RA_MSB USBTMC_ARB_RW4_RA_MSB_PTR
#define USBTMC_ARB_RW4_WA     USBTMC_ARB_RW4_WA_PTR
#define USBTMC_ARB_RW4_WA_MSB USBTMC_ARB_RW4_WA_MSB_PTR

#define USBTMC_ARB_RW5_DR     USBTMC_ARB_RW5_DR_PTR
#define USBTMC_ARB_RW5_RA     USBTMC_ARB_RW5_RA_PTR
#define USBTMC_ARB_RW5_RA_MSB USBTMC_ARB_RW5_RA_MSB_PTR
#define USBTMC_ARB_RW5_WA     USBTMC_ARB_RW5_WA_PTR
#define USBTMC_ARB_RW5_WA_MSB USBTMC_ARB_RW5_WA_MSB_PTR

#define USBTMC_ARB_RW6_DR     USBTMC_ARB_RW6_DR_PTR
#define USBTMC_ARB_RW6_RA     USBTMC_ARB_RW6_RA_PTR
#define USBTMC_ARB_RW6_RA_MSB USBTMC_ARB_RW6_RA_MSB_PTR
#define USBTMC_ARB_RW6_WA     USBTMC_ARB_RW6_WA_PTR
#define USBTMC_ARB_RW6_WA_MSB USBTMC_ARB_RW6_WA_MSB_PTR

#define USBTMC_ARB_RW7_DR     USBTMC_ARB_RW7_DR_PTR
#define USBTMC_ARB_RW7_RA     USBTMC_ARB_RW7_RA_PTR
#define USBTMC_ARB_RW7_RA_MSB USBTMC_ARB_RW7_RA_MSB_PTR
#define USBTMC_ARB_RW7_WA     USBTMC_ARB_RW7_WA_PTR
#define USBTMC_ARB_RW7_WA_MSB USBTMC_ARB_RW7_WA_MSB_PTR

#define USBTMC_ARB_RW8_DR     USBTMC_ARB_RW8_DR_PTR
#define USBTMC_ARB_RW8_RA     USBTMC_ARB_RW8_RA_PTR
#define USBTMC_ARB_RW8_RA_MSB USBTMC_ARB_RW8_RA_MSB_PTR
#define USBTMC_ARB_RW8_WA     USBTMC_ARB_RW8_WA_PTR
#define USBTMC_ARB_RW8_WA_MSB USBTMC_ARB_RW8_WA_MSB_PTR

#define USBTMC_BUF_SIZE       USBTMC_BUF_SIZE_PTR
#define USBTMC_BUS_RST_CNT    USBTMC_BUS_RST_CNT_PTR
#define USBTMC_CR0            USBTMC_CR0_PTR
#define USBTMC_CR1            USBTMC_CR1_PTR
#define USBTMC_CWA            USBTMC_CWA_PTR
#define USBTMC_CWA_MSB        USBTMC_CWA_MSB_PTR

#define USBTMC_DMA_THRES      USBTMC_DMA_THRES_PTR
#define USBTMC_DMA_THRES_MSB  USBTMC_DMA_THRES_MSB_PTR

#define USBTMC_EP_ACTIVE      USBTMC_EP_ACTIVE_PTR
#define USBTMC_EP_TYPE        USBTMC_EP_TYPE_PTR

#define USBTMC_EP0_CNT        USBTMC_EP0_CNT_PTR
#define USBTMC_EP0_CR         USBTMC_EP0_CR_PTR
#define USBTMC_EP0_DR0        USBTMC_EP0_DR0_PTR
#define USBTMC_EP0_DR1        USBTMC_EP0_DR1_PTR
#define USBTMC_EP0_DR2        USBTMC_EP0_DR2_PTR
#define USBTMC_EP0_DR3        USBTMC_EP0_DR3_PTR
#define USBTMC_EP0_DR4        USBTMC_EP0_DR4_PTR
#define USBTMC_EP0_DR5        USBTMC_EP0_DR5_PTR
#define USBTMC_EP0_DR6        USBTMC_EP0_DR6_PTR
#define USBTMC_EP0_DR7        USBTMC_EP0_DR7_PTR

#define USBTMC_OSCLK_DR0      USBTMC_OSCLK_DR0_PTR
#define USBTMC_OSCLK_DR1      USBTMC_OSCLK_DR1_PTR

#define USBTMC_PM_ACT_CFG     USBTMC_PM_ACT_CFG_PTR
#define USBTMC_PM_STBY_CFG    USBTMC_PM_STBY_CFG_PTR

#define USBTMC_SIE_EP_INT_EN  USBTMC_SIE_EP_INT_EN_PTR
#define USBTMC_SIE_EP_INT_SR  USBTMC_SIE_EP_INT_SR_PTR

#define USBTMC_SIE_EP1_CNT0   USBTMC_SIE_EP1_CNT0_PTR
#define USBTMC_SIE_EP1_CNT1   USBTMC_SIE_EP1_CNT1_PTR
#define USBTMC_SIE_EP1_CR0    USBTMC_SIE_EP1_CR0_PTR

#define USBTMC_SIE_EP2_CNT0   USBTMC_SIE_EP2_CNT0_PTR
#define USBTMC_SIE_EP2_CNT1   USBTMC_SIE_EP2_CNT1_PTR
#define USBTMC_SIE_EP2_CR0    USBTMC_SIE_EP2_CR0_PTR

#define USBTMC_SIE_EP3_CNT0   USBTMC_SIE_EP3_CNT0_PTR
#define USBTMC_SIE_EP3_CNT1   USBTMC_SIE_EP3_CNT1_PTR
#define USBTMC_SIE_EP3_CR0    USBTMC_SIE_EP3_CR0_PTR

#define USBTMC_SIE_EP4_CNT0   USBTMC_SIE_EP4_CNT0_PTR
#define USBTMC_SIE_EP4_CNT1   USBTMC_SIE_EP4_CNT1_PTR
#define USBTMC_SIE_EP4_CR0    USBTMC_SIE_EP4_CR0_PTR

#define USBTMC_SIE_EP5_CNT0   USBTMC_SIE_EP5_CNT0_PTR
#define USBTMC_SIE_EP5_CNT1   USBTMC_SIE_EP5_CNT1_PTR
#define USBTMC_SIE_EP5_CR0    USBTMC_SIE_EP5_CR0_PTR

#define USBTMC_SIE_EP6_CNT0   USBTMC_SIE_EP6_CNT0_PTR
#define USBTMC_SIE_EP6_CNT1   USBTMC_SIE_EP6_CNT1_PTR
#define USBTMC_SIE_EP6_CR0    USBTMC_SIE_EP6_CR0_PTR

#define USBTMC_SIE_EP7_CNT0   USBTMC_SIE_EP7_CNT0_PTR
#define USBTMC_SIE_EP7_CNT1   USBTMC_SIE_EP7_CNT1_PTR
#define USBTMC_SIE_EP7_CR0    USBTMC_SIE_EP7_CR0_PTR

#define USBTMC_SIE_EP8_CNT0   USBTMC_SIE_EP8_CNT0_PTR
#define USBTMC_SIE_EP8_CNT1   USBTMC_SIE_EP8_CNT1_PTR
#define USBTMC_SIE_EP8_CR0    USBTMC_SIE_EP8_CR0_PTR

#define USBTMC_SOF0           USBTMC_SOF0_PTR
#define USBTMC_SOF1           USBTMC_SOF1_PTR

#define USBTMC_USB_CLK_EN     USBTMC_USB_CLK_EN_PTR

#define USBTMC_USBIO_CR0      USBTMC_USBIO_CR0_PTR
#define USBTMC_USBIO_CR1      USBTMC_USBIO_CR1_PTR
#define USBTMC_USBIO_CR2      USBTMC_USBIO_CR2_PTR

#define USBTMC_DM_INP_DIS_PTR     ( (reg32 *) USBTMC_Dm__INP_DIS)
#define USBTMC_DM_INP_DIS_REG     (*(reg32 *) USBTMC_Dm__INP_DIS)
#define USBTMC_DP_INP_DIS_PTR     ( (reg32 *) USBTMC_Dp__INP_DIS)
#define USBTMC_DP_INP_DIS_REG     (*(reg32 *) USBTMC_Dp__INP_DIS)
#define USBTMC_DP_INTSTAT_PTR     ( (reg32 *) USBTMC_Dp__INTSTAT)
#define USBTMC_DP_INTSTAT_REG     (*(reg32 *) USBTMC_Dp__INTSTAT)
#define USBTMC_DM_MASK            USBTMC_Dm__0__MASK
#define USBTMC_DP_MASK            USBTMC_Dp__0__MASK

#define USBFS_SIE_EP_INT_EP1_MASK        (0x01u)
#define USBFS_SIE_EP_INT_EP2_MASK        (0x02u)
#define USBFS_SIE_EP_INT_EP3_MASK        (0x04u)
#define USBFS_SIE_EP_INT_EP4_MASK        (0x08u)
#define USBFS_SIE_EP_INT_EP5_MASK        (0x10u)
#define USBFS_SIE_EP_INT_EP6_MASK        (0x20u)
#define USBFS_SIE_EP_INT_EP7_MASK        (0x40u)
#define USBFS_SIE_EP_INT_EP8_MASK        (0x80u)

#define USBTMC_ARB_EPX_SR_IN_BUF_FULL     (0x01u)
#define USBTMC_ARB_EPX_SR_DMA_GNT         (0x02u)
#define USBTMC_ARB_EPX_SR_BUF_OVER        (0x04u)
#define USBTMC_ARB_EPX_SR_BUF_UNDER       (0x08u)

#define USBTMC_ARB_EPX_INT_EN_ALL USBTMC_ARB_EPX_INT_ALL

#define USBTMC_CR1_BUS_ACTIVITY_SHIFT     (0x02u)

#define USBTMC_BUS_RST_COUNT      USBTMC_DEFUALT_BUS_RST_CNT

#define USBTMC_ARB_INT_MASK       USBTMC_DEFAULT_ARB_INT_EN

#if (CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
    /* CY_PSOC3 interrupt registers */
    #define USBTMC_USB_ISR_PRIOR  ((reg8 *) CYDEV_INTC_PRIOR0)
    #define USBTMC_USB_ISR_SET_EN ((reg8 *) CYDEV_INTC_SET_EN0)
    #define USBTMC_USB_ISR_CLR_EN ((reg8 *) CYDEV_INTC_CLR_EN0)
    #define USBTMC_USB_ISR_VECT   ((cyisraddress *) CYDEV_INTC_VECT_MBASE)
#elif (CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_PANTHER)
    /* CY_PSOC5LP interrupt registers */
    #define USBTMC_USB_ISR_PRIOR  ((reg8 *) CYDEV_NVIC_PRI_0)
    #define USBTMC_USB_ISR_SET_EN ((reg8 *) CYDEV_NVIC_SETENA0)
    #define USBTMC_USB_ISR_CLR_EN ((reg8 *) CYDEV_NVIC_CLRENA0)
    #define USBTMC_USB_ISR_VECT   ((cyisraddress *) CYDEV_NVIC_VECT_OFFSET)
#endif /*  CYDEV_CHIP_DIE_EXPECT */


#endif /* (CY_USBFS_USBTMC_H) */


/* [] END OF FILE */
