/***************************************************************************//**
* \file USBCOMP_std.c
* \version 3.20
*
* \brief
*  This file contains the USB Standard request handler.
*
********************************************************************************
* \copyright
* Copyright 2008-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USBCOMP_pvt.h"

/***************************************
*   Static data allocation
***************************************/

#if defined(USBCOMP_ENABLE_FWSN_STRING)
    static volatile uint8* USBCOMP_fwSerialNumberStringDescriptor;
    static volatile uint8  USBCOMP_snStringConfirm = USBCOMP_FALSE;
#endif  /* (USBCOMP_ENABLE_FWSN_STRING) */

#if defined(USBCOMP_ENABLE_FWSN_STRING)
    /***************************************************************************
    * Function Name: USBCOMP_SerialNumString
    ************************************************************************//**
    *
    *  This function is available only when the User Call Back option in the 
    *  Serial Number String descriptor properties is selected. Application 
    *  firmware can provide the source of the USB device serial number string 
    *  descriptor during run time. The default string is used if the application 
    *  firmware does not use this function or sets the wrong string descriptor.
    *
    *  \param snString:  Pointer to the user-defined string descriptor. The 
    *  string descriptor should meet the Universal Serial Bus Specification 
    *  revision 2.0 chapter 9.6.7
    *  Offset|Size|Value|Description
    *  ------|----|------|---------------------------------
    *  0     |1   |N     |Size of this descriptor in bytes
    *  1     |1   |0x03  |STRING Descriptor Type
    *  2     |N-2 |Number|UNICODE encoded string
    *  
    * *For example:* uint8 snString[16]={0x0E,0x03,'F',0,'W',0,'S',0,'N',0,'0',0,'1',0};
    *
    * \reentrant
    *  No.
    *
    ***************************************************************************/
    void  USBCOMP_SerialNumString(uint8 snString[]) 
    {
        USBCOMP_snStringConfirm = USBCOMP_FALSE;
        
        if (snString != NULL)
        {
            /* Check descriptor validation */
            if ((snString[0u] > 1u) && (snString[1u] == USBCOMP_DESCR_STRING))
            {
                USBCOMP_fwSerialNumberStringDescriptor = snString;
                USBCOMP_snStringConfirm = USBCOMP_TRUE;
            }
        }
    }
#endif  /* USBCOMP_ENABLE_FWSN_STRING */


/*******************************************************************************
* Function Name: USBCOMP_HandleStandardRqst
****************************************************************************//**
*
*  This Routine dispatches standard requests
*
*
* \return
*  TRUE if request handled.
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBCOMP_HandleStandardRqst(void) 
{
    uint8 requestHandled = USBCOMP_FALSE;
    uint32 interfaceNumber;
    uint32 configurationN;
    uint32 bmRequestType = USBCOMP_bmRequestTypeReg;

#if defined(USBCOMP_ENABLE_STRINGS)
    volatile uint8 *pStr = 0u;
    #if defined(USBCOMP_ENABLE_DESCRIPTOR_STRINGS)
        uint8 nStr;
        uint8 descrLength;
    #endif /* (USBCOMP_ENABLE_DESCRIPTOR_STRINGS) */
