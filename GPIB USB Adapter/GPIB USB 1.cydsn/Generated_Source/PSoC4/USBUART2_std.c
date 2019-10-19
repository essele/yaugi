/***************************************************************************//**
* \file USBUART2_std.c
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

#include "USBUART2_pvt.h"

/***************************************
*   Static data allocation
***************************************/

#if defined(USBUART2_ENABLE_FWSN_STRING)
    static volatile uint8* USBUART2_fwSerialNumberStringDescriptor;
    static volatile uint8  USBUART2_snStringConfirm = USBUART2_FALSE;
#endif  /* (USBUART2_ENABLE_FWSN_STRING) */

#if defined(USBUART2_ENABLE_FWSN_STRING)
    /***************************************************************************
    * Function Name: USBUART2_SerialNumString
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
    void  USBUART2_SerialNumString(uint8 snString[]) 
    {
        USBUART2_snStringConfirm = USBUART2_FALSE;
        
        if (snString != NULL)
        {
            /* Check descriptor validation */
            if ((snString[0u] > 1u) && (snString[1u] == USBUART2_DESCR_STRING))
            {
                USBUART2_fwSerialNumberStringDescriptor = snString;
                USBUART2_snStringConfirm = USBUART2_TRUE;
            }
        }
    }
#endif  /* USBUART2_ENABLE_FWSN_STRING */


/*******************************************************************************
* Function Name: USBUART2_HandleStandardRqst
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
uint8 USBUART2_HandleStandardRqst(void) 
{
    uint8 requestHandled = USBUART2_FALSE;
    uint32 interfaceNumber;
    uint32 configurationN;
    uint32 bmRequestType = USBUART2_bmRequestTypeReg;

#if defined(USBUART2_ENABLE_STRINGS)
    volatile uint8 *pStr = 0u;
    #if defined(USBUART2_ENABLE_DESCRIPTOR_STRINGS)
        uint8 nStr;
        uint8 descrLength;
    #endif /* (USBUART2_ENABLE_DESCRIPTOR_STRINGS) */
