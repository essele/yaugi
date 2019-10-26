/***************************************************************************//**
* \file USBTMC_audio.c
* \version 3.20
*
* \brief
*  This file contains the USB AUDIO Class request handler.
*
* Related Document:
*  Universal Serial Bus Device Class Definition for Audio Devices Release 1.0
*
********************************************************************************
* \copyright
* Copyright 2008-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USBTMC_audio.h"
#include "USBTMC_pvt.h"
#include "cyapicallbacks.h"

#if defined(USBTMC_ENABLE_AUDIO_CLASS)

/***************************************
* Custom Declarations
***************************************/

/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


#if !defined(USER_SUPPLIED_AUDIO_HANDLER)

/***************************************
*    AUDIO Variables
***************************************/

#if defined(USBTMC_ENABLE_AUDIO_STREAMING)
    /** Contains the current audio sample frequency. It is set by the host using a SET_CUR request to the endpoint.*/
    volatile uint8 USBTMC_currentSampleFrequency[USBTMC_MAX_EP][USBTMC_SAMPLE_FREQ_LEN];
    /** Used as a flag for the user code, to inform it that the host has been sent a request 
     * to change the sample frequency. The sample frequency will be sent on the next OUT transaction. 
     * It contains the endpoint address when set. The following code is recommended for 
     * detecting new sample frequency in main code:
     * \snippet  /USBFS_sut_02.cydsn/main.c Detecting new Sample Frequency
     *     
     * The USBFS_transferState variable is checked to make sure that the transfer completes. */
    volatile uint8 USBTMC_frequencyChanged;
    /** Contains the mute configuration set by the host.*/
    volatile uint8 USBTMC_currentMute;
    /** Contains the volume level set by the host.*/
    volatile uint8 USBTMC_currentVolume[USBTMC_VOLUME_LEN];
    volatile uint8 USBTMC_minimumVolume[USBTMC_VOLUME_LEN] = {USBTMC_VOL_MIN_LSB,
                                                                                  USBTMC_VOL_MIN_MSB};
    volatile uint8 USBTMC_maximumVolume[USBTMC_VOLUME_LEN] = {USBTMC_VOL_MAX_LSB,
                                                                                  USBTMC_VOL_MAX_MSB};
    volatile uint8 USBTMC_resolutionVolume[USBTMC_VOLUME_LEN] = {USBTMC_VOL_RES_LSB,
                                                                                     USBTMC_VOL_RES_MSB};
#endif /*  USBTMC_ENABLE_AUDIO_STREAMING */


