/*******************************************************************************
* File Name: Timer1.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the Timer1
*  component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_TCPWM_Timer1_H)
#define CY_TCPWM_Timer1_H


#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/*******************************************************************************
* Internal Type defines
*******************************************************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} Timer1_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  Timer1_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define Timer1_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define Timer1_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define Timer1_CONFIG                         (1lu)

/* Quad Mode */
/* Parameters */
#define Timer1_QUAD_ENCODING_MODES            (0lu)
#define Timer1_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define Timer1_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define Timer1_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define Timer1_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define Timer1_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define Timer1_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define Timer1_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define Timer1_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define Timer1_TC_RUN_MODE                    (1lu)
#define Timer1_TC_COUNTER_MODE                (0lu)
#define Timer1_TC_COMP_CAP_MODE               (2lu)
#define Timer1_TC_PRESCALER                   (0lu)

/* Signal modes */
#define Timer1_TC_RELOAD_SIGNAL_MODE          (0lu)
#define Timer1_TC_COUNT_SIGNAL_MODE           (3lu)
#define Timer1_TC_START_SIGNAL_MODE           (0lu)
#define Timer1_TC_STOP_SIGNAL_MODE            (0lu)
#define Timer1_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define Timer1_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define Timer1_TC_COUNT_SIGNAL_PRESENT        (0lu)
#define Timer1_TC_START_SIGNAL_PRESENT        (0lu)
#define Timer1_TC_STOP_SIGNAL_PRESENT         (0lu)
#define Timer1_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define Timer1_TC_INTERRUPT_MASK              (0lu)

/* PWM Mode */
/* Parameters */
#define Timer1_PWM_KILL_EVENT                 (0lu)
#define Timer1_PWM_STOP_EVENT                 (0lu)
#define Timer1_PWM_MODE                       (4lu)
#define Timer1_PWM_OUT_N_INVERT               (0lu)
#define Timer1_PWM_OUT_INVERT                 (0lu)
#define Timer1_PWM_ALIGN                      (0lu)
#define Timer1_PWM_RUN_MODE                   (0lu)
#define Timer1_PWM_DEAD_TIME_CYCLE            (0lu)
#define Timer1_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define Timer1_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define Timer1_PWM_COUNT_SIGNAL_MODE          (3lu)
#define Timer1_PWM_START_SIGNAL_MODE          (0lu)
#define Timer1_PWM_STOP_SIGNAL_MODE           (0lu)
#define Timer1_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define Timer1_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define Timer1_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define Timer1_PWM_START_SIGNAL_PRESENT       (0lu)
#define Timer1_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define Timer1_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define Timer1_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define Timer1_TC_PERIOD_VALUE                (65535lu)
#define Timer1_TC_COMPARE_VALUE               (65535lu)
#define Timer1_TC_COMPARE_BUF_VALUE           (65535lu)
#define Timer1_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define Timer1_PWM_PERIOD_VALUE               (65535lu)
#define Timer1_PWM_PERIOD_BUF_VALUE           (65535lu)
#define Timer1_PWM_PERIOD_SWAP                (0lu)
#define Timer1_PWM_COMPARE_VALUE              (65535lu)
#define Timer1_PWM_COMPARE_BUF_VALUE          (65535lu)
#define Timer1_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define Timer1__LEFT 0
#define Timer1__RIGHT 1
#define Timer1__CENTER 2
#define Timer1__ASYMMETRIC 3

#define Timer1__X1 0
#define Timer1__X2 1
#define Timer1__X4 2

#define Timer1__PWM 4
#define Timer1__PWM_DT 5
#define Timer1__PWM_PR 6

#define Timer1__INVERSE 1
#define Timer1__DIRECT 0

#define Timer1__CAPTURE 2
#define Timer1__COMPARE 0

#define Timer1__TRIG_LEVEL 3
#define Timer1__TRIG_RISING 0
#define Timer1__TRIG_FALLING 1
#define Timer1__TRIG_BOTH 2

#define Timer1__INTR_MASK_TC 1
#define Timer1__INTR_MASK_CC_MATCH 2
#define Timer1__INTR_MASK_NONE 0
#define Timer1__INTR_MASK_TC_CC 3