#endif /* (USBCOMP_ENABLE_STRINGS) */
    
    static volatile uint8 USBCOMP_tBuffer[USBCOMP_STATUS_LENGTH_MAX];
    const T_USBCOMP_LUT CYCODE *pTmp;

    USBCOMP_currentTD.count = 0u;

    if (USBCOMP_RQST_DIR_D2H == (bmRequestType & USBCOMP_RQST_DIR_MASK))
    {
        /* Control Read */
        switch (USBCOMP_bRequestReg)
        {
            case USBCOMP_GET_DESCRIPTOR:
                if (USBCOMP_DESCR_DEVICE ==USBCOMP_wValueHiReg)
                {
                    pTmp = USBCOMP_GetDeviceTablePtr();
                    USBCOMP_currentTD.pData = (volatile uint8 *)pTmp->p_list;
                    USBCOMP_currentTD.count = USBCOMP_DEVICE_DESCR_LENGTH;
                    
                    requestHandled  = USBCOMP_InitControlRead();
                }
                else if (USBCOMP_DESCR_CONFIG == USBCOMP_wValueHiReg)
                {
                    pTmp = USBCOMP_GetConfigTablePtr((uint8) USBCOMP_wValueLoReg);
                    
                    /* Verify that requested descriptor exists */
                    if (pTmp != NULL)
                    {
                        USBCOMP_currentTD.pData = (volatile uint8 *)pTmp->p_list;
                        USBCOMP_currentTD.count = (uint16)((uint16)(USBCOMP_currentTD.pData)[USBCOMP_CONFIG_DESCR_TOTAL_LENGTH_HI] << 8u) | \
                                                                            (USBCOMP_currentTD.pData)[USBCOMP_CONFIG_DESCR_TOTAL_LENGTH_LOW];
                        requestHandled  = USBCOMP_InitControlRead();
                    }
                }
                
            #if(USBCOMP_BOS_ENABLE)
                else if (USBCOMP_DESCR_BOS == USBCOMP_wValueHiReg)
                {
                    pTmp = USBCOMP_GetBOSPtr();
                    
                    /* Verify that requested descriptor exists */
                    if (pTmp != NULL)
                    {
                        USBCOMP_currentTD.pData = (volatile uint8 *)pTmp;
                        USBCOMP_currentTD.count = ((uint16)((uint16)(USBCOMP_currentTD.pData)[USBCOMP_BOS_DESCR_TOTAL_LENGTH_HI] << 8u)) | \
                                                                             (USBCOMP_currentTD.pData)[USBCOMP_BOS_DESCR_TOTAL_LENGTH_LOW];
                        requestHandled  = USBCOMP_InitControlRead();
                    }
                }
            #endif /*(USBCOMP_BOS_ENABLE)*/
            
            #if defined(USBCOMP_ENABLE_STRINGS)
                else if (USBCOMP_DESCR_STRING == USBCOMP_wValueHiReg)
                {
                /* Descriptor Strings */
                #if defined(USBCOMP_ENABLE_DESCRIPTOR_STRINGS)
                    nStr = 0u;
                    pStr = (volatile uint8 *) &USBCOMP_STRING_DESCRIPTORS[0u];
                    
                    while ((USBCOMP_wValueLoReg > nStr) && (*pStr != 0u))
                    {
                        /* Read descriptor length from 1st byte */
                        descrLength = *pStr;
                        /* Move to next string descriptor */
                        pStr = &pStr[descrLength];
                        nStr++;
                    }
                #endif /* (USBCOMP_ENABLE_DESCRIPTOR_STRINGS) */
                
                /* Microsoft OS String */
                #if defined(USBCOMP_ENABLE_MSOS_STRING)
                    if (USBCOMP_STRING_MSOS == USBCOMP_wValueLoReg)
                    {
                        pStr = (volatile uint8 *)& USBCOMP_MSOS_DESCRIPTOR[0u];
                    }
                #endif /* (USBCOMP_ENABLE_MSOS_STRING) */
                
                /* SN string */
                #if defined(USBCOMP_ENABLE_SN_STRING)
                    if ((USBCOMP_wValueLoReg != 0u) && 
                        (USBCOMP_wValueLoReg == USBCOMP_DEVICE0_DESCR[USBCOMP_DEVICE_DESCR_SN_SHIFT]))
                    {
                    #if defined(USBCOMP_ENABLE_IDSN_STRING)
                        /* Read DIE ID and generate string descriptor in RAM */
                        USBCOMP_ReadDieID(USBCOMP_idSerialNumberStringDescriptor);
                        pStr = USBCOMP_idSerialNumberStringDescriptor;
                    #elif defined(USBCOMP_ENABLE_FWSN_STRING)
                        
                        if(USBCOMP_snStringConfirm != USBCOMP_FALSE)
                        {
                            pStr = USBCOMP_fwSerialNumberStringDescriptor;
                        }
                        else
                        {
                            pStr = (volatile uint8 *)&USBCOMP_SN_STRING_DESCRIPTOR[0u];
                        }
                    #else
                        pStr = (volatile uint8 *)&USBCOMP_SN_STRING_DESCRIPTOR[0u];
                    #endif  /* (USBCOMP_ENABLE_IDSN_STRING) */
                    }
                #endif /* (USBCOMP_ENABLE_SN_STRING) */
                
                    if (*pStr != 0u)
                    {
                        USBCOMP_currentTD.count = *pStr;
                        USBCOMP_currentTD.pData = pStr;
                        requestHandled  = USBCOMP_InitControlRead();
                    }
                }
            #endif /*  USBCOMP_ENABLE_STRINGS */
                else
                {
                    requestHandled = USBCOMP_DispatchClassRqst();
                }
                break;
                
            case USBCOMP_GET_STATUS:
                switch (bmRequestType & USBCOMP_RQST_RCPT_MASK)
                {
                    case USBCOMP_RQST_RCPT_EP:
                        USBCOMP_currentTD.count = USBCOMP_EP_STATUS_LENGTH;
                        USBCOMP_tBuffer[0u]     = USBCOMP_EP[USBCOMP_wIndexLoReg & USBCOMP_DIR_UNUSED].hwEpState;
                        USBCOMP_tBuffer[1u]     = 0u;
                        USBCOMP_currentTD.pData = &USBCOMP_tBuffer[0u];
                        
                        requestHandled  = USBCOMP_InitControlRead();
                        break;
                    case USBCOMP_RQST_RCPT_DEV:
                        USBCOMP_currentTD.count = USBCOMP_DEVICE_STATUS_LENGTH;
                        USBCOMP_tBuffer[0u]     = USBCOMP_deviceStatus;
                        USBCOMP_tBuffer[1u]     = 0u;
                        USBCOMP_currentTD.pData = &USBCOMP_tBuffer[0u];
                        
                        requestHandled  = USBCOMP_InitControlRead();
                        break;
                    default:    /* requestHandled is initialized as FALSE by default */
                        break;
                }
                break;
                
            case USBCOMP_GET_CONFIGURATION:
                USBCOMP_currentTD.count = 1u;
                USBCOMP_currentTD.pData = (volatile uint8 *) &USBCOMP_configuration;
                requestHandled  = USBCOMP_InitControlRead();
                break;
                
            case USBCOMP_GET_INTERFACE:
                USBCOMP_currentTD.count = 1u;
                USBCOMP_currentTD.pData = (volatile uint8 *) &USBCOMP_interfaceSetting[USBCOMP_wIndexLoReg];
                requestHandled  = USBCOMP_InitControlRead();
                break;
                
            default: /* requestHandled is initialized as FALSE by default */
                break;
        }
    }
    else
    {
        /* Control Write */
        switch (USBCOMP_bRequestReg)
        {
            case USBCOMP_SET_ADDRESS:
                /* Store address to be set in USBCOMP_NoDataControlStatusStage(). */
                USBCOMP_deviceAddress = (uint8) USBCOMP_wValueLoReg;
                requestHandled = USBCOMP_InitNoDataControlTransfer();
                break;
                
            case USBCOMP_SET_CONFIGURATION:
                configurationN = USBCOMP_wValueLoReg;
                
                /* Verify that configuration descriptor exists */
                if(configurationN > 0u)
                {
                    pTmp = USBCOMP_GetConfigTablePtr((uint8) configurationN - 1u);
                }
                
                /* Responds with a Request Error when configuration number is invalid */
                if (((configurationN > 0u) && (pTmp != NULL)) || (configurationN == 0u))
                {
                    /* Set new configuration if it has been changed */
                    if(configurationN != USBCOMP_configuration)
                    {
                        USBCOMP_configuration = (uint8) configurationN;
                        USBCOMP_configurationChanged = USBCOMP_TRUE;
                        USBCOMP_Config(USBCOMP_TRUE);
                    }
                    requestHandled = USBCOMP_InitNoDataControlTransfer();
                }
                break;
                
            case USBCOMP_SET_INTERFACE:
                if (0u != USBCOMP_ValidateAlternateSetting())
                {
                    /* Get interface number from the request. */
                    interfaceNumber = USBCOMP_wIndexLoReg;
                    USBCOMP_interfaceNumber = (uint8) USBCOMP_wIndexLoReg;
                     
                    /* Check if alternate settings is changed for interface. */
                    if (USBCOMP_interfaceSettingLast[interfaceNumber] != USBCOMP_interfaceSetting[interfaceNumber])
                    {
                        USBCOMP_configurationChanged = USBCOMP_TRUE;
                    
                        /* Change alternate setting for the endpoints. */
                    #if (USBCOMP_EP_MANAGEMENT_MANUAL && USBCOMP_EP_ALLOC_DYNAMIC)
                        USBCOMP_Config(USBCOMP_FALSE);
                    #else
                        USBCOMP_ConfigAltChanged();
                    #endif /* (USBCOMP_EP_MANAGEMENT_MANUAL && USBCOMP_EP_ALLOC_DYNAMIC) */
                    }
                    
                    requestHandled = USBCOMP_InitNoDataControlTransfer();
                }
                break;
                
            case USBCOMP_CLEAR_FEATURE:
                switch (bmRequestType & USBCOMP_RQST_RCPT_MASK)
                {
                    case USBCOMP_RQST_RCPT_EP:
                        if (USBCOMP_wValueLoReg == USBCOMP_ENDPOINT_HALT)
                        {
                            requestHandled = USBCOMP_ClearEndpointHalt();
                        }
                        break;
                    case USBCOMP_RQST_RCPT_DEV:
                        /* Clear device REMOTE_WAKEUP */
                        if (USBCOMP_wValueLoReg == USBCOMP_DEVICE_REMOTE_WAKEUP)
                        {
                            USBCOMP_deviceStatus &= (uint8)~USBCOMP_DEVICE_STATUS_REMOTE_WAKEUP;
                            requestHandled = USBCOMP_InitNoDataControlTransfer();
                        }
                        break;
                    case USBCOMP_RQST_RCPT_IFC:
                        /* Validate interfaceNumber */
                        if (USBCOMP_wIndexLoReg < USBCOMP_MAX_INTERFACES_NUMBER)
                        {
                            USBCOMP_interfaceStatus[USBCOMP_wIndexLoReg] &= (uint8) ~USBCOMP_wValueLoReg;
                            requestHandled = USBCOMP_InitNoDataControlTransfer();
                        }
                        break;
                    default:    /* requestHandled is initialized as FALSE by default */
                        break;
                }
                break;
                
            case USBCOMP_SET_FEATURE:
                switch (bmRequestType & USBCOMP_RQST_RCPT_MASK)
                {
                    case USBCOMP_RQST_RCPT_EP:
                        if (USBCOMP_wValueLoReg == USBCOMP_ENDPOINT_HALT)
                        {
                            requestHandled = USBCOMP_SetEndpointHalt();
                        }
                        break;
                        
                    case USBCOMP_RQST_RCPT_DEV:
                        /* Set device REMOTE_WAKEUP */
                        if (USBCOMP_wValueLoReg == USBCOMP_DEVICE_REMOTE_WAKEUP)
                        {
                            USBCOMP_deviceStatus |= USBCOMP_DEVICE_STATUS_REMOTE_WAKEUP;
                            requestHandled = USBCOMP_InitNoDataControlTransfer();
                        }
                        break;
                        
                    case USBCOMP_RQST_RCPT_IFC:
                        /* Validate interfaceNumber */
                        if (USBCOMP_wIndexLoReg < USBCOMP_MAX_INTERFACES_NUMBER)
                        {
                            USBCOMP_interfaceStatus[USBCOMP_wIndexLoReg] &= (uint8) ~USBCOMP_wValueLoReg;
                            requestHandled = USBCOMP_InitNoDataControlTransfer();
                        }
                        break;
                    
                    default:    /* requestHandled is initialized as FALSE by default */
                        break;
                }
                break;
                
            default:    /* requestHandled is initialized as FALSE by default */
                break;
        }
    }
    
    return (requestHandled);
}


