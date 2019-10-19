/*******************************************************************************
* File Name: USBUART2_Dp.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_USBUART2_Dp_H) /* Pins USBUART2_Dp_H */
#define CY_PINS_USBUART2_Dp_H

#include "cytypes.h"
#include "cyfitter.h"
#include "USBUART2_Dp_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} USBUART2_Dp_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   USBUART2_Dp_Read(void);
void    USBUART2_Dp_Write(uint8 value);
uint8   USBUART2_Dp_ReadDataReg(void);
#if defined(USBUART2_Dp__PC) || (CY_PSOC4_4200L) 
    void    USBUART2_Dp_SetDriveMode(uint8 mode);
#endif
void    USBUART2_Dp_SetInterruptMode(uint16 position, uint16 mode);
uint8   USBUART2_Dp_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void USBUART2_Dp_Sleep(void); 
void USBUART2_Dp_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(USBUART2_Dp__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define USBUART2_Dp_DRIVE_MODE_BITS        (3)
    #define USBUART2_Dp_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - USBUART2_Dp_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the USBUART2_Dp_SetDriveMode() function.
         *  @{
         */
        #define USBUART2_Dp_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define USBUART2_Dp_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define USBUART2_Dp_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define USBUART2_Dp_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define USBUART2_Dp_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define USBUART2_Dp_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define USBUART2_Dp_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define USBUART2_Dp_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define USBUART2_Dp_MASK               USBUART2_Dp__MASK
#define USBUART2_Dp_SHIFT              USBUART2_Dp__SHIFT
#define USBUART2_Dp_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in USBUART2_Dp_SetInterruptMode() function.
     *  @{
     */
        #define USBUART2_Dp_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define USBUART2_Dp_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define USBUART2_Dp_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define USBUART2_Dp_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(USBUART2_Dp__SIO)
    #define USBUART2_Dp_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(USBUART2_Dp__PC) && (CY_PSOC4_4200L)
    #define USBUART2_Dp_USBIO_ENABLE               ((uint32)0x80000000u)
    #define USBUART2_Dp_USBIO_DISABLE              ((uint32)(~USBUART2_Dp_USBIO_ENABLE))
    #define USBUART2_Dp_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define USBUART2_Dp_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define USBUART2_Dp_USBIO_ENTER_SLEEP          ((uint32)((1u << USBUART2_Dp_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << USBUART2_Dp_USBIO_SUSPEND_DEL_SHIFT)))
    #define USBUART2_Dp_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << USBUART2_Dp_USBIO_SUSPEND_SHIFT)))
    #define USBUART2_Dp_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << USBUART2_Dp_USBIO_SUSPEND_DEL_SHIFT)))
    #define USBUART2_Dp_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(USBUART2_Dp__PC)
    /* Port Configuration */
    #define USBUART2_Dp_PC                 (* (reg32 *) USBUART2_Dp__PC)
#endif
/* Pin State */
#define USBUART2_Dp_PS                     (* (reg32 *) USBUART2_Dp__PS)
/* Data Register */
#define USBUART2_Dp_DR                     (* (reg32 *) USBUART2_Dp__DR)
/* Input Buffer Disable Override */
#define USBUART2_Dp_INP_DIS                (* (reg32 *) USBUART2_Dp__PC2)

/* Interrupt configuration Registers */
#define USBUART2_Dp_INTCFG                 (* (reg32 *) USBUART2_Dp__INTCFG)
#define USBUART2_Dp_INTSTAT                (* (reg32 *) USBUART2_Dp__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define USBUART2_Dp_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(USBUART2_Dp__SIO)
    #define USBUART2_Dp_SIO_REG            (* (reg32 *) USBUART2_Dp__SIO)
#endif /* (USBUART2_Dp__SIO_CFG) */

/* USBIO registers */
#if !defined(USBUART2_Dp__PC) && (CY_PSOC4_4200L)
    #define USBUART2_Dp_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define USBUART2_Dp_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define USBUART2_Dp_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define USBUART2_Dp_DRIVE_MODE_SHIFT       (0x00u)
#define USBUART2_Dp_DRIVE_MODE_MASK        (0x07u << USBUART2_Dp_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins USBUART2_Dp_H */


/* [] END OF FILE */
