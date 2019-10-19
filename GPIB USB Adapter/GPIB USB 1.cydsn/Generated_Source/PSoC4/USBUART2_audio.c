/***************************************************************************//**
* \file USBUART2_audio.c
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

#include "USBUART2_audio.h"
#include "USBUART2_pvt.h"
#include "cyapicallbacks.h"

#if defined(USBUART2_ENABLE_AUDIO_CLASS)

/***************************************
* Custom Declarations
***************************************/

/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


#if !defined(USER_SUPPLIED_AUDIO_HANDLER)

/***************************************
*    AUDIO Variables
***************************************/

#if defined(USBUART2_ENABLE_AUDIO_STREAMING)
    /** Contains the current audio sample frequency. It is set by the host using a SET_CUR request to the endpoint.*/
    volatile uint8 USBUART2_currentSampleFrequency[USBUART2_MAX_EP][USBUART2_SAMPLE_FREQ_LEN];
    /** Used as a flag for the user code, to inform it that the host has been sent a request 
     * to change the sample frequency. The sample frequency will be sent on the next OUT transaction. 
     * It contains the endpoint address when set. The following code is recommended for 
     * detecting new sample frequency in main code:
     * \snippet  /USBFS_sut_02.cydsn/main.c Detecting new Sample Frequency
     *     
     * The USBFS_transferState variable is checked to make sure that the transfer completes. */
    volatile uint8 USBUART2_frequencyChanged;
    /** Contains the mute configuration set by the host.*/
    volatile uint8 USBUART2_currentMute;
    /** Contains the volume level set by the host.*/
    volatile uint8 USBUART2_currentVolume[USBUART2_VOLUME_LEN];
    volatile uint8 USBUART2_minimumVolume[USBUART2_VOLUME_LEN] = {USBUART2_VOL_MIN_LSB,
                                                                                  USBUART2_VOL_MIN_MSB};
    volatile uint8 USBUART2_maximumVolume[USBUART2_VOLUME_LEN] = {USBUART2_VOL_MAX_LSB,
                                                                                  USBUART2_VOL_MAX_MSB};
    volatile uint8 USBUART2_resolutionVolume[USBUART2_VOLUME_LEN] = {USBUART2_VOL_RES_LSB,
                                                                                     USBUART2_VOL_RES_MSB};
#endif /*  USBUART2_ENABLE_AUDIO_STREAMING */