#if defined(USBCOMP_ENABLE_IDSN_STRING)
    /***************************************************************************
    * Function Name: USBCOMP_ReadDieID
    ************************************************************************//**
    *
    *  This routine read Die ID and generate Serial Number string descriptor.
    *
    *  \param descr:  pointer on string descriptor. This string size has to be equal or
    *          greater than USBCOMP_IDSN_DESCR_LENGTH.
    *
    *
    * \reentrant
    *  No.
    *
    ***************************************************************************/
    void USBCOMP_ReadDieID(uint8 descr[]) 
    {
        const char8 CYCODE hex[] = "0123456789ABCDEF";
        uint32 i;
        uint32 j = 0u;
        uint8 uniqueId[8u];

        if (NULL != descr)
        {
            /* Initialize descriptor header. */
            descr[0u] = USBCOMP_IDSN_DESCR_LENGTH;
            descr[1u] = USBCOMP_DESCR_STRING;
            
            /* Unique ID size is 8 bytes. */
            CyGetUniqueId((uint32 *) uniqueId);

            /* Fill descriptor with unique device ID. */
            for (i = 2u; i < USBCOMP_IDSN_DESCR_LENGTH; i += 4u)
            {
                descr[i]      = (uint8) hex[(uniqueId[j] >> 4u)];
                descr[i + 1u] = 0u;
                descr[i + 2u] = (uint8) hex[(uniqueId[j] & 0x0Fu)];
                descr[i + 3u] = 0u;
                ++j;
            }
        }
    }
#endif /* (USBCOMP_ENABLE_IDSN_STRING) */


