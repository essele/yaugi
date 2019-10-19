/*******************************************************************************
* File Name: USBUART2_Dm.c  
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
#include "USBUART2_Dm.h"

static USBUART2_Dm_BACKUP_STRUCT  USBUART2_Dm_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: USBUART2_Dm_Sleep
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
*  \snippet USBUART2_Dm_SUT.c usage_USBUART2_Dm_Sleep_Wakeup
*******************************************************************************/
void USBUART2_Dm_Sleep(void)
{
    #if defined(USBUART2_Dm__PC)
        USBUART2_Dm_backup.pcState = USBUART2_Dm_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            USBUART2_Dm_backup.usbState = USBUART2_Dm_CR1_REG;
            USBUART2_Dm_USB_POWER_REG |= USBUART2_Dm_USBIO_ENTER_SLEEP;
            USBUART2_Dm_CR1_REG &= USBUART2_Dm_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(USBUART2_Dm__SIO)
        USBUART2_Dm_backup.sioState = USBUART2_Dm_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        USBUART2_Dm_SIO_REG &= (uint32)(~USBUART2_Dm_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: USBUART2_Dm_Wakeup
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
*  Refer to USBUART2_Dm_Sleep() for an example usage.
*******************************************************************************/
void USBUART2_Dm_Wakeup(void)
{
    #if defined(USBUART2_Dm__PC)
        USBUART2_Dm_PC = USBUART2_Dm_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            USBUART2_Dm_USB_POWER_REG &= USBUART2_Dm_USBIO_EXIT_SLEEP_PH1;
            USBUART2_Dm_CR1_REG = USBUART2_Dm_backup.usbState;
            USBUART2_Dm_USB_POWER_REG &= USBUART2_Dm_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(USBUART2_Dm__SIO)
        USBUART2_Dm_SIO_REG = USBUART2_Dm_backup.sioState;
    #endif
}


/* [] END OF FILE */
