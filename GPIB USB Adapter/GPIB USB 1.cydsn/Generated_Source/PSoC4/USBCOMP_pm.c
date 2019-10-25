/***************************************************************************//**
* \file USBCOMP_pm.c
* \version 3.20
*
* \brief
*  This file provides Suspend/Resume APIs implementation.
*
********************************************************************************
* \copyright
* Copyright 2008-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USBCOMP_pvt.h"
#include "USBCOMP_Dp.h"
#include "cyapicallbacks.h"

/***************************************
* Custom Declarations
***************************************/
/* `#START PM_CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


/***************************************
* Local data allocation
***************************************/

static USBCOMP_BACKUP_STRUCT  USBCOMP_backup;

#if (USBCOMP_DP_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBCOMP_DP_ISR
    ****************************************************************************//**
    *
    *  This Interrupt Service Routine handles DP pin changes for wake-up from
    *  the sleep mode.
    *
    *******************************************************************************/
    CY_ISR(USBCOMP_DP_ISR)
    {
    #ifdef USBCOMP_DP_ISR_ENTRY_CALLBACK
        USBCOMP_DP_ISR_EntryCallback();
    #endif /* (USBCOMP_DP_ISR_ENTRY_CALLBACK) */

        /* `#START DP_USER_CODE` Place your code here */

        /* `#END` */

        (void) USBCOMP_Dp_ClearInterrupt();
		
	#ifdef USBCOMP_DP_ISR_EXIT_CALLBACK
        USBCOMP_DP_ISR_ExitCallback();
    #endif /* (USBCOMP_DP_ISR_EXIT_CALLBACK) */
    }
#endif /* (USBCOMP_DP_ISR_ACTIVE) */


/*******************************************************************************
* Function Name: USBCOMP_SaveConfig
****************************************************************************//**
*
*  Saves the current user configuration.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_SaveConfig(void) 
{
    /* Empty function added for the compatibility purpose. */
}


/*******************************************************************************
* Function Name: USBCOMP_RestoreConfig
****************************************************************************//**
*
*  Restores the current user configuration.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_RestoreConfig(void) 
{
    if (USBCOMP_configuration != 0u)
    {
        USBCOMP_ConfigReg();
        USBCOMP_EpStateInit();
    }
}


/*******************************************************************************
* Function Name: USBCOMP_Suspend
****************************************************************************//**
*
*  This function prepares the USBFS component to enter low power mode. The 
*  interrupt on falling edge on Dp pin is configured to wakeup device when the 
*  host drives resume condition. The pull-up is enabled on the Dp line while 
*  device is in low power mode. The supported low power modes are Deep Sleep 
*  (PSoC 4200L) and Sleep (PSoC 3/ PSoC 5LP).
*  
*  *Note* For PSoC 4200L devices, this function should not be called before 
*  entering Sleep.
*  
*  *Note* After enter low power mode, the data which is left in the IN or OUT 
*  endpoint buffers is not restored after wakeup and lost. Therefore it should 
*  be stored in the SRAM for OUT endpoint or read by the host for IN endpoint 
*  before enter low power mode.
*
* \globalvars
*  USBCOMP_backup.enable:  modified.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_Suspend(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    if (0u != (USBCOMP_CR0_REG & USBCOMP_CR0_ENABLE))
    {
        /* USB block is enabled. */
        USBCOMP_backup.enableState = 1u;

    #if (USBCOMP_EP_MANAGEMENT_DMA)
        USBCOMP_Stop_DMA(USBCOMP_MAX_EP);
    #endif /* (USBCOMP_EP_MANAGEMENT_DMA) */

    #if (CY_PSOC4)
        /* Suspend enter sequence. */
        USBCOMP_POWER_CTRL_REG |= (USBCOMP_POWER_CTRL_SUSPEND |
                                            USBCOMP_POWER_CTRL_SUSPEND_DEL);

        /* Store state of USB regulator and disable it. */
        USBCOMP_backup.mode = (uint8)  (USBCOMP_CR1_REG & USBCOMP_CR1_REG_ENABLE);
        USBCOMP_CR1_REG    &= (uint32) ~USBCOMP_CR1_REG_ENABLE;
        
        /* Store SIE interrupt sources. Valid bits are 0 - 4. */
        USBCOMP_backup.intrSeiMask = (uint8) USBCOMP_INTR_SIE_MASK_REG;

    #else
        /* Ensure USB transmit enable is low (USB_USBIO_CR0.ten). - Manual Transmission - Disabled. */
        USBCOMP_USBIO_CR0_REG &= (uint8) ~USBCOMP_USBIO_CR0_TEN;
        CyDelayUs(USBCOMP_WAIT_REG_STABILITY_50NS);  /*~50ns delay. */

        /* Disable the USBIO by asserting PM.USB_CR0.fsusbio_pd_n(Inverted) and pd_pullup_hv(Inverted) high. */
        USBCOMP_PM_USB_CR0_REG &= (uint8) ~(USBCOMP_PM_USB_CR0_PD_N |
                                                     USBCOMP_PM_USB_CR0_PD_PULLUP_N);

        /* Disable the SIE. */
        USBCOMP_CR0_REG &= (uint8) ~USBCOMP_CR0_ENABLE;

        CyDelayUs(USBCOMP_WAIT_REG_STABILITY_50NS);  /* ~50ns delay. */
        /* Store mode and disable VRegulator. */
        USBCOMP_backup.mode = (uint8) (USBCOMP_CR1_REG & USBCOMP_CR1_REG_ENABLE);
        USBCOMP_CR1_REG    &= (uint8) ~USBCOMP_CR1_REG_ENABLE;

        CyDelayUs(USBCOMP_WAIT_REG_STABILITY_1US); /* min 0.5us delay required. */

        /* Disable the USBIO reference by setting PM.USB_CR0.fsusbio_ref_en.*/
        USBCOMP_PM_USB_CR0_REG &= (uint8) ~USBCOMP_PM_USB_CR0_REF_EN;

        /* Switch DP and DM terminals to GPIO mode and disconnect 1.5k pull-up. */
        USBCOMP_USBIO_CR1_REG |= USBCOMP_USBIO_CR1_IOMODE;

        /* Disable USBFS block. */
        /* Clear power active and standby mode templates: disable USB block. */
        USBCOMP_PM_ACT_CFG_REG  &= (uint8) ~USBCOMP_PM_ACT_EN_FSUSB;
        USBCOMP_PM_STBY_CFG_REG &= (uint8) ~USBCOMP_PM_STBY_EN_FSUSB;

        CyDelayUs(USBCOMP_WAIT_REG_STABILITY_1US); /* min 0.5us delay required. */
    #endif /* (CY_PSOC4) */
    }
    else
    {
        USBCOMP_backup.enableState = 0u;
    }

    CyExitCriticalSection(enableInterrupts);