/*******************************************************************************
* Function Name: USBCOMP_ConfigReg
****************************************************************************//**
*
*  This routine configures hardware registers from the variables.
*  It is called from USBCOMP_Config() function and from RestoreConfig
*  after Wakeup.
*
*******************************************************************************/
void USBCOMP_ConfigReg(void) 
{
    uint32 ep;

#if (USBCOMP_EP_MANAGEMENT_DMA_AUTO)
    uint8 epType = 0u;
#endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO) */

    /* Go thought all endpoints and set hardware configuration */
    for (ep = USBCOMP_EP1; ep < USBCOMP_MAX_EP; ++ep)
    {
        USBCOMP_ARB_EP_BASE.arbEp[ep].epCfg = USBCOMP_ARB_EPX_CFG_DEFAULT;
        
    #if (USBCOMP_EP_MANAGEMENT_DMA)
        /* Enable arbiter endpoint interrupt sources */
        USBCOMP_ARB_EP_BASE.arbEp[ep].epIntEn = USBCOMP_ARB_EPX_INT_MASK;
    #endif /* (USBCOMP_EP_MANAGEMENT_DMA) */
    
        if (USBCOMP_EP[ep].epMode != USBCOMP_MODE_DISABLE)
        {
            if (0u != (USBCOMP_EP[ep].addr & USBCOMP_DIR_IN))
            {
                USBCOMP_SIE_EP_BASE.sieEp[ep].epCr0 = USBCOMP_MODE_NAK_IN;
                
            #if (USBCOMP_EP_MANAGEMENT_DMA_AUTO && CY_PSOC4)
                /* Clear DMA_TERMIN for IN endpoint. */
                USBCOMP_ARB_EP_BASE.arbEp[ep].epIntEn &= (uint32) ~USBCOMP_ARB_EPX_INT_DMA_TERMIN;
            #endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO && CY_PSOC4) */
            }
            else
            {
                USBCOMP_SIE_EP_BASE.sieEp[ep].epCr0 = USBCOMP_MODE_NAK_OUT;

            #if (USBCOMP_EP_MANAGEMENT_DMA_AUTO)
                /* (CY_PSOC4): DMA_TERMIN for OUT endpoint is set above. */
                
                /* Prepare endpoint type mask. */
                epType |= (uint8) (0x01u << (ep - USBCOMP_EP1));
            #endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO) */
            }
        }
        else
        {
            USBCOMP_SIE_EP_BASE.sieEp[ep].epCr0 = USBCOMP_MODE_STALL_DATA_EP;
        }
        
    #if (!USBCOMP_EP_MANAGEMENT_DMA_AUTO)
        #if (CY_PSOC4)
            USBCOMP_ARB_EP16_BASE.arbEp[ep].rwRa16  = (uint32) USBCOMP_EP[ep].buffOffset;
            USBCOMP_ARB_EP16_BASE.arbEp[ep].rwWa16  = (uint32) USBCOMP_EP[ep].buffOffset;
        #else
            USBCOMP_ARB_EP_BASE.arbEp[ep].rwRa    = LO8(USBCOMP_EP[ep].buffOffset);
            USBCOMP_ARB_EP_BASE.arbEp[ep].rwRaMsb = HI8(USBCOMP_EP[ep].buffOffset);
            USBCOMP_ARB_EP_BASE.arbEp[ep].rwWa    = LO8(USBCOMP_EP[ep].buffOffset);
            USBCOMP_ARB_EP_BASE.arbEp[ep].rwWaMsb = HI8(USBCOMP_EP[ep].buffOffset);
        #endif /* (CY_PSOC4) */
    #endif /* (!USBCOMP_EP_MANAGEMENT_DMA_AUTO) */
    }

#if (USBCOMP_EP_MANAGEMENT_DMA_AUTO)
     /* BUF_SIZE depend on DMA_THRESS value:0x55-32 bytes  0x44-16 bytes 0x33-8 bytes 0x22-4 bytes 0x11-2 bytes */
    USBCOMP_BUF_SIZE_REG = USBCOMP_DMA_BUF_SIZE;

    /* Configure DMA burst threshold */
#if (CY_PSOC4)
    USBCOMP_DMA_THRES16_REG   = USBCOMP_DMA_BYTES_PER_BURST;
#else
    USBCOMP_DMA_THRES_REG     = USBCOMP_DMA_BYTES_PER_BURST;
    USBCOMP_DMA_THRES_MSB_REG = 0u;
#endif /* (CY_PSOC4) */
    USBCOMP_EP_ACTIVE_REG = USBCOMP_DEFAULT_ARB_INT_EN;
    USBCOMP_EP_TYPE_REG   = epType;
    
    /* Cfg_cmp bit set to 1 once configuration is complete. */
    /* Lock arbiter configtuation */
    USBCOMP_ARB_CFG_REG |= (uint32)  USBCOMP_ARB_CFG_CFG_CMP;
    /* Cfg_cmp bit set to 0 during configuration of PFSUSB Registers. */
    USBCOMP_ARB_CFG_REG &= (uint32) ~USBCOMP_ARB_CFG_CFG_CMP;

#endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO) */

    /* Enable interrupt SIE interurpt source from EP0-EP1 */
    USBCOMP_SIE_EP_INT_EN_REG = (uint32) USBCOMP_DEFAULT_SIE_EP_INT_EN;
}


/*******************************************************************************
* Function Name: USBCOMP_EpStateInit
****************************************************************************//**
*
*  This routine initialize state of Data end points based of its type: 
*   IN  - USBCOMP_IN_BUFFER_EMPTY (USBCOMP_EVENT_PENDING)
*   OUT - USBCOMP_OUT_BUFFER_EMPTY (USBCOMP_NO_EVENT_PENDING)
*
*******************************************************************************/
void USBCOMP_EpStateInit(void) 
{
    uint8 i;

    for (i = USBCOMP_EP1; i < USBCOMP_MAX_EP; i++)
    { 
        if (0u != (USBCOMP_EP[i].addr & USBCOMP_DIR_IN))
        {
            /* IN Endpoint */
            USBCOMP_EP[i].apiEpState = USBCOMP_EVENT_PENDING;
        }
        else
        {
            /* OUT Endpoint */
            USBCOMP_EP[i].apiEpState = USBCOMP_NO_EVENT_PENDING;
        }
    }
                    
}