/*******************************************************************************
* Function Name: USBTMC_DispatchAUDIOClassRqst
****************************************************************************//**
*
*  This routine dispatches class requests
*
* \return
*  Results of Audio Class request handling: 
*  - USBTMC_TRUE  - request was handled without errors
*  - USBTMC_FALSE - error occurs during handling of request     
*
* \globalvars
*   USBTMC_currentSampleFrequency: Contains the current audio Sample
*       Frequency. It is set by the Host using SET_CUR request to the endpoint.
*   USBTMC_frequencyChanged: This variable is used as a flag for the
*       user code, to be aware that Host has been sent request for changing
*       Sample Frequency. Sample frequency will be sent on the next OUT
*       transaction. It is contains endpoint address when set. The following
*       code is recommended for detecting new Sample Frequency in main code:
*       
*  \snippet  /USBFS_sut_02.cydsn/main.c Detecting new Sample Frequency
*      
*   USBTMC_transferState variable is checked to be sure that transfer
*              completes.
*   USBTMC_currentMute: Contains mute configuration set by Host.
*   USBTMC_currentVolume: Contains volume level set by Host.
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBTMC_DispatchAUDIOClassRqst(void) 
{
    uint8 requestHandled = USBTMC_FALSE;
    
    uint8 RqstRcpt = (uint8)(USBTMC_bmRequestTypeReg & USBTMC_RQST_RCPT_MASK);
#if defined(USBTMC_ENABLE_AUDIO_STREAMING)
    uint8 wValueHi = (uint8) USBTMC_wValueHiReg;
    uint8 epNumber = (uint8) USBTMC_wIndexLoReg & USBTMC_DIR_UNUSED;
#endif /* (USBTMC_ENABLE_AUDIO_STREAMING) */
    
    /* Check request direction: D2H or H2D. */
    if (0u != (USBTMC_bmRequestTypeReg & USBTMC_RQST_DIR_D2H))
    {
        /* Handle direction from device to host. */
        
        if (USBTMC_RQST_RCPT_EP == RqstRcpt)
        {
            /* Request recipient is to endpoint. */
            switch (USBTMC_bRequestReg)
            {
                case USBTMC_GET_CUR:
                #if defined(USBTMC_ENABLE_AUDIO_STREAMING)
                    if (wValueHi == USBTMC_SAMPLING_FREQ_CONTROL)
                    {
                         /* point Control Selector is Sampling Frequency */
                        USBTMC_currentTD.wCount = USBTMC_SAMPLE_FREQ_LEN;
                        USBTMC_currentTD.pData  = USBTMC_currentSampleFrequency[epNumber];
                        
                        requestHandled   = USBTMC_InitControlRead();
                    }
                #endif /* (USBTMC_ENABLE_AUDIO_STREAMING) */
                
                    /* `#START AUDIO_READ_REQUESTS` Place other request handler here */

                    /* `#END` */
                
                #ifdef USBTMC_DISPATCH_AUDIO_CLASS_AUDIO_READ_REQUESTS_CALLBACK    
                    USBTMC_DispatchAUDIOClass_AUDIO_READ_REQUESTS_Callback();
                #endif /* (USBTMC_DISPATCH_AUDIO_CLASS_AUDIO_READ_REQUESTS_CALLBACK) */                   
                break;
                
                default:
                    /* Do not handle this request unless callback is defined. */
                    break;
            }
        
        }
        else if (USBTMC_RQST_RCPT_IFC == RqstRcpt)
        {
            /* Request recipient is interface or entity ID. */
            switch (USBTMC_bRequestReg)
            {
                case USBTMC_GET_CUR:
                #if defined(USBTMC_ENABLE_AUDIO_STREAMING)
                    if (wValueHi == USBTMC_MUTE_CONTROL)
                    {
                        /* `#START MUTE_CONTROL_GET_REQUEST` Place multi-channel handler here */

                        /* `#END` */

                    #ifdef USBTMC_DISPATCH_AUDIO_CLASS_MUTE_CONTROL_GET_REQUEST_CALLBACK
                        USBTMC_DispatchAUDIOClass_MUTE_CONTROL_GET_REQUEST_Callback();
                    #endif /* (USBTMC_DISPATCH_AUDIO_CLASS_MUTE_CONTROL_GET_REQUEST_CALLBACK) */

                        /* Entity ID Control Selector is MUTE */
                        USBTMC_currentTD.wCount = 1u;
                        USBTMC_currentTD.pData  = &USBTMC_currentMute;
                        
                        requestHandled = USBTMC_InitControlRead();
                    }
                    else if (wValueHi == USBTMC_VOLUME_CONTROL)
                    {
                        /* `#START VOLUME_CONTROL_GET_REQUEST` Place multi-channel handler here */

                        /* `#END` */

                    #ifdef USBTMC_DISPATCH_AUDIO_CLASS_VOLUME_CONTROL_GET_REQUEST_CALLBACK
                        USBTMC_DispatchAUDIOClass_VOLUME_CONTROL_GET_REQUEST_Callback();
                    #endif /* (USBTMC_DISPATCH_AUDIO_CLASS_VOLUME_CONTROL_GET_REQUEST_CALLBACK) */

                        /* Entity ID Control Selector is VOLUME, */
                        USBTMC_currentTD.wCount = USBTMC_VOLUME_LEN;
                        USBTMC_currentTD.pData  = USBTMC_currentVolume;
                        
                        requestHandled = USBTMC_InitControlRead();
                    }
                    else
                    {
                        /* `#START OTHER_GET_CUR_REQUESTS` Place other request handler here */

                        /* `#END` */

                    #ifdef USBTMC_DISPATCH_AUDIO_CLASS_OTHER_GET_CUR_REQUESTS_CALLBACK
                        USBTMC_DispatchAUDIOClass_OTHER_GET_CUR_REQUESTS_Callback();
                    #endif /* (USBTMC_DISPATCH_AUDIO_CLASS_OTHER_GET_CUR_REQUESTS_CALLBACK) */
                    }
                    break;
                    
                case USBTMC_GET_MIN:
                    if (wValueHi == USBTMC_VOLUME_CONTROL)
                    {
                        /* Entity ID Control Selector is VOLUME, */
                        USBTMC_currentTD.wCount = USBTMC_VOLUME_LEN;
                        USBTMC_currentTD.pData  = &USBTMC_minimumVolume[0];
                        
                        requestHandled = USBTMC_InitControlRead();
                    }
                    break;
                    
                case USBTMC_GET_MAX:
                    if (wValueHi == USBTMC_VOLUME_CONTROL)
                    {
                        /* Entity ID Control Selector is VOLUME, */
                        USBTMC_currentTD.wCount = USBTMC_VOLUME_LEN;
                        USBTMC_currentTD.pData  = &USBTMC_maximumVolume[0];
                        
                        requestHandled = USBTMC_InitControlRead();
                    }
                    break;
                    
                case USBTMC_GET_RES:
                    if (wValueHi == USBTMC_VOLUME_CONTROL)
                    {
                         /* Entity ID Control Selector is VOLUME, */
                        USBTMC_currentTD.wCount = USBTMC_VOLUME_LEN;
                        USBTMC_currentTD.pData  = &USBTMC_resolutionVolume[0];
                        
                        requestHandled   = USBTMC_InitControlRead();
                    }
                    break;
                    
                /* The contents of the status message is reserved for future use.
                * For the time being, a null packet should be returned in the data stage of the
                * control transfer, and the received null packet should be ACKed.
                */
                case USBTMC_GET_STAT:
                    USBTMC_currentTD.wCount = 0u;    
                    
                    requestHandled = USBTMC_InitControlWrite();

                #endif /* (USBTMC_ENABLE_AUDIO_STREAMING) */
                
                    /* `#START AUDIO_WRITE_REQUESTS` Place other request handler here */

                    /* `#END` */
                
                #ifdef USBTMC_DISPATCH_AUDIO_CLASS_AUDIO_WRITE_REQUESTS_CALLBACK
                    USBTMC_DispatchAUDIOClass_AUDIO_WRITE_REQUESTS_Callback();
                #endif /* (USBTMC_DISPATCH_AUDIO_CLASS_AUDIO_WRITE_REQUESTS_CALLBACK) */
                    break;
                
                default:
                    /* Do not handle this request. */
                    break;
            }
        }
        else
        {   
            /* Do not handle other requests recipients. */
        }
    }
    else
    {
        /* Handle direction from host to device. */
        
        if (USBTMC_RQST_RCPT_EP == RqstRcpt)
        {
            /* Request recipient is endpoint. */
            switch (USBTMC_bRequestReg)
            {
                case USBTMC_SET_CUR:
                #if defined(USBTMC_ENABLE_AUDIO_STREAMING)
                    if (wValueHi == USBTMC_SAMPLING_FREQ_CONTROL)
                    {
                         /* point Control Selector is Sampling Frequency */
                        USBTMC_currentTD.wCount = USBTMC_SAMPLE_FREQ_LEN;
                        USBTMC_currentTD.pData  = USBTMC_currentSampleFrequency[epNumber];
                        USBTMC_frequencyChanged = (uint8) epNumber;
                        
                        requestHandled   = USBTMC_InitControlWrite();
                    }
                #endif /* (USBTMC_ENABLE_AUDIO_STREAMING) */

                    /* `#START AUDIO_SAMPLING_FREQ_REQUESTS` Place other request handler here */

                    /* `#END` */

                #ifdef USBTMC_DISPATCH_AUDIO_CLASS_AUDIO_SAMPLING_FREQ_REQUESTS_CALLBACK
                    USBTMC_DispatchAUDIOClass_AUDIO_SAMPLING_FREQ_REQUESTS_Callback();
                #endif /* (USBTMC_DISPATCH_AUDIO_CLASS_AUDIO_SAMPLING_FREQ_REQUESTS_CALLBACK) */
                    break;
                
                default:
                    /* Do not handle this request. */
                    break;
            }
        }
        else if(USBTMC_RQST_RCPT_IFC == RqstRcpt)
        {
            /* Request recipient is interface or entity ID. */
            switch (USBTMC_bRequestReg)
            {
                case USBTMC_SET_CUR:
                #if defined(USBTMC_ENABLE_AUDIO_STREAMING)
                    if (wValueHi == USBTMC_MUTE_CONTROL)
                    {
                        /* `#START MUTE_SET_REQUEST` Place multi-channel handler here */

                        /* `#END` */

                    #ifdef USBTMC_DISPATCH_AUDIO_CLASS_MUTE_SET_REQUEST_CALLBACK
                        USBTMC_DispatchAUDIOClass_MUTE_SET_REQUEST_Callback();
                    #endif /* (USBTMC_DISPATCH_AUDIO_CLASS_MUTE_SET_REQUEST_CALLBACK) */

                        /* Entity ID Control Selector is MUTE */
                        USBTMC_currentTD.wCount = 1u;
                        USBTMC_currentTD.pData  = &USBTMC_currentMute;
                        
                        requestHandled = USBTMC_InitControlWrite();
                    }
                    else if (wValueHi == USBTMC_VOLUME_CONTROL)
                    {
                        /* `#START VOLUME_CONTROL_SET_REQUEST` Place multi-channel handler here */

                        /* `#END` */

                    #ifdef USBTMC_DISPATCH_AUDIO_CLASS_VOLUME_CONTROL_SET_REQUEST_CALLBACK
                        USBTMC_DispatchAUDIOClass_VOLUME_CONTROL_SET_REQUEST_Callback();
                    #endif /* (USBTMC_DISPATCH_AUDIO_CLASS_VOLUME_CONTROL_SET_REQUEST_CALLBACK) */

                        /* Entity ID Control Selector is VOLUME */
                        USBTMC_currentTD.wCount = USBTMC_VOLUME_LEN;
                        USBTMC_currentTD.pData  = USBTMC_currentVolume;
                        
                        requestHandled = USBTMC_InitControlWrite();
                    }
                    else
                    {
                        /* `#START OTHER_SET_CUR_REQUESTS` Place other request handler here */

                        /* `#END` */

                    #ifdef USBTMC_DISPATCH_AUDIO_CLASS_OTHER_SET_CUR_REQUESTS_CALLBACK
                        USBTMC_DispatchAUDIOClass_OTHER_SET_CUR_REQUESTS_Callback();
                    #endif /* (USBTMC_DISPATCH_AUDIO_CLASS_OTHER_SET_CUR_REQUESTS_CALLBACK) */
                    }
                #endif /*  USBTMC_ENABLE_AUDIO_STREAMING */
                
                
                    /* `#START AUDIO_CONTROL_SEL_REQUESTS` Place other request handler here */

                    /* `#END` */
                    
                #ifdef USBTMC_DISPATCH_AUDIO_CLASS_AUDIO_CONTROL_SEL_REQUESTS_CALLBACK
                    USBTMC_DispatchAUDIOClass_AUDIO_CONTROL_SEL_REQUESTS_Callback();
                #endif /* (USBTMC_DISPATCH_AUDIO_CLASS_AUDIO_CONTROL_SEL_REQUESTS_CALLBACK) */
                break;

                default:
                    /* Do not handle this request. */
                break;
            }
        }
        else
        {
            /* Do not handle other requests recipients. */
        }
    }

    return (requestHandled);
}
#endif /* (USER_SUPPLIED_AUDIO_HANDLER) */


/*******************************************************************************
* Additional user functions supporting AUDIO Requests
********************************************************************************/

/* `#START AUDIO_FUNCTIONS` Place any additional functions here */

/* `#END` */

#endif  /* (USBTMC_ENABLE_AUDIO_CLASS) */


/* [] END OF FILE */