#if (USBCOMP_DP_ISR_ACTIVE)
    /* Clear active mode Dp interrupt source history. */
    (void) USBCOMP_Dp_ClearInterrupt();
    CyIntClearPending(USBCOMP_DP_INTC_VECT_NUM);

    CyIntEnable      (USBCOMP_DP_INTC_VECT_NUM);
#endif /* (USBCOMP_DP_ISR_ACTIVE). */
}


/*******************************************************************************
* Function Name: USBCOMP_Resume
****************************************************************************//**
*
*  This function prepares the USBFS component for active mode operation after 
*  exit low power mode. It restores the component active mode configuration such 
*  as device address assigned previously by the host, endpoints buffer and disables 
*  interrupt on Dp pin.
*  The supported low power modes are Deep Sleep (PSoC 4200L) and Sleep 
*  (PSoC 3/ PSoC 5LP).
*  
*  *Note* For PSoC 4200L devices, this function should not be called after 
*  exiting Sleep.
*  
*  *Note* To resume communication with the host, the data endpoints must be 
*  managed: the OUT endpoints must be enabled and IN endpoints must be loaded 
*  with data. For DMA with Automatic Buffer Management, all endpoints buffers 
*  must be initialized again before making them available to the host.
*
*
* \globalvars
*  USBCOMP_backup - checked.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_Resume(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    if (0u != USBCOMP_backup.enableState)
    {
    #if (USBCOMP_DP_ISR_ACTIVE)
        CyIntDisable(USBCOMP_DP_INTC_VECT_NUM);
    #endif /* (USBCOMP_DP_ISR_ACTIVE) */

    #if (CY_PSOC4)
        /* Enable clock to USB IP. */
        USBCOMP_USB_CLK_EN_REG |= USBCOMP_USB_CLK_CSR_CLK_EN;

        /* Restore arbiter configuration for DMA transfers. */
        #if (USBCOMP_EP_MANAGEMENT_DMA)
            #if (USBCOMP_ARB_ISR_ACTIVE)
                /* Enable ARB EP interrupt sources. */
                USBCOMP_ARB_INT_EN_REG = USBCOMP_DEFAULT_ARB_INT_EN;
            #endif /* (USBCOMP_EP_MANAGEMENT_DMA) */

            /* Configure arbiter for Manual or Auto DMA operation and clear 
            * configuration completion. 
            */
            USBCOMP_ARB_CFG_REG = USBCOMP_DEFAULT_ARB_CFG;
        #endif /* (USBCOMP_EP_MANAGEMENT_DMA) */

        /* Restore level (hi, lo, med) for each interrupt source. */
        USBCOMP_INTR_LVL_SEL_REG = USBCOMP_DEFAULT_INTR_LVL_SEL;
        
        /* Store SIE interrupt sources. */
        USBCOMP_INTR_SIE_MASK_REG = (uint32) USBCOMP_backup.intrSeiMask;

        /* Set EP0.CR: ACK Setup, NAK IN/OUT. */
        USBCOMP_EP0_CR_REG = USBCOMP_MODE_NAK_IN_OUT;

        /* Restore data EP1-8 configuration. */
        USBCOMP_RestoreConfig();

        /* Restore state of USB regulator and wait until it supples stable power. */
        USBCOMP_CR1_REG |= USBCOMP_backup.mode;
        CyDelayUs(USBCOMP_WAIT_VREF_STABILITY);

        /* Suspend exit sequence. */
        USBCOMP_POWER_CTRL_REG &= (uint32) ~USBCOMP_POWER_CTRL_SUSPEND;
        CyDelayUs(USBCOMP_WAIT_SUSPEND_DEL_DISABLE);
        USBCOMP_POWER_CTRL_REG &= (uint32) ~USBCOMP_POWER_CTRL_SUSPEND_DEL;

    #else
        /* Set power active and standby mode templates: enable USB block. */
        USBCOMP_PM_ACT_CFG_REG  |= USBCOMP_PM_ACT_EN_FSUSB;
        USBCOMP_PM_STBY_CFG_REG |= USBCOMP_PM_STBY_EN_FSUSB;

        /* Enable core clock. */
        USBCOMP_USB_CLK_EN_REG |= USBCOMP_USB_CLK_ENABLE;

        /* Enable the USBIO reference by setting PM.USB_CR0.fsusbio_ref_en.*/
        USBCOMP_PM_USB_CR0_REG |= USBCOMP_PM_USB_CR0_REF_EN;

        /* The reference is available ~40us after power restored. */
        CyDelayUs(USBCOMP_WAIT_VREF_RESTORE);
        /* Restore state of USB regulator and wait until it supples stable power. */
        USBCOMP_CR1_REG |= USBCOMP_backup.mode;
        CyDelayUs(USBCOMP_WAIT_VREF_STABILITY);   /*~50ns delay. */

        /* Enable USBIO. */
        USBCOMP_PM_USB_CR0_REG |= USBCOMP_PM_USB_CR0_PD_N;
        CyDelayUs(USBCOMP_WAIT_PD_PULLUP_N_ENABLE);
        /* Set the USBIO pull-up enable. */
        USBCOMP_PM_USB_CR0_REG |= USBCOMP_PM_USB_CR0_PD_PULLUP_N;

        /* Restore arbiter configuration for DMA transfers. */
        #if (USBCOMP_EP_MANAGEMENT_DMA)
            #if (USBCOMP_ARB_ISR_ACTIVE)
                /* Enable ARB EP interrupt sources. */
                USBCOMP_ARB_INT_EN_REG = USBCOMP_DEFAULT_ARB_INT_EN;
            #endif /* (USBCOMP_EP_MANAGEMENT_DMA) */

            /* Configure arbiter for Manual or Auto DMA operation and clear 
            * configuration completion. 
            */
            USBCOMP_ARB_CFG_REG = USBCOMP_DEFAULT_ARB_CFG;
        #endif /* (USBCOMP_EP_MANAGEMENT_DMA) */

        /* Set EP0.CR: ACK Setup, STALL IN/OUT. */
        USBCOMP_EP0_CR_REG = USBCOMP_MODE_STALL_IN_OUT;

        /* Enable the USB IP to respond to USB traffic with the last address. */
        USBCOMP_CR0_REG |= USBCOMP_CR0_ENABLE;
        CyDelayCycles(USBCOMP_WAIT_CR0_REG_STABILITY);

        /* Enable D+ pull-up and keep USB control on IO. */
        USBCOMP_USBIO_CR1_REG = USBCOMP_USBIO_CR1_USBPUEN;

        /* Restore data EP1-8 configuration. */
        USBCOMP_RestoreConfig();
    #endif /* (CY_PSOC4) */
    }

    CyExitCriticalSection(enableInterrupts);
}


/* [] END OF FILE */
