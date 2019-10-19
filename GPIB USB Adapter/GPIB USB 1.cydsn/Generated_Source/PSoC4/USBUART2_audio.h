/***************************************************************************//**
* \file USBUART2_audio.h
* \version 3.20
*
* \brief
*  This file provides function prototypes and constants for the USBFS component 
*  Audio class.
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

#if !defined(CY_USBFS_USBUART2_audio_H)
#define CY_USBFS_USBUART2_audio_H

#include "USBUART2.h"


/***************************************
* Custom Declarations
***************************************/

/* `#START CUSTOM_CONSTANTS` Place your declaration here */

/* `#END` */


/***************************************
*  Constants for USBUART2_audio API.
***************************************/

/* Audio Class-Specific Request Codes (AUDIO Table A-9) */
#define USBUART2_REQUEST_CODE_UNDEFINED     (0x00u)
#define USBUART2_SET_CUR                    (0x01u)
#define USBUART2_GET_CUR                    (0x81u)
#define USBUART2_SET_MIN                    (0x02u)
#define USBUART2_GET_MIN                    (0x82u)
#define USBUART2_SET_MAX                    (0x03u)
#define USBUART2_GET_MAX                    (0x83u)
#define USBUART2_SET_RES                    (0x04u)
#define USBUART2_GET_RES                    (0x84u)
#define USBUART2_SET_MEM                    (0x05u)
#define USBUART2_GET_MEM                    (0x85u)
#define USBUART2_GET_STAT                   (0xFFu)

/* point Control Selectors (AUDIO Table A-19) */
#define USBUART2_EP_CONTROL_UNDEFINED       (0x00u)
#define USBUART2_SAMPLING_FREQ_CONTROL      (0x01u)
#define USBUART2_PITCH_CONTROL              (0x02u)

/* Feature Unit Control Selectors (AUDIO Table A-11) */
#define USBUART2_FU_CONTROL_UNDEFINED       (0x00u)
#define USBUART2_MUTE_CONTROL               (0x01u)
#define USBUART2_VOLUME_CONTROL             (0x02u)
#define USBUART2_BASS_CONTROL               (0x03u)
#define USBUART2_MID_CONTROL                (0x04u)
#define USBUART2_TREBLE_CONTROL             (0x05u)
#define USBUART2_GRAPHIC_EQUALIZER_CONTROL  (0x06u)
#define USBUART2_AUTOMATIC_GAIN_CONTROL     (0x07u)
#define USBUART2_DELAY_CONTROL              (0x08u)
#define USBUART2_BASS_BOOST_CONTROL         (0x09u)
#define USBUART2_LOUDNESS_CONTROL           (0x0Au)

#define USBUART2_SAMPLE_FREQ_LEN            (3u)
#define USBUART2_VOLUME_LEN                 (2u)

#if !defined(USER_SUPPLIED_DEFAULT_VOLUME_VALUE)
    #define USBUART2_VOL_MIN_MSB            (0x80u)
    #define USBUART2_VOL_MIN_LSB            (0x01u)
    #define USBUART2_VOL_MAX_MSB            (0x7Fu)
    #define USBUART2_VOL_MAX_LSB            (0xFFu)
    #define USBUART2_VOL_RES_MSB            (0x00u)
    #define USBUART2_VOL_RES_LSB            (0x01u)
#endif /* USER_SUPPLIED_DEFAULT_VOLUME_VALUE */


/***************************************
* External data references
***************************************/
/**
* \addtogroup group_audio
* @{
*/
extern volatile uint8 USBUART2_currentSampleFrequency[USBUART2_MAX_EP][USBUART2_SAMPLE_FREQ_LEN];
extern volatile uint8 USBUART2_frequencyChanged;
extern volatile uint8 USBUART2_currentMute;
extern volatile uint8 USBUART2_currentVolume[USBUART2_VOLUME_LEN];
/** @} audio */

extern volatile uint8 USBUART2_minimumVolume[USBUART2_VOLUME_LEN];
extern volatile uint8 USBUART2_maximumVolume[USBUART2_VOLUME_LEN];
extern volatile uint8 USBUART2_resolutionVolume[USBUART2_VOLUME_LEN];

#endif /*  CY_USBFS_USBUART2_audio_H */


/* [] END OF FILE */