#define Timer1__UNCONFIG 8
#define Timer1__TIMER 1
#define Timer1__QUAD 3
#define Timer1__PWM_SEL 7

#define Timer1__COUNT_UP 0
#define Timer1__COUNT_DOWN 1
#define Timer1__COUNT_UPDOWN0 2
#define Timer1__COUNT_UPDOWN1 3


/* Prescaler */
#define Timer1_PRESCALE_DIVBY1                ((uint32)(0u << Timer1_PRESCALER_SHIFT))
#define Timer1_PRESCALE_DIVBY2                ((uint32)(1u << Timer1_PRESCALER_SHIFT))
#define Timer1_PRESCALE_DIVBY4                ((uint32)(2u << Timer1_PRESCALER_SHIFT))
#define Timer1_PRESCALE_DIVBY8                ((uint32)(3u << Timer1_PRESCALER_SHIFT))
#define Timer1_PRESCALE_DIVBY16               ((uint32)(4u << Timer1_PRESCALER_SHIFT))
#define Timer1_PRESCALE_DIVBY32               ((uint32)(5u << Timer1_PRESCALER_SHIFT))
#define Timer1_PRESCALE_DIVBY64               ((uint32)(6u << Timer1_PRESCALER_SHIFT))
#define Timer1_PRESCALE_DIVBY128              ((uint32)(7u << Timer1_PRESCALER_SHIFT))

/* TCPWM set modes */
#define Timer1_MODE_TIMER_COMPARE             ((uint32)(Timer1__COMPARE         <<  \
                                                                  Timer1_MODE_SHIFT))
#define Timer1_MODE_TIMER_CAPTURE             ((uint32)(Timer1__CAPTURE         <<  \
                                                                  Timer1_MODE_SHIFT))
#define Timer1_MODE_QUAD                      ((uint32)(Timer1__QUAD            <<  \
                                                                  Timer1_MODE_SHIFT))
#define Timer1_MODE_PWM                       ((uint32)(Timer1__PWM             <<  \
                                                                  Timer1_MODE_SHIFT))
#define Timer1_MODE_PWM_DT                    ((uint32)(Timer1__PWM_DT          <<  \
                                                                  Timer1_MODE_SHIFT))
#define Timer1_MODE_PWM_PR                    ((uint32)(Timer1__PWM_PR          <<  \
                                                                  Timer1_MODE_SHIFT))

/* Quad Modes */
#define Timer1_MODE_X1                        ((uint32)(Timer1__X1              <<  \
                                                                  Timer1_QUAD_MODE_SHIFT))
#define Timer1_MODE_X2                        ((uint32)(Timer1__X2              <<  \
                                                                  Timer1_QUAD_MODE_SHIFT))
#define Timer1_MODE_X4                        ((uint32)(Timer1__X4              <<  \
                                                                  Timer1_QUAD_MODE_SHIFT))

/* Counter modes */
#define Timer1_COUNT_UP                       ((uint32)(Timer1__COUNT_UP        <<  \
                                                                  Timer1_UPDOWN_SHIFT))
#define Timer1_COUNT_DOWN                     ((uint32)(Timer1__COUNT_DOWN      <<  \
                                                                  Timer1_UPDOWN_SHIFT))
#define Timer1_COUNT_UPDOWN0                  ((uint32)(Timer1__COUNT_UPDOWN0   <<  \
                                                                  Timer1_UPDOWN_SHIFT))
#define Timer1_COUNT_UPDOWN1                  ((uint32)(Timer1__COUNT_UPDOWN1   <<  \
                                                                  Timer1_UPDOWN_SHIFT))

/* PWM output invert */
#define Timer1_INVERT_LINE                    ((uint32)(Timer1__INVERSE         <<  \
                                                                  Timer1_INV_OUT_SHIFT))
#define Timer1_INVERT_LINE_N                  ((uint32)(Timer1__INVERSE         <<  \
                                                                  Timer1_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define Timer1_TRIG_RISING                    ((uint32)Timer1__TRIG_RISING)