/*******************************************************************************
* Function Name: USBCOMP_Config
****************************************************************************//**
*
*  This routine configures endpoints for the entire configuration by scanning
*  the configuration descriptor.
*
*  \param clearAltSetting: It configures the bAlternateSetting 0 for each interface.
*
* USBCOMP_interfaceClass - Initialized class array for each interface.
*   It is used for handling Class specific requests depend on interface class.
*   Different classes in multiple Alternate settings does not supported.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_Config(uint8 clearAltSetting) 
{
    uint8 ep;
    uint8 curEp;
    uint8 i;
    uint8 epType;
    const uint8 *pDescr;
    
    #if (!USBCOMP_EP_MANAGEMENT_DMA_AUTO)
        uint16 buffCount = 0u;
    #endif /* (!USBCOMP_EP_MANAGEMENT_DMA_AUTO) */

    const T_USBCOMP_LUT CYCODE *pTmp;
    const T_USBCOMP_EP_SETTINGS_BLOCK CYCODE *pEP;

    /* Clear endpoints settings */
    for (ep = 0u; ep < USBCOMP_MAX_EP; ++ep)
    {
        USBCOMP_EP[ep].attrib     = 0u;
        USBCOMP_EP[ep].hwEpState  = 0u;
        USBCOMP_EP[ep].epToggle   = 0u;
        USBCOMP_EP[ep].bufferSize = 0u;
        USBCOMP_EP[ep].interface  = 0u;
        USBCOMP_EP[ep].apiEpState = USBCOMP_NO_EVENT_PENDING;
        USBCOMP_EP[ep].epMode     = USBCOMP_MODE_DISABLE;   
    }

    /* Clear Alternate settings for all interfaces. */
    if (0u != clearAltSetting)
    {
        for (i = 0u; i < USBCOMP_MAX_INTERFACES_NUMBER; ++i)
        {
            USBCOMP_interfaceSetting[i]     = 0u;
            USBCOMP_interfaceSettingLast[i] = 0u;
        }
    }

    /* Init Endpoints and Device Status if configured */
    if (USBCOMP_configuration > 0u)
    {
        #if defined(USBCOMP_ENABLE_CDC_CLASS)
            uint8 cdcComNums = 0u;
        #endif  /* (USBCOMP_ENABLE_CDC_CLASS) */  

        pTmp = USBCOMP_GetConfigTablePtr(USBCOMP_configuration - 1u);
        
        /* Set Power status for current configuration */
        pDescr = (const uint8 *)pTmp->p_list;
        if ((pDescr[USBCOMP_CONFIG_DESCR_ATTRIB] & USBCOMP_CONFIG_DESCR_ATTRIB_SELF_POWERED) != 0u)
        {
            USBCOMP_deviceStatus |= (uint8)  USBCOMP_DEVICE_STATUS_SELF_POWERED;
        }
        else
        {
            USBCOMP_deviceStatus &= (uint8) ~USBCOMP_DEVICE_STATUS_SELF_POWERED;
        }
        
        /* Move to next element */
        pTmp = &pTmp[1u];
        ep = pTmp->c;  /* For this table, c is the number of endpoints configurations  */

        #if (USBCOMP_EP_MANAGEMENT_MANUAL && USBCOMP_EP_ALLOC_DYNAMIC)
            /* Configure for dynamic EP memory allocation */
            /* p_list points the endpoint setting table. */
            pEP = (T_USBCOMP_EP_SETTINGS_BLOCK *) pTmp->p_list;
            
            for (i = 0u; i < ep; i++)
            {     
                /* Compare current Alternate setting with EP Alt */
                if (USBCOMP_interfaceSetting[pEP->interface] == pEP->altSetting)
                {                                                          
                    curEp  = pEP->addr & USBCOMP_DIR_UNUSED;
                    epType = pEP->attributes & USBCOMP_EP_TYPE_MASK;
                    
                    USBCOMP_EP[curEp].addr       = pEP->addr;
                    USBCOMP_EP[curEp].attrib     = pEP->attributes;
                    USBCOMP_EP[curEp].bufferSize = pEP->bufferSize;

                    if (0u != (pEP->addr & USBCOMP_DIR_IN))
                    {
                        /* IN Endpoint */
                        USBCOMP_EP[curEp].epMode     = USBCOMP_GET_ACTIVE_IN_EP_CR0_MODE(epType);
                        USBCOMP_EP[curEp].apiEpState = USBCOMP_EVENT_PENDING;
                    
                    #if (defined(USBCOMP_ENABLE_MIDI_STREAMING) && (USBCOMP_MIDI_IN_BUFF_SIZE > 0))
                        if ((pEP->bMisc == USBCOMP_CLASS_AUDIO) && (epType == USBCOMP_EP_TYPE_BULK))
                        {
                            USBCOMP_midi_in_ep = curEp;
                        }
                    #endif  /* (USBCOMP_ENABLE_MIDI_STREAMING) */
                    }
                    else
                    {
                        /* OUT Endpoint */
                        USBCOMP_EP[curEp].epMode     = USBCOMP_GET_ACTIVE_OUT_EP_CR0_MODE(epType);
                        USBCOMP_EP[curEp].apiEpState = USBCOMP_NO_EVENT_PENDING;
                        
                    #if (defined(USBCOMP_ENABLE_MIDI_STREAMING) && (USBCOMP_MIDI_OUT_BUFF_SIZE > 0))
                        if ((pEP->bMisc == USBCOMP_CLASS_AUDIO) && (epType == USBCOMP_EP_TYPE_BULK))
                        {
                            USBCOMP_midi_out_ep = curEp;
                        }
                    #endif  /* (USBCOMP_ENABLE_MIDI_STREAMING) */
                    }

                #if(defined (USBCOMP_ENABLE_CDC_CLASS))
                    if((pEP->bMisc == USBCOMP_CLASS_CDC_DATA) ||(pEP->bMisc == USBCOMP_CLASS_CDC))
                    {
                        cdcComNums = USBCOMP_Cdc_EpInit(pEP, curEp, cdcComNums);
                    }
                #endif  /* (USBCOMP_ENABLE_CDC_CLASS) */
                }
                
                pEP = &pEP[1u];
            }
            
        #else
            for (i = USBCOMP_EP1; i < USBCOMP_MAX_EP; ++i)
            {
                /* p_list points the endpoint setting table. */
                pEP = (const T_USBCOMP_EP_SETTINGS_BLOCK CYCODE *) pTmp->p_list;
                /* Find max length for each EP and select it (length could be different in different Alt settings) */
                /* but other settings should be correct with regards to Interface alt Setting */
                
                for (curEp = 0u; curEp < ep; ++curEp)
                {
                    if (i == (pEP->addr & USBCOMP_DIR_UNUSED))
                    {
                        /* Compare endpoint buffers size with current size to find greater. */
                        if (USBCOMP_EP[i].bufferSize < pEP->bufferSize)
                        {
                            USBCOMP_EP[i].bufferSize = pEP->bufferSize;
                        }
                        
                        /* Compare current Alternate setting with EP Alt */
                        if (USBCOMP_interfaceSetting[pEP->interface] == pEP->altSetting)
                        {                            
                            USBCOMP_EP[i].addr = pEP->addr;
                            USBCOMP_EP[i].attrib = pEP->attributes;
                            
                            epType = pEP->attributes & USBCOMP_EP_TYPE_MASK;
                            
                            if (0u != (pEP->addr & USBCOMP_DIR_IN))
                            {
                                /* IN Endpoint */
                                USBCOMP_EP[i].epMode     = USBCOMP_GET_ACTIVE_IN_EP_CR0_MODE(epType);
                                USBCOMP_EP[i].apiEpState = USBCOMP_EVENT_PENDING;
                                
                            #if (defined(USBCOMP_ENABLE_MIDI_STREAMING) && (USBCOMP_MIDI_IN_BUFF_SIZE > 0))
                                if ((pEP->bMisc == USBCOMP_CLASS_AUDIO) && (epType == USBCOMP_EP_TYPE_BULK))
                                {
                                    USBCOMP_midi_in_ep = i;
                                }
                            #endif  /* (USBCOMP_ENABLE_MIDI_STREAMING) */
                            }
                            else
                            {
                                /* OUT Endpoint */
                                USBCOMP_EP[i].epMode     = USBCOMP_GET_ACTIVE_OUT_EP_CR0_MODE(epType);
                                USBCOMP_EP[i].apiEpState = USBCOMP_NO_EVENT_PENDING;
                                
                            #if (defined(USBCOMP_ENABLE_MIDI_STREAMING) && (USBCOMP_MIDI_OUT_BUFF_SIZE > 0))
                                if ((pEP->bMisc == USBCOMP_CLASS_AUDIO) && (epType == USBCOMP_EP_TYPE_BULK))
                                {
                                    USBCOMP_midi_out_ep = i;
                                }
                            #endif  /* (USBCOMP_ENABLE_MIDI_STREAMING) */
                            }

                        #if (defined(USBCOMP_ENABLE_CDC_CLASS))
                            if((pEP->bMisc == USBCOMP_CLASS_CDC_DATA) ||(pEP->bMisc == USBCOMP_CLASS_CDC))
                            {
                                cdcComNums = USBCOMP_Cdc_EpInit(pEP, i, cdcComNums);
                            }
                        #endif  /* (USBCOMP_ENABLE_CDC_CLASS) */

                            #if (USBCOMP_EP_MANAGEMENT_DMA_AUTO)
                                break;  /* Use first EP setting in Auto memory management */
                            #endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO) */
                        }
                    }
                    
                    pEP = &pEP[1u];
                }
            }
        #endif /*  (USBCOMP_EP_MANAGEMENT_MANUAL && USBCOMP_EP_ALLOC_DYNAMIC) */

        /* Init class array for each interface and interface number for each EP.
        *  It is used for handling Class specific requests directed to either an
        *  interface or the endpoint.
        */
        /* p_list points the endpoint setting table. */
        pEP = (const T_USBCOMP_EP_SETTINGS_BLOCK CYCODE *) pTmp->p_list;
        for (i = 0u; i < ep; i++)
        {
            /* Configure interface number for each EP */
            USBCOMP_EP[pEP->addr & USBCOMP_DIR_UNUSED].interface = pEP->interface;
            pEP = &pEP[1u];
        }
        
        /* Init pointer on interface class table */
        USBCOMP_interfaceClass = USBCOMP_GetInterfaceClassTablePtr();
        
    /* Set the endpoint buffer addresses */
    #if (!USBCOMP_EP_MANAGEMENT_DMA_AUTO)
        buffCount = 0u;
        for (ep = USBCOMP_EP1; ep < USBCOMP_MAX_EP; ++ep)
        {
            USBCOMP_EP[ep].buffOffset = buffCount;        
            buffCount += USBCOMP_EP[ep].bufferSize;
            
        #if (USBCOMP_GEN_16BITS_EP_ACCESS)
            /* Align EP buffers to be event size to access 16-bits DR register. */
            buffCount += (0u != (buffCount & 0x01u)) ? 1u : 0u;
        #endif /* (USBCOMP_GEN_16BITS_EP_ACCESS) */            
        }
    #endif /* (!USBCOMP_EP_MANAGEMENT_DMA_AUTO) */

        /* Configure hardware registers */
        USBCOMP_ConfigReg();
    }
}


