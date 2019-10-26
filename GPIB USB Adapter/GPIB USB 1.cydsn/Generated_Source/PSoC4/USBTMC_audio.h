/***************************************************************************//**
* \file USBTMC_audio.h
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

#if !defined(CY_USBFS_USBTMC_audio_H)
#define CY_USBFS_USBTMC_audio_H

#include "USBTMC.h"


/***************************************
* Custom Declarations
***************************************/

/* `#START CUSTOM_CONSTANTS` Place your declaration here */

/* `#END` */


/***************************************
*  Constants for USBTMC_audio API.
***************************************/

/* Audio Class-Specific Request Codes (AUDIO Table A-9) */
#define USBTMC_REQUEST_CODE_UNDEFINED     (0x00u)
#define USBTMC_SET_CUR                    (0x01u)
#define USBTMC_GET_CUR                    (0x81u)
#define USBTMC_SET_MIN                    (0x02u)
#define USBTMC_GET_MIN                    (0x82u)
#define USBTMC_SET_MAX                    (0x03u)
#define USBTMC_GET_MAX                    (0x83u)
#define USBTMC_SET_RES                    (0x04u)
#define USBTMC_GET_RES                    (0x84u)
#define USBTMC_SET_MEM                    (0x05u)
#define USBTMC_GET_MEM                    (0x85u)
#define USBTMC_GET_STAT                   (0xFFu)

/* point Control Selectors (AUDIO Table A-19) */
#define USBTMC_EP_CONTROL_UNDEFINED       (0x00u)
#define USBTMC_SAMPLING_FREQ_CONTROL      (0x01u)
#define USBTMC_PITCH_CONTROL              (0x02u)

/* Feature Unit Control Selectors (AUDIO Table A-11) */
#define USBTMC_FU_CONTROL_UNDEFINED       (0x00u)
#define USBTMC_MUTE_CONTROL               (0x01u)
#define USBTMC_VOLUME_CONTROL             (0x02u)
#define USBTMC_BASS_CONTROL               (0x03u)
#define USBTMC_MID_CONTROL                (0x04u)
#define USBTMC_TREBLE_CONTROL             (0x05u)
#define USBTMC_GRAPHIC_EQUALIZER_CONTROL  (0x06u)
#define USBTMC_AUTOMATIC_GAIN_CONTROL     (0x07u)
#define USBTMC_DELAY_CONTROL              (0x08u)
#define USBTMC_BASS_BOOST_CONTROL         (0x09u)
#define USBTMC_LOUDNESS_CONTROL           (0x0Au)

#define USBTMC_SAMPLE_FREQ_LEN            (3u)
#define USBTMC_VOLUME_LEN                 (2u)

#if !defined(USER_SUPPLIED_DEFAULT_VOLUME_VALUE)
    #define USBTMC_VOL_MIN_MSB            (0x80u)
    #define USBTMC_VOL_MIN_LSB            (0x01u)
    #define USBTMC_VOL_MAX_MSB            (0x7Fu)
    #define USBTMC_VOL_MAX_LSB            (0xFFu)
    #define USBTMC_VOL_RES_MSB            (0x00u)
    #define USBTMC_VOL_RES_LSB            (0x01u)
#endif /* USER_SUPPLIED_DEFAULT_VOLUME_VALUE */


/***************************************
* External data references
***************************************/
/**
* \addtogroup group_audio
* @{
*/
extern volatile uint8 USBTMC_currentSampleFrequency[USBTMC_MAX_EP][USBTMC_SAMPLE_FREQ_LEN];
extern volatile uint8 USBTMC_frequencyChanged;
extern volatile uint8 USBTMC_currentMute;
extern volatile uint8 USBTMC_currentVolume[USBTMC_VOLUME_LEN];
/** @} audio */

extern volatile uint8 USBTMC_minimumVolume[USBTMC_VOLUME_LEN];
extern volatile uint8 USBTMC_maximumVolume[USBTMC_VOLUME_LEN];
extern volatile uint8 USBTMC_resolutionVolume[USBTMC_VOLUME_LEN];

#endif /*  CY_USBFS_USBTMC_audio_H */


/* [] END OF FILE */