#define Timer1_TRIG_FALLING                   ((uint32)Timer1__TRIG_FALLING)
#define Timer1_TRIG_BOTH                      ((uint32)Timer1__TRIG_BOTH)
#define Timer1_TRIG_LEVEL                     ((uint32)Timer1__TRIG_LEVEL)

/* Interrupt mask */
#define Timer1_INTR_MASK_TC                   ((uint32)Timer1__INTR_MASK_TC)
#define Timer1_INTR_MASK_CC_MATCH             ((uint32)Timer1__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define Timer1_CC_MATCH_SET                   (0x00u)
#define Timer1_CC_MATCH_CLEAR                 (0x01u)
#define Timer1_CC_MATCH_INVERT                (0x02u)
#define Timer1_CC_MATCH_NO_CHANGE             (0x03u)
#define Timer1_OVERLOW_SET                    (0x00u)
#define Timer1_OVERLOW_CLEAR                  (0x04u)
#define Timer1_OVERLOW_INVERT                 (0x08u)
#define Timer1_OVERLOW_NO_CHANGE              (0x0Cu)
#define Timer1_UNDERFLOW_SET                  (0x00u)
#define Timer1_UNDERFLOW_CLEAR                (0x10u)
#define Timer1_UNDERFLOW_INVERT               (0x20u)
#define Timer1_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define Timer1_PWM_MODE_LEFT                  (Timer1_CC_MATCH_CLEAR        |   \
                                                         Timer1_OVERLOW_SET           |   \
                                                         Timer1_UNDERFLOW_NO_CHANGE)
#define Timer1_PWM_MODE_RIGHT                 (Timer1_CC_MATCH_SET          |   \
                                                         Timer1_OVERLOW_NO_CHANGE     |   \
                                                         Timer1_UNDERFLOW_CLEAR)
#define Timer1_PWM_MODE_ASYM                  (Timer1_CC_MATCH_INVERT       |   \
                                                         Timer1_OVERLOW_SET           |   \
                                                         Timer1_UNDERFLOW_CLEAR)

#if (Timer1_CY_TCPWM_V2)
    #if(Timer1_CY_TCPWM_4000)
        #define Timer1_PWM_MODE_CENTER                (Timer1_CC_MATCH_INVERT       |   \
                                                                 Timer1_OVERLOW_NO_CHANGE     |   \
                                                                 Timer1_UNDERFLOW_CLEAR)
    #else
        #define Timer1_PWM_MODE_CENTER                (Timer1_CC_MATCH_INVERT       |   \
                                                                 Timer1_OVERLOW_SET           |   \
                                                                 Timer1_UNDERFLOW_CLEAR)
    #endif /* (Timer1_CY_TCPWM_4000) */
#else
    #define Timer1_PWM_MODE_CENTER                (Timer1_CC_MATCH_INVERT       |   \
                                                             Timer1_OVERLOW_NO_CHANGE     |   \
                                                             Timer1_UNDERFLOW_CLEAR)
#endif /* (Timer1_CY_TCPWM_NEW) */

/* Command operations without condition */
#define Timer1_CMD_CAPTURE                    (0u)
#define Timer1_CMD_RELOAD                     (8u)
#define Timer1_CMD_STOP                       (16u)
#define Timer1_CMD_START                      (24u)

/* Status */
#define Timer1_STATUS_DOWN                    (1u)
#define Timer1_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   Timer1_Init(void);
void   Timer1_Enable(void);
void   Timer1_Start(void);
void   Timer1_Stop(void);

void   Timer1_SetMode(uint32 mode);
void   Timer1_SetCounterMode(uint32 counterMode);
void   Timer1_SetPWMMode(uint32 modeMask);
void   Timer1_SetQDMode(uint32 qdMode);

void   Timer1_SetPrescaler(uint32 prescaler);
void   Timer1_TriggerCommand(uint32 mask, uint32 command);
void   Timer1_SetOneShot(uint32 oneShotEnable);
uint32 Timer1_ReadStatus(void);

void   Timer1_SetPWMSyncKill(uint32 syncKillEnable);
void   Timer1_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   Timer1_SetPWMDeadTime(uint32 deadTime);
void   Timer1_SetPWMInvert(uint32 mask);

