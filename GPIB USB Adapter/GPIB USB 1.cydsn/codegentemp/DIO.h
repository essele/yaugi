/*******************************************************************************
* File Name: DIO.h  
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

#if !defined(CY_PINS_DIO_H) /* Pins DIO_H */
#define CY_PINS_DIO_H

#include "cytypes.h"
#include "cyfitter.h"
#include "DIO_aliases.h"


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
} DIO_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   DIO_Read(void);
void    DIO_Write(uint8 value);
uint8   DIO_ReadDataReg(void);
#if defined(DIO__PC) || (CY_PSOC4_4200L) 
    void    DIO_SetDriveMode(uint8 mode);
#endif
void    DIO_SetInterruptMode(uint16 position, uint16 mode);
uint8   DIO_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void DIO_Sleep(void); 
void DIO_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(DIO__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define DIO_DRIVE_MODE_BITS        (3)
    #define DIO_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - DIO_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the DIO_SetDriveMode() function.
         *  @{
         */
        #define DIO_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define DIO_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define DIO_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define DIO_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define DIO_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define DIO_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define DIO_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define DIO_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define DIO_MASK               DIO__MASK
#define DIO_SHIFT              DIO__SHIFT
#define DIO_WIDTH              8u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in DIO_SetInterruptMode() function.
     *  @{
     */
        #define DIO_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define DIO_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define DIO_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define DIO_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(DIO__SIO)
    #define DIO_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(DIO__PC) && (CY_PSOC4_4200L)
    #define DIO_USBIO_ENABLE               ((uint32)0x80000000u)
    #define DIO_USBIO_DISABLE              ((uint32)(~DIO_USBIO_ENABLE))
    #define DIO_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define DIO_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define DIO_USBIO_ENTER_SLEEP          ((uint32)((1u << DIO_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << DIO_USBIO_SUSPEND_DEL_SHIFT)))
    #define DIO_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << DIO_USBIO_SUSPEND_SHIFT)))
    #define DIO_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << DIO_USBIO_SUSPEND_DEL_SHIFT)))
    #define DIO_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(DIO__PC)
    /* Port Configuration */
    #define DIO_PC                 (* (reg32 *) DIO__PC)
#endif
/* Pin State */
#define DIO_PS                     (* (reg32 *) DIO__PS)
/* Data Register */
#define DIO_DR                     (* (reg32 *) DIO__DR)
/* Input Buffer Disable Override */
#define DIO_INP_DIS                (* (reg32 *) DIO__PC2)

/* Interrupt configuration Registers */
#define DIO_INTCFG                 (* (reg32 *) DIO__INTCFG)
#define DIO_INTSTAT                (* (reg32 *) DIO__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define DIO_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(DIO__SIO)
    #define DIO_SIO_REG            (* (reg32 *) DIO__SIO)
#endif /* (DIO__SIO_CFG) */

/* USBIO registers */
#if !defined(DIO__PC) && (CY_PSOC4_4200L)
    #define DIO_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define DIO_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define DIO_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define DIO_DRIVE_MODE_SHIFT       (0x00u)
#define DIO_DRIVE_MODE_MASK        (0x07u << DIO_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins DIO_H */


/* [] END OF FILE */
