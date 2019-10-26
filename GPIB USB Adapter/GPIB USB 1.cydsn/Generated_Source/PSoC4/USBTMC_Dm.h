/*******************************************************************************
* File Name: USBTMC_Dm.h  
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

#if !defined(CY_PINS_USBTMC_Dm_H) /* Pins USBTMC_Dm_H */
#define CY_PINS_USBTMC_Dm_H

#include "cytypes.h"
#include "cyfitter.h"
#include "USBTMC_Dm_aliases.h"


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
} USBTMC_Dm_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   USBTMC_Dm_Read(void);
void    USBTMC_Dm_Write(uint8 value);
uint8   USBTMC_Dm_ReadDataReg(void);
#if defined(USBTMC_Dm__PC) || (CY_PSOC4_4200L) 
    void    USBTMC_Dm_SetDriveMode(uint8 mode);
#endif
void    USBTMC_Dm_SetInterruptMode(uint16 position, uint16 mode);
uint8   USBTMC_Dm_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void USBTMC_Dm_Sleep(void); 
void USBTMC_Dm_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(USBTMC_Dm__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define USBTMC_Dm_DRIVE_MODE_BITS        (3)
    #define USBTMC_Dm_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - USBTMC_Dm_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the USBTMC_Dm_SetDriveMode() function.
         *  @{
         */
        #define USBTMC_Dm_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define USBTMC_Dm_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define USBTMC_Dm_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define USBTMC_Dm_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define USBTMC_Dm_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define USBTMC_Dm_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define USBTMC_Dm_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define USBTMC_Dm_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define USBTMC_Dm_MASK               USBTMC_Dm__MASK
#define USBTMC_Dm_SHIFT              USBTMC_Dm__SHIFT
#define USBTMC_Dm_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in USBTMC_Dm_SetInterruptMode() function.
     *  @{
     */
        #define USBTMC_Dm_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define USBTMC_Dm_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define USBTMC_Dm_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define USBTMC_Dm_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(USBTMC_Dm__SIO)
    #define USBTMC_Dm_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(USBTMC_Dm__PC) && (CY_PSOC4_4200L)
    #define USBTMC_Dm_USBIO_ENABLE               ((uint32)0x80000000u)
    #define USBTMC_Dm_USBIO_DISABLE              ((uint32)(~USBTMC_Dm_USBIO_ENABLE))
    #define USBTMC_Dm_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define USBTMC_Dm_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define USBTMC_Dm_USBIO_ENTER_SLEEP          ((uint32)((1u << USBTMC_Dm_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << USBTMC_Dm_USBIO_SUSPEND_DEL_SHIFT)))
    #define USBTMC_Dm_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << USBTMC_Dm_USBIO_SUSPEND_SHIFT)))
    #define USBTMC_Dm_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << USBTMC_Dm_USBIO_SUSPEND_DEL_SHIFT)))
    #define USBTMC_Dm_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(USBTMC_Dm__PC)
    /* Port Configuration */
    #define USBTMC_Dm_PC                 (* (reg32 *) USBTMC_Dm__PC)
#endif
/* Pin State */
#define USBTMC_Dm_PS                     (* (reg32 *) USBTMC_Dm__PS)
/* Data Register */
#define USBTMC_Dm_DR                     (* (reg32 *) USBTMC_Dm__DR)
/* Input Buffer Disable Override */
#define USBTMC_Dm_INP_DIS                (* (reg32 *) USBTMC_Dm__PC2)

/* Interrupt configuration Registers */
#define USBTMC_Dm_INTCFG                 (* (reg32 *) USBTMC_Dm__INTCFG)
#define USBTMC_Dm_INTSTAT                (* (reg32 *) USBTMC_Dm__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define USBTMC_Dm_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(USBTMC_Dm__SIO)
    #define USBTMC_Dm_SIO_REG            (* (reg32 *) USBTMC_Dm__SIO)
#endif /* (USBTMC_Dm__SIO_CFG) */

/* USBIO registers */
#if !defined(USBTMC_Dm__PC) && (CY_PSOC4_4200L)
    #define USBTMC_Dm_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define USBTMC_Dm_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define USBTMC_Dm_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define USBTMC_Dm_DRIVE_MODE_SHIFT       (0x00u)
#define USBTMC_Dm_DRIVE_MODE_MASK        (0x07u << USBTMC_Dm_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins USBTMC_Dm_H */


/* [] END OF FILE */