/*******************************************************************************
* Function Name: USBCOMP_ConfigAltChanged
****************************************************************************//**
*
*  This routine update configuration for the required endpoints only.
*  It is called after SET_INTERFACE request when Static memory allocation used.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_ConfigAltChanged(void) 
{
    uint32 ep;
    uint32 curEp;
    uint32 epType;
    uint32 i;
    uint8 interfaceNum;

    const T_USBCOMP_LUT CYCODE *pTmp;
    const T_USBCOMP_EP_SETTINGS_BLOCK CYCODE *pEP;

    /* Init Endpoints and Device Status if configured */
    if (USBCOMP_configuration > 0u)
    {
        /* Get number of endpoints configurations (ep). */
        pTmp = USBCOMP_GetConfigTablePtr(USBCOMP_configuration - 1u);
        pTmp = &pTmp[1u];
        ep = pTmp->c;

        /* Get pointer to endpoints setting table (pEP). */
        pEP = (const T_USBCOMP_EP_SETTINGS_BLOCK CYCODE *) pTmp->p_list;
        
        /* Look through all possible endpoint configurations. Find endpoints 
        * which belong to current interface and alternate settings for 
        * re-configuration.
        */
        interfaceNum = USBCOMP_interfaceNumber;
        for (i = 0u; i < ep; i++)
        {
            /* Find endpoints which belong to current interface and alternate settings. */
            if ((interfaceNum == pEP->interface) && 
                (USBCOMP_interfaceSetting[interfaceNum] == pEP->altSetting))
            {
                curEp  = ((uint32) pEP->addr & USBCOMP_DIR_UNUSED);
                epType = ((uint32) pEP->attributes & USBCOMP_EP_TYPE_MASK);
                
                /* Change the SIE mode for the selected EP to NAK ALL */
                USBCOMP_EP[curEp].epToggle   = 0u;
                USBCOMP_EP[curEp].addr       = pEP->addr;
                USBCOMP_EP[curEp].attrib     = pEP->attributes;
                USBCOMP_EP[curEp].bufferSize = pEP->bufferSize;

                if (0u != (pEP->addr & USBCOMP_DIR_IN))
                {
                    /* IN Endpoint */
                    USBCOMP_EP[curEp].epMode     = USBCOMP_GET_ACTIVE_IN_EP_CR0_MODE(epType);
                    USBCOMP_EP[curEp].apiEpState = USBCOMP_EVENT_PENDING;
                }
                else
                {
                    /* OUT Endpoint */
                    USBCOMP_EP[curEp].epMode     = USBCOMP_GET_ACTIVE_OUT_EP_CR0_MODE(epType);
                    USBCOMP_EP[curEp].apiEpState = USBCOMP_NO_EVENT_PENDING;
                }
                
                /* Make SIE to NAK any endpoint requests */
                USBCOMP_SIE_EP_BASE.sieEp[curEp].epCr0 = USBCOMP_MODE_NAK_IN_OUT;

            #if (USBCOMP_EP_MANAGEMENT_DMA_AUTO)
                /* Clear IN data ready. */
                USBCOMP_ARB_EP_BASE.arbEp[curEp].epCfg &= (uint32) ~USBCOMP_ARB_EPX_CFG_IN_DATA_RDY;

                /* Select endpoint number of reconfiguration */
                USBCOMP_DYN_RECONFIG_REG = (uint32) ((curEp - 1u) << USBCOMP_DYN_RECONFIG_EP_SHIFT);
                
                /* Request for dynamic re-configuration of endpoint. */
                USBCOMP_DYN_RECONFIG_REG |= USBCOMP_DYN_RECONFIG_ENABLE;
                
                /* Wait until block is ready for re-configuration */
                while (0u == (USBCOMP_DYN_RECONFIG_REG & USBCOMP_DYN_RECONFIG_RDY_STS))
                {
                }
                
                /* Once DYN_RECONFIG_RDY_STS bit is set, FW can change the EP configuration. */
                /* Change EP Type with new direction */
                if (0u != (pEP->addr & USBCOMP_DIR_IN))
                {
                    /* Set endpoint type: 0 - IN and 1 - OUT. */
                    USBCOMP_EP_TYPE_REG &= (uint32) ~(uint32)((uint32) 0x01u << (curEp - 1u));
                    
                #if (CY_PSOC4)
                    /* Clear DMA_TERMIN for IN endpoint */
                    USBCOMP_ARB_EP_BASE.arbEp[curEp].epIntEn &= (uint32) ~USBCOMP_ARB_EPX_INT_DMA_TERMIN;
                #endif /* (CY_PSOC4) */
                }
                else
                {
                    /* Set endpoint type: 0 - IN and 1- OUT. */
                    USBCOMP_EP_TYPE_REG |= (uint32) ((uint32) 0x01u << (curEp - 1u));
                    
                #if (CY_PSOC4)
                    /* Set DMA_TERMIN for OUT endpoint */
                    USBCOMP_ARB_EP_BASE.arbEp[curEp].epIntEn |= (uint32) USBCOMP_ARB_EPX_INT_DMA_TERMIN;
                #endif /* (CY_PSOC4) */
                }
                
                /* Complete dynamic re-configuration: all endpoint related status and signals 
                * are set into the default state.
                */
                USBCOMP_DYN_RECONFIG_REG &= (uint32) ~USBCOMP_DYN_RECONFIG_ENABLE;

            #else
                USBCOMP_SIE_EP_BASE.sieEp[curEp].epCnt0 = HI8(USBCOMP_EP[curEp].bufferSize);
                USBCOMP_SIE_EP_BASE.sieEp[curEp].epCnt1 = LO8(USBCOMP_EP[curEp].bufferSize);
                
                #if (CY_PSOC4)
                    USBCOMP_ARB_EP16_BASE.arbEp[curEp].rwRa16  = (uint32) USBCOMP_EP[curEp].buffOffset;
                    USBCOMP_ARB_EP16_BASE.arbEp[curEp].rwWa16  = (uint32) USBCOMP_EP[curEp].buffOffset;
                #else
                    USBCOMP_ARB_EP_BASE.arbEp[curEp].rwRa    = LO8(USBCOMP_EP[curEp].buffOffset);
                    USBCOMP_ARB_EP_BASE.arbEp[curEp].rwRaMsb = HI8(USBCOMP_EP[curEp].buffOffset);
                    USBCOMP_ARB_EP_BASE.arbEp[curEp].rwWa    = LO8(USBCOMP_EP[curEp].buffOffset);
                    USBCOMP_ARB_EP_BASE.arbEp[curEp].rwWaMsb = HI8(USBCOMP_EP[curEp].buffOffset);
                #endif /* (CY_PSOC4) */                
            #endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO) */
            }
            
            pEP = &pEP[1u]; /* Get next EP element */
        }
        
        /* The main loop has to re-enable DMA and OUT endpoint */
    }
}


