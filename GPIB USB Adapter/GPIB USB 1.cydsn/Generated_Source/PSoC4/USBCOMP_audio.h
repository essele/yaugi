/***************************************************************************//**
* \file USBCOMP_audio.h
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

#if !defined(CY_USBFS_USBCOMP_audio_H)
#define CY_USBFS_USBCOMP_audio_H

#include "USBCOMP.h"


/***************************************
* Custom Declarations
***************************************/

/* `#START CUSTOM_CONSTANTS` Place your declaration here */

/* `#END` */


/***************************************
*  Constants for USBCOMP_audio API.
***************************************/

/* Audio Class-Specific Request Codes (AUDIO Table A-9) */
#define USBCOMP_REQUEST_CODE_UNDEFINED     (0x00u)
#define USBCOMP_SET_CUR                    (0x01u)
#define USBCOMP_GET_CUR                    (0x81u)
#define USBCOMP_SET_MIN                    (0x02u)
#define USBCOMP_GET_MIN                    (0x82u)
#define USBCOMP_SET_MAX                    (0x03u)
#define USBCOMP_GET_MAX                    (0x83u)
#define USBCOMP_SET_RES                    (0x04u)
#define USBCOMP_GET_RES                    (0x84u)
#define USBCOMP_SET_MEM                    (0x05u)
#define USBCOMP_GET_MEM                    (0x85u)
#define USBCOMP_GET_STAT                   (0xFFu)

/* point Control Selectors (AUDIO Table A-19) */
#define USBCOMP_EP_CONTROL_UNDEFINED       (0x00u)
#define USBCOMP_SAMPLING_FREQ_CONTROL      (0x01u)
#define USBCOMP_PITCH_CONTROL              (0x02u)

/* Feature Unit Control Selectors (AUDIO Table A-11) */
#define USBCOMP_FU_CONTROL_UNDEFINED       (0x00u)
#define USBCOMP_MUTE_CONTROL               (0x01u)
#define USBCOMP_VOLUME_CONTROL             (0x02u)
#define USBCOMP_BASS_CONTROL               (0x03u)
#define USBCOMP_MID_CONTROL                (0x04u)
#define USBCOMP_TREBLE_CONTROL             (0x05u)
#define USBCOMP_GRAPHIC_EQUALIZER_CONTROL  (0x06u)
#define USBCOMP_AUTOMATIC_GAIN_CONTROL     (0x07u)
#define USBCOMP_DELAY_CONTROL              (0x08u)
#define USBCOMP_BASS_BOOST_CONTROL         (0x09u)
#define USBCOMP_LOUDNESS_CONTROL           (0x0Au)

#define USBCOMP_SAMPLE_FREQ_LEN            (3u)
#define USBCOMP_VOLUME_LEN                 (2u)

#if !defined(USER_SUPPLIED_DEFAULT_VOLUME_VALUE)
    #define USBCOMP_VOL_MIN_MSB            (0x80u)
    #define USBCOMP_VOL_MIN_LSB            (0x01u)
    #define USBCOMP_VOL_MAX_MSB            (0x7Fu)
    #define USBCOMP_VOL_MAX_LSB            (0xFFu)
    #define USBCOMP_VOL_RES_MSB            (0x00u)
    #define USBCOMP_VOL_RES_LSB            (0x01u)
#endif /* USER_SUPPLIED_DEFAULT_VOLUME_VALUE */


/***************************************
* External data references
***************************************/
/**
* \addtogroup group_audio
* @{
*/
extern volatile uint8 USBCOMP_currentSampleFrequency[USBCOMP_MAX_EP][USBCOMP_SAMPLE_FREQ_LEN];
extern volatile uint8 USBCOMP_frequencyChanged;
extern volatile uint8 USBCOMP_currentMute;
extern volatile uint8 USBCOMP_currentVolume[USBCOMP_VOLUME_LEN];
/** @} audio */

extern volatile uint8 USBCOMP_minimumVolume[USBCOMP_VOLUME_LEN];
extern volatile uint8 USBCOMP_maximumVolume[USBCOMP_VOLUME_LEN];
extern volatile uint8 USBCOMP_resolutionVolume[USBCOMP_VOLUME_LEN];

#endif /*  CY_USBFS_USBCOMP_audio_H */


/* [] END OF FILE */
