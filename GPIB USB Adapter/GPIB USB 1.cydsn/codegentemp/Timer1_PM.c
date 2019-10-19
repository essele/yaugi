/*******************************************************************************
* File Name: Timer1_PM.c
* Version 2.10
*
* Description:
*  This file contains the setup, control, and status commands to support
*  the component operations in the low power mode.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Timer1.h"

static Timer1_BACKUP_STRUCT Timer1_backup;


/*******************************************************************************
* Function Name: Timer1_SaveConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to save here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Timer1_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: Timer1_Sleep
********************************************************************************
*
* Summary:
*  Stops the component operation and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Timer1_Sleep(void)
{
    if(0u != (Timer1_BLOCK_CONTROL_REG & Timer1_MASK))
    {
        Timer1_backup.enableState = 1u;
    }
    else
    {
        Timer1_backup.enableState = 0u;
    }

    Timer1_Stop();
    Timer1_SaveConfig();
}


/*******************************************************************************
* Function Name: Timer1_RestoreConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to restore here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Timer1_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: Timer1_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and restores the enable state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Timer1_Wakeup(void)
{
    Timer1_RestoreConfig();

    if(0u != Timer1_backup.enableState)
    {
        Timer1_Enable();
    }
}


/* [] END OF FILE */
