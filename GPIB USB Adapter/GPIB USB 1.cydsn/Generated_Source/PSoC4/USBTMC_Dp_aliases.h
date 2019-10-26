/*******************************************************************************
* File Name: USBTMC_Dp.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_USBTMC_Dp_ALIASES_H) /* Pins USBTMC_Dp_ALIASES_H */
#define CY_PINS_USBTMC_Dp_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define USBTMC_Dp_0			(USBTMC_Dp__0__PC)
#define USBTMC_Dp_0_PS		(USBTMC_Dp__0__PS)
#define USBTMC_Dp_0_PC		(USBTMC_Dp__0__PC)
#define USBTMC_Dp_0_DR		(USBTMC_Dp__0__DR)
#define USBTMC_Dp_0_SHIFT	(USBTMC_Dp__0__SHIFT)
#define USBTMC_Dp_0_INTR	((uint16)((uint16)0x0003u << (USBTMC_Dp__0__SHIFT*2u)))

#define USBTMC_Dp_INTR_ALL	 ((uint16)(USBTMC_Dp_0_INTR))


#endif /* End Pins USBTMC_Dp_ALIASES_H */


/* [] END OF FILE */
