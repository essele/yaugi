/*******************************************************************************
* File Name: USBTMC_Dm.h  
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

#if !defined(CY_PINS_USBTMC_Dm_ALIASES_H) /* Pins USBTMC_Dm_ALIASES_H */
#define CY_PINS_USBTMC_Dm_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define USBTMC_Dm_0			(USBTMC_Dm__0__PC)
#define USBTMC_Dm_0_PS		(USBTMC_Dm__0__PS)
#define USBTMC_Dm_0_PC		(USBTMC_Dm__0__PC)
#define USBTMC_Dm_0_DR		(USBTMC_Dm__0__DR)
#define USBTMC_Dm_0_SHIFT	(USBTMC_Dm__0__SHIFT)
#define USBTMC_Dm_0_INTR	((uint16)((uint16)0x0003u << (USBTMC_Dm__0__SHIFT*2u)))

#define USBTMC_Dm_INTR_ALL	 ((uint16)(USBTMC_Dm_0_INTR))


#endif /* End Pins USBTMC_Dm_ALIASES_H */


/* [] END OF FILE */
