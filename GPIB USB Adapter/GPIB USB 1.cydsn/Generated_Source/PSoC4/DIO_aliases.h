/*******************************************************************************
* File Name: DIO.h  
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

#if !defined(CY_PINS_DIO_ALIASES_H) /* Pins DIO_ALIASES_H */
#define CY_PINS_DIO_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define DIO_0			(DIO__0__PC)
#define DIO_0_PS		(DIO__0__PS)
#define DIO_0_PC		(DIO__0__PC)
#define DIO_0_DR		(DIO__0__DR)
#define DIO_0_SHIFT	(DIO__0__SHIFT)
#define DIO_0_INTR	((uint16)((uint16)0x0003u << (DIO__0__SHIFT*2u)))

#define DIO_1			(DIO__1__PC)
#define DIO_1_PS		(DIO__1__PS)
#define DIO_1_PC		(DIO__1__PC)
#define DIO_1_DR		(DIO__1__DR)
#define DIO_1_SHIFT	(DIO__1__SHIFT)
#define DIO_1_INTR	((uint16)((uint16)0x0003u << (DIO__1__SHIFT*2u)))

#define DIO_2			(DIO__2__PC)
#define DIO_2_PS		(DIO__2__PS)
#define DIO_2_PC		(DIO__2__PC)
#define DIO_2_DR		(DIO__2__DR)
#define DIO_2_SHIFT	(DIO__2__SHIFT)
#define DIO_2_INTR	((uint16)((uint16)0x0003u << (DIO__2__SHIFT*2u)))

#define DIO_3			(DIO__3__PC)
#define DIO_3_PS		(DIO__3__PS)
#define DIO_3_PC		(DIO__3__PC)
#define DIO_3_DR		(DIO__3__DR)
#define DIO_3_SHIFT	(DIO__3__SHIFT)
#define DIO_3_INTR	((uint16)((uint16)0x0003u << (DIO__3__SHIFT*2u)))

#define DIO_4			(DIO__4__PC)
#define DIO_4_PS		(DIO__4__PS)
#define DIO_4_PC		(DIO__4__PC)
#define DIO_4_DR		(DIO__4__DR)
#define DIO_4_SHIFT	(DIO__4__SHIFT)
#define DIO_4_INTR	((uint16)((uint16)0x0003u << (DIO__4__SHIFT*2u)))

#define DIO_5			(DIO__5__PC)
#define DIO_5_PS		(DIO__5__PS)
#define DIO_5_PC		(DIO__5__PC)
#define DIO_5_DR		(DIO__5__DR)
#define DIO_5_SHIFT	(DIO__5__SHIFT)
#define DIO_5_INTR	((uint16)((uint16)0x0003u << (DIO__5__SHIFT*2u)))

#define DIO_6			(DIO__6__PC)
#define DIO_6_PS		(DIO__6__PS)
#define DIO_6_PC		(DIO__6__PC)
#define DIO_6_DR		(DIO__6__DR)
#define DIO_6_SHIFT	(DIO__6__SHIFT)
#define DIO_6_INTR	((uint16)((uint16)0x0003u << (DIO__6__SHIFT*2u)))

#define DIO_7			(DIO__7__PC)
#define DIO_7_PS		(DIO__7__PS)
#define DIO_7_PC		(DIO__7__PC)
#define DIO_7_DR		(DIO__7__DR)
#define DIO_7_SHIFT	(DIO__7__SHIFT)
#define DIO_7_INTR	((uint16)((uint16)0x0003u << (DIO__7__SHIFT*2u)))

#define DIO_INTR_ALL	 ((uint16)(DIO_0_INTR| DIO_1_INTR| DIO_2_INTR| DIO_3_INTR| DIO_4_INTR| DIO_5_INTR| DIO_6_INTR| DIO_7_INTR))


#endif /* End Pins DIO_ALIASES_H */


/* [] END OF FILE */
