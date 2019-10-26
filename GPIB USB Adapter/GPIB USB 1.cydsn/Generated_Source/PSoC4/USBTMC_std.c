/***************************************************************************//**
* \file USBTMC_std.c
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

#include "USBTMC_pvt.h"

/***************************************
*   Static data allocation
***************************************/

#if defined(USBTMC_ENABLE_FWSN_STRING)
    static volatile uint8* USBTMC_fwSerialNumberStringDescriptor;
    static volatile uint8  USBTMC_snStringConfirm = USBTMC_FALSE;
#endif  /* (USBTMC_ENABLE_FWSN_STRING) */

#if defined(USBTMC_ENABLE_FWSN_STRING)
    /***************************************************************************
    * Function Name: USBTMC_SerialNumString
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
    void  USBTMC_SerialNumString(uint8 snString[]) 
    {
        USBTMC_snStringConfirm = USBTMC_FALSE;
        
        if (snString != NULL)
        {
            /* Check descriptor validation */
            if ((snString[0u] > 1u) && (snString[1u] == USBTMC_DESCR_STRING))
            {
                USBTMC_fwSerialNumberStringDescriptor = snString;
                USBTMC_snStringConfirm = USBTMC_TRUE;
            }
        }
    }
#endif  /* USBTMC_ENABLE_FWSN_STRING */


/*******************************************************************************
* Function Name: USBTMC_HandleStandardRqst
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
uint8 USBTMC_HandleStandardRqst(void) 
{
    uint8 requestHandled = USBTMC_FALSE;
    uint32 interfaceNumber;
    uint32 configurationN;
    uint32 bmRequestType = USBTMC_bmRequestTypeReg;

#if defined(USBTMC_ENABLE_STRINGS)
    volatile uint8 *pStr = 0u;
    #if defined(USBTMC_ENABLE_DESCRIPTOR_STRINGS)
        uint8 nStr;
        uint8 descrLength;
    #endif /* (USBTMC_ENABLE_DESCRIPTOR_STRINGS) */