/*******************************************************************************
* Function Name: USBUART2_DispatchAUDIOClassRqst
****************************************************************************//**
*
*  This routine dispatches class requests
*
* \return
*  Results of Audio Class request handling: 
*  - USBUART2_TRUE  - request was handled without errors
*  - USBUART2_FALSE - error occurs during handling of request     
*
* \globalvars
*   USBUART2_currentSampleFrequency: Contains the current audio Sample
*       Frequency. It is set by the Host using SET_CUR request to the endpoint.
*   USBUART2_frequencyChanged: This variable is used as a flag for the
*       user code, to be aware that Host has been sent request for changing
*       Sample Frequency. Sample frequency will be sent on the next OUT
*       transaction. It is contains endpoint address when set. The following
*       code is recommended for detecting new Sample Frequency in main code:
*       
*  \snippet  /USBFS_sut_02.cydsn/main.c Detecting new Sample Frequency
*      
*   USBUART2_transferState variable is checked to be sure that transfer
*              completes.
*   USBUART2_currentMute: Contains mute configuration set by Host.
*   USBUART2_currentVolume: Contains volume level set by Host.
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBUART2_DispatchAUDIOClassRqst(void) 
{
    uint8 requestHandled = USBUART2_FALSE;
    
    uint8 RqstRcpt = (uint8)(USBUART2_bmRequestTypeReg & USBUART2_RQST_RCPT_MASK);
#if defined(USBUART2_ENABLE_AUDIO_STREAMING)
    uint8 wValueHi = (uint8) USBUART2_wValueHiReg;
    uint8 epNumber = (uint8) USBUART2_wIndexLoReg & USBUART2_DIR_UNUSED;
#endif /* (USBUART2_ENABLE_AUDIO_STREAMING) */
    
    /* Check request direction: D2H or H2D. */
    if (0u != (USBUART2_bmRequestTypeReg & USBUART2_RQST_DIR_D2H))
    {
        /* Handle direction from device to host. */
        
        if (USBUART2_RQST_RCPT_EP == RqstRcpt)
        {
            /* Request recipient is to endpoint. */
            switch (USBUART2_bRequestReg)
            {
                case USBUART2_GET_CUR:
                #if defined(USBUART2_ENABLE_AUDIO_STREAMING)
                    if (wValueHi == USBUART2_SAMPLING_FREQ_CONTROL)
                    {
                         /* point Control Selector is Sampling Frequency */
                        USBUART2_currentTD.wCount = USBUART2_SAMPLE_FREQ_LEN;
                        USBUART2_currentTD.pData  = USBUART2_currentSampleFrequency[epNumber];
                        
                        requestHandled   = USBUART2_InitControlRead();
                    }
                #endif /* (USBUART2_ENABLE_AUDIO_STREAMING) */
                
                    /* `#START AUDIO_READ_REQUESTS` Place other request handler here */

                    /* `#END` */
                
                #ifdef USBUART2_DISPATCH_AUDIO_CLASS_AUDIO_READ_REQUESTS_CALLBACK    
                    USBUART2_DispatchAUDIOClass_AUDIO_READ_REQUESTS_Callback();
                #endif /* (USBUART2_DISPATCH_AUDIO_CLASS_AUDIO_READ_REQUESTS_CALLBACK) */                   
                break;
                
                default:
                    /* Do not handle this request unless callback is defined. */
                    break;
            }
        
        }
        else if (USBUART2_RQST_RCPT_IFC == RqstRcpt)
        {
            /* Request recipient is interface or entity ID. */
            switch (USBUART2_bRequestReg)
            {
                case USBUART2_GET_CUR:
                #if defined(USBUART2_ENABLE_AUDIO_STREAMING)
                    if (wValueHi == USBUART2_MUTE_CONTROL)
                    {
                        /* `#START MUTE_CONTROL_GET_REQUEST` Place multi-channel handler here */

                        /* `#END` */

                    #ifdef USBUART2_DISPATCH_AUDIO_CLASS_MUTE_CONTROL_GET_REQUEST_CALLBACK
                        USBUART2_DispatchAUDIOClass_MUTE_CONTROL_GET_REQUEST_Callback();
                    #endif /* (USBUART2_DISPATCH_AUDIO_CLASS_MUTE_CONTROL_GET_REQUEST_CALLBACK) */

                        /* Entity ID Control Selector is MUTE */
                        USBUART2_currentTD.wCount = 1u;
                        USBUART2_currentTD.pData  = &USBUART2_currentMute;
                        
                        requestHandled = USBUART2_InitControlRead();
                    }
                    else if (wValueHi == USBUART2_VOLUME_CONTROL)
                    {
                        /* `#START VOLUME_CONTROL_GET_REQUEST` Place multi-channel handler here */

                        /* `#END` */

                    #ifdef USBUART2_DISPATCH_AUDIO_CLASS_VOLUME_CONTROL_GET_REQUEST_CALLBACK
                        USBUART2_DispatchAUDIOClass_VOLUME_CONTROL_GET_REQUEST_Callback();
                    #endif /* (USBUART2_DISPATCH_AUDIO_CLASS_VOLUME_CONTROL_GET_REQUEST_CALLBACK) */

                        /* Entity ID Control Selector is VOLUME, */
                        USBUART2_currentTD.wCount = USBUART2_VOLUME_LEN;
                        USBUART2_currentTD.pData  = USBUART2_currentVolume;
                        
                        requestHandled = USBUART2_InitControlRead();
                    }
                    else
                    {
                        /* `#START OTHER_GET_CUR_REQUESTS` Place other request handler here */

                        /* `#END` */

                    #ifdef USBUART2_DISPATCH_AUDIO_CLASS_OTHER_GET_CUR_REQUESTS_CALLBACK
                        USBUART2_DispatchAUDIOClass_OTHER_GET_CUR_REQUESTS_Callback();
                    #endif /* (USBUART2_DISPATCH_AUDIO_CLASS_OTHER_GET_CUR_REQUESTS_CALLBACK) */
                    }
                    break;
                    
                case USBUART2_GET_MIN:
                    if (wValueHi == USBUART2_VOLUME_CONTROL)
                    {
                        /* Entity ID Control Selector is VOLUME, */
                        USBUART2_currentTD.wCount = USBUART2_VOLUME_LEN;
                        USBUART2_currentTD.pData  = &USBUART2_minimumVolume[0];
                        
                        requestHandled = USBUART2_InitControlRead();
                    }
                    break;
                    
                case USBUART2_GET_MAX:
                    if (wValueHi == USBUART2_VOLUME_CONTROL)
                    {
                        /* Entity ID Control Selector is VOLUME, */
                        USBUART2_currentTD.wCount = USBUART2_VOLUME_LEN;
                        USBUART2_currentTD.pData  = &USBUART2_maximumVolume[0];
                        
                        requestHandled = USBUART2_InitControlRead();
                    }
                    break;
                    
                case USBUART2_GET_RES:
                    if (wValueHi == USBUART2_VOLUME_CONTROL)
                    {
                         /* Entity ID Control Selector is VOLUME, */
                        USBUART2_currentTD.wCount = USBUART2_VOLUME_LEN;
                        USBUART2_currentTD.pData  = &USBUART2_resolutionVolume[0];
                        
                        requestHandled   = USBUART2_InitControlRead();
                    }
                    break;
                    
                /* The contents of the status message is reserved for future use.
                * For the time being, a null packet should be returned in the data stage of the
                * control transfer, and the received null packet should be ACKed.
                */
                case USBUART2_GET_STAT:
                    USBUART2_currentTD.wCount = 0u;    
                    
                    requestHandled = USBUART2_InitControlWrite();

                #endif /* (USBUART2_ENABLE_AUDIO_STREAMING) */
                
                    /* `#START AUDIO_WRITE_REQUESTS` Place other request handler here */

                    /* `#END` */
                
                #ifdef USBUART2_DISPATCH_AUDIO_CLASS_AUDIO_WRITE_REQUESTS_CALLBACK
                    USBUART2_DispatchAUDIOClass_AUDIO_WRITE_REQUESTS_Callback();
                #endif /* (USBUART2_DISPATCH_AUDIO_CLASS_AUDIO_WRITE_REQUESTS_CALLBACK) */
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
        
        if (USBUART2_RQST_RCPT_EP == RqstRcpt)
        {
            /* Request recipient is endpoint. */
            switch (USBUART2_bRequestReg)
            {
                case USBUART2_SET_CUR:
                #if defined(USBUART2_ENABLE_AUDIO_STREAMING)
                    if (wValueHi == USBUART2_SAMPLING_FREQ_CONTROL)
                    {
                         /* point Control Selector is Sampling Frequency */
                        USBUART2_currentTD.wCount = USBUART2_SAMPLE_FREQ_LEN;
                        USBUART2_currentTD.pData  = USBUART2_currentSampleFrequency[epNumber];
                        USBUART2_frequencyChanged = (uint8) epNumber;
                        
                        requestHandled   = USBUART2_InitControlWrite();
                    }
                #endif /* (USBUART2_ENABLE_AUDIO_STREAMING) */

                    /* `#START AUDIO_SAMPLING_FREQ_REQUESTS` Place other request handler here */

                    /* `#END` */

                #ifdef USBUART2_DISPATCH_AUDIO_CLASS_AUDIO_SAMPLING_FREQ_REQUESTS_CALLBACK
                    USBUART2_DispatchAUDIOClass_AUDIO_SAMPLING_FREQ_REQUESTS_Callback();
                #endif /* (USBUART2_DISPATCH_AUDIO_CLASS_AUDIO_SAMPLING_FREQ_REQUESTS_CALLBACK) */
                    break;
                
                default:
                    /* Do not handle this request. */
                    break;
            }
        }
        else if(USBUART2_RQST_RCPT_IFC == RqstRcpt)
        {
            /* Request recipient is interface or entity ID. */
            switch (USBUART2_bRequestReg)
            {
                case USBUART2_SET_CUR:
                #if defined(USBUART2_ENABLE_AUDIO_STREAMING)
                    if (wValueHi == USBUART2_MUTE_CONTROL)
                    {
                        /* `#START MUTE_SET_REQUEST` Place multi-channel handler here */

                        /* `#END` */

                    #ifdef USBUART2_DISPATCH_AUDIO_CLASS_MUTE_SET_REQUEST_CALLBACK
                        USBUART2_DispatchAUDIOClass_MUTE_SET_REQUEST_Callback();
                    #endif /* (USBUART2_DISPATCH_AUDIO_CLASS_MUTE_SET_REQUEST_CALLBACK) */

                        /* Entity ID Control Selector is MUTE */
                        USBUART2_currentTD.wCount = 1u;
                        USBUART2_currentTD.pData  = &USBUART2_currentMute;
                        
                        requestHandled = USBUART2_InitControlWrite();
                    }
                    else if (wValueHi == USBUART2_VOLUME_CONTROL)
                    {
                        /* `#START VOLUME_CONTROL_SET_REQUEST` Place multi-channel handler here */

                        /* `#END` */

                    #ifdef USBUART2_DISPATCH_AUDIO_CLASS_VOLUME_CONTROL_SET_REQUEST_CALLBACK
                        USBUART2_DispatchAUDIOClass_VOLUME_CONTROL_SET_REQUEST_Callback();
                    #endif /* (USBUART2_DISPATCH_AUDIO_CLASS_VOLUME_CONTROL_SET_REQUEST_CALLBACK) */

                        /* Entity ID Control Selector is VOLUME */
                        USBUART2_currentTD.wCount = USBUART2_VOLUME_LEN;
                        USBUART2_currentTD.pData  = USBUART2_currentVolume;
                        
                        requestHandled = USBUART2_InitControlWrite();
                    }
                    else
                    {
                        /* `#START OTHER_SET_CUR_REQUESTS` Place other request handler here */

                        /* `#END` */

                    #ifdef USBUART2_DISPATCH_AUDIO_CLASS_OTHER_SET_CUR_REQUESTS_CALLBACK
                        USBUART2_DispatchAUDIOClass_OTHER_SET_CUR_REQUESTS_Callback();
                    #endif /* (USBUART2_DISPATCH_AUDIO_CLASS_OTHER_SET_CUR_REQUESTS_CALLBACK) */
                    }
                #endif /*  USBUART2_ENABLE_AUDIO_STREAMING */
                
                
                    /* `#START AUDIO_CONTROL_SEL_REQUESTS` Place other request handler here */

                    /* `#END` */
                    
                #ifdef USBUART2_DISPATCH_AUDIO_CLASS_AUDIO_CONTROL_SEL_REQUESTS_CALLBACK
                    USBUART2_DispatchAUDIOClass_AUDIO_CONTROL_SEL_REQUESTS_Callback();
                #endif /* (USBUART2_DISPATCH_AUDIO_CLASS_AUDIO_CONTROL_SEL_REQUESTS_CALLBACK) */
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

#endif  /* (USBUART2_ENABLE_AUDIO_CLASS) */


/* [] END OF FILE */
