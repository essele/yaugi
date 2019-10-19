/*******************************************************************************
* File Name: SIG.h  
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

#if !defined(CY_PINS_SIG_H) /* Pins SIG_H */
#define CY_PINS_SIG_H

#include "cytypes.h"
#include "cyfitter.h"
#include "SIG_aliases.h"


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
} SIG_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   SIG_Read(void);
void    SIG_Write(uint8 value);
uint8   SIG_ReadDataReg(void);
#if defined(SIG__PC) || (CY_PSOC4_4200L) 
    void    SIG_SetDriveMode(uint8 mode);
#endif
void    SIG_SetInterruptMode(uint16 position, uint16 mode);
uint8   SIG_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void SIG_Sleep(void); 
void SIG_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(SIG__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define SIG_DRIVE_MODE_BITS        (3)
    #define SIG_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - SIG_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the SIG_SetDriveMode() function.
         *  @{
         */
        #define SIG_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define SIG_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define SIG_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define SIG_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define SIG_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define SIG_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define SIG_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define SIG_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define SIG_MASK               SIG__MASK
#define SIG_SHIFT              SIG__SHIFT
#define SIG_WIDTH              8u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in SIG_SetInterruptMode() function.
     *  @{
     */
        #define SIG_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define SIG_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define SIG_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define SIG_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(SIG__SIO)
    #define SIG_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(SIG__PC) && (CY_PSOC4_4200L)
    #define SIG_USBIO_ENABLE               ((uint32)0x80000000u)
    #define SIG_USBIO_DISABLE              ((uint32)(~SIG_USBIO_ENABLE))
    #define SIG_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define SIG_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define SIG_USBIO_ENTER_SLEEP          ((uint32)((1u << SIG_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << SIG_USBIO_SUSPEND_DEL_SHIFT)))
    #define SIG_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << SIG_USBIO_SUSPEND_SHIFT)))
    #define SIG_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << SIG_USBIO_SUSPEND_DEL_SHIFT)))
    #define SIG_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(SIG__PC)
    /* Port Configuration */
    #define SIG_PC                 (* (reg32 *) SIG__PC)
#endif
/* Pin State */
#define SIG_PS                     (* (reg32 *) SIG__PS)
/* Data Register */
#define SIG_DR                     (* (reg32 *) SIG__DR)
/* Input Buffer Disable Override */
#define SIG_INP_DIS                (* (reg32 *) SIG__PC2)

/* Interrupt configuration Registers */
#define SIG_INTCFG                 (* (reg32 *) SIG__INTCFG)
#define SIG_INTSTAT                (* (reg32 *) SIG__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define SIG_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(SIG__SIO)
    #define SIG_SIO_REG            (* (reg32 *) SIG__SIO)
#endif /* (SIG__SIO_CFG) */

/* USBIO registers */
#if !defined(SIG__PC) && (CY_PSOC4_4200L)
    #define SIG_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define SIG_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define SIG_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define SIG_DRIVE_MODE_SHIFT       (0x00u)
#define SIG_DRIVE_MODE_MASK        (0x07u << SIG_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins SIG_H */


/* [] END OF FILE */