#endif /* (USBTMC_ENABLE_STRINGS) */
    
    static volatile uint8 USBTMC_tBuffer[USBTMC_STATUS_LENGTH_MAX];
    const T_USBTMC_LUT CYCODE *pTmp;

    USBTMC_currentTD.count = 0u;

    if (USBTMC_RQST_DIR_D2H == (bmRequestType & USBTMC_RQST_DIR_MASK))
    {
        /* Control Read */
        switch (USBTMC_bRequestReg)
        {
            case USBTMC_GET_DESCRIPTOR:
                if (USBTMC_DESCR_DEVICE ==USBTMC_wValueHiReg)
                {
                    pTmp = USBTMC_GetDeviceTablePtr();
                    USBTMC_currentTD.pData = (volatile uint8 *)pTmp->p_list;
                    USBTMC_currentTD.count = USBTMC_DEVICE_DESCR_LENGTH;
                    
                    requestHandled  = USBTMC_InitControlRead();
                }
                else if (USBTMC_DESCR_CONFIG == USBTMC_wValueHiReg)
                {
                    pTmp = USBTMC_GetConfigTablePtr((uint8) USBTMC_wValueLoReg);
                    
                    /* Verify that requested descriptor exists */
                    if (pTmp != NULL)
                    {
                        USBTMC_currentTD.pData = (volatile uint8 *)pTmp->p_list;
                        USBTMC_currentTD.count = (uint16)((uint16)(USBTMC_currentTD.pData)[USBTMC_CONFIG_DESCR_TOTAL_LENGTH_HI] << 8u) | \
                                                                            (USBTMC_currentTD.pData)[USBTMC_CONFIG_DESCR_TOTAL_LENGTH_LOW];
                        requestHandled  = USBTMC_InitControlRead();
                    }
                }
                
            #if(USBTMC_BOS_ENABLE)
                else if (USBTMC_DESCR_BOS == USBTMC_wValueHiReg)
                {
                    pTmp = USBTMC_GetBOSPtr();
                    
                    /* Verify that requested descriptor exists */
                    if (pTmp != NULL)
                    {
                        USBTMC_currentTD.pData = (volatile uint8 *)pTmp;
                        USBTMC_currentTD.count = ((uint16)((uint16)(USBTMC_currentTD.pData)[USBTMC_BOS_DESCR_TOTAL_LENGTH_HI] << 8u)) | \
                                                                             (USBTMC_currentTD.pData)[USBTMC_BOS_DESCR_TOTAL_LENGTH_LOW];
                        requestHandled  = USBTMC_InitControlRead();
                    }
                }
            #endif /*(USBTMC_BOS_ENABLE)*/
            
            #if defined(USBTMC_ENABLE_STRINGS)
                else if (USBTMC_DESCR_STRING == USBTMC_wValueHiReg)
                {
                /* Descriptor Strings */
                #if defined(USBTMC_ENABLE_DESCRIPTOR_STRINGS)
                    nStr = 0u;
                    pStr = (volatile uint8 *) &USBTMC_STRING_DESCRIPTORS[0u];
                    
                    while ((USBTMC_wValueLoReg > nStr) && (*pStr != 0u))
                    {
                        /* Read descriptor length from 1st byte */
                        descrLength = *pStr;
                        /* Move to next string descriptor */
                        pStr = &pStr[descrLength];
                        nStr++;
                    }
                #endif /* (USBTMC_ENABLE_DESCRIPTOR_STRINGS) */
                
                /* Microsoft OS String */
                #if defined(USBTMC_ENABLE_MSOS_STRING)
                    if (USBTMC_STRING_MSOS == USBTMC_wValueLoReg)
                    {
                        pStr = (volatile uint8 *)& USBTMC_MSOS_DESCRIPTOR[0u];
                    }
                #endif /* (USBTMC_ENABLE_MSOS_STRING) */
                
                /* SN string */
                #if defined(USBTMC_ENABLE_SN_STRING)
                    if ((USBTMC_wValueLoReg != 0u) && 
                        (USBTMC_wValueLoReg == USBTMC_DEVICE0_DESCR[USBTMC_DEVICE_DESCR_SN_SHIFT]))
                    {
                    #if defined(USBTMC_ENABLE_IDSN_STRING)
                        /* Read DIE ID and generate string descriptor in RAM */
                        USBTMC_ReadDieID(USBTMC_idSerialNumberStringDescriptor);
                        pStr = USBTMC_idSerialNumberStringDescriptor;
                    #elif defined(USBTMC_ENABLE_FWSN_STRING)
                        
                        if(USBTMC_snStringConfirm != USBTMC_FALSE)
                        {
                            pStr = USBTMC_fwSerialNumberStringDescriptor;
                        }
                        else
                        {
                            pStr = (volatile uint8 *)&USBTMC_SN_STRING_DESCRIPTOR[0u];
                        }
                    #else
                        pStr = (volatile uint8 *)&USBTMC_SN_STRING_DESCRIPTOR[0u];
                    #endif  /* (USBTMC_ENABLE_IDSN_STRING) */
                    }
                #endif /* (USBTMC_ENABLE_SN_STRING) */
                
                    if (*pStr != 0u)
                    {
                        USBTMC_currentTD.count = *pStr;
                        USBTMC_currentTD.pData = pStr;
                        requestHandled  = USBTMC_InitControlRead();
                    }
                }
            #endif /*  USBTMC_ENABLE_STRINGS */
                else
                {
                    requestHandled = USBTMC_DispatchClassRqst();
                }
                break;
                
            case USBTMC_GET_STATUS:
                switch (bmRequestType & USBTMC_RQST_RCPT_MASK)
                {
                    case USBTMC_RQST_RCPT_EP:
                        USBTMC_currentTD.count = USBTMC_EP_STATUS_LENGTH;
                        USBTMC_tBuffer[0u]     = USBTMC_EP[USBTMC_wIndexLoReg & USBTMC_DIR_UNUSED].hwEpState;
                        USBTMC_tBuffer[1u]     = 0u;
                        USBTMC_currentTD.pData = &USBTMC_tBuffer[0u];
                        
                        requestHandled  = USBTMC_InitControlRead();
                        break;
                    case USBTMC_RQST_RCPT_DEV:
                        USBTMC_currentTD.count = USBTMC_DEVICE_STATUS_LENGTH;
                        USBTMC_tBuffer[0u]     = USBTMC_deviceStatus;
                        USBTMC_tBuffer[1u]     = 0u;
                        USBTMC_currentTD.pData = &USBTMC_tBuffer[0u];
                        
                        requestHandled  = USBTMC_InitControlRead();
                        break;
                    default:    /* requestHandled is initialized as FALSE by default */
                        break;
                }
                break;
                
            case USBTMC_GET_CONFIGURATION:
                USBTMC_currentTD.count = 1u;
                USBTMC_currentTD.pData = (volatile uint8 *) &USBTMC_configuration;
                requestHandled  = USBTMC_InitControlRead();
                break;
                
            case USBTMC_GET_INTERFACE:
                USBTMC_currentTD.count = 1u;
                USBTMC_currentTD.pData = (volatile uint8 *) &USBTMC_interfaceSetting[USBTMC_wIndexLoReg];
                requestHandled  = USBTMC_InitControlRead();
                break;
                
            default: /* requestHandled is initialized as FALSE by default */
                break;
        }
    }
    else
    {
        /* Control Write */
        switch (USBTMC_bRequestReg)
        {
            case USBTMC_SET_ADDRESS:
                /* Store address to be set in USBTMC_NoDataControlStatusStage(). */
                USBTMC_deviceAddress = (uint8) USBTMC_wValueLoReg;
                requestHandled = USBTMC_InitNoDataControlTransfer();
                break;
                
            case USBTMC_SET_CONFIGURATION:
                configurationN = USBTMC_wValueLoReg;
                
                /* Verify that configuration descriptor exists */
                if(configurationN > 0u)
                {
                    pTmp = USBTMC_GetConfigTablePtr((uint8) configurationN - 1u);
                }
                
                /* Responds with a Request Error when configuration number is invalid */
                if (((configurationN > 0u) && (pTmp != NULL)) || (configurationN == 0u))
                {
                    /* Set new configuration if it has been changed */
                    if(configurationN != USBTMC_configuration)
                    {
                        USBTMC_configuration = (uint8) configurationN;
                        USBTMC_configurationChanged = USBTMC_TRUE;
                        USBTMC_Config(USBTMC_TRUE);
                    }
                    requestHandled = USBTMC_InitNoDataControlTransfer();
                }
                break;
                
            case USBTMC_SET_INTERFACE:
                if (0u != USBTMC_ValidateAlternateSetting())
                {
                    /* Get interface number from the request. */
                    interfaceNumber = USBTMC_wIndexLoReg;
                    USBTMC_interfaceNumber = (uint8) USBTMC_wIndexLoReg;
                     
                    /* Check if alternate settings is changed for interface. */
                    if (USBTMC_interfaceSettingLast[interfaceNumber] != USBTMC_interfaceSetting[interfaceNumber])
                    {
                        USBTMC_configurationChanged = USBTMC_TRUE;
                    
                        /* Change alternate setting for the endpoints. */
                    #if (USBTMC_EP_MANAGEMENT_MANUAL && USBTMC_EP_ALLOC_DYNAMIC)
                        USBTMC_Config(USBTMC_FALSE);
                    #else
                        USBTMC_ConfigAltChanged();
                    #endif /* (USBTMC_EP_MANAGEMENT_MANUAL && USBTMC_EP_ALLOC_DYNAMIC) */
                    }
                    
                    requestHandled = USBTMC_InitNoDataControlTransfer();
                }
                break;
                
            case USBTMC_CLEAR_FEATURE:
                switch (bmRequestType & USBTMC_RQST_RCPT_MASK)
                {
                    case USBTMC_RQST_RCPT_EP:
                        if (USBTMC_wValueLoReg == USBTMC_ENDPOINT_HALT)
                        {
                            requestHandled = USBTMC_ClearEndpointHalt();
                        }
                        break;
                    case USBTMC_RQST_RCPT_DEV:
                        /* Clear device REMOTE_WAKEUP */
                        if (USBTMC_wValueLoReg == USBTMC_DEVICE_REMOTE_WAKEUP)
                        {
                            USBTMC_deviceStatus &= (uint8)~USBTMC_DEVICE_STATUS_REMOTE_WAKEUP;
                            requestHandled = USBTMC_InitNoDataControlTransfer();
                        }
                        break;
                    case USBTMC_RQST_RCPT_IFC:
                        /* Validate interfaceNumber */
                        if (USBTMC_wIndexLoReg < USBTMC_MAX_INTERFACES_NUMBER)
                        {
                            USBTMC_interfaceStatus[USBTMC_wIndexLoReg] &= (uint8) ~USBTMC_wValueLoReg;
                            requestHandled = USBTMC_InitNoDataControlTransfer();
                        }
                        break;
                    default:    /* requestHandled is initialized as FALSE by default */
                        break;
                }
                break;
                
            case USBTMC_SET_FEATURE:
                switch (bmRequestType & USBTMC_RQST_RCPT_MASK)
                {
                    case USBTMC_RQST_RCPT_EP:
                        if (USBTMC_wValueLoReg == USBTMC_ENDPOINT_HALT)
                        {
                            requestHandled = USBTMC_SetEndpointHalt();
                        }
                        break;
                        
                    case USBTMC_RQST_RCPT_DEV:
                        /* Set device REMOTE_WAKEUP */
                        if (USBTMC_wValueLoReg == USBTMC_DEVICE_REMOTE_WAKEUP)
                        {
                            USBTMC_deviceStatus |= USBTMC_DEVICE_STATUS_REMOTE_WAKEUP;
                            requestHandled = USBTMC_InitNoDataControlTransfer();
                        }
                        break;
                        
                    case USBTMC_RQST_RCPT_IFC:
                        /* Validate interfaceNumber */
                        if (USBTMC_wIndexLoReg < USBTMC_MAX_INTERFACES_NUMBER)
                        {
                            USBTMC_interfaceStatus[USBTMC_wIndexLoReg] &= (uint8) ~USBTMC_wValueLoReg;
                            requestHandled = USBTMC_InitNoDataControlTransfer();
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


#if defined(USBTMC_ENABLE_IDSN_STRING)
    /***************************************************************************
    * Function Name: USBTMC_ReadDieID
    ************************************************************************//**
    *
    *  This routine read Die ID and generate Serial Number string descriptor.
    *
    *  \param descr:  pointer on string descriptor. This string size has to be equal or
    *          greater than USBTMC_IDSN_DESCR_LENGTH.
    *
    *
    * \reentrant
    *  No.
    *
    ***************************************************************************/
    void USBTMC_ReadDieID(uint8 descr[]) 
    {
        const char8 CYCODE hex[] = "0123456789ABCDEF";
        uint32 i;
        uint32 j = 0u;
        uint8 uniqueId[8u];

        if (NULL != descr)
        {
            /* Initialize descriptor header. */
            descr[0u] = USBTMC_IDSN_DESCR_LENGTH;
            descr[1u] = USBTMC_DESCR_STRING;
            
            /* Unique ID size is 8 bytes. */
            CyGetUniqueId((uint32 *) uniqueId);

            /* Fill descriptor with unique device ID. */
            for (i = 2u; i < USBTMC_IDSN_DESCR_LENGTH; i += 4u)
            {
                descr[i]      = (uint8) hex[(uniqueId[j] >> 4u)];
                descr[i + 1u] = 0u;
                descr[i + 2u] = (uint8) hex[(uniqueId[j] & 0x0Fu)];
                descr[i + 3u] = 0u;
                ++j;
            }
        }
    }
#endif /* (USBTMC_ENABLE_IDSN_STRING) */


/*******************************************************************************
* Function Name: USBTMC_ConfigReg
****************************************************************************//**
*
*  This routine configures hardware registers from the variables.
*  It is called from USBTMC_Config() function and from RestoreConfig
*  after Wakeup.
*
*******************************************************************************/
void USBTMC_ConfigReg(void) 
{
    uint32 ep;

#if (USBTMC_EP_MANAGEMENT_DMA_AUTO)
    uint8 epType = 0u;
#endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */

    /* Go thought all endpoints and set hardware configuration */
    for (ep = USBTMC_EP1; ep < USBTMC_MAX_EP; ++ep)
    {
        USBTMC_ARB_EP_BASE.arbEp[ep].epCfg = USBTMC_ARB_EPX_CFG_DEFAULT;
        
    #if (USBTMC_EP_MANAGEMENT_DMA)
        /* Enable arbiter endpoint interrupt sources */
        USBTMC_ARB_EP_BASE.arbEp[ep].epIntEn = USBTMC_ARB_EPX_INT_MASK;
    #endif /* (USBTMC_EP_MANAGEMENT_DMA) */
    
        if (USBTMC_EP[ep].epMode != USBTMC_MODE_DISABLE)
        {
            if (0u != (USBTMC_EP[ep].addr & USBTMC_DIR_IN))
            {
                USBTMC_SIE_EP_BASE.sieEp[ep].epCr0 = USBTMC_MODE_NAK_IN;
                
            #if (USBTMC_EP_MANAGEMENT_DMA_AUTO && CY_PSOC4)
                /* Clear DMA_TERMIN for IN endpoint. */
                USBTMC_ARB_EP_BASE.arbEp[ep].epIntEn &= (uint32) ~USBTMC_ARB_EPX_INT_DMA_TERMIN;
            #endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO && CY_PSOC4) */
            }
            else
            {
                USBTMC_SIE_EP_BASE.sieEp[ep].epCr0 = USBTMC_MODE_NAK_OUT;

            #if (USBTMC_EP_MANAGEMENT_DMA_AUTO)
                /* (CY_PSOC4): DMA_TERMIN for OUT endpoint is set above. */
                
                /* Prepare endpoint type mask. */
                epType |= (uint8) (0x01u << (ep - USBTMC_EP1));
            #endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */
            }
        }
        else
        {
            USBTMC_SIE_EP_BASE.sieEp[ep].epCr0 = USBTMC_MODE_STALL_DATA_EP;
        }
        
    #if (!USBTMC_EP_MANAGEMENT_DMA_AUTO)
        #if (CY_PSOC4)
            USBTMC_ARB_EP16_BASE.arbEp[ep].rwRa16  = (uint32) USBTMC_EP[ep].buffOffset;
            USBTMC_ARB_EP16_BASE.arbEp[ep].rwWa16  = (uint32) USBTMC_EP[ep].buffOffset;
        #else
            USBTMC_ARB_EP_BASE.arbEp[ep].rwRa    = LO8(USBTMC_EP[ep].buffOffset);
            USBTMC_ARB_EP_BASE.arbEp[ep].rwRaMsb = HI8(USBTMC_EP[ep].buffOffset);
            USBTMC_ARB_EP_BASE.arbEp[ep].rwWa    = LO8(USBTMC_EP[ep].buffOffset);
            USBTMC_ARB_EP_BASE.arbEp[ep].rwWaMsb = HI8(USBTMC_EP[ep].buffOffset);
        #endif /* (CY_PSOC4) */
    #endif /* (!USBTMC_EP_MANAGEMENT_DMA_AUTO) */
    }

#if (USBTMC_EP_MANAGEMENT_DMA_AUTO)
     /* BUF_SIZE depend on DMA_THRESS value:0x55-32 bytes  0x44-16 bytes 0x33-8 bytes 0x22-4 bytes 0x11-2 bytes */
    USBTMC_BUF_SIZE_REG = USBTMC_DMA_BUF_SIZE;

    /* Configure DMA burst threshold */
#if (CY_PSOC4)
    USBTMC_DMA_THRES16_REG   = USBTMC_DMA_BYTES_PER_BURST;
#else
    USBTMC_DMA_THRES_REG     = USBTMC_DMA_BYTES_PER_BURST;
    USBTMC_DMA_THRES_MSB_REG = 0u;
#endif /* (CY_PSOC4) */
    USBTMC_EP_ACTIVE_REG = USBTMC_DEFAULT_ARB_INT_EN;
    USBTMC_EP_TYPE_REG   = epType;
    
    /* Cfg_cmp bit set to 1 once configuration is complete. */
    /* Lock arbiter configtuation */
    USBTMC_ARB_CFG_REG |= (uint32)  USBTMC_ARB_CFG_CFG_CMP;
    /* Cfg_cmp bit set to 0 during configuration of PFSUSB Registers. */
    USBTMC_ARB_CFG_REG &= (uint32) ~USBTMC_ARB_CFG_CFG_CMP;

#endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */

    /* Enable interrupt SIE interurpt source from EP0-EP1 */
    USBTMC_SIE_EP_INT_EN_REG = (uint32) USBTMC_DEFAULT_SIE_EP_INT_EN;
}


/*******************************************************************************
* Function Name: USBTMC_EpStateInit
****************************************************************************//**
*
*  This routine initialize state of Data end points based of its type: 
*   IN  - USBTMC_IN_BUFFER_EMPTY (USBTMC_EVENT_PENDING)
*   OUT - USBTMC_OUT_BUFFER_EMPTY (USBTMC_NO_EVENT_PENDING)
*
*******************************************************************************/
void USBTMC_EpStateInit(void) 
{
    uint8 i;

    for (i = USBTMC_EP1; i < USBTMC_MAX_EP; i++)
    { 
        if (0u != (USBTMC_EP[i].addr & USBTMC_DIR_IN))
        {
            /* IN Endpoint */
            USBTMC_EP[i].apiEpState = USBTMC_EVENT_PENDING;
        }
        else
        {
            /* OUT Endpoint */
            USBTMC_EP[i].apiEpState = USBTMC_NO_EVENT_PENDING;
        }
    }
                    
}


/*******************************************************************************
* Function Name: USBTMC_Config
****************************************************************************//**
*
*  This routine configures endpoints for the entire configuration by scanning
*  the configuration descriptor.
*
*  \param clearAltSetting: It configures the bAlternateSetting 0 for each interface.
*
* USBTMC_interfaceClass - Initialized class array for each interface.
*   It is used for handling Class specific requests depend on interface class.
*   Different classes in multiple Alternate settings does not supported.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_Config(uint8 clearAltSetting) 
{
    uint8 ep;
    uint8 curEp;
    uint8 i;
    uint8 epType;
    const uint8 *pDescr;
    
    #if (!USBTMC_EP_MANAGEMENT_DMA_AUTO)
        uint16 buffCount = 0u;
    #endif /* (!USBTMC_EP_MANAGEMENT_DMA_AUTO) */

    const T_USBTMC_LUT CYCODE *pTmp;
    const T_USBTMC_EP_SETTINGS_BLOCK CYCODE *pEP;

    /* Clear endpoints settings */
    for (ep = 0u; ep < USBTMC_MAX_EP; ++ep)
    {
        USBTMC_EP[ep].attrib     = 0u;
        USBTMC_EP[ep].hwEpState  = 0u;
        USBTMC_EP[ep].epToggle   = 0u;
        USBTMC_EP[ep].bufferSize = 0u;
        USBTMC_EP[ep].interface  = 0u;
        USBTMC_EP[ep].apiEpState = USBTMC_NO_EVENT_PENDING;
        USBTMC_EP[ep].epMode     = USBTMC_MODE_DISABLE;   
    }

    /* Clear Alternate settings for all interfaces. */
    if (0u != clearAltSetting)
    {
        for (i = 0u; i < USBTMC_MAX_INTERFACES_NUMBER; ++i)
        {
            USBTMC_interfaceSetting[i]     = 0u;
            USBTMC_interfaceSettingLast[i] = 0u;
        }
    }

    /* Init Endpoints and Device Status if configured */
    if (USBTMC_configuration > 0u)
    {
        #if defined(USBTMC_ENABLE_CDC_CLASS)
            uint8 cdcComNums = 0u;
        #endif  /* (USBTMC_ENABLE_CDC_CLASS) */  

        pTmp = USBTMC_GetConfigTablePtr(USBTMC_configuration - 1u);
        
        /* Set Power status for current configuration */
        pDescr = (const uint8 *)pTmp->p_list;
        if ((pDescr[USBTMC_CONFIG_DESCR_ATTRIB] & USBTMC_CONFIG_DESCR_ATTRIB_SELF_POWERED) != 0u)
        {
            USBTMC_deviceStatus |= (uint8)  USBTMC_DEVICE_STATUS_SELF_POWERED;
        }
        else
        {
            USBTMC_deviceStatus &= (uint8) ~USBTMC_DEVICE_STATUS_SELF_POWERED;
        }
        
        /* Move to next element */
        pTmp = &pTmp[1u];
        ep = pTmp->c;  /* For this table, c is the number of endpoints configurations  */

        #if (USBTMC_EP_MANAGEMENT_MANUAL && USBTMC_EP_ALLOC_DYNAMIC)
            /* Configure for dynamic EP memory allocation */
            /* p_list points the endpoint setting table. */
            pEP = (T_USBTMC_EP_SETTINGS_BLOCK *) pTmp->p_list;
            
            for (i = 0u; i < ep; i++)
            {     
                /* Compare current Alternate setting with EP Alt */
                if (USBTMC_interfaceSetting[pEP->interface] == pEP->altSetting)
                {                                                          
                    curEp  = pEP->addr & USBTMC_DIR_UNUSED;
                    epType = pEP->attributes & USBTMC_EP_TYPE_MASK;
                    
                    USBTMC_EP[curEp].addr       = pEP->addr;
                    USBTMC_EP[curEp].attrib     = pEP->attributes;
                    USBTMC_EP[curEp].bufferSize = pEP->bufferSize;

                    if (0u != (pEP->addr & USBTMC_DIR_IN))
                    {
                        /* IN Endpoint */
                        USBTMC_EP[curEp].epMode     = USBTMC_GET_ACTIVE_IN_EP_CR0_MODE(epType);
                        USBTMC_EP[curEp].apiEpState = USBTMC_EVENT_PENDING;
                    
                    #if (defined(USBTMC_ENABLE_MIDI_STREAMING) && (USBTMC_MIDI_IN_BUFF_SIZE > 0))
                        if ((pEP->bMisc == USBTMC_CLASS_AUDIO) && (epType == USBTMC_EP_TYPE_BULK))
                        {
                            USBTMC_midi_in_ep = curEp;
                        }
                    #endif  /* (USBTMC_ENABLE_MIDI_STREAMING) */
                    }
                    else
                    {
                        /* OUT Endpoint */
                        USBTMC_EP[curEp].epMode     = USBTMC_GET_ACTIVE_OUT_EP_CR0_MODE(epType);
                        USBTMC_EP[curEp].apiEpState = USBTMC_NO_EVENT_PENDING;
                        
                    #if (defined(USBTMC_ENABLE_MIDI_STREAMING) && (USBTMC_MIDI_OUT_BUFF_SIZE > 0))
                        if ((pEP->bMisc == USBTMC_CLASS_AUDIO) && (epType == USBTMC_EP_TYPE_BULK))
                        {
                            USBTMC_midi_out_ep = curEp;
                        }
                    #endif  /* (USBTMC_ENABLE_MIDI_STREAMING) */
                    }

                #if(defined (USBTMC_ENABLE_CDC_CLASS))
                    if((pEP->bMisc == USBTMC_CLASS_CDC_DATA) ||(pEP->bMisc == USBTMC_CLASS_CDC))
                    {
                        cdcComNums = USBTMC_Cdc_EpInit(pEP, curEp, cdcComNums);
                    }
                #endif  /* (USBTMC_ENABLE_CDC_CLASS) */
                }
                
                pEP = &pEP[1u];
            }
            
        #else
            for (i = USBTMC_EP1; i < USBTMC_MAX_EP; ++i)
            {
                /* p_list points the endpoint setting table. */
                pEP = (const T_USBTMC_EP_SETTINGS_BLOCK CYCODE *) pTmp->p_list;
                /* Find max length for each EP and select it (length could be different in different Alt settings) */
                /* but other settings should be correct with regards to Interface alt Setting */
                
                for (curEp = 0u; curEp < ep; ++curEp)
                {
                    if (i == (pEP->addr & USBTMC_DIR_UNUSED))
                    {
                        /* Compare endpoint buffers size with current size to find greater. */
                        if (USBTMC_EP[i].bufferSize < pEP->bufferSize)
                        {
                            USBTMC_EP[i].bufferSize = pEP->bufferSize;
                        }
                        
                        /* Compare current Alternate setting with EP Alt */
                        if (USBTMC_interfaceSetting[pEP->interface] == pEP->altSetting)
                        {                            
                            USBTMC_EP[i].addr = pEP->addr;
                            USBTMC_EP[i].attrib = pEP->attributes;
                            
                            epType = pEP->attributes & USBTMC_EP_TYPE_MASK;
                            
                            if (0u != (pEP->addr & USBTMC_DIR_IN))
                            {
                                /* IN Endpoint */
                                USBTMC_EP[i].epMode     = USBTMC_GET_ACTIVE_IN_EP_CR0_MODE(epType);
                                USBTMC_EP[i].apiEpState = USBTMC_EVENT_PENDING;
                                
                            #if (defined(USBTMC_ENABLE_MIDI_STREAMING) && (USBTMC_MIDI_IN_BUFF_SIZE > 0))
                                if ((pEP->bMisc == USBTMC_CLASS_AUDIO) && (epType == USBTMC_EP_TYPE_BULK))
                                {
                                    USBTMC_midi_in_ep = i;
                                }
                            #endif  /* (USBTMC_ENABLE_MIDI_STREAMING) */
                            }
                            else
                            {
                                /* OUT Endpoint */
                                USBTMC_EP[i].epMode     = USBTMC_GET_ACTIVE_OUT_EP_CR0_MODE(epType);
                                USBTMC_EP[i].apiEpState = USBTMC_NO_EVENT_PENDING;
                                
                            #if (defined(USBTMC_ENABLE_MIDI_STREAMING) && (USBTMC_MIDI_OUT_BUFF_SIZE > 0))
                                if ((pEP->bMisc == USBTMC_CLASS_AUDIO) && (epType == USBTMC_EP_TYPE_BULK))
                                {
                                    USBTMC_midi_out_ep = i;
                                }
                            #endif  /* (USBTMC_ENABLE_MIDI_STREAMING) */
                            }

                        #if (defined(USBTMC_ENABLE_CDC_CLASS))
                            if((pEP->bMisc == USBTMC_CLASS_CDC_DATA) ||(pEP->bMisc == USBTMC_CLASS_CDC))
                            {
                                cdcComNums = USBTMC_Cdc_EpInit(pEP, i, cdcComNums);
                            }
                        #endif  /* (USBTMC_ENABLE_CDC_CLASS) */

                            #if (USBTMC_EP_MANAGEMENT_DMA_AUTO)
                                break;  /* Use first EP setting in Auto memory management */
                            #endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */
                        }
                    }
                    
                    pEP = &pEP[1u];
                }
            }
        #endif /*  (USBTMC_EP_MANAGEMENT_MANUAL && USBTMC_EP_ALLOC_DYNAMIC) */

        /* Init class array for each interface and interface number for each EP.
        *  It is used for handling Class specific requests directed to either an
        *  interface or the endpoint.
        */
        /* p_list points the endpoint setting table. */
        pEP = (const T_USBTMC_EP_SETTINGS_BLOCK CYCODE *) pTmp->p_list;
        for (i = 0u; i < ep; i++)
        {
            /* Configure interface number for each EP */
            USBTMC_EP[pEP->addr & USBTMC_DIR_UNUSED].interface = pEP->interface;
            pEP = &pEP[1u];
        }
        
        /* Init pointer on interface class table */
        USBTMC_interfaceClass = USBTMC_GetInterfaceClassTablePtr();
        
    /* Set the endpoint buffer addresses */
    #if (!USBTMC_EP_MANAGEMENT_DMA_AUTO)
        buffCount = 0u;
        for (ep = USBTMC_EP1; ep < USBTMC_MAX_EP; ++ep)
        {
            USBTMC_EP[ep].buffOffset = buffCount;        
            buffCount += USBTMC_EP[ep].bufferSize;
            
        #if (USBTMC_GEN_16BITS_EP_ACCESS)
            /* Align EP buffers to be event size to access 16-bits DR register. */
            buffCount += (0u != (buffCount & 0x01u)) ? 1u : 0u;
        #endif /* (USBTMC_GEN_16BITS_EP_ACCESS) */            
        }
    #endif /* (!USBTMC_EP_MANAGEMENT_DMA_AUTO) */

        /* Configure hardware registers */
        USBTMC_ConfigReg();
    }
}


/*******************************************************************************
* Function Name: USBTMC_ConfigAltChanged
****************************************************************************//**
*
*  This routine update configuration for the required endpoints only.
*  It is called after SET_INTERFACE request when Static memory allocation used.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_ConfigAltChanged(void) 
{
    uint32 ep;
    uint32 curEp;
    uint32 epType;
    uint32 i;
    uint8 interfaceNum;

    const T_USBTMC_LUT CYCODE *pTmp;
    const T_USBTMC_EP_SETTINGS_BLOCK CYCODE *pEP;

    /* Init Endpoints and Device Status if configured */
    if (USBTMC_configuration > 0u)
    {
        /* Get number of endpoints configurations (ep). */
        pTmp = USBTMC_GetConfigTablePtr(USBTMC_configuration - 1u);
        pTmp = &pTmp[1u];
        ep = pTmp->c;

        /* Get pointer to endpoints setting table (pEP). */
        pEP = (const T_USBTMC_EP_SETTINGS_BLOCK CYCODE *) pTmp->p_list;
        
        /* Look through all possible endpoint configurations. Find endpoints 
        * which belong to current interface and alternate settings for 
        * re-configuration.
        */
        interfaceNum = USBTMC_interfaceNumber;
        for (i = 0u; i < ep; i++)
        {
            /* Find endpoints which belong to current interface and alternate settings. */
            if ((interfaceNum == pEP->interface) && 
                (USBTMC_interfaceSetting[interfaceNum] == pEP->altSetting))
            {
                curEp  = ((uint32) pEP->addr & USBTMC_DIR_UNUSED);
                epType = ((uint32) pEP->attributes & USBTMC_EP_TYPE_MASK);
                
                /* Change the SIE mode for the selected EP to NAK ALL */
                USBTMC_EP[curEp].epToggle   = 0u;
                USBTMC_EP[curEp].addr       = pEP->addr;
                USBTMC_EP[curEp].attrib     = pEP->attributes;
                USBTMC_EP[curEp].bufferSize = pEP->bufferSize;

                if (0u != (pEP->addr & USBTMC_DIR_IN))
                {
                    /* IN Endpoint */
                    USBTMC_EP[curEp].epMode     = USBTMC_GET_ACTIVE_IN_EP_CR0_MODE(epType);
                    USBTMC_EP[curEp].apiEpState = USBTMC_EVENT_PENDING;
                }
                else
                {
                    /* OUT Endpoint */
                    USBTMC_EP[curEp].epMode     = USBTMC_GET_ACTIVE_OUT_EP_CR0_MODE(epType);
                    USBTMC_EP[curEp].apiEpState = USBTMC_NO_EVENT_PENDING;
                }
                
                /* Make SIE to NAK any endpoint requests */
                USBTMC_SIE_EP_BASE.sieEp[curEp].epCr0 = USBTMC_MODE_NAK_IN_OUT;

            #if (USBTMC_EP_MANAGEMENT_DMA_AUTO)
                /* Clear IN data ready. */
                USBTMC_ARB_EP_BASE.arbEp[curEp].epCfg &= (uint32) ~USBTMC_ARB_EPX_CFG_IN_DATA_RDY;

                /* Select endpoint number of reconfiguration */
                USBTMC_DYN_RECONFIG_REG = (uint32) ((curEp - 1u) << USBTMC_DYN_RECONFIG_EP_SHIFT);
                
                /* Request for dynamic re-configuration of endpoint. */
                USBTMC_DYN_RECONFIG_REG |= USBTMC_DYN_RECONFIG_ENABLE;
                
                /* Wait until block is ready for re-configuration */
                while (0u == (USBTMC_DYN_RECONFIG_REG & USBTMC_DYN_RECONFIG_RDY_STS))
                {
                }
                
                /* Once DYN_RECONFIG_RDY_STS bit is set, FW can change the EP configuration. */
                /* Change EP Type with new direction */
                if (0u != (pEP->addr & USBTMC_DIR_IN))
                {
                    /* Set endpoint type: 0 - IN and 1 - OUT. */
                    USBTMC_EP_TYPE_REG &= (uint32) ~(uint32)((uint32) 0x01u << (curEp - 1u));
                    
                #if (CY_PSOC4)
                    /* Clear DMA_TERMIN for IN endpoint */
                    USBTMC_ARB_EP_BASE.arbEp[curEp].epIntEn &= (uint32) ~USBTMC_ARB_EPX_INT_DMA_TERMIN;
                #endif /* (CY_PSOC4) */
                }
                else
                {
                    /* Set endpoint type: 0 - IN and 1- OUT. */
                    USBTMC_EP_TYPE_REG |= (uint32) ((uint32) 0x01u << (curEp - 1u));
                    
                #if (CY_PSOC4)
                    /* Set DMA_TERMIN for OUT endpoint */
                    USBTMC_ARB_EP_BASE.arbEp[curEp].epIntEn |= (uint32) USBTMC_ARB_EPX_INT_DMA_TERMIN;
                #endif /* (CY_PSOC4) */
                }
                
                /* Complete dynamic re-configuration: all endpoint related status and signals 
                * are set into the default state.
                */
                USBTMC_DYN_RECONFIG_REG &= (uint32) ~USBTMC_DYN_RECONFIG_ENABLE;

            #else
                USBTMC_SIE_EP_BASE.sieEp[curEp].epCnt0 = HI8(USBTMC_EP[curEp].bufferSize);
                USBTMC_SIE_EP_BASE.sieEp[curEp].epCnt1 = LO8(USBTMC_EP[curEp].bufferSize);
                
                #if (CY_PSOC4)
                    USBTMC_ARB_EP16_BASE.arbEp[curEp].rwRa16  = (uint32) USBTMC_EP[curEp].buffOffset;
                    USBTMC_ARB_EP16_BASE.arbEp[curEp].rwWa16  = (uint32) USBTMC_EP[curEp].buffOffset;
                #else
                    USBTMC_ARB_EP_BASE.arbEp[curEp].rwRa    = LO8(USBTMC_EP[curEp].buffOffset);
                    USBTMC_ARB_EP_BASE.arbEp[curEp].rwRaMsb = HI8(USBTMC_EP[curEp].buffOffset);
                    USBTMC_ARB_EP_BASE.arbEp[curEp].rwWa    = LO8(USBTMC_EP[curEp].buffOffset);
                    USBTMC_ARB_EP_BASE.arbEp[curEp].rwWaMsb = HI8(USBTMC_EP[curEp].buffOffset);
                #endif /* (CY_PSOC4) */                
            #endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */
            }
            
            pEP = &pEP[1u]; /* Get next EP element */
        }
        
        /* The main loop has to re-enable DMA and OUT endpoint */
    }
}


/*******************************************************************************
* Function Name: USBTMC_GetConfigTablePtr
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
const T_USBTMC_LUT CYCODE *USBTMC_GetConfigTablePtr(uint8 confIndex)
                                                        
{
    /* Device Table */
    const T_USBTMC_LUT CYCODE *pTmp;

    pTmp = (const T_USBTMC_LUT CYCODE *) USBTMC_TABLE[USBTMC_device].p_list;

    /* The first entry points to the Device Descriptor,
    *  the second entry point to the BOS Descriptor
    *  the rest configuration entries.
    *  Set pointer to the first Configuration Descriptor
    */
    pTmp = &pTmp[2u];
    /* For this table, c is the number of configuration descriptors  */
    if(confIndex >= pTmp->c)   /* Verify that required configuration descriptor exists */
    {
        pTmp = (const T_USBTMC_LUT CYCODE *) NULL;
    }
    else
    {
        pTmp = (const T_USBTMC_LUT CYCODE *) pTmp[confIndex].p_list;
    }

    return (pTmp);
}


#if (USBTMC_BOS_ENABLE)
    /*******************************************************************************
    * Function Name: USBTMC_GetBOSPtr
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
    const T_USBTMC_LUT CYCODE *USBTMC_GetBOSPtr(void)
                                                            
    {
        /* Device Table */
        const T_USBTMC_LUT CYCODE *pTmp;

        pTmp = (const T_USBTMC_LUT CYCODE *) USBTMC_TABLE[USBTMC_device].p_list;

        /* The first entry points to the Device Descriptor,
        *  the second entry points to the BOS Descriptor
        */
        pTmp = &pTmp[1u];
        pTmp = (const T_USBTMC_LUT CYCODE *) pTmp->p_list;
        return (pTmp);
    }
