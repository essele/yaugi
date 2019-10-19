/*******************************************************************************
* File Name: LED2.h  
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

#if !defined(CY_PINS_LED2_H) /* Pins LED2_H */
#define CY_PINS_LED2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "LED2_aliases.h"


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
} LED2_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   LED2_Read(void);
void    LED2_Write(uint8 value);
uint8   LED2_ReadDataReg(void);
#if defined(LED2__PC) || (CY_PSOC4_4200L) 
    void    LED2_SetDriveMode(uint8 mode);
#endif
void    LED2_SetInterruptMode(uint16 position, uint16 mode);
uint8   LED2_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void LED2_Sleep(void); 
void LED2_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(LED2__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define LED2_DRIVE_MODE_BITS        (3)
    #define LED2_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - LED2_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the LED2_SetDriveMode() function.
         *  @{
         */
        #define LED2_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define LED2_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define LED2_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define LED2_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define LED2_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define LED2_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define LED2_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define LED2_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define LED2_MASK               LED2__MASK
#define LED2_SHIFT              LED2__SHIFT
#define LED2_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in LED2_SetInterruptMode() function.
     *  @{
     */
        #define LED2_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define LED2_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define LED2_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define LED2_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(LED2__SIO)
    #define LED2_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(LED2__PC) && (CY_PSOC4_4200L)
    #define LED2_USBIO_ENABLE               ((uint32)0x80000000u)
    #define LED2_USBIO_DISABLE              ((uint32)(~LED2_USBIO_ENABLE))
    #define LED2_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define LED2_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define LED2_USBIO_ENTER_SLEEP          ((uint32)((1u << LED2_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << LED2_USBIO_SUSPEND_DEL_SHIFT)))
    #define LED2_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << LED2_USBIO_SUSPEND_SHIFT)))
    #define LED2_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << LED2_USBIO_SUSPEND_DEL_SHIFT)))
    #define LED2_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(LED2__PC)
    /* Port Configuration */
    #define LED2_PC                 (* (reg32 *) LED2__PC)
#endif
/* Pin State */
#define LED2_PS                     (* (reg32 *) LED2__PS)
/* Data Register */
#define LED2_DR                     (* (reg32 *) LED2__DR)
/* Input Buffer Disable Override */
#define LED2_INP_DIS                (* (reg32 *) LED2__PC2)

/* Interrupt configuration Registers */
#define LED2_INTCFG                 (* (reg32 *) LED2__INTCFG)
#define LED2_INTSTAT                (* (reg32 *) LED2__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define LED2_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(LED2__SIO)
    #define LED2_SIO_REG            (* (reg32 *) LED2__SIO)
#endif /* (LED2__SIO_CFG) */

/* USBIO registers */
#if !defined(LED2__PC) && (CY_PSOC4_4200L)
    #define LED2_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define LED2_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define LED2_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define LED2_DRIVE_MODE_SHIFT       (0x00u)
#define LED2_DRIVE_MODE_MASK        (0x07u << LED2_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins LED2_H */


/* [] END OF FILE */
