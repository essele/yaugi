/*******************************************************************************
* File Name: USBUART2_Dp.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "USBUART2_Dp.h"

static USBUART2_Dp_BACKUP_STRUCT  USBUART2_Dp_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: USBUART2_Dp_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet USBUART2_Dp_SUT.c usage_USBUART2_Dp_Sleep_Wakeup
*******************************************************************************/
void USBUART2_Dp_Sleep(void)
{
    #if defined(USBUART2_Dp__PC)
        USBUART2_Dp_backup.pcState = USBUART2_Dp_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            USBUART2_Dp_backup.usbState = USBUART2_Dp_CR1_REG;
            USBUART2_Dp_USB_POWER_REG |= USBUART2_Dp_USBIO_ENTER_SLEEP;
            USBUART2_Dp_CR1_REG &= USBUART2_Dp_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(USBUART2_Dp__SIO)
        USBUART2_Dp_backup.sioState = USBUART2_Dp_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        USBUART2_Dp_SIO_REG &= (uint32)(~USBUART2_Dp_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: USBUART2_Dp_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to USBUART2_Dp_Sleep() for an example usage.
*******************************************************************************/
void USBUART2_Dp_Wakeup(void)
{
    #if defined(USBUART2_Dp__PC)
        USBUART2_Dp_PC = USBUART2_Dp_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            USBUART2_Dp_USB_POWER_REG &= USBUART2_Dp_USBIO_EXIT_SLEEP_PH1;
            USBUART2_Dp_CR1_REG = USBUART2_Dp_backup.usbState;
            USBUART2_Dp_USB_POWER_REG &= USBUART2_Dp_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(USBUART2_Dp__SIO)
        USBUART2_Dp_SIO_REG = USBUART2_Dp_backup.sioState;
    #endif
}


/* [] END OF FILE */