#endif /* (USBTMC_BOS_ENABLE) */


/*******************************************************************************
* Function Name: USBTMC_GetDeviceTablePtr
****************************************************************************//**
*
*  This routine returns a pointer to the Device table
*
* \return
*  Device Table pointer
*
*******************************************************************************/
const T_USBTMC_LUT CYCODE *USBTMC_GetDeviceTablePtr(void)
                                                            
{
    /* Device Table */
    return( (const T_USBTMC_LUT CYCODE *) USBTMC_TABLE[USBTMC_device].p_list );
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
const uint8 CYCODE *USBTMC_GetInterfaceClassTablePtr(void)
                                                        
{
    const T_USBTMC_LUT CYCODE *pTmp;
    const uint8 CYCODE *pInterfaceClass;
    uint8 currentInterfacesNum;

    pTmp = USBTMC_GetConfigTablePtr(USBTMC_configuration - 1u);
    if (pTmp != NULL)
    {
        currentInterfacesNum  = ((const uint8 *) pTmp->p_list)[USBTMC_CONFIG_DESCR_NUM_INTERFACES];
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
* Function Name: USBTMC_TerminateEP
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
void USBTMC_TerminateEP(uint8 epNumber) 
{
    /* Get endpoint number */
    epNumber &= USBTMC_DIR_UNUSED;

    if ((epNumber > USBTMC_EP0) && (epNumber < USBTMC_MAX_EP))
    {
        /* Set the endpoint Halt */
        USBTMC_EP[epNumber].hwEpState |= USBTMC_ENDPOINT_STATUS_HALT;

        /* Clear the data toggle */
        USBTMC_EP[epNumber].epToggle = 0u;
        USBTMC_EP[epNumber].apiEpState = USBTMC_NO_EVENT_ALLOWED;

        if ((USBTMC_EP[epNumber].addr & USBTMC_DIR_IN) != 0u)
        {   
            /* IN Endpoint */
            USBTMC_SIE_EP_BASE.sieEp[epNumber].epCr0 = USBTMC_MODE_NAK_IN;
        }
        else
        {
            /* OUT Endpoint */
            USBTMC_SIE_EP_BASE.sieEp[epNumber].epCr0 = USBTMC_MODE_NAK_OUT;
        }
    }
}


/*******************************************************************************
* Function Name: USBTMC_SetEndpointHalt
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
uint8 USBTMC_SetEndpointHalt(void) 
{
    uint8 requestHandled = USBTMC_FALSE;
    uint32 ep;
    
    /* Set endpoint halt */
    ep = USBTMC_wIndexLoReg & USBTMC_DIR_UNUSED;

    if ((ep > USBTMC_EP0) && (ep < USBTMC_MAX_EP))
    {
        /* Set the endpoint Halt */
        USBTMC_EP[ep].hwEpState |= (USBTMC_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USBTMC_EP[ep].epToggle = 0u;
        USBTMC_EP[ep].apiEpState |= USBTMC_NO_EVENT_ALLOWED;

        if ((USBTMC_EP[ep].addr & USBTMC_DIR_IN) != 0u)
        {
            /* IN Endpoint */
            USBTMC_SIE_EP_BASE.sieEp[ep].epCr0 = (USBTMC_MODE_STALL_DATA_EP | 
                                                            USBTMC_MODE_ACK_IN);
        }
        else
        {
            /* OUT Endpoint */
            USBTMC_SIE_EP_BASE.sieEp[ep].epCr0 = (USBTMC_MODE_STALL_DATA_EP | 
                                                            USBTMC_MODE_ACK_OUT);
        }
        requestHandled = USBTMC_InitNoDataControlTransfer();
    }

    return (requestHandled);
}


/*******************************************************************************
* Function Name: USBTMC_ClearEndpointHalt
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
uint8 USBTMC_ClearEndpointHalt(void) 
{
    uint8 requestHandled = USBTMC_FALSE;
    uint32 ep;

    /* Clear endpoint halt */
    ep = USBTMC_wIndexLoReg & USBTMC_DIR_UNUSED;

    if ((ep > USBTMC_EP0) && (ep < USBTMC_MAX_EP))
    {
        /* Clear the endpoint Halt */
        USBTMC_EP[ep].hwEpState &= (uint8) ~USBTMC_ENDPOINT_STATUS_HALT;

        /* Clear the data toggle */
        USBTMC_EP[ep].epToggle = 0u;
        
        /* Clear toggle bit for already armed packet */
        USBTMC_SIE_EP_BASE.sieEp[ep].epCnt0 &= (uint32) ~(uint32)USBTMC_EPX_CNT_DATA_TOGGLE;
        
        /* Return API State as it was defined before */
        USBTMC_EP[ep].apiEpState &= (uint8) ~USBTMC_NO_EVENT_ALLOWED;

        if ((USBTMC_EP[ep].addr & USBTMC_DIR_IN) != 0u)
        {
            /* IN Endpoint */
            if(USBTMC_EP[ep].apiEpState == USBTMC_IN_BUFFER_EMPTY)
            {       
                /* Wait for next packet from application */
                USBTMC_SIE_EP_BASE.sieEp[ep].epCr0 = USBTMC_MODE_NAK_IN;
            }
            else    /* Continue armed transfer */
            {
                USBTMC_SIE_EP_BASE.sieEp[ep].epCr0 = USBTMC_MODE_ACK_IN;
            }
        }
        else
        {
            /* OUT Endpoint */
            if (USBTMC_EP[ep].apiEpState == USBTMC_OUT_BUFFER_FULL)
            {       
                /* Allow application to read full buffer */
                USBTMC_SIE_EP_BASE.sieEp[ep].epCr0 = USBTMC_MODE_NAK_OUT;
            }
            else    /* Mark endpoint as empty, so it will be reloaded */
            {
                USBTMC_SIE_EP_BASE.sieEp[ep].epCr0 = USBTMC_MODE_ACK_OUT;
            }
        }
        
        requestHandled = USBTMC_InitNoDataControlTransfer();
    }

    return(requestHandled);
}


/*******************************************************************************
* Function Name: USBTMC_ValidateAlternateSetting
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
uint8 USBTMC_ValidateAlternateSetting(void) 
{
    uint8 requestHandled = USBTMC_FALSE;
    
    uint8 interfaceNum;
    uint8 curInterfacesNum;
    const T_USBTMC_LUT CYCODE *pTmp;
    
    /* Get interface number from the request. */
    interfaceNum = (uint8) USBTMC_wIndexLoReg;
    
    /* Get number of interfaces for current configuration. */
    pTmp = USBTMC_GetConfigTablePtr(USBTMC_configuration - 1u);
    curInterfacesNum  = ((const uint8 *) pTmp->p_list)[USBTMC_CONFIG_DESCR_NUM_INTERFACES];

    /* Validate that interface number is within range. */
    if ((interfaceNum <= curInterfacesNum) || (interfaceNum <= USBTMC_MAX_INTERFACES_NUMBER))
    {
        /* Save current and new alternate settings (come with request) to make 
        * desicion about following endpoint re-configuration.
        */
        USBTMC_interfaceSettingLast[interfaceNum] = USBTMC_interfaceSetting[interfaceNum];
        USBTMC_interfaceSetting[interfaceNum]     = (uint8) USBTMC_wValueLoReg;
        
        requestHandled = USBTMC_TRUE;
    }

    return (requestHandled);
}


/* [] END OF FILE */