void   Timer1_SetInterruptMode(uint32 interruptMask);
uint32 Timer1_GetInterruptSourceMasked(void);
uint32 Timer1_GetInterruptSource(void);
void   Timer1_ClearInterrupt(uint32 interruptMask);
void   Timer1_SetInterrupt(uint32 interruptMask);

void   Timer1_WriteCounter(uint32 count);
uint32 Timer1_ReadCounter(void);

uint32 Timer1_ReadCapture(void);
uint32 Timer1_ReadCaptureBuf(void);

void   Timer1_WritePeriod(uint32 period);
uint32 Timer1_ReadPeriod(void);
void   Timer1_WritePeriodBuf(uint32 periodBuf);
uint32 Timer1_ReadPeriodBuf(void);

void   Timer1_WriteCompare(uint32 compare);
uint32 Timer1_ReadCompare(void);
void   Timer1_WriteCompareBuf(uint32 compareBuf);
uint32 Timer1_ReadCompareBuf(void);

void   Timer1_SetPeriodSwap(uint32 swapEnable);
void   Timer1_SetCompareSwap(uint32 swapEnable);

void   Timer1_SetCaptureMode(uint32 triggerMode);
void   Timer1_SetReloadMode(uint32 triggerMode);
void   Timer1_SetStartMode(uint32 triggerMode);
void   Timer1_SetStopMode(uint32 triggerMode);
void   Timer1_SetCountMode(uint32 triggerMode);

void   Timer1_SaveConfig(void);
void   Timer1_RestoreConfig(void);
void   Timer1_Sleep(void);
void   Timer1_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define Timer1_BLOCK_CONTROL_REG              (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define Timer1_BLOCK_CONTROL_PTR              ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define Timer1_COMMAND_REG                    (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define Timer1_COMMAND_PTR                    ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define Timer1_INTRRUPT_CAUSE_REG             (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define Timer1_INTRRUPT_CAUSE_PTR             ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define Timer1_CONTROL_REG                    (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__CTRL )
#define Timer1_CONTROL_PTR                    ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__CTRL )
#define Timer1_STATUS_REG                     (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__STATUS )
#define Timer1_STATUS_PTR                     ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__STATUS )
#define Timer1_COUNTER_REG                    (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__COUNTER )
#define Timer1_COUNTER_PTR                    ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__COUNTER )
#define Timer1_COMP_CAP_REG                   (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__CC )
#define Timer1_COMP_CAP_PTR                   ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__CC )
#define Timer1_COMP_CAP_BUF_REG               (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__CC_BUFF )
#define Timer1_COMP_CAP_BUF_PTR               ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__CC_BUFF )
#define Timer1_PERIOD_REG                     (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__PERIOD )
#define Timer1_PERIOD_PTR                     ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__PERIOD )
#define Timer1_PERIOD_BUF_REG                 (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define Timer1_PERIOD_BUF_PTR                 ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define Timer1_TRIG_CONTROL0_REG              (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define Timer1_TRIG_CONTROL0_PTR              ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define Timer1_TRIG_CONTROL1_REG              (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define Timer1_TRIG_CONTROL1_PTR              ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define Timer1_TRIG_CONTROL2_REG              (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define Timer1_TRIG_CONTROL2_PTR              ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define Timer1_INTERRUPT_REQ_REG              (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__INTR )
#define Timer1_INTERRUPT_REQ_PTR              ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__INTR )
#define Timer1_INTERRUPT_SET_REG              (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__INTR_SET )
#define Timer1_INTERRUPT_SET_PTR              ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__INTR_SET )
#define Timer1_INTERRUPT_MASK_REG             (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__INTR_MASK )
#define Timer1_INTERRUPT_MASK_PTR             ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__INTR_MASK )
#define Timer1_INTERRUPT_MASKED_REG           (*(reg32 *) Timer1_cy_m0s8_tcpwm_1__INTR_MASKED )
#define Timer1_INTERRUPT_MASKED_PTR           ( (reg32 *) Timer1_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define Timer1_MASK                           ((uint32)Timer1_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define Timer1_RELOAD_CC_SHIFT                (0u)
#define Timer1_RELOAD_PERIOD_SHIFT            (1u)
#define Timer1_PWM_SYNC_KILL_SHIFT            (2u)
#define Timer1_PWM_STOP_KILL_SHIFT            (3u)
#define Timer1_PRESCALER_SHIFT                (8u)
#define Timer1_UPDOWN_SHIFT                   (16u)
#define Timer1_ONESHOT_SHIFT                  (18u)
#define Timer1_QUAD_MODE_SHIFT                (20u)
#define Timer1_INV_OUT_SHIFT                  (20u)
#define Timer1_INV_COMPL_OUT_SHIFT            (21u)
#define Timer1_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define Timer1_RELOAD_CC_MASK                 ((uint32)(Timer1_1BIT_MASK        <<  \
                                                                            Timer1_RELOAD_CC_SHIFT))
#define Timer1_RELOAD_PERIOD_MASK             ((uint32)(Timer1_1BIT_MASK        <<  \
                                                                            Timer1_RELOAD_PERIOD_SHIFT))
