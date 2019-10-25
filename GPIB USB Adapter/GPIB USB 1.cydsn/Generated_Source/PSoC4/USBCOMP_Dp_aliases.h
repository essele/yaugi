/*******************************************************************************
* File Name: USBCOMP_Dp.h  
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

#if !defined(CY_PINS_USBCOMP_Dp_ALIASES_H) /* Pins USBCOMP_Dp_ALIASES_H */
#define CY_PINS_USBCOMP_Dp_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define USBCOMP_Dp_0			(USBCOMP_Dp__0__PC)
#define USBCOMP_Dp_0_PS		(USBCOMP_Dp__0__PS)
#define USBCOMP_Dp_0_PC		(USBCOMP_Dp__0__PC)
#define USBCOMP_Dp_0_DR		(USBCOMP_Dp__0__DR)
#define USBCOMP_Dp_0_SHIFT	(USBCOMP_Dp__0__SHIFT)
#define USBCOMP_Dp_0_INTR	((uint16)((uint16)0x0003u << (USBCOMP_Dp__0__SHIFT*2u)))

#define USBCOMP_Dp_INTR_ALL	 ((uint16)(USBCOMP_Dp_0_INTR))


#endif /* End Pins USBCOMP_Dp_ALIASES_H */


/* [] END OF FILE */