#endif /* (USBUART2_ENABLE_STRINGS) */
    
    static volatile uint8 USBUART2_tBuffer[USBUART2_STATUS_LENGTH_MAX];
    const T_USBUART2_LUT CYCODE *pTmp;

    USBUART2_currentTD.count = 0u;

    if (USBUART2_RQST_DIR_D2H == (bmRequestType & USBUART2_RQST_DIR_MASK))
    {
        /* Control Read */
        switch (USBUART2_bRequestReg)
        {
            case USBUART2_GET_DESCRIPTOR:
                if (USBUART2_DESCR_DEVICE ==USBUART2_wValueHiReg)
                {
                    pTmp = USBUART2_GetDeviceTablePtr();
                    USBUART2_currentTD.pData = (volatile uint8 *)pTmp->p_list;
                    USBUART2_currentTD.count = USBUART2_DEVICE_DESCR_LENGTH;
                    
                    requestHandled  = USBUART2_InitControlRead();
                }
                else if (USBUART2_DESCR_CONFIG == USBUART2_wValueHiReg)
                {
                    pTmp = USBUART2_GetConfigTablePtr((uint8) USBUART2_wValueLoReg);
                    
                    /* Verify that requested descriptor exists */
                    if (pTmp != NULL)
                    {
                        USBUART2_currentTD.pData = (volatile uint8 *)pTmp->p_list;
                        USBUART2_currentTD.count = (uint16)((uint16)(USBUART2_currentTD.pData)[USBUART2_CONFIG_DESCR_TOTAL_LENGTH_HI] << 8u) | \
                                                                            (USBUART2_currentTD.pData)[USBUART2_CONFIG_DESCR_TOTAL_LENGTH_LOW];
                        requestHandled  = USBUART2_InitControlRead();
                    }
                }
                
            #if(USBUART2_BOS_ENABLE)
                else if (USBUART2_DESCR_BOS == USBUART2_wValueHiReg)
                {
                    pTmp = USBUART2_GetBOSPtr();
                    
                    /* Verify that requested descriptor exists */
                    if (pTmp != NULL)
                    {
                        USBUART2_currentTD.pData = (volatile uint8 *)pTmp;
                        USBUART2_currentTD.count = ((uint16)((uint16)(USBUART2_currentTD.pData)[USBUART2_BOS_DESCR_TOTAL_LENGTH_HI] << 8u)) | \
                                                                             (USBUART2_currentTD.pData)[USBUART2_BOS_DESCR_TOTAL_LENGTH_LOW];
                        requestHandled  = USBUART2_InitControlRead();
                    }
                }
            #endif /*(USBUART2_BOS_ENABLE)*/
            
            #if defined(USBUART2_ENABLE_STRINGS)
                else if (USBUART2_DESCR_STRING == USBUART2_wValueHiReg)
                {
                /* Descriptor Strings */
                #if defined(USBUART2_ENABLE_DESCRIPTOR_STRINGS)
                    nStr = 0u;
                    pStr = (volatile uint8 *) &USBUART2_STRING_DESCRIPTORS[0u];
                    
                    while ((USBUART2_wValueLoReg > nStr) && (*pStr != 0u))
                    {
                        /* Read descriptor length from 1st byte */
                        descrLength = *pStr;
                        /* Move to next string descriptor */
                        pStr = &pStr[descrLength];
                        nStr++;
                    }
                #endif /* (USBUART2_ENABLE_DESCRIPTOR_STRINGS) */
                
                /* Microsoft OS String */
                #if defined(USBUART2_ENABLE_MSOS_STRING)
                    if (USBUART2_STRING_MSOS == USBUART2_wValueLoReg)
                    {
                        pStr = (volatile uint8 *)& USBUART2_MSOS_DESCRIPTOR[0u];
                    }
                #endif /* (USBUART2_ENABLE_MSOS_STRING) */
                
                /* SN string */
                #if defined(USBUART2_ENABLE_SN_STRING)
                    if ((USBUART2_wValueLoReg != 0u) && 
                        (USBUART2_wValueLoReg == USBUART2_DEVICE0_DESCR[USBUART2_DEVICE_DESCR_SN_SHIFT]))
                    {
                    #if defined(USBUART2_ENABLE_IDSN_STRING)
                        /* Read DIE ID and generate string descriptor in RAM */
                        USBUART2_ReadDieID(USBUART2_idSerialNumberStringDescriptor);
                        pStr = USBUART2_idSerialNumberStringDescriptor;
                    #elif defined(USBUART2_ENABLE_FWSN_STRING)
                        
                        if(USBUART2_snStringConfirm != USBUART2_FALSE)
                        {
                            pStr = USBUART2_fwSerialNumberStringDescriptor;
                        }
                        else
                        {
                            pStr = (volatile uint8 *)&USBUART2_SN_STRING_DESCRIPTOR[0u];
                        }
                    #else
                        pStr = (volatile uint8 *)&USBUART2_SN_STRING_DESCRIPTOR[0u];
                    #endif  /* (USBUART2_ENABLE_IDSN_STRING) */
                    }
                #endif /* (USBUART2_ENABLE_SN_STRING) */
                
                    if (*pStr != 0u)
                    {
                        USBUART2_currentTD.count = *pStr;
                        USBUART2_currentTD.pData = pStr;
                        requestHandled  = USBUART2_InitControlRead();
                    }
                }
            #endif /*  USBUART2_ENABLE_STRINGS */
                else
                {
                    requestHandled = USBUART2_DispatchClassRqst();
                }
                break;
                
            case USBUART2_GET_STATUS:
                switch (bmRequestType & USBUART2_RQST_RCPT_MASK)
                {
                    case USBUART2_RQST_RCPT_EP:
                        USBUART2_currentTD.count = USBUART2_EP_STATUS_LENGTH;
                        USBUART2_tBuffer[0u]     = USBUART2_EP[USBUART2_wIndexLoReg & USBUART2_DIR_UNUSED].hwEpState;
                        USBUART2_tBuffer[1u]     = 0u;
                        USBUART2_currentTD.pData = &USBUART2_tBuffer[0u];
                        
                        requestHandled  = USBUART2_InitControlRead();
                        break;
                    case USBUART2_RQST_RCPT_DEV:
                        USBUART2_currentTD.count = USBUART2_DEVICE_STATUS_LENGTH;
                        USBUART2_tBuffer[0u]     = USBUART2_deviceStatus;
                        USBUART2_tBuffer[1u]     = 0u;
                        USBUART2_currentTD.pData = &USBUART2_tBuffer[0u];
                        
                        requestHandled  = USBUART2_InitControlRead();
                        break;
                    default:    /* requestHandled is initialized as FALSE by default */
                        break;
                }
                break;
                
            case USBUART2_GET_CONFIGURATION:
                USBUART2_currentTD.count = 1u;
                USBUART2_currentTD.pData = (volatile uint8 *) &USBUART2_configuration;
                requestHandled  = USBUART2_InitControlRead();
                break;
                
            case USBUART2_GET_INTERFACE:
                USBUART2_currentTD.count = 1u;
                USBUART2_currentTD.pData = (volatile uint8 *) &USBUART2_interfaceSetting[USBUART2_wIndexLoReg];
                requestHandled  = USBUART2_InitControlRead();
                break;
                
            default: /* requestHandled is initialized as FALSE by default */
                break;
        }
    }
    else
    {
        /* Control Write */
        switch (USBUART2_bRequestReg)
        {
            case USBUART2_SET_ADDRESS:
                /* Store address to be set in USBUART2_NoDataControlStatusStage(). */
                USBUART2_deviceAddress = (uint8) USBUART2_wValueLoReg;
                requestHandled = USBUART2_InitNoDataControlTransfer();
                break;
                
            case USBUART2_SET_CONFIGURATION:
                configurationN = USBUART2_wValueLoReg;
                
                /* Verify that configuration descriptor exists */
                if(configurationN > 0u)
                {
                    pTmp = USBUART2_GetConfigTablePtr((uint8) configurationN - 1u);
                }
                
                /* Responds with a Request Error when configuration number is invalid */
                if (((configurationN > 0u) && (pTmp != NULL)) || (configurationN == 0u))
                {
                    /* Set new configuration if it has been changed */
                    if(configurationN != USBUART2_configuration)
                    {
                        USBUART2_configuration = (uint8) configurationN;
                        USBUART2_configurationChanged = USBUART2_TRUE;
                        USBUART2_Config(USBUART2_TRUE);
                    }
                    requestHandled = USBUART2_InitNoDataControlTransfer();
                }
                break;
                
            case USBUART2_SET_INTERFACE:
                if (0u != USBUART2_ValidateAlternateSetting())
                {
                    /* Get interface number from the request. */
                    interfaceNumber = USBUART2_wIndexLoReg;
                    USBUART2_interfaceNumber = (uint8) USBUART2_wIndexLoReg;
                     
                    /* Check if alternate settings is changed for interface. */
                    if (USBUART2_interfaceSettingLast[interfaceNumber] != USBUART2_interfaceSetting[interfaceNumber])
                    {
                        USBUART2_configurationChanged = USBUART2_TRUE;
                    
                        /* Change alternate setting for the endpoints. */
                    #if (USBUART2_EP_MANAGEMENT_MANUAL && USBUART2_EP_ALLOC_DYNAMIC)
                        USBUART2_Config(USBUART2_FALSE);
                    #else
                        USBUART2_ConfigAltChanged();
                    #endif /* (USBUART2_EP_MANAGEMENT_MANUAL && USBUART2_EP_ALLOC_DYNAMIC) */
                    }
                    
                    requestHandled = USBUART2_InitNoDataControlTransfer();
                }
                break;
                
            case USBUART2_CLEAR_FEATURE:
                switch (bmRequestType & USBUART2_RQST_RCPT_MASK)
                {
                    case USBUART2_RQST_RCPT_EP:
                        if (USBUART2_wValueLoReg == USBUART2_ENDPOINT_HALT)
                        {
                            requestHandled = USBUART2_ClearEndpointHalt();
                        }
                        break;
                    case USBUART2_RQST_RCPT_DEV:
                        /* Clear device REMOTE_WAKEUP */
                        if (USBUART2_wValueLoReg == USBUART2_DEVICE_REMOTE_WAKEUP)
                        {
                            USBUART2_deviceStatus &= (uint8)~USBUART2_DEVICE_STATUS_REMOTE_WAKEUP;
                            requestHandled = USBUART2_InitNoDataControlTransfer();
                        }
                        break;
                    case USBUART2_RQST_RCPT_IFC:
                        /* Validate interfaceNumber */
                        if (USBUART2_wIndexLoReg < USBUART2_MAX_INTERFACES_NUMBER)
                        {
                            USBUART2_interfaceStatus[USBUART2_wIndexLoReg] &= (uint8) ~USBUART2_wValueLoReg;
                            requestHandled = USBUART2_InitNoDataControlTransfer();
                        }
                        break;
                    default:    /* requestHandled is initialized as FALSE by default */
                        break;
                }
                break;
                
            case USBUART2_SET_FEATURE:
                switch (bmRequestType & USBUART2_RQST_RCPT_MASK)
                {
                    case USBUART2_RQST_RCPT_EP:
                        if (USBUART2_wValueLoReg == USBUART2_ENDPOINT_HALT)
                        {
                            requestHandled = USBUART2_SetEndpointHalt();
                        }
                        break;
                        
                    case USBUART2_RQST_RCPT_DEV:
                        /* Set device REMOTE_WAKEUP */
                        if (USBUART2_wValueLoReg == USBUART2_DEVICE_REMOTE_WAKEUP)
                        {
                            USBUART2_deviceStatus |= USBUART2_DEVICE_STATUS_REMOTE_WAKEUP;
                            requestHandled = USBUART2_InitNoDataControlTransfer();
                        }
                        break;
                        
                    case USBUART2_RQST_RCPT_IFC:
                        /* Validate interfaceNumber */
                        if (USBUART2_wIndexLoReg < USBUART2_MAX_INTERFACES_NUMBER)
                        {
                            USBUART2_interfaceStatus[USBUART2_wIndexLoReg] &= (uint8) ~USBUART2_wValueLoReg;
                            requestHandled = USBUART2_InitNoDataControlTransfer();
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


#if defined(USBUART2_ENABLE_IDSN_STRING)
    /***************************************************************************
    * Function Name: USBUART2_ReadDieID
    ************************************************************************//**
    *
    *  This routine read Die ID and generate Serial Number string descriptor.
    *
    *  \param descr:  pointer on string descriptor. This string size has to be equal or
    *          greater than USBUART2_IDSN_DESCR_LENGTH.
    *
    *
    * \reentrant
    *  No.
    *
    ***************************************************************************/
    void USBUART2_ReadDieID(uint8 descr[]) 
    {
        const char8 CYCODE hex[] = "0123456789ABCDEF";
        uint32 i;
        uint32 j = 0u;
        uint8 uniqueId[8u];

        if (NULL != descr)
        {
            /* Initialize descriptor header. */
            descr[0u] = USBUART2_IDSN_DESCR_LENGTH;
            descr[1u] = USBUART2_DESCR_STRING;
            
            /* Unique ID size is 8 bytes. */
            CyGetUniqueId((uint32 *) uniqueId);

            /* Fill descriptor with unique device ID. */
            for (i = 2u; i < USBUART2_IDSN_DESCR_LENGTH; i += 4u)
            {
                descr[i]      = (uint8) hex[(uniqueId[j] >> 4u)];
                descr[i + 1u] = 0u;
                descr[i + 2u] = (uint8) hex[(uniqueId[j] & 0x0Fu)];
                descr[i + 3u] = 0u;
                ++j;
            }
        }
    }
#endif /* (USBUART2_ENABLE_IDSN_STRING) */


/*******************************************************************************
* Function Name: USBUART2_ConfigReg
****************************************************************************//**
*
*  This routine configures hardware registers from the variables.
*  It is called from USBUART2_Config() function and from RestoreConfig
*  after Wakeup.
*
*******************************************************************************/
void USBUART2_ConfigReg(void) 
{
    uint32 ep;

#if (USBUART2_EP_MANAGEMENT_DMA_AUTO)
    uint8 epType = 0u;
#endif /* (USBUART2_EP_MANAGEMENT_DMA_AUTO) */

    /* Go thought all endpoints and set hardware configuration */
    for (ep = USBUART2_EP1; ep < USBUART2_MAX_EP; ++ep)
    {
        USBUART2_ARB_EP_BASE.arbEp[ep].epCfg = USBUART2_ARB_EPX_CFG_DEFAULT;
        
    #if (USBUART2_EP_MANAGEMENT_DMA)
        /* Enable arbiter endpoint interrupt sources */
        USBUART2_ARB_EP_BASE.arbEp[ep].epIntEn = USBUART2_ARB_EPX_INT_MASK;
    #endif /* (USBUART2_EP_MANAGEMENT_DMA) */
    
        if (USBUART2_EP[ep].epMode != USBUART2_MODE_DISABLE)
        {
            if (0u != (USBUART2_EP[ep].addr & USBUART2_DIR_IN))
            {
                USBUART2_SIE_EP_BASE.sieEp[ep].epCr0 = USBUART2_MODE_NAK_IN;
                
            #if (USBUART2_EP_MANAGEMENT_DMA_AUTO && CY_PSOC4)
                /* Clear DMA_TERMIN for IN endpoint. */
                USBUART2_ARB_EP_BASE.arbEp[ep].epIntEn &= (uint32) ~USBUART2_ARB_EPX_INT_DMA_TERMIN;
            #endif /* (USBUART2_EP_MANAGEMENT_DMA_AUTO && CY_PSOC4) */
            }
            else
            {
                USBUART2_SIE_EP_BASE.sieEp[ep].epCr0 = USBUART2_MODE_NAK_OUT;

            #if (USBUART2_EP_MANAGEMENT_DMA_AUTO)
                /* (CY_PSOC4): DMA_TERMIN for OUT endpoint is set above. */
                
                /* Prepare endpoint type mask. */
                epType |= (uint8) (0x01u << (ep - USBUART2_EP1));
            #endif /* (USBUART2_EP_MANAGEMENT_DMA_AUTO) */
            }
        }
        else
        {
            USBUART2_SIE_EP_BASE.sieEp[ep].epCr0 = USBUART2_MODE_STALL_DATA_EP;
        }
        
    #if (!USBUART2_EP_MANAGEMENT_DMA_AUTO)
        #if (CY_PSOC4)
            USBUART2_ARB_EP16_BASE.arbEp[ep].rwRa16  = (uint32) USBUART2_EP[ep].buffOffset;
            USBUART2_ARB_EP16_BASE.arbEp[ep].rwWa16  = (uint32) USBUART2_EP[ep].buffOffset;
        #else
            USBUART2_ARB_EP_BASE.arbEp[ep].rwRa    = LO8(USBUART2_EP[ep].buffOffset);
            USBUART2_ARB_EP_BASE.arbEp[ep].rwRaMsb = HI8(USBUART2_EP[ep].buffOffset);
            USBUART2_ARB_EP_BASE.arbEp[ep].rwWa    = LO8(USBUART2_EP[ep].buffOffset);
            USBUART2_ARB_EP_BASE.arbEp[ep].rwWaMsb = HI8(USBUART2_EP[ep].buffOffset);
        #endif /* (CY_PSOC4) */
    #endif /* (!USBUART2_EP_MANAGEMENT_DMA_AUTO) */
    }

#if (USBUART2_EP_MANAGEMENT_DMA_AUTO)
     /* BUF_SIZE depend on DMA_THRESS value:0x55-32 bytes  0x44-16 bytes 0x33-8 bytes 0x22-4 bytes 0x11-2 bytes */
    USBUART2_BUF_SIZE_REG = USBUART2_DMA_BUF_SIZE;

    /* Configure DMA burst threshold */
#if (CY_PSOC4)
    USBUART2_DMA_THRES16_REG   = USBUART2_DMA_BYTES_PER_BURST;
#else
    USBUART2_DMA_THRES_REG     = USBUART2_DMA_BYTES_PER_BURST;
    USBUART2_DMA_THRES_MSB_REG = 0u;
#endif /* (CY_PSOC4) */
    USBUART2_EP_ACTIVE_REG = USBUART2_DEFAULT_ARB_INT_EN;
    USBUART2_EP_TYPE_REG   = epType;
    
    /* Cfg_cmp bit set to 1 once configuration is complete. */
    /* Lock arbiter configtuation */
    USBUART2_ARB_CFG_REG |= (uint32)  USBUART2_ARB_CFG_CFG_CMP;
    /* Cfg_cmp bit set to 0 during configuration of PFSUSB Registers. */
    USBUART2_ARB_CFG_REG &= (uint32) ~USBUART2_ARB_CFG_CFG_CMP;

#endif /* (USBUART2_EP_MANAGEMENT_DMA_AUTO) */

    /* Enable interrupt SIE interurpt source from EP0-EP1 */
    USBUART2_SIE_EP_INT_EN_REG = (uint32) USBUART2_DEFAULT_SIE_EP_INT_EN;
}


/*******************************************************************************
* Function Name: USBUART2_EpStateInit
****************************************************************************//**
*
*  This routine initialize state of Data end points based of its type: 
*   IN  - USBUART2_IN_BUFFER_EMPTY (USBUART2_EVENT_PENDING)
*   OUT - USBUART2_OUT_BUFFER_EMPTY (USBUART2_NO_EVENT_PENDING)
*
*******************************************************************************/
void USBUART2_EpStateInit(void) 
{
    uint8 i;

    for (i = USBUART2_EP1; i < USBUART2_MAX_EP; i++)
    { 
        if (0u != (USBUART2_EP[i].addr & USBUART2_DIR_IN))
        {
            /* IN Endpoint */
            USBUART2_EP[i].apiEpState = USBUART2_EVENT_PENDING;
        }
        else
        {
            /* OUT Endpoint */
            USBUART2_EP[i].apiEpState = USBUART2_NO_EVENT_PENDING;
        }
    }
                    
}


/*******************************************************************************
* Function Name: USBUART2_Config
****************************************************************************//**
*
*  This routine configures endpoints for the entire configuration by scanning
*  the configuration descriptor.
*
*  \param clearAltSetting: It configures the bAlternateSetting 0 for each interface.
*
* USBUART2_interfaceClass - Initialized class array for each interface.
*   It is used for handling Class specific requests depend on interface class.
*   Different classes in multiple Alternate settings does not supported.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBUART2_Config(uint8 clearAltSetting) 
{
    uint8 ep;
    uint8 curEp;
    uint8 i;
    uint8 epType;
    const uint8 *pDescr;
    
    #if (!USBUART2_EP_MANAGEMENT_DMA_AUTO)
        uint16 buffCount = 0u;
    #endif /* (!USBUART2_EP_MANAGEMENT_DMA_AUTO) */

    const T_USBUART2_LUT CYCODE *pTmp;
    const T_USBUART2_EP_SETTINGS_BLOCK CYCODE *pEP;

    /* Clear endpoints settings */
    for (ep = 0u; ep < USBUART2_MAX_EP; ++ep)
    {
        USBUART2_EP[ep].attrib     = 0u;
        USBUART2_EP[ep].hwEpState  = 0u;
        USBUART2_EP[ep].epToggle   = 0u;
        USBUART2_EP[ep].bufferSize = 0u;
        USBUART2_EP[ep].interface  = 0u;
        USBUART2_EP[ep].apiEpState = USBUART2_NO_EVENT_PENDING;
        USBUART2_EP[ep].epMode     = USBUART2_MODE_DISABLE;   
    }

    /* Clear Alternate settings for all interfaces. */
    if (0u != clearAltSetting)
    {
        for (i = 0u; i < USBUART2_MAX_INTERFACES_NUMBER; ++i)
        {
            USBUART2_interfaceSetting[i]     = 0u;
            USBUART2_interfaceSettingLast[i] = 0u;
        }
    }

    /* Init Endpoints and Device Status if configured */
    if (USBUART2_configuration > 0u)
    {
        #if defined(USBUART2_ENABLE_CDC_CLASS)
            uint8 cdcComNums = 0u;
        #endif  /* (USBUART2_ENABLE_CDC_CLASS) */  

        pTmp = USBUART2_GetConfigTablePtr(USBUART2_configuration - 1u);
        
        /* Set Power status for current configuration */
        pDescr = (const uint8 *)pTmp->p_list;
        if ((pDescr[USBUART2_CONFIG_DESCR_ATTRIB] & USBUART2_CONFIG_DESCR_ATTRIB_SELF_POWERED) != 0u)
        {
            USBUART2_deviceStatus |= (uint8)  USBUART2_DEVICE_STATUS_SELF_POWERED;
        }
        else
        {
            USBUART2_deviceStatus &= (uint8) ~USBUART2_DEVICE_STATUS_SELF_POWERED;
        }
        
        /* Move to next element */
        pTmp = &pTmp[1u];
        ep = pTmp->c;  /* For this table, c is the number of endpoints configurations  */

        #if (USBUART2_EP_MANAGEMENT_MANUAL && USBUART2_EP_ALLOC_DYNAMIC)
            /* Configure for dynamic EP memory allocation */
            /* p_list points the endpoint setting table. */
            pEP = (T_USBUART2_EP_SETTINGS_BLOCK *) pTmp->p_list;
            
            for (i = 0u; i < ep; i++)
            {     
                /* Compare current Alternate setting with EP Alt */
                if (USBUART2_interfaceSetting[pEP->interface] == pEP->altSetting)
                {                                                          
                    curEp  = pEP->addr & USBUART2_DIR_UNUSED;
                    epType = pEP->attributes & USBUART2_EP_TYPE_MASK;
                    
                    USBUART2_EP[curEp].addr       = pEP->addr;
                    USBUART2_EP[curEp].attrib     = pEP->attributes;
                    USBUART2_EP[curEp].bufferSize = pEP->bufferSize;

                    if (0u != (pEP->addr & USBUART2_DIR_IN))
                    {
                        /* IN Endpoint */
                        USBUART2_EP[curEp].epMode     = USBUART2_GET_ACTIVE_IN_EP_CR0_MODE(epType);
                        USBUART2_EP[curEp].apiEpState = USBUART2_EVENT_PENDING;
                    
                    #if (defined(USBUART2_ENABLE_MIDI_STREAMING) && (USBUART2_MIDI_IN_BUFF_SIZE > 0))
                        if ((pEP->bMisc == USBUART2_CLASS_AUDIO) && (epType == USBUART2_EP_TYPE_BULK))
                        {
                            USBUART2_midi_in_ep = curEp;
                        }
                    #endif  /* (USBUART2_ENABLE_MIDI_STREAMING) */
                    }
                    else
                    {
                        /* OUT Endpoint */
                        USBUART2_EP[curEp].epMode     = USBUART2_GET_ACTIVE_OUT_EP_CR0_MODE(epType);
                        USBUART2_EP[curEp].apiEpState = USBUART2_NO_EVENT_PENDING;
                        
                    #if (defined(USBUART2_ENABLE_MIDI_STREAMING) && (USBUART2_MIDI_OUT_BUFF_SIZE > 0))
                        if ((pEP->bMisc == USBUART2_CLASS_AUDIO) && (epType == USBUART2_EP_TYPE_BULK))
                        {
                            USBUART2_midi_out_ep = curEp;
                        }
                    #endif  /* (USBUART2_ENABLE_MIDI_STREAMING) */
                    }

                #if(defined (USBUART2_ENABLE_CDC_CLASS))
                    if((pEP->bMisc == USBUART2_CLASS_CDC_DATA) ||(pEP->bMisc == USBUART2_CLASS_CDC))
                    {
                        cdcComNums = USBUART2_Cdc_EpInit(pEP, curEp, cdcComNums);
                    }
                #endif  /* (USBUART2_ENABLE_CDC_CLASS) */
                }
                
                pEP = &pEP[1u];
            }
            
        #else
            for (i = USBUART2_EP1; i < USBUART2_MAX_EP; ++i)
            {
                /* p_list points the endpoint setting table. */
                pEP = (const T_USBUART2_EP_SETTINGS_BLOCK CYCODE *) pTmp->p_list;
                /* Find max length for each EP and select it (length could be different in different Alt settings) */
                /* but other settings should be correct with regards to Interface alt Setting */
                
                for (curEp = 0u; curEp < ep; ++curEp)
                {
                    if (i == (pEP->addr & USBUART2_DIR_UNUSED))
                    {
                        /* Compare endpoint buffers size with current size to find greater. */
                        if (USBUART2_EP[i].bufferSize < pEP->bufferSize)
                        {
                            USBUART2_EP[i].bufferSize = pEP->bufferSize;
                        }
                        
                        /* Compare current Alternate setting with EP Alt */
                        if (USBUART2_interfaceSetting[pEP->interface] == pEP->altSetting)
                        {                            
                            USBUART2_EP[i].addr = pEP->addr;
                            USBUART2_EP[i].attrib = pEP->attributes;
                            
                            epType = pEP->attributes & USBUART2_EP_TYPE_MASK;
                            
                            if (0u != (pEP->addr & USBUART2_DIR_IN))
                            {
                                /* IN Endpoint */
                                USBUART2_EP[i].epMode     = USBUART2_GET_ACTIVE_IN_EP_CR0_MODE(epType);
                                USBUART2_EP[i].apiEpState = USBUART2_EVENT_PENDING;
                                
                            #if (defined(USBUART2_ENABLE_MIDI_STREAMING) && (USBUART2_MIDI_IN_BUFF_SIZE > 0))
                                if ((pEP->bMisc == USBUART2_CLASS_AUDIO) && (epType == USBUART2_EP_TYPE_BULK))
                                {
                                    USBUART2_midi_in_ep = i;
                                }
                            #endif  /* (USBUART2_ENABLE_MIDI_STREAMING) */
                            }
                            else
                            {
                                /* OUT Endpoint */
                                USBUART2_EP[i].epMode     = USBUART2_GET_ACTIVE_OUT_EP_CR0_MODE(epType);
                                USBUART2_EP[i].apiEpState = USBUART2_NO_EVENT_PENDING;
                                
                            #if (defined(USBUART2_ENABLE_MIDI_STREAMING) && (USBUART2_MIDI_OUT_BUFF_SIZE > 0))
                                if ((pEP->bMisc == USBUART2_CLASS_AUDIO) && (epType == USBUART2_EP_TYPE_BULK))
                                {
                                    USBUART2_midi_out_ep = i;
                                }
                            #endif  /* (USBUART2_ENABLE_MIDI_STREAMING) */
                            }

                        #if (defined(USBUART2_ENABLE_CDC_CLASS))
                            if((pEP->bMisc == USBUART2_CLASS_CDC_DATA) ||(pEP->bMisc == USBUART2_CLASS_CDC))
                            {
                                cdcComNums = USBUART2_Cdc_EpInit(pEP, i, cdcComNums);
                            }
                        #endif  /* (USBUART2_ENABLE_CDC_CLASS) */

                            #if (USBUART2_EP_MANAGEMENT_DMA_AUTO)
                                break;  /* Use first EP setting in Auto memory management */
                            #endif /* (USBUART2_EP_MANAGEMENT_DMA_AUTO) */
                        }
                    }
                    
                    pEP = &pEP[1u];
                }
            }
        #endif /*  (USBUART2_EP_MANAGEMENT_MANUAL && USBUART2_EP_ALLOC_DYNAMIC) */

        /* Init class array for each interface and interface number for each EP.
        *  It is used for handling Class specific requests directed to either an
        *  interface or the endpoint.
        */
        /* p_list points the endpoint setting table. */
        pEP = (const T_USBUART2_EP_SETTINGS_BLOCK CYCODE *) pTmp->p_list;
        for (i = 0u; i < ep; i++)
        {
            /* Configure interface number for each EP */
            USBUART2_EP[pEP->addr & USBUART2_DIR_UNUSED].interface = pEP->interface;
            pEP = &pEP[1u];
        }
        
        /* Init pointer on interface class table */
        USBUART2_interfaceClass = USBUART2_GetInterfaceClassTablePtr();
        
    /* Set the endpoint buffer addresses */
    #if (!USBUART2_EP_MANAGEMENT_DMA_AUTO)
        buffCount = 0u;
        for (ep = USBUART2_EP1; ep < USBUART2_MAX_EP; ++ep)
        {
            USBUART2_EP[ep].buffOffset = buffCount;        
            buffCount += USBUART2_EP[ep].bufferSize;
            
        #if (USBUART2_GEN_16BITS_EP_ACCESS)
            /* Align EP buffers to be event size to access 16-bits DR register. */
            buffCount += (0u != (buffCount & 0x01u)) ? 1u : 0u;
        #endif /* (USBUART2_GEN_16BITS_EP_ACCESS) */            
        }
    #endif /* (!USBUART2_EP_MANAGEMENT_DMA_AUTO) */

        /* Configure hardware registers */
        USBUART2_ConfigReg();
    }
}


/*******************************************************************************
* Function Name: USBUART2_ConfigAltChanged
****************************************************************************//**
*
*  This routine update configuration for the required endpoints only.
*  It is called after SET_INTERFACE request when Static memory allocation used.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBUART2_ConfigAltChanged(void) 
{
    uint32 ep;
    uint32 curEp;
    uint32 epType;
    uint32 i;
    uint8 interfaceNum;

    const T_USBUART2_LUT CYCODE *pTmp;
    const T_USBUART2_EP_SETTINGS_BLOCK CYCODE *pEP;

    /* Init Endpoints and Device Status if configured */
    if (USBUART2_configuration > 0u)
    {
        /* Get number of endpoints configurations (ep). */
        pTmp = USBUART2_GetConfigTablePtr(USBUART2_configuration - 1u);
        pTmp = &pTmp[1u];
        ep = pTmp->c;

        /* Get pointer to endpoints setting table (pEP). */
        pEP = (const T_USBUART2_EP_SETTINGS_BLOCK CYCODE *) pTmp->p_list;
        
        /* Look through all possible endpoint configurations. Find endpoints 
        * which belong to current interface and alternate settings for 
        * re-configuration.
        */
        interfaceNum = USBUART2_interfaceNumber;
        for (i = 0u; i < ep; i++)
        {
            /* Find endpoints which belong to current interface and alternate settings. */
            if ((interfaceNum == pEP->interface) && 
                (USBUART2_interfaceSetting[interfaceNum] == pEP->altSetting))
            {
                curEp  = ((uint32) pEP->addr & USBUART2_DIR_UNUSED);
                epType = ((uint32) pEP->attributes & USBUART2_EP_TYPE_MASK);
                
                /* Change the SIE mode for the selected EP to NAK ALL */
                USBUART2_EP[curEp].epToggle   = 0u;
                USBUART2_EP[curEp].addr       = pEP->addr;
                USBUART2_EP[curEp].attrib     = pEP->attributes;
                USBUART2_EP[curEp].bufferSize = pEP->bufferSize;

                if (0u != (pEP->addr & USBUART2_DIR_IN))
                {
                    /* IN Endpoint */
                    USBUART2_EP[curEp].epMode     = USBUART2_GET_ACTIVE_IN_EP_CR0_MODE(epType);
                    USBUART2_EP[curEp].apiEpState = USBUART2_EVENT_PENDING;
                }
                else
                {
                    /* OUT Endpoint */
                    USBUART2_EP[curEp].epMode     = USBUART2_GET_ACTIVE_OUT_EP_CR0_MODE(epType);
                    USBUART2_EP[curEp].apiEpState = USBUART2_NO_EVENT_PENDING;
                }
                
                /* Make SIE to NAK any endpoint requests */
                USBUART2_SIE_EP_BASE.sieEp[curEp].epCr0 = USBUART2_MODE_NAK_IN_OUT;

            #if (USBUART2_EP_MANAGEMENT_DMA_AUTO)
                /* Clear IN data ready. */
                USBUART2_ARB_EP_BASE.arbEp[curEp].epCfg &= (uint32) ~USBUART2_ARB_EPX_CFG_IN_DATA_RDY;

                /* Select endpoint number of reconfiguration */
                USBUART2_DYN_RECONFIG_REG = (uint32) ((curEp - 1u) << USBUART2_DYN_RECONFIG_EP_SHIFT);
                
                /* Request for dynamic re-configuration of endpoint. */
                USBUART2_DYN_RECONFIG_REG |= USBUART2_DYN_RECONFIG_ENABLE;
                
                /* Wait until block is ready for re-configuration */
                while (0u == (USBUART2_DYN_RECONFIG_REG & USBUART2_DYN_RECONFIG_RDY_STS))
                {
                }
                
                /* Once DYN_RECONFIG_RDY_STS bit is set, FW can change the EP configuration. */
                /* Change EP Type with new direction */
                if (0u != (pEP->addr & USBUART2_DIR_IN))
                {
                    /* Set endpoint type: 0 - IN and 1 - OUT. */
                    USBUART2_EP_TYPE_REG &= (uint32) ~(uint32)((uint32) 0x01u << (curEp - 1u));
                    
                #if (CY_PSOC4)
                    /* Clear DMA_TERMIN for IN endpoint */
                    USBUART2_ARB_EP_BASE.arbEp[curEp].epIntEn &= (uint32) ~USBUART2_ARB_EPX_INT_DMA_TERMIN;
                #endif /* (CY_PSOC4) */
                }
                else
                {
                    /* Set endpoint type: 0 - IN and 1- OUT. */
                    USBUART2_EP_TYPE_REG |= (uint32) ((uint32) 0x01u << (curEp - 1u));
                    
                #if (CY_PSOC4)
                    /* Set DMA_TERMIN for OUT endpoint */
                    USBUART2_ARB_EP_BASE.arbEp[curEp].epIntEn |= (uint32) USBUART2_ARB_EPX_INT_DMA_TERMIN;
                #endif /* (CY_PSOC4) */
                }
                
                /* Complete dynamic re-configuration: all endpoint related status and signals 
                * are set into the default state.
                */
                USBUART2_DYN_RECONFIG_REG &= (uint32) ~USBUART2_DYN_RECONFIG_ENABLE;

            #else
                USBUART2_SIE_EP_BASE.sieEp[curEp].epCnt0 = HI8(USBUART2_EP[curEp].bufferSize);
                USBUART2_SIE_EP_BASE.sieEp[curEp].epCnt1 = LO8(USBUART2_EP[curEp].bufferSize);
                
                #if (CY_PSOC4)
                    USBUART2_ARB_EP16_BASE.arbEp[curEp].rwRa16  = (uint32) USBUART2_EP[curEp].buffOffset;
                    USBUART2_ARB_EP16_BASE.arbEp[curEp].rwWa16  = (uint32) USBUART2_EP[curEp].buffOffset;
                #else
                    USBUART2_ARB_EP_BASE.arbEp[curEp].rwRa    = LO8(USBUART2_EP[curEp].buffOffset);
                    USBUART2_ARB_EP_BASE.arbEp[curEp].rwRaMsb = HI8(USBUART2_EP[curEp].buffOffset);
                    USBUART2_ARB_EP_BASE.arbEp[curEp].rwWa    = LO8(USBUART2_EP[curEp].buffOffset);
                    USBUART2_ARB_EP_BASE.arbEp[curEp].rwWaMsb = HI8(USBUART2_EP[curEp].buffOffset);
                #endif /* (CY_PSOC4) */                
            #endif /* (USBUART2_EP_MANAGEMENT_DMA_AUTO) */
            }
            
            pEP = &pEP[1u]; /* Get next EP element */
        }
        
        /* The main loop has to re-enable DMA and OUT endpoint */
    }
}


/*******************************************************************************
* Function Name: USBUART2_GetConfigTablePtr
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
const T_USBUART2_LUT CYCODE *USBUART2_GetConfigTablePtr(uint8 confIndex)
                                                        
{
    /* Device Table */
    const T_USBUART2_LUT CYCODE *pTmp;

    pTmp = (const T_USBUART2_LUT CYCODE *) USBUART2_TABLE[USBUART2_device].p_list;

    /* The first entry points to the Device Descriptor,
    *  the second entry point to the BOS Descriptor
    *  the rest configuration entries.
    *  Set pointer to the first Configuration Descriptor
    */
    pTmp = &pTmp[2u];
    /* For this table, c is the number of configuration descriptors  */
    if(confIndex >= pTmp->c)   /* Verify that required configuration descriptor exists */
    {
        pTmp = (const T_USBUART2_LUT CYCODE *) NULL;
    }
    else
    {
        pTmp = (const T_USBUART2_LUT CYCODE *) pTmp[confIndex].p_list;
    }

    return (pTmp);
}


#if (USBUART2_BOS_ENABLE)
    /*******************************************************************************
    * Function Name: USBUART2_GetBOSPtr
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
    const T_USBUART2_LUT CYCODE *USBUART2_GetBOSPtr(void)
                                                            
    {
        /* Device Table */
        const T_USBUART2_LUT CYCODE *pTmp;

        pTmp = (const T_USBUART2_LUT CYCODE *) USBUART2_TABLE[USBUART2_device].p_list;

        /* The first entry points to the Device Descriptor,
        *  the second entry points to the BOS Descriptor
        */
        pTmp = &pTmp[1u];
        pTmp = (const T_USBUART2_LUT CYCODE *) pTmp->p_list;
        return (pTmp);
    }