/*******************************************************************************
* Function Name: USBCOMP_GetConfigTablePtr
****************************************************************************//**
*
*  This routine returns a pointer a configuration table entry
*
*  \param confIndex:  Configuration Index
*
* \return
*  Device Descriptor pointer or NULL when descriptor does not exist.
*
*******************************************************************************/
const T_USBCOMP_LUT CYCODE *USBCOMP_GetConfigTablePtr(uint8 confIndex)
                                                        
{
    /* Device Table */
    const T_USBCOMP_LUT CYCODE *pTmp;

    pTmp = (const T_USBCOMP_LUT CYCODE *) USBCOMP_TABLE[USBCOMP_device].p_list;

    /* The first entry points to the Device Descriptor,
    *  the second entry point to the BOS Descriptor
    *  the rest configuration entries.
    *  Set pointer to the first Configuration Descriptor
    */
    pTmp = &pTmp[2u];
    /* For this table, c is the number of configuration descriptors  */
    if(confIndex >= pTmp->c)   /* Verify that required configuration descriptor exists */
    {
        pTmp = (const T_USBCOMP_LUT CYCODE *) NULL;
    }
    else
    {
        pTmp = (const T_USBCOMP_LUT CYCODE *) pTmp[confIndex].p_list;
    }

    return (pTmp);
}


#if (USBCOMP_BOS_ENABLE)
    /*******************************************************************************
    * Function Name: USBCOMP_GetBOSPtr
    ****************************************************************************//**
    *
    *  This routine returns a pointer a BOS table entry
    *
    *  
    *
    * \return
    *  BOS Descriptor pointer or NULL when descriptor does not exist.
    *
    *******************************************************************************/
    const T_USBCOMP_LUT CYCODE *USBCOMP_GetBOSPtr(void)
                                                            
    {
        /* Device Table */
        const T_USBCOMP_LUT CYCODE *pTmp;

        pTmp = (const T_USBCOMP_LUT CYCODE *) USBCOMP_TABLE[USBCOMP_device].p_list;

        /* The first entry points to the Device Descriptor,
        *  the second entry points to the BOS Descriptor
        */
        pTmp = &pTmp[1u];
        pTmp = (const T_USBCOMP_LUT CYCODE *) pTmp->p_list;
        return (pTmp);
    }
#endif /* (USBCOMP_BOS_ENABLE) */


/*******************************************************************************
* Function Name: USBCOMP_GetDeviceTablePtr
****************************************************************************//**
*
*  This routine returns a pointer to the Device table
*
* \return
*  Device Table pointer
*
*******************************************************************************/
const T_USBCOMP_LUT CYCODE *USBCOMP_GetDeviceTablePtr(void)
                                                            
{
    /* Device Table */
    return( (const T_USBCOMP_LUT CYCODE *) USBCOMP_TABLE[USBCOMP_device].p_list );
}


/*******************************************************************************
* Function Name: USB_GetInterfaceClassTablePtr
****************************************************************************//**
*
*  This routine returns Interface Class table pointer, which contains
*  the relation between interface number and interface class.
*
* \return
*  Interface Class table pointer.
*
*******************************************************************************/
const uint8 CYCODE *USBCOMP_GetInterfaceClassTablePtr(void)
                                                        
{
    const T_USBCOMP_LUT CYCODE *pTmp;
    const uint8 CYCODE *pInterfaceClass;
    uint8 currentInterfacesNum;

    pTmp = USBCOMP_GetConfigTablePtr(USBCOMP_configuration - 1u);
    if (pTmp != NULL)
    {
        currentInterfacesNum  = ((const uint8 *) pTmp->p_list)[USBCOMP_CONFIG_DESCR_NUM_INTERFACES];
        /* Third entry in the LUT starts the Interface Table pointers */
        /* The INTERFACE_CLASS table is located after all interfaces */
        pTmp = &pTmp[currentInterfacesNum + 2u];
        pInterfaceClass = (const uint8 CYCODE *) pTmp->p_list;
    }
    else
    {
        pInterfaceClass = (const uint8 CYCODE *) NULL;
    }

    return (pInterfaceClass);
}


