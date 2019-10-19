/*******************************************************************************
* File Name: SIG.h  
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

#if !defined(CY_PINS_SIG_ALIASES_H) /* Pins SIG_ALIASES_H */
#define CY_PINS_SIG_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define SIG_0			(SIG__0__PC)
#define SIG_0_PS		(SIG__0__PS)
#define SIG_0_PC		(SIG__0__PC)
#define SIG_0_DR		(SIG__0__DR)
#define SIG_0_SHIFT	(SIG__0__SHIFT)
#define SIG_0_INTR	((uint16)((uint16)0x0003u << (SIG__0__SHIFT*2u)))

#define SIG_1			(SIG__1__PC)
#define SIG_1_PS		(SIG__1__PS)
#define SIG_1_PC		(SIG__1__PC)
#define SIG_1_DR		(SIG__1__DR)
#define SIG_1_SHIFT	(SIG__1__SHIFT)
#define SIG_1_INTR	((uint16)((uint16)0x0003u << (SIG__1__SHIFT*2u)))

#define SIG_2			(SIG__2__PC)
#define SIG_2_PS		(SIG__2__PS)
#define SIG_2_PC		(SIG__2__PC)
#define SIG_2_DR		(SIG__2__DR)
#define SIG_2_SHIFT	(SIG__2__SHIFT)
#define SIG_2_INTR	((uint16)((uint16)0x0003u << (SIG__2__SHIFT*2u)))

#define SIG_3			(SIG__3__PC)
#define SIG_3_PS		(SIG__3__PS)
#define SIG_3_PC		(SIG__3__PC)
#define SIG_3_DR		(SIG__3__DR)
#define SIG_3_SHIFT	(SIG__3__SHIFT)
#define SIG_3_INTR	((uint16)((uint16)0x0003u << (SIG__3__SHIFT*2u)))

#define SIG_4			(SIG__4__PC)
#define SIG_4_PS		(SIG__4__PS)
#define SIG_4_PC		(SIG__4__PC)
#define SIG_4_DR		(SIG__4__DR)
#define SIG_4_SHIFT	(SIG__4__SHIFT)
#define SIG_4_INTR	((uint16)((uint16)0x0003u << (SIG__4__SHIFT*2u)))

#define SIG_5			(SIG__5__PC)
#define SIG_5_PS		(SIG__5__PS)
#define SIG_5_PC		(SIG__5__PC)
#define SIG_5_DR		(SIG__5__DR)
#define SIG_5_SHIFT	(SIG__5__SHIFT)
#define SIG_5_INTR	((uint16)((uint16)0x0003u << (SIG__5__SHIFT*2u)))

#define SIG_6			(SIG__6__PC)
#define SIG_6_PS		(SIG__6__PS)
#define SIG_6_PC		(SIG__6__PC)
#define SIG_6_DR		(SIG__6__DR)
#define SIG_6_SHIFT	(SIG__6__SHIFT)
#define SIG_6_INTR	((uint16)((uint16)0x0003u << (SIG__6__SHIFT*2u)))

#define SIG_7			(SIG__7__PC)
#define SIG_7_PS		(SIG__7__PS)
#define SIG_7_PC		(SIG__7__PC)
#define SIG_7_DR		(SIG__7__DR)
#define SIG_7_SHIFT	(SIG__7__SHIFT)
#define SIG_7_INTR	((uint16)((uint16)0x0003u << (SIG__7__SHIFT*2u)))

#define SIG_INTR_ALL	 ((uint16)(SIG_0_INTR| SIG_1_INTR| SIG_2_INTR| SIG_3_INTR| SIG_4_INTR| SIG_5_INTR| SIG_6_INTR| SIG_7_INTR))


#endif /* End Pins SIG_ALIASES_H */


/* [] END OF FILE */