#endif /* (USBUART2_BOS_ENABLE) */


/*******************************************************************************
* Function Name: USBUART2_GetDeviceTablePtr
****************************************************************************//**
*
*  This routine returns a pointer to the Device table
*
* \return
*  Device Table pointer
*
*******************************************************************************/
const T_USBUART2_LUT CYCODE *USBUART2_GetDeviceTablePtr(void)
                                                            
{
    /* Device Table */
    return( (const T_USBUART2_LUT CYCODE *) USBUART2_TABLE[USBUART2_device].p_list );
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
const uint8 CYCODE *USBUART2_GetInterfaceClassTablePtr(void)
                                                        
{
    const T_USBUART2_LUT CYCODE *pTmp;
    const uint8 CYCODE *pInterfaceClass;
    uint8 currentInterfacesNum;

    pTmp = USBUART2_GetConfigTablePtr(USBUART2_configuration - 1u);
    if (pTmp != NULL)
    {
        currentInterfacesNum  = ((const uint8 *) pTmp->p_list)[USBUART2_CONFIG_DESCR_NUM_INTERFACES];
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
* Function Name: USBUART2_TerminateEP
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
void USBUART2_TerminateEP(uint8 epNumber) 
{
    /* Get endpoint number */
    epNumber &= USBUART2_DIR_UNUSED;

    if ((epNumber > USBUART2_EP0) && (epNumber < USBUART2_MAX_EP))
    {
        /* Set the endpoint Halt */
        USBUART2_EP[epNumber].hwEpState |= USBUART2_ENDPOINT_STATUS_HALT;

        /* Clear the data toggle */
        USBUART2_EP[epNumber].epToggle = 0u;
        USBUART2_EP[epNumber].apiEpState = USBUART2_NO_EVENT_ALLOWED;

        if ((USBUART2_EP[epNumber].addr & USBUART2_DIR_IN) != 0u)
        {   
            /* IN Endpoint */
            USBUART2_SIE_EP_BASE.sieEp[epNumber].epCr0 = USBUART2_MODE_NAK_IN;
        }
        else
        {
            /* OUT Endpoint */
            USBUART2_SIE_EP_BASE.sieEp[epNumber].epCr0 = USBUART2_MODE_NAK_OUT;
        }
    }
}


/*******************************************************************************
* Function Name: USBUART2_SetEndpointHalt
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
uint8 USBUART2_SetEndpointHalt(void) 
{
    uint8 requestHandled = USBUART2_FALSE;
    uint32 ep;
    
    /* Set endpoint halt */
    ep = USBUART2_wIndexLoReg & USBUART2_DIR_UNUSED;

    if ((ep > USBUART2_EP0) && (ep < USBUART2_MAX_EP))
    {
        /* Set the endpoint Halt */
        USBUART2_EP[ep].hwEpState |= (USBUART2_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USBUART2_EP[ep].epToggle = 0u;
        USBUART2_EP[ep].apiEpState |= USBUART2_NO_EVENT_ALLOWED;

        if ((USBUART2_EP[ep].addr & USBUART2_DIR_IN) != 0u)
        {
            /* IN Endpoint */
            USBUART2_SIE_EP_BASE.sieEp[ep].epCr0 = (USBUART2_MODE_STALL_DATA_EP | 
                                                            USBUART2_MODE_ACK_IN);
        }
        else
        {
            /* OUT Endpoint */
            USBUART2_SIE_EP_BASE.sieEp[ep].epCr0 = (USBUART2_MODE_STALL_DATA_EP | 
                                                            USBUART2_MODE_ACK_OUT);
        }
        requestHandled = USBUART2_InitNoDataControlTransfer();
    }

    return (requestHandled);
}


/*******************************************************************************
* Function Name: USBUART2_ClearEndpointHalt
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
uint8 USBUART2_ClearEndpointHalt(void) 
{
    uint8 requestHandled = USBUART2_FALSE;
    uint32 ep;

    /* Clear endpoint halt */
    ep = USBUART2_wIndexLoReg & USBUART2_DIR_UNUSED;

    if ((ep > USBUART2_EP0) && (ep < USBUART2_MAX_EP))
    {
        /* Clear the endpoint Halt */
        USBUART2_EP[ep].hwEpState &= (uint8) ~USBUART2_ENDPOINT_STATUS_HALT;

        /* Clear the data toggle */
        USBUART2_EP[ep].epToggle = 0u;
        
        /* Clear toggle bit for already armed packet */
        USBUART2_SIE_EP_BASE.sieEp[ep].epCnt0 &= (uint32) ~(uint32)USBUART2_EPX_CNT_DATA_TOGGLE;
        
        /* Return API State as it was defined before */
        USBUART2_EP[ep].apiEpState &= (uint8) ~USBUART2_NO_EVENT_ALLOWED;

        if ((USBUART2_EP[ep].addr & USBUART2_DIR_IN) != 0u)
        {
            /* IN Endpoint */
            if(USBUART2_EP[ep].apiEpState == USBUART2_IN_BUFFER_EMPTY)
            {       
                /* Wait for next packet from application */
                USBUART2_SIE_EP_BASE.sieEp[ep].epCr0 = USBUART2_MODE_NAK_IN;
            }
            else    /* Continue armed transfer */
            {
                USBUART2_SIE_EP_BASE.sieEp[ep].epCr0 = USBUART2_MODE_ACK_IN;
            }
        }
        else
        {
            /* OUT Endpoint */
            if (USBUART2_EP[ep].apiEpState == USBUART2_OUT_BUFFER_FULL)
            {       
                /* Allow application to read full buffer */
                USBUART2_SIE_EP_BASE.sieEp[ep].epCr0 = USBUART2_MODE_NAK_OUT;
            }
            else    /* Mark endpoint as empty, so it will be reloaded */
            {
                USBUART2_SIE_EP_BASE.sieEp[ep].epCr0 = USBUART2_MODE_ACK_OUT;
            }
        }
        
        requestHandled = USBUART2_InitNoDataControlTransfer();
    }

    return(requestHandled);
}


/*******************************************************************************
* Function Name: USBUART2_ValidateAlternateSetting
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
uint8 USBUART2_ValidateAlternateSetting(void) 
{
    uint8 requestHandled = USBUART2_FALSE;
    
    uint8 interfaceNum;
    uint8 curInterfacesNum;
    const T_USBUART2_LUT CYCODE *pTmp;
    
    /* Get interface number from the request. */
    interfaceNum = (uint8) USBUART2_wIndexLoReg;
    
    /* Get number of interfaces for current configuration. */
    pTmp = USBUART2_GetConfigTablePtr(USBUART2_configuration - 1u);
    curInterfacesNum  = ((const uint8 *) pTmp->p_list)[USBUART2_CONFIG_DESCR_NUM_INTERFACES];

    /* Validate that interface number is within range. */
    if ((interfaceNum <= curInterfacesNum) || (interfaceNum <= USBUART2_MAX_INTERFACES_NUMBER))
    {
        /* Save current and new alternate settings (come with request) to make 
        * desicion about following endpoint re-configuration.
        */
        USBUART2_interfaceSettingLast[interfaceNum] = USBUART2_interfaceSetting[interfaceNum];
        USBUART2_interfaceSetting[interfaceNum]     = (uint8) USBUART2_wValueLoReg;
        
        requestHandled = USBUART2_TRUE;
    }

    return (requestHandled);
}


/* [] END OF FILE */