/*******************************************************************************
* Function Name: USBCOMP_TerminateEP
****************************************************************************//**
*
*  This function terminates the specified USBFS endpoint.
*  This function should be used before endpoint reconfiguration.
*
*  \param ep Contains the data endpoint number.
*
*  \reentrant
*  No.
*
* \sideeffect
* 
* The device responds with a NAK for any transactions on the selected endpoint.
*   
*******************************************************************************/
void USBCOMP_TerminateEP(uint8 epNumber) 
{
    /* Get endpoint number */
    epNumber &= USBCOMP_DIR_UNUSED;

    if ((epNumber > USBCOMP_EP0) && (epNumber < USBCOMP_MAX_EP))
    {
        /* Set the endpoint Halt */
        USBCOMP_EP[epNumber].hwEpState |= USBCOMP_ENDPOINT_STATUS_HALT;

        /* Clear the data toggle */
        USBCOMP_EP[epNumber].epToggle = 0u;
        USBCOMP_EP[epNumber].apiEpState = USBCOMP_NO_EVENT_ALLOWED;

        if ((USBCOMP_EP[epNumber].addr & USBCOMP_DIR_IN) != 0u)
        {   
            /* IN Endpoint */
            USBCOMP_SIE_EP_BASE.sieEp[epNumber].epCr0 = USBCOMP_MODE_NAK_IN;
        }
        else
        {
            /* OUT Endpoint */
            USBCOMP_SIE_EP_BASE.sieEp[epNumber].epCr0 = USBCOMP_MODE_NAK_OUT;
        }
    }
}


/*******************************************************************************
* Function Name: USBCOMP_SetEndpointHalt
****************************************************************************//**
*
*  This routine handles set endpoint halt.
*
* \return
*  requestHandled.
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBCOMP_SetEndpointHalt(void) 
{
    uint8 requestHandled = USBCOMP_FALSE;
    uint32 ep;
    
    /* Set endpoint halt */
    ep = USBCOMP_wIndexLoReg & USBCOMP_DIR_UNUSED;

    if ((ep > USBCOMP_EP0) && (ep < USBCOMP_MAX_EP))
    {
        /* Set the endpoint Halt */
        USBCOMP_EP[ep].hwEpState |= (USBCOMP_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USBCOMP_EP[ep].epToggle = 0u;
        USBCOMP_EP[ep].apiEpState |= USBCOMP_NO_EVENT_ALLOWED;

        if ((USBCOMP_EP[ep].addr & USBCOMP_DIR_IN) != 0u)
        {
            /* IN Endpoint */
            USBCOMP_SIE_EP_BASE.sieEp[ep].epCr0 = (USBCOMP_MODE_STALL_DATA_EP | 
                                                            USBCOMP_MODE_ACK_IN);
        }
        else
        {
            /* OUT Endpoint */
            USBCOMP_SIE_EP_BASE.sieEp[ep].epCr0 = (USBCOMP_MODE_STALL_DATA_EP | 
                                                            USBCOMP_MODE_ACK_OUT);
        }
        requestHandled = USBCOMP_InitNoDataControlTransfer();
    }

    return (requestHandled);
}


/*******************************************************************************
* Function Name: USBCOMP_ClearEndpointHalt
****************************************************************************//**
*
*  This routine handles clear endpoint halt.
*
* \return
*  requestHandled.
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBCOMP_ClearEndpointHalt(void) 
{
    uint8 requestHandled = USBCOMP_FALSE;
    uint32 ep;

    /* Clear endpoint halt */
    ep = USBCOMP_wIndexLoReg & USBCOMP_DIR_UNUSED;

    if ((ep > USBCOMP_EP0) && (ep < USBCOMP_MAX_EP))
    {
        /* Clear the endpoint Halt */
        USBCOMP_EP[ep].hwEpState &= (uint8) ~USBCOMP_ENDPOINT_STATUS_HALT;

        /* Clear the data toggle */
        USBCOMP_EP[ep].epToggle = 0u;
        
        /* Clear toggle bit for already armed packet */
        USBCOMP_SIE_EP_BASE.sieEp[ep].epCnt0 &= (uint32) ~(uint32)USBCOMP_EPX_CNT_DATA_TOGGLE;
        
        /* Return API State as it was defined before */
        USBCOMP_EP[ep].apiEpState &= (uint8) ~USBCOMP_NO_EVENT_ALLOWED;

        if ((USBCOMP_EP[ep].addr & USBCOMP_DIR_IN) != 0u)
        {
            /* IN Endpoint */
            if(USBCOMP_EP[ep].apiEpState == USBCOMP_IN_BUFFER_EMPTY)
            {       
                /* Wait for next packet from application */
                USBCOMP_SIE_EP_BASE.sieEp[ep].epCr0 = USBCOMP_MODE_NAK_IN;
            }
            else    /* Continue armed transfer */
            {
                USBCOMP_SIE_EP_BASE.sieEp[ep].epCr0 = USBCOMP_MODE_ACK_IN;
            }
        }
        else
        {
            /* OUT Endpoint */
            if (USBCOMP_EP[ep].apiEpState == USBCOMP_OUT_BUFFER_FULL)
            {       
                /* Allow application to read full buffer */
                USBCOMP_SIE_EP_BASE.sieEp[ep].epCr0 = USBCOMP_MODE_NAK_OUT;
            }
            else    /* Mark endpoint as empty, so it will be reloaded */
            {
                USBCOMP_SIE_EP_BASE.sieEp[ep].epCr0 = USBCOMP_MODE_ACK_OUT;
            }
        }
        
        requestHandled = USBCOMP_InitNoDataControlTransfer();
    }

    return(requestHandled);
}


/*******************************************************************************
* Function Name: USBCOMP_ValidateAlternateSetting
****************************************************************************//**
*
*  Validates (and records) a SET INTERFACE request.
*
* \return
*  requestHandled.
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBCOMP_ValidateAlternateSetting(void) 
{
    uint8 requestHandled = USBCOMP_FALSE;
    
    uint8 interfaceNum;
    uint8 curInterfacesNum;
    const T_USBCOMP_LUT CYCODE *pTmp;
    
    /* Get interface number from the request. */
    interfaceNum = (uint8) USBCOMP_wIndexLoReg;
    
    /* Get number of interfaces for current configuration. */
    pTmp = USBCOMP_GetConfigTablePtr(USBCOMP_configuration - 1u);
    curInterfacesNum  = ((const uint8 *) pTmp->p_list)[USBCOMP_CONFIG_DESCR_NUM_INTERFACES];

    /* Validate that interface number is within range. */
    if ((interfaceNum <= curInterfacesNum) || (interfaceNum <= USBCOMP_MAX_INTERFACES_NUMBER))
    {
        /* Save current and new alternate settings (come with request) to make 
        * desicion about following endpoint re-configuration.
        */
        USBCOMP_interfaceSettingLast[interfaceNum] = USBCOMP_interfaceSetting[interfaceNum];
        USBCOMP_interfaceSetting[interfaceNum]     = (uint8) USBCOMP_wValueLoReg;
        
        requestHandled = USBCOMP_TRUE;
    }

    return (requestHandled);
}


/* [] END OF FILE */