#define Timer1_PWM_SYNC_KILL_MASK             ((uint32)(Timer1_1BIT_MASK        <<  \
                                                                            Timer1_PWM_SYNC_KILL_SHIFT))
#define Timer1_PWM_STOP_KILL_MASK             ((uint32)(Timer1_1BIT_MASK        <<  \
                                                                            Timer1_PWM_STOP_KILL_SHIFT))
#define Timer1_PRESCALER_MASK                 ((uint32)(Timer1_8BIT_MASK        <<  \
                                                                            Timer1_PRESCALER_SHIFT))
#define Timer1_UPDOWN_MASK                    ((uint32)(Timer1_2BIT_MASK        <<  \
                                                                            Timer1_UPDOWN_SHIFT))
#define Timer1_ONESHOT_MASK                   ((uint32)(Timer1_1BIT_MASK        <<  \
                                                                            Timer1_ONESHOT_SHIFT))
#define Timer1_QUAD_MODE_MASK                 ((uint32)(Timer1_3BIT_MASK        <<  \
                                                                            Timer1_QUAD_MODE_SHIFT))
#define Timer1_INV_OUT_MASK                   ((uint32)(Timer1_2BIT_MASK        <<  \
                                                                            Timer1_INV_OUT_SHIFT))
#define Timer1_MODE_MASK                      ((uint32)(Timer1_3BIT_MASK        <<  \
                                                                            Timer1_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define Timer1_CAPTURE_SHIFT                  (0u)
#define Timer1_COUNT_SHIFT                    (2u)
#define Timer1_RELOAD_SHIFT                   (4u)
#define Timer1_STOP_SHIFT                     (6u)
#define Timer1_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define Timer1_CAPTURE_MASK                   ((uint32)(Timer1_2BIT_MASK        <<  \
                                                                  Timer1_CAPTURE_SHIFT))
#define Timer1_COUNT_MASK                     ((uint32)(Timer1_2BIT_MASK        <<  \
                                                                  Timer1_COUNT_SHIFT))
#define Timer1_RELOAD_MASK                    ((uint32)(Timer1_2BIT_MASK        <<  \
                                                                  Timer1_RELOAD_SHIFT))
#define Timer1_STOP_MASK                      ((uint32)(Timer1_2BIT_MASK        <<  \
                                                                  Timer1_STOP_SHIFT))
#define Timer1_START_MASK                     ((uint32)(Timer1_2BIT_MASK        <<  \
                                                                  Timer1_START_SHIFT))

/* MASK */
#define Timer1_1BIT_MASK                      ((uint32)0x01u)
#define Timer1_2BIT_MASK                      ((uint32)0x03u)
#define Timer1_3BIT_MASK                      ((uint32)0x07u)
#define Timer1_6BIT_MASK                      ((uint32)0x3Fu)
#define Timer1_8BIT_MASK                      ((uint32)0xFFu)
#define Timer1_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define Timer1_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define Timer1_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(Timer1_QUAD_ENCODING_MODES     << Timer1_QUAD_MODE_SHIFT))       |\
         ((uint32)(Timer1_CONFIG                  << Timer1_MODE_SHIFT)))

#define Timer1_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(Timer1_PWM_STOP_EVENT          << Timer1_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(Timer1_PWM_OUT_INVERT          << Timer1_INV_OUT_SHIFT))         |\
         ((uint32)(Timer1_PWM_OUT_N_INVERT        << Timer1_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(Timer1_PWM_MODE                << Timer1_MODE_SHIFT)))

#define Timer1_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(Timer1_PWM_RUN_MODE         << Timer1_ONESHOT_SHIFT))
            
#define Timer1_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(Timer1_PWM_ALIGN            << Timer1_UPDOWN_SHIFT))

#define Timer1_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(Timer1_PWM_KILL_EVENT      << Timer1_PWM_SYNC_KILL_SHIFT))

#define Timer1_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(Timer1_PWM_DEAD_TIME_CYCLE  << Timer1_PRESCALER_SHIFT))

#define Timer1_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(Timer1_PWM_PRESCALER        << Timer1_PRESCALER_SHIFT))

#define Timer1_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(Timer1_TC_PRESCALER            << Timer1_PRESCALER_SHIFT))       |\
         ((uint32)(Timer1_TC_COUNTER_MODE         << Timer1_UPDOWN_SHIFT))          |\
         ((uint32)(Timer1_TC_RUN_MODE             << Timer1_ONESHOT_SHIFT))         |\
         ((uint32)(Timer1_TC_COMP_CAP_MODE        << Timer1_MODE_SHIFT)))
        
#define Timer1_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(Timer1_QUAD_PHIA_SIGNAL_MODE   << Timer1_COUNT_SHIFT))           |\
         ((uint32)(Timer1_QUAD_INDEX_SIGNAL_MODE  << Timer1_RELOAD_SHIFT))          |\
         ((uint32)(Timer1_QUAD_STOP_SIGNAL_MODE   << Timer1_STOP_SHIFT))            |\
         ((uint32)(Timer1_QUAD_PHIB_SIGNAL_MODE   << Timer1_START_SHIFT)))

#define Timer1_PWM_SIGNALS_MODES                                                              \
        (((uint32)(Timer1_PWM_SWITCH_SIGNAL_MODE  << Timer1_CAPTURE_SHIFT))         |\
         ((uint32)(Timer1_PWM_COUNT_SIGNAL_MODE   << Timer1_COUNT_SHIFT))           |\
         ((uint32)(Timer1_PWM_RELOAD_SIGNAL_MODE  << Timer1_RELOAD_SHIFT))          |\
         ((uint32)(Timer1_PWM_STOP_SIGNAL_MODE    << Timer1_STOP_SHIFT))            |\
         ((uint32)(Timer1_PWM_START_SIGNAL_MODE   << Timer1_START_SHIFT)))

#define Timer1_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(Timer1_TC_CAPTURE_SIGNAL_MODE  << Timer1_CAPTURE_SHIFT))         |\
         ((uint32)(Timer1_TC_COUNT_SIGNAL_MODE    << Timer1_COUNT_SHIFT))           |\
         ((uint32)(Timer1_TC_RELOAD_SIGNAL_MODE   << Timer1_RELOAD_SHIFT))          |\
         ((uint32)(Timer1_TC_STOP_SIGNAL_MODE     << Timer1_STOP_SHIFT))            |\
         ((uint32)(Timer1_TC_START_SIGNAL_MODE    << Timer1_START_SHIFT)))
        
#define Timer1_TIMER_UPDOWN_CNT_USED                                                          \
                ((Timer1__COUNT_UPDOWN0 == Timer1_TC_COUNTER_MODE)                  ||\
                 (Timer1__COUNT_UPDOWN1 == Timer1_TC_COUNTER_MODE))

#define Timer1_PWM_UPDOWN_CNT_USED                                                            \
                ((Timer1__CENTER == Timer1_PWM_ALIGN)                               ||\
                 (Timer1__ASYMMETRIC == Timer1_PWM_ALIGN))               
        
#define Timer1_PWM_PR_INIT_VALUE              (1u)
#define Timer1_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_Timer1_H */

/* [] END OF FILE */
