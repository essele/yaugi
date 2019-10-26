/***************************************************************************//**
* \file USBTMC_episr.c
* \version 3.20
*
* \brief
*  This file contains the Data endpoint Interrupt Service Routines.
*
********************************************************************************
* \copyright
* Copyright 2008-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USBTMC_pvt.h"
#include "USBTMC_cydmac.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


#if (USBTMC_EP1_ISR_ACTIVE)
    /******************************************************************************
    * Function Name: USBTMC_EP_1_ISR
    ***************************************************************************//**
    *
    *  Endpoint 1 Interrupt Service Routine
    *
    ******************************************************************************/
    CY_ISR(USBTMC_EP_1_ISR)
    {

    #ifdef USBTMC_EP_1_ISR_ENTRY_CALLBACK
        USBTMC_EP_1_ISR_EntryCallback();
    #endif /* (USBTMC_EP_1_ISR_ENTRY_CALLBACK) */

        /* `#START EP1_USER_CODE` Place your code here */

        /* `#END` */

    #if (CY_PSOC3 && defined(USBTMC_ENABLE_MIDI_STREAMING) && \
        !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
        {
            uint8 intEn = EA;
            CyGlobalIntEnable;  /* Enable nested interrupts. */
    #endif /* (CY_PSOC3 && USBTMC_ISR_SERVICE_MIDI_OUT) */
    
        USBTMC_ClearSieEpInterruptSource(USBTMC_SIE_INT_EP1_INTR);
            
        /* Notifies user that transfer IN or OUT transfer is completed.
        * IN endpoint: endpoint buffer can be reloaded, Host is read data.
        * OUT endpoint: data is ready to be read from endpoint buffer. 
        */
    #if (CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)
        if (0u != (USBTMC_EP[USBTMC_EP1].addr & USBTMC_DIR_IN))
    #endif /* (CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO) */
        {
            /* Read CR0 register to clear SIE lock. */
            (void) USBTMC_SIE_EP_BASE.sieEp[USBTMC_EP1].epCr0;
            
            /* Toggle all endpoint types except ISOC. */
            if (USBTMC_GET_EP_TYPE(USBTMC_EP1) != USBTMC_EP_TYPE_ISOC)
            {
                USBTMC_EP[USBTMC_EP1].epToggle ^= USBTMC_EPX_CNT_DATA_TOGGLE;
            }

            /* EP_MANAGEMENT_DMA_AUTO (Ticket ID# 214187): For OUT endpoint this event is used to notify
            * user that DMA has completed copying data from OUT endpoint which is not completely true.
            * Because last chunk of data is being copied.
            * For CY_PSOC 3/5LP: it is acceptable as DMA is really fast.
            * For CY_PSOC4: this event is set in Arbiter interrupt (source is DMA_TERMIN).
            */
            USBTMC_EP[USBTMC_EP1].apiEpState = USBTMC_EVENT_PENDING;
        }

    #if (!(CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO))
        #if (defined(USBTMC_ENABLE_MIDI_STREAMING) && \
            !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
            if (USBTMC_midi_out_ep == USBTMC_EP1)
            {
                USBTMC_MIDI_OUT_Service();
            }
        #endif /* (USBTMC_ISR_SERVICE_MIDI_OUT) */
    #endif /* (!(CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)) */
    
        /* `#START EP1_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBTMC_EP_1_ISR_EXIT_CALLBACK
        USBTMC_EP_1_ISR_ExitCallback();
    #endif /* (USBTMC_EP_1_ISR_EXIT_CALLBACK) */

    #if (CY_PSOC3 && defined(USBTMC_ENABLE_MIDI_STREAMING) && \
        !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
        
            EA = intEn; /* Restore nested interrupt configuration. */
        }
    #endif /* (CY_PSOC3 && USBTMC_ISR_SERVICE_MIDI_OUT) */
    }

#endif /* (USBTMC_EP1_ISR_ACTIVE) */


#if (USBTMC_EP2_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBTMC_EP_2_ISR
    ****************************************************************************//**
    *
    *  Endpoint 2 Interrupt Service Routine.
    *
    *******************************************************************************/
    CY_ISR(USBTMC_EP_2_ISR)
    {
    #ifdef USBTMC_EP_2_ISR_ENTRY_CALLBACK
        USBTMC_EP_2_ISR_EntryCallback();
    #endif /* (USBTMC_EP_2_ISR_ENTRY_CALLBACK) */

        /* `#START EP2_USER_CODE` Place your code here */

        /* `#END` */

    #if (CY_PSOC3 && defined(USBTMC_ENABLE_MIDI_STREAMING) && \
        !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
        {
            uint8 intEn = EA;
            CyGlobalIntEnable;  /* Enable nested interrupts. */
    #endif /* (CY_PSOC3 && USBTMC_ISR_SERVICE_MIDI_OUT) */

        USBTMC_ClearSieEpInterruptSource(USBTMC_SIE_INT_EP2_INTR);

        /* Notifies user that transfer IN or OUT transfer is completed.
        * IN endpoint: endpoint buffer can be reloaded, Host is read data.
        * OUT endpoint: data is ready to be read from endpoint buffer. 
        */
    #if (CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)
        if (0u != (USBTMC_EP[USBTMC_EP2].addr & USBTMC_DIR_IN))
    #endif /* (CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO) */
        {            
            /* Read CR0 register to clear SIE lock. */
            (void) USBTMC_SIE_EP_BASE.sieEp[USBTMC_EP2].epCr0;
            
            /* Toggle all endpoint types except ISOC. */
            if (USBTMC_GET_EP_TYPE(USBTMC_EP2) != USBTMC_EP_TYPE_ISOC)
            {
                USBTMC_EP[USBTMC_EP2].epToggle ^= USBTMC_EPX_CNT_DATA_TOGGLE;
            }

            /* EP_MANAGEMENT_DMA_AUTO (Ticket ID# 214187): For OUT endpoint this event is used to notify
            * user that DMA has completed copying data from OUT endpoint which is not completely true.
            * Because last chunk of data is being copied.
            * For CY_PSOC 3/5LP: it is acceptable as DMA is really fast.
            * For CY_PSOC4: this event is set in Arbiter interrupt (source is DMA_TERMIN).
            */
            USBTMC_EP[USBTMC_EP2].apiEpState = USBTMC_EVENT_PENDING;
        }

    #if (!(CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO))
        #if (defined(USBTMC_ENABLE_MIDI_STREAMING) && \
            !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
            if (USBTMC_midi_out_ep == USBTMC_EP2)
            {
                USBTMC_MIDI_OUT_Service();
            }
        #endif /* (USBTMC_ISR_SERVICE_MIDI_OUT) */
    #endif /* (!(CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)) */        
    
        /* `#START EP2_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBTMC_EP_2_ISR_EXIT_CALLBACK
        USBTMC_EP_2_ISR_ExitCallback();
    #endif /* (USBTMC_EP_2_ISR_EXIT_CALLBACK) */

    #if (CY_PSOC3 && defined(USBTMC_ENABLE_MIDI_STREAMING) && \
        !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
        
            EA = intEn; /* Restore nested interrupt configuration. */
        }
    #endif /* (CY_PSOC3 && USBTMC_ISR_SERVICE_MIDI_OUT) */
    }
#endif /* (USBTMC_EP2_ISR_ACTIVE) */


#if (USBTMC_EP3_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBTMC_EP_3_ISR
    ****************************************************************************//**
    *
    *  Endpoint 3 Interrupt Service Routine.
    *
    *******************************************************************************/
    CY_ISR(USBTMC_EP_3_ISR)
    {
    #ifdef USBTMC_EP_3_ISR_ENTRY_CALLBACK
        USBTMC_EP_3_ISR_EntryCallback();
    #endif /* (USBTMC_EP_3_ISR_ENTRY_CALLBACK) */

        /* `#START EP3_USER_CODE` Place your code here */

        /* `#END` */

    #if (CY_PSOC3 && defined(USBTMC_ENABLE_MIDI_STREAMING) && \
        !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
        {
            uint8 intEn = EA;
            CyGlobalIntEnable;  /* Enable nested interrupts. */
    #endif /* (CY_PSOC3 && USBTMC_ISR_SERVICE_MIDI_OUT) */

        USBTMC_ClearSieEpInterruptSource(USBTMC_SIE_INT_EP3_INTR);    

        /* Notifies user that transfer IN or OUT transfer is completed.
        * IN endpoint: endpoint buffer can be reloaded, Host is read data.
        * OUT endpoint: data is ready to be read from endpoint buffer. 
        */
    #if (CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)
        if (0u != (USBTMC_EP[USBTMC_EP3].addr & USBTMC_DIR_IN))
    #endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */
        {            
            /* Read CR0 register to clear SIE lock. */
            (void) USBTMC_SIE_EP_BASE.sieEp[USBTMC_EP3].epCr0;

            /* Toggle all endpoint types except ISOC. */
            if (USBTMC_GET_EP_TYPE(USBTMC_EP3) != USBTMC_EP_TYPE_ISOC)
            {
                USBTMC_EP[USBTMC_EP3].epToggle ^= USBTMC_EPX_CNT_DATA_TOGGLE;
            }

            /* EP_MANAGEMENT_DMA_AUTO (Ticket ID# 214187): For OUT endpoint this event is used to notify
            * user that DMA has completed copying data from OUT endpoint which is not completely true.
            * Because last chunk of data is being copied.
            * For CY_PSOC 3/5LP: it is acceptable as DMA is really fast.
            * For CY_PSOC4: this event is set in Arbiter interrupt (source is DMA_TERMIN).
            */
            USBTMC_EP[USBTMC_EP3].apiEpState = USBTMC_EVENT_PENDING;
        }

    #if (!(CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO))
        #if (defined(USBTMC_ENABLE_MIDI_STREAMING) && \
            !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
            if (USBTMC_midi_out_ep == USBTMC_EP3)
            {
                USBTMC_MIDI_OUT_Service();
            }
        #endif /* (USBTMC_ISR_SERVICE_MIDI_OUT) */
    #endif /* (!(CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)) */        

        /* `#START EP3_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBTMC_EP_3_ISR_EXIT_CALLBACK
        USBTMC_EP_3_ISR_ExitCallback();
    #endif /* (USBTMC_EP_3_ISR_EXIT_CALLBACK) */

    #if (CY_PSOC3 && defined(USBTMC_ENABLE_MIDI_STREAMING) && \
        !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
        
            EA = intEn; /* Restore nested interrupt configuration. */
        }
    #endif /* (CY_PSOC3 && USBTMC_ISR_SERVICE_MIDI_OUT) */
    }
#endif /* (USBTMC_EP3_ISR_ACTIVE) */


#if (USBTMC_EP4_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBTMC_EP_4_ISR
    ****************************************************************************//**
    *
    *  Endpoint 4 Interrupt Service Routine.
    *
    *******************************************************************************/
    CY_ISR(USBTMC_EP_4_ISR)
    {
    #ifdef USBTMC_EP_4_ISR_ENTRY_CALLBACK
        USBTMC_EP_4_ISR_EntryCallback();
    #endif /* (USBTMC_EP_4_ISR_ENTRY_CALLBACK) */

        /* `#START EP4_USER_CODE` Place your code here */

        /* `#END` */

    #if (CY_PSOC3 && defined(USBTMC_ENABLE_MIDI_STREAMING) && \
        !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
        {
            uint8 intEn = EA;
            CyGlobalIntEnable;  /* Enable nested interrupts. */
    #endif /* (CY_PSOC3 && USBTMC_ISR_SERVICE_MIDI_OUT) */

        USBTMC_ClearSieEpInterruptSource(USBTMC_SIE_INT_EP4_INTR);
        
        /* Notifies user that transfer IN or OUT transfer is completed.
        * IN endpoint: endpoint buffer can be reloaded, Host is read data.
        * OUT endpoint: data is ready to read from endpoint buffer. 
        */
    #if (CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)
        if (0u != (USBTMC_EP[USBTMC_EP4].addr & USBTMC_DIR_IN))
    #endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */
        {
            /* Read CR0 register to clear SIE lock. */
            (void) USBTMC_SIE_EP_BASE.sieEp[USBTMC_EP4].epCr0;

            /* Toggle all endpoint types except ISOC. */
            if (USBTMC_GET_EP_TYPE(USBTMC_EP4) != USBTMC_EP_TYPE_ISOC)
            {
                USBTMC_EP[USBTMC_EP4].epToggle ^= USBTMC_EPX_CNT_DATA_TOGGLE;
            }

            /* EP_MANAGEMENT_DMA_AUTO (Ticket ID# 214187): For OUT endpoint this event is used to notify
            * user that DMA has completed copying data from OUT endpoint which is not completely true.
            * Because last chunk of data is being copied.
            * For CY_PSOC 3/5LP: it is acceptable as DMA is really fast.
            * For CY_PSOC4: this event is set in Arbiter interrupt (source is DMA_TERMIN).
            */
            USBTMC_EP[USBTMC_EP4].apiEpState = USBTMC_EVENT_PENDING;
        }

    #if (!(CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO))
        #if (defined(USBTMC_ENABLE_MIDI_STREAMING) && \
            !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
            if(USBTMC_midi_out_ep == USBTMC_EP4)
            {
                USBTMC_MIDI_OUT_Service();
            }
        #endif /* (USBTMC_ISR_SERVICE_MIDI_OUT) */
    #endif /* (!(CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)) */        

        /* `#START EP4_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBTMC_EP_4_ISR_EXIT_CALLBACK
        USBTMC_EP_4_ISR_ExitCallback();
    #endif /* (USBTMC_EP_4_ISR_EXIT_CALLBACK) */

    #if (CY_PSOC3 && defined(USBTMC_ENABLE_MIDI_STREAMING) && \
        !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
        
            EA = intEn; /* Restore nested interrupt configuration. */
        }
    #endif /* (CY_PSOC3 && USBTMC_ISR_SERVICE_MIDI_OUT) */
    }
#endif /* (USBTMC_EP4_ISR_ACTIVE) */


#if (USBTMC_EP5_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBTMC_EP_5_ISR
    ****************************************************************************//**
    *
    *  Endpoint 5 Interrupt Service Routine
    *
    *
    *******************************************************************************/
    CY_ISR(USBTMC_EP_5_ISR)
    {
    #ifdef USBTMC_EP_5_ISR_ENTRY_CALLBACK
        USBTMC_EP_5_ISR_EntryCallback();
    #endif /* (USBTMC_EP_5_ISR_ENTRY_CALLBACK) */

        /* `#START EP5_USER_CODE` Place your code here */

        /* `#END` */

    #if (defined(USBTMC_ENABLE_MIDI_STREAMING) && !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && \
                 USBTMC_ISR_SERVICE_MIDI_OUT && CY_PSOC3)
        {
            uint8 intEn = EA;
            CyGlobalIntEnable;  /* Enable nested interrupts. */
    #endif /* (CY_PSOC3 && USBTMC_ISR_SERVICE_MIDI_OUT) */

        USBTMC_ClearSieEpInterruptSource(USBTMC_SIE_INT_EP5_INTR);
    
        /* Notifies user that transfer IN or OUT transfer is completed.
        * IN endpoint: endpoint buffer can be reloaded, Host is read data.
        * OUT endpoint: data is ready to read from endpoint buffer. 
        */
    #if (CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)
        if (0u != (USBTMC_EP[USBTMC_EP5].addr & USBTMC_DIR_IN))
    #endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */
        {            
            /* Read CR0 register to clear SIE lock. */
            (void) USBTMC_SIE_EP_BASE.sieEp[USBTMC_EP5].epCr0;

            /* Toggle all endpoint types except ISOC. */
            if (USBTMC_GET_EP_TYPE(USBTMC_EP5) != USBTMC_EP_TYPE_ISOC)
            {
                USBTMC_EP[USBTMC_EP5].epToggle ^= USBTMC_EPX_CNT_DATA_TOGGLE;
            }

            /* EP_MANAGEMENT_DMA_AUTO (Ticket ID# 214187): For OUT endpoint this event is used to notify
            * user that DMA has completed copying data from OUT endpoint which is not completely true.
            * Because last chunk of data is being copied.
            * For CY_PSOC 3/5LP: it is acceptable as DMA is really fast.
            * For CY_PSOC4: this event is set in Arbiter interrupt (source is DMA_TERMIN).
            */
            USBTMC_EP[USBTMC_EP5].apiEpState = USBTMC_EVENT_PENDING;
        }

    #if (!(CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO))        
        #if (defined(USBTMC_ENABLE_MIDI_STREAMING) && \
            !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
            if (USBTMC_midi_out_ep == USBTMC_EP5)
            {
                USBTMC_MIDI_OUT_Service();
            }
        #endif /* (USBTMC_ISR_SERVICE_MIDI_OUT) */
    #endif /* (!(CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)) */

        /* `#START EP5_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBTMC_EP_5_ISR_EXIT_CALLBACK
        USBTMC_EP_5_ISR_ExitCallback();
    #endif /* (USBTMC_EP_5_ISR_EXIT_CALLBACK) */

    #if (CY_PSOC3 && defined(USBTMC_ENABLE_MIDI_STREAMING) && \
        !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
        
            EA = intEn; /* Restore nested interrupt configuration. */
        }
    #endif /* (CY_PSOC3 && USBTMC_ISR_SERVICE_MIDI_OUT) */
    }
#endif /* (USBTMC_EP5_ISR_ACTIVE) */


#if (USBTMC_EP6_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBTMC_EP_6_ISR
    ****************************************************************************//**
    *
    *  Endpoint 6 Interrupt Service Routine.
    *
    *
    *******************************************************************************/
    CY_ISR(USBTMC_EP_6_ISR)
    {
    #ifdef USBTMC_EP_6_ISR_ENTRY_CALLBACK
        USBTMC_EP_6_ISR_EntryCallback();
    #endif /* (USBTMC_EP_6_ISR_ENTRY_CALLBACK) */

        /* `#START EP6_USER_CODE` Place your code here */

        /* `#END` */

    #if (CY_PSOC3 && defined(USBTMC_ENABLE_MIDI_STREAMING) && \
        !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
        {
            uint8 intEn = EA;
            CyGlobalIntEnable;  /* Enable nested interrupts. */
    #endif /* (CY_PSOC3 && USBTMC_ISR_SERVICE_MIDI_OUT) */

        USBTMC_ClearSieEpInterruptSource(USBTMC_SIE_INT_EP6_INTR);
        
        /* Notifies user that transfer IN or OUT transfer is completed.
        * IN endpoint: endpoint buffer can be reloaded, Host is read data.
        * OUT endpoint: data is ready to read from endpoint buffer. 
        */
    #if (CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)
        if (0u != (USBTMC_EP[USBTMC_EP6].addr & USBTMC_DIR_IN))
    #endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */
        {
            /* Read CR0 register to clear SIE lock. */
            (void) USBTMC_SIE_EP_BASE.sieEp[USBTMC_EP6].epCr0;

            /* Toggle all endpoint types except ISOC. */
            if (USBTMC_GET_EP_TYPE(USBTMC_EP6) != USBTMC_EP_TYPE_ISOC)
            {
                USBTMC_EP[USBTMC_EP6].epToggle ^= USBTMC_EPX_CNT_DATA_TOGGLE;
            }
            
            /* EP_MANAGEMENT_DMA_AUTO (Ticket ID# 214187): For OUT endpoint this event is used to notify
            * user that DMA has completed copying data from OUT endpoint which is not completely true.
            * Because last chunk of data is being copied.
            * For CY_PSOC 3/5LP: it is acceptable as DMA is really fast.
            * For CY_PSOC4: this event is set in Arbiter interrupt (source is DMA_TERMIN).
            */
            USBTMC_EP[USBTMC_EP6].apiEpState = USBTMC_EVENT_PENDING;
        }

    #if (!(CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO))
        #if (defined(USBTMC_ENABLE_MIDI_STREAMING) && \
            !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
            if (USBTMC_midi_out_ep == USBTMC_EP6)
            {
                USBTMC_MIDI_OUT_Service();
            }
        #endif /* (USBTMC_ISR_SERVICE_MIDI_OUT) */
    #endif /* (!(CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)) */

        /* `#START EP6_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBTMC_EP_6_ISR_EXIT_CALLBACK
        USBTMC_EP_6_ISR_ExitCallback();
    #endif /* (USBTMC_EP_6_ISR_EXIT_CALLBACK) */

    #if (CY_PSOC3 && defined(USBTMC_ENABLE_MIDI_STREAMING) && \
        !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
        
            EA = intEn; /* Restore nested interrupt configuration. */
        }
    #endif /* (CY_PSOC3 && USBTMC_ISR_SERVICE_MIDI_OUT) */
    }
#endif /* (USBTMC_EP6_ISR_ACTIVE) */


#if (USBTMC_EP7_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBTMC_EP_7_ISR
    ****************************************************************************//**
    *
    *  Endpoint 7 Interrupt Service Routine.
    *
    *
    *******************************************************************************/
    CY_ISR(USBTMC_EP_7_ISR)
    {
    #ifdef USBTMC_EP_7_ISR_ENTRY_CALLBACK
        USBTMC_EP_7_ISR_EntryCallback();
    #endif /* (USBTMC_EP_7_ISR_ENTRY_CALLBACK) */

        /* `#START EP7_USER_CODE` Place your code here */

        /* `#END` */

    #if (CY_PSOC3 && defined(USBTMC_ENABLE_MIDI_STREAMING) && \
        !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
        {
            uint8 intEn = EA;
            CyGlobalIntEnable;  /* Enable nested interrupts. */
    #endif /* (CY_PSOC3 && USBTMC_ISR_SERVICE_MIDI_OUT) */
    
        USBTMC_ClearSieEpInterruptSource(USBTMC_SIE_INT_EP7_INTR);
        
        /* Notifies user that transfer IN or OUT transfer is completed.
        * IN endpoint: endpoint buffer can be reloaded, Host is read data.
        * OUT endpoint: data is ready to read from endpoint buffer. 
        */
    #if (CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)
        if (0u != (USBTMC_EP[USBTMC_EP7].addr & USBTMC_DIR_IN))
    #endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */
        {           
            /* Read CR0 register to clear SIE lock. */
            (void) USBTMC_SIE_EP_BASE.sieEp[USBTMC_EP7].epCr0;

            /* Toggle all endpoint types except ISOC. */
            if (USBTMC_GET_EP_TYPE(USBTMC_EP7) != USBTMC_EP_TYPE_ISOC)
            {
                USBTMC_EP[USBTMC_EP7].epToggle ^= USBTMC_EPX_CNT_DATA_TOGGLE;
            }
            
            /* EP_MANAGEMENT_DMA_AUTO (Ticket ID# 214187): For OUT endpoint this event is used to notify
            * user that DMA has completed copying data from OUT endpoint which is not completely true.
            * Because last chunk of data is being copied.
            * For CY_PSOC 3/5LP: it is acceptable as DMA is really fast.
            * For CY_PSOC4: this event is set in Arbiter interrupt (source is DMA_TERMIN).
            */
            USBTMC_EP[USBTMC_EP7].apiEpState = USBTMC_EVENT_PENDING;
        }


    #if (!(CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO))
        #if (defined(USBTMC_ENABLE_MIDI_STREAMING) && \
            !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
            if(USBTMC_midi_out_ep == USBTMC_EP7)
            {
                USBTMC_MIDI_OUT_Service();
            }
        #endif /* (USBTMC_ISR_SERVICE_MIDI_OUT) */
    #endif /* (!(CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)) */

        /* `#START EP7_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBTMC_EP_7_ISR_EXIT_CALLBACK
        USBTMC_EP_7_ISR_ExitCallback();
    #endif /* (USBTMC_EP_7_ISR_EXIT_CALLBACK) */

    #if (CY_PSOC3 && defined(USBTMC_ENABLE_MIDI_STREAMING) && \
        !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
        
            EA = intEn; /* Restore nested interrupt configuration. */
        }
    #endif /* (CY_PSOC3 && USBTMC_ISR_SERVICE_MIDI_OUT) */
    }
#endif /* (USBTMC_EP7_ISR_ACTIVE) */


#if (USBTMC_EP8_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBTMC_EP_8_ISR
    ****************************************************************************//**
    *
    *  Endpoint 8 Interrupt Service Routine
    *
    *
    *******************************************************************************/
    CY_ISR(USBTMC_EP_8_ISR)
    {
    #ifdef USBTMC_EP_8_ISR_ENTRY_CALLBACK
        USBTMC_EP_8_ISR_EntryCallback();
    #endif /* (USBTMC_EP_8_ISR_ENTRY_CALLBACK) */

        /* `#START EP8_USER_CODE` Place your code here */

        /* `#END` */

    #if (CY_PSOC3 && defined(USBTMC_ENABLE_MIDI_STREAMING) && \
        !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
        {
            uint8 intEn = EA;
            CyGlobalIntEnable;  /* Enable nested interrupts. */
    #endif /* (CY_PSOC3 && USBTMC_ISR_SERVICE_MIDI_OUT) */

        USBTMC_ClearSieEpInterruptSource(USBTMC_SIE_INT_EP8_INTR);
        
        /* Notifies user that transfer IN or OUT transfer is completed.
        * IN endpoint: endpoint buffer can be reloaded, Host is read data.
        * OUT endpoint: data is ready to read from endpoint buffer. 
        */
    #if (CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)
        if (0u != (USBTMC_EP[USBTMC_EP8].addr & USBTMC_DIR_IN))
    #endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */
        {
            /* Read CR0 register to clear SIE lock. */
            (void) USBTMC_SIE_EP_BASE.sieEp[USBTMC_EP8].epCr0;

            /* Toggle all endpoint types except ISOC. */
            if (USBTMC_GET_EP_TYPE(USBTMC_EP8) != USBTMC_EP_TYPE_ISOC)
            {
                USBTMC_EP[USBTMC_EP8].epToggle ^= USBTMC_EPX_CNT_DATA_TOGGLE;
            }

            /* EP_MANAGEMENT_DMA_AUTO (Ticket ID# 214187): For OUT endpoint this event is used to notify
            * user that DMA has completed copying data from OUT endpoint which is not completely true.
            * Because last chunk of data is being copied.
            * For CY_PSOC 3/5LP: it is acceptable as DMA is really fast.
            * For CY_PSOC4: this event is set in Arbiter interrupt (source is DMA_TERMIN).
            */
            USBTMC_EP[USBTMC_EP8].apiEpState = USBTMC_EVENT_PENDING;
        }

    #if (!(CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO))
        #if (defined(USBTMC_ENABLE_MIDI_STREAMING) && \
            !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
            if (USBTMC_midi_out_ep == USBTMC_EP8)
            {
                USBTMC_MIDI_OUT_Service();
            }
        #endif /* (USBTMC_ISR_SERVICE_MIDI_OUT) */
    #endif /* (!(CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)) */

        /* `#START EP8_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBTMC_EP_8_ISR_EXIT_CALLBACK
        USBTMC_EP_8_ISR_ExitCallback();
    #endif /* (USBTMC_EP_8_ISR_EXIT_CALLBACK) */

    #if (CY_PSOC3 && defined(USBTMC_ENABLE_MIDI_STREAMING) && \
        !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
        
            EA = intEn; /* Restore nested interrupt configuration. */
        }
    #endif /* (CY_PSOC3 && USBTMC_ISR_SERVICE_MIDI_OUT) */
    }
#endif /* (USBTMC_EP8_ISR_ACTIVE) */


#if (USBTMC_SOF_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBTMC_SOF_ISR
    ****************************************************************************//**
    *
    *  Start of Frame Interrupt Service Routine.
    *
    *
    *******************************************************************************/
    CY_ISR(USBTMC_SOF_ISR)
    {
    #ifdef USBTMC_SOF_ISR_ENTRY_CALLBACK
        USBTMC_SOF_ISR_EntryCallback();
    #endif /* (USBTMC_SOF_ISR_ENTRY_CALLBACK) */

        /* `#START SOF_USER_CODE` Place your code here */

        /* `#END` */

        USBTMC_ClearSieInterruptSource(USBTMC_INTR_SIE_SOF_INTR);

    #ifdef USBTMC_SOF_ISR_EXIT_CALLBACK
        USBTMC_SOF_ISR_ExitCallback();
    #endif /* (USBTMC_SOF_ISR_EXIT_CALLBACK) */
    }
#endif /* (USBTMC_SOF_ISR_ACTIVE) */


#if (USBTMC_BUS_RESET_ISR_ACTIVE)
/*******************************************************************************
* Function Name: USBTMC_BUS_RESET_ISR
****************************************************************************//**
*
*  USB Bus Reset Interrupt Service Routine.  Calls _Start with the same
*  parameters as the last USER call to _Start
*
*
*******************************************************************************/
CY_ISR(USBTMC_BUS_RESET_ISR)
{
#ifdef USBTMC_BUS_RESET_ISR_ENTRY_CALLBACK
    USBTMC_BUS_RESET_ISR_EntryCallback();
#endif /* (USBTMC_BUS_RESET_ISR_ENTRY_CALLBACK) */

    /* `#START BUS_RESET_USER_CODE` Place your code here */

    /* `#END` */

    USBTMC_ClearSieInterruptSource(USBTMC_INTR_SIE_BUS_RESET_INTR);

    USBTMC_ReInitComponent();

#ifdef USBTMC_BUS_RESET_ISR_EXIT_CALLBACK
    USBTMC_BUS_RESET_ISR_ExitCallback();
#endif /* (USBTMC_BUS_RESET_ISR_EXIT_CALLBACK) */
}
#endif /* (USBTMC_BUS_RESET_ISR_ACTIVE) */


#if (USBTMC_LPM_ACTIVE)
/***************************************************************************
* Function Name: USBTMC_INTR_LPM_ISR
************************************************************************//**
*
*   Interrupt Service Routine for LPM of the interrupt sources.
*
*
***************************************************************************/
CY_ISR(USBTMC_LPM_ISR)
{
#ifdef USBTMC_LPM_ISR_ENTRY_CALLBACK
    USBTMC_LPM_ISR_EntryCallback();
#endif /* (USBTMC_LPM_ISR_ENTRY_CALLBACK) */

    /* `#START LPM_BEGIN_USER_CODE` Place your code here */

    /* `#END` */

    USBTMC_ClearSieInterruptSource(USBTMC_INTR_SIE_LPM_INTR);

    /* `#START LPM_END_USER_CODE` Place your code here */

    /* `#END` */

#ifdef USBTMC_LPM_ISR_EXIT_CALLBACK
    USBTMC_LPM_ISR_ExitCallback();
#endif /* (USBTMC_LPM_ISR_EXIT_CALLBACK) */
}
#endif /* (USBTMC_LPM_ACTIVE) */


#if (USBTMC_EP_MANAGEMENT_DMA && USBTMC_ARB_ISR_ACTIVE)
    /***************************************************************************
    * Function Name: USBTMC_ARB_ISR
    ************************************************************************//**
    *
    *  Arbiter Interrupt Service Routine.
    *
    *
    ***************************************************************************/
    CY_ISR(USBTMC_ARB_ISR)
    {
        uint32 arbIntrStatus;
        uint32 epStatus;
        uint32 ep = USBTMC_EP1;

    #ifdef USBTMC_ARB_ISR_ENTRY_CALLBACK
        USBTMC_ARB_ISR_EntryCallback();
    #endif /* (USBTMC_ARB_ISR_ENTRY_CALLBACK) */

        /* `#START ARB_BEGIN_USER_CODE` Place your code here */

        /* `#END` */

        /* Get pending ARB interrupt sources. */
        arbIntrStatus = USBTMC_ARB_INT_SR_REG;

        while (0u != arbIntrStatus)
        {
            /* Check which EP is interrupt source. */
            if (0u != (arbIntrStatus & 0x01u))
            {
                /* Get endpoint enable interrupt sources. */
                epStatus = (USBTMC_ARB_EP_BASE.arbEp[ep].epSr & USBTMC_ARB_EP_BASE.arbEp[ep].epIntEn);

                /* Handle IN endpoint buffer full event: happens only once when endpoint buffer is loaded. */
                if (0u != (epStatus & USBTMC_ARB_EPX_INT_IN_BUF_FULL))
                {
                    if (0u != (USBTMC_EP[ep].addr & USBTMC_DIR_IN))
                    {
                        /* Clear data ready status. */
                        USBTMC_ARB_EP_BASE.arbEp[ep].epCfg &= (uint32) ~USBTMC_ARB_EPX_CFG_IN_DATA_RDY;

                    #if (CY_PSOC3 || CY_PSOC5LP)
                        #if (USBTMC_EP_MANAGEMENT_DMA_AUTO && (USBTMC_EP_DMA_AUTO_OPT == 0u))
                            /* Set up common area DMA with rest of data. */
                            if(USBTMC_inLength[ep] > USBTMC_DMA_BYTES_PER_BURST)
                            {
                                USBTMC_LoadNextInEP(ep, 0u);
                            }
                            else
                            {
                                USBTMC_inBufFull[ep] = 1u;
                            }
                        #endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO && (USBTMC_EP_DMA_AUTO_OPT == 0u)) */
                    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

                        /* Arm IN endpoint. */
                        USBTMC_SIE_EP_BASE.sieEp[ep].epCr0 = USBTMC_EP[ep].epMode;

                    #if (defined(USBTMC_ENABLE_MIDI_STREAMING) && USBTMC_ISR_SERVICE_MIDI_IN)
                        if (ep == USBTMC_midi_in_ep)
                        {
                            /* Clear MIDI input pointer. */
                            USBTMC_midiInPointer = 0u;
                        }
                    #endif /* (USBTMC_ENABLE_MIDI_STREAMING) */
                    }
                }

            #if (USBTMC_EP_MANAGEMENT_DMA_MANUAL)
                /* Handle DMA completion event for OUT endpoints. */
                if (0u != (epStatus & USBTMC_ARB_EPX_SR_DMA_GNT))
                {
                    if (0u == (USBTMC_EP[ep].addr & USBTMC_DIR_IN))
                    {
                        /* Notify user that data has been copied from endpoint buffer. */
                        USBTMC_EP[ep].apiEpState = USBTMC_NO_EVENT_PENDING;

                        /* DMA done coping data: OUT endpoint has to be re-armed by user. */
                    }
                }
            #endif /* (USBTMC_EP_MANAGEMENT_DMA_MANUAL) */

            #if (CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO)
                /* Handle DMA completion event for OUT endpoints. */
                if (0u != (epStatus & USBTMC_ARB_EPX_INT_DMA_TERMIN))
                {
                    uint32 channelNum = USBTMC_DmaChan[ep];

                    /* Restore burst counter for endpoint. */
                    USBTMC_DmaEpBurstCnt[ep] = USBTMC_DMA_GET_BURST_CNT(USBTMC_DmaEpBurstCntBackup[ep]);

                    /* Disable DMA channel to restore descriptor configuration. The on-going transfer is aborted. */
                    USBTMC_CyDmaChDisable(channelNum);

                    /* Generate DMA tr_out signal to notify USB IP that DMA is done. This signal is not generated
                    * when transfer was aborted (it occurs when host writes less bytes than buffer size).
                    */
                    USBTMC_CyDmaTriggerOut(USBTMC_DmaBurstEndOut[ep]);

                    /* Restore destination address for output endpoint. */
                    USBTMC_CyDmaSetDstAddress(channelNum, USBTMC_DMA_DESCR0, (void*) ((uint32) USBTMC_DmaEpBufferAddrBackup[ep]));
                    USBTMC_CyDmaSetDstAddress(channelNum, USBTMC_DMA_DESCR1, (void*) ((uint32) USBTMC_DmaEpBufferAddrBackup[ep] +
                                                                                                                   USBTMC_DMA_BYTES_PER_BURST));

                    /* Restore number of data elements to transfer which was adjusted for last burst. */
                    if (0u != (USBTMC_DmaEpLastBurstEl[ep] & USBTMC_DMA_DESCR_REVERT))
                    {
                        USBTMC_CyDmaSetNumDataElements(channelNum, (USBTMC_DmaEpLastBurstEl[ep] >> USBTMC_DMA_DESCR_SHIFT),
                                                                             USBTMC_DMA_GET_MAX_ELEM_PER_BURST(USBTMC_DmaEpLastBurstEl[ep]));
                    }

                    /* Validate descriptor 0 and 1 (also reset current state). Command to start with descriptor 0. */
                    USBTMC_CyDmaValidateDescriptor(channelNum, USBTMC_DMA_DESCR0);
                    if (USBTMC_DmaEpBurstCntBackup[ep] > 1u)
                    {
                        USBTMC_CyDmaValidateDescriptor(channelNum, USBTMC_DMA_DESCR1);
                    }
                    USBTMC_CyDmaSetDescriptor0Next(channelNum);

                    /* Enable DMA channel: configuration complete. */
                    USBTMC_CyDmaChEnable(channelNum);
                    
                    
                    /* Read CR0 register to clear SIE lock. */
                    (void) USBTMC_SIE_EP_BASE.sieEp[ep].epCr0;
                    
                    /* Toggle all endpoint types except ISOC. */
                    if (USBTMC_GET_EP_TYPE(ep) != USBTMC_EP_TYPE_ISOC)
                    {
                        USBTMC_EP[ep].epToggle ^= USBTMC_EPX_CNT_DATA_TOGGLE;
                    }
            
                    /* Notify user that data has been copied from endpoint buffer. */
                    USBTMC_EP[ep].apiEpState = USBTMC_EVENT_PENDING;
                    
                #if (defined(USBTMC_ENABLE_MIDI_STREAMING) && \
                    !defined(USBTMC_MAIN_SERVICE_MIDI_OUT) && USBTMC_ISR_SERVICE_MIDI_OUT)
                    if (USBTMC_midi_out_ep == ep)
                    {
                        USBTMC_MIDI_OUT_Service();
                    }
                #endif /* (USBTMC_ISR_SERVICE_MIDI_OUT) */
                }
            #endif /* (CY_PSOC4 && USBTMC_EP_MANAGEMENT_DMA_AUTO) */


                /* `#START ARB_USER_CODE` Place your code here for handle Buffer Underflow/Overflow */

                /* `#END` */

            #ifdef USBTMC_ARB_ISR_CALLBACK
                USBTMC_ARB_ISR_Callback(ep, epStatus);
            #endif /* (USBTMC_ARB_ISR_CALLBACK) */

                /* Clear serviced endpoint interrupt sources. */
                USBTMC_ARB_EP_BASE.arbEp[ep].epSr = epStatus;
            }

            ++ep;
            arbIntrStatus >>= 1u;
        }

        /* `#START ARB_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBTMC_ARB_ISR_EXIT_CALLBACK
        USBTMC_ARB_ISR_ExitCallback();
    #endif /* (USBTMC_ARB_ISR_EXIT_CALLBACK) */
    }

#endif /*  (USBTMC_ARB_ISR_ACTIVE && USBTMC_EP_MANAGEMENT_DMA) */


#if (USBTMC_EP_MANAGEMENT_DMA_AUTO)
#if (CY_PSOC4)

    /******************************************************************************
    * Function Name: USBTMC_EPxDmaDone
    ***************************************************************************//**
    *
    * \internal
    *  Endpoint  DMA Done Interrupt Service Routine basic function .
    *  
    *  \param dmaCh
    *  number of DMA channel
    *  
    *  \param ep
    *  number of USB end point
    *  
    *  \param dmaDone
    *  transfer completion flag
    *  
    *  \return
    *   updated transfer completion flag
    *
    ******************************************************************************/
    CY_INLINE static void USBTMC_EPxDmaDone(uint8 dmaCh, uint8 ep)
    {
        uint32 nextAddr;

        /* Manage data elements which remain to transfer. */
        if (0u != USBTMC_DmaEpBurstCnt[ep])
        {
            if(USBTMC_DmaEpBurstCnt[ep] <= 2u)
            {
                /* Adjust length of last burst. */
                USBTMC_CyDmaSetNumDataElements(dmaCh,
                                                    ((uint32) USBTMC_DmaEpLastBurstEl[ep] >> USBTMC_DMA_DESCR_SHIFT),
                                                    ((uint32) USBTMC_DmaEpLastBurstEl[ep] &  USBTMC_DMA_BURST_BYTES_MASK));
            }
            

            /* Advance source for input endpoint or destination for output endpoint. */
            if (0u != (USBTMC_EP[ep].addr & USBTMC_DIR_IN))
            {
                /* Change source for descriptor 0. */
                nextAddr = (uint32) USBTMC_CyDmaGetSrcAddress(dmaCh, USBTMC_DMA_DESCR0);
                nextAddr += (2u * USBTMC_DMA_BYTES_PER_BURST);
                USBTMC_CyDmaSetSrcAddress(dmaCh, USBTMC_DMA_DESCR0, (void *) nextAddr);

                /* Change source for descriptor 1. */
                nextAddr += USBTMC_DMA_BYTES_PER_BURST;
                USBTMC_CyDmaSetSrcAddress(dmaCh, USBTMC_DMA_DESCR1, (void *) nextAddr);
            }
            else
            {
                /* Change destination for descriptor 0. */
                nextAddr  = (uint32) USBTMC_CyDmaGetDstAddress(dmaCh, USBTMC_DMA_DESCR0);
                nextAddr += (2u * USBTMC_DMA_BYTES_PER_BURST);
                USBTMC_CyDmaSetDstAddress(dmaCh, USBTMC_DMA_DESCR0, (void *) nextAddr);

                /* Change destination for descriptor 1. */
                nextAddr += USBTMC_DMA_BYTES_PER_BURST;
                USBTMC_CyDmaSetDstAddress(dmaCh, USBTMC_DMA_DESCR1, (void *) nextAddr);
            }

            /* Enable DMA to execute transfer as it was disabled because there were no valid descriptor. */
            USBTMC_CyDmaValidateDescriptor(dmaCh, USBTMC_DMA_DESCR0);
            
            --USBTMC_DmaEpBurstCnt[ep];
            if (0u != USBTMC_DmaEpBurstCnt[ep])
            {
                USBTMC_CyDmaValidateDescriptor(dmaCh, USBTMC_DMA_DESCR1);
                --USBTMC_DmaEpBurstCnt[ep];
            }
            
            USBTMC_CyDmaChEnable (dmaCh);
            USBTMC_CyDmaTriggerIn(USBTMC_DmaReqOut[ep]);
        }
        else
        {
            /* No data to transfer. False DMA trig. Ignore.  */
        }

    }

    #if (USBTMC_DMA1_ACTIVE)
        /******************************************************************************
        * Function Name: USBTMC_EP1_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  Endpoint 1 DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        void USBTMC_EP1_DMA_DONE_ISR(void)
        {

            USBTMC_EPxDmaDone((uint8)USBTMC_EP1_DMA_CH,
                                                  USBTMC_EP1);
                
        }
    #endif /* (USBTMC_DMA1_ACTIVE) */


    #if (USBTMC_DMA2_ACTIVE)
        /******************************************************************************
        * Function Name: USBTMC_EP2_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  Endpoint 2 DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        void USBTMC_EP2_DMA_DONE_ISR(void)
        {

            USBTMC_EPxDmaDone((uint8)USBTMC_EP2_DMA_CH,
                                                  USBTMC_EP2);
        }
    #endif /* (USBTMC_DMA2_ACTIVE) */


    #if (USBTMC_DMA3_ACTIVE)
        /******************************************************************************
        * Function Name: USBTMC_EP3_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  Endpoint 3 DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        void USBTMC_EP3_DMA_DONE_ISR(void)
        {

            USBTMC_EPxDmaDone((uint8)USBTMC_EP3_DMA_CH,
                                                  USBTMC_EP3);
        }
    #endif /* (USBTMC_DMA3_ACTIVE) */


    #if (USBTMC_DMA4_ACTIVE)
        /******************************************************************************
        * Function Name: USBTMC_EP4_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  Endpoint 4 DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        void USBTMC_EP4_DMA_DONE_ISR(void)
        {

            USBTMC_EPxDmaDone((uint8)USBTMC_EP4_DMA_CH,
                                                  USBTMC_EP4);
        }
    #endif /* (USBTMC_DMA4_ACTIVE) */


    #if (USBTMC_DMA5_ACTIVE)
        /******************************************************************************
        * Function Name: USBTMC_EP5_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  Endpoint 5 DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        void USBTMC_EP5_DMA_DONE_ISR(void)
        {

            USBTMC_EPxDmaDone((uint8)USBTMC_EP5_DMA_CH,
                                                  USBTMC_EP5);
        }
    #endif /* (USBTMC_DMA5_ACTIVE) */


    #if (USBTMC_DMA6_ACTIVE)
        /******************************************************************************
        * Function Name: USBTMC_EP6_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  Endpoint 6 DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        void USBTMC_EP6_DMA_DONE_ISR(void)
        {

            USBTMC_EPxDmaDone((uint8)USBTMC_EP6_DMA_CH,
                                                  USBTMC_EP6);
        }
    #endif /* (USBTMC_DMA6_ACTIVE) */


    #if (USBTMC_DMA7_ACTIVE)
        /******************************************************************************
        * Function Name: USBTMC_EP7_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  Endpoint 7 DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        void USBTMC_EP7_DMA_DONE_ISR(void)
        {

            USBTMC_EPxDmaDone((uint8)USBTMC_EP7_DMA_CH,
                                                  USBTMC_EP7);
        }
    #endif /* (USBTMC_DMA7_ACTIVE) */


    #if (USBTMC_DMA8_ACTIVE)
        /******************************************************************************
        * Function Name: USBTMC_EP8_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  Endpoint 8 DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        void USBTMC_EP8_DMA_DONE_ISR(void)
        {

            USBTMC_EPxDmaDone((uint8)USBTMC_EP8_DMA_CH,
                                                  USBTMC_EP8);
        }
    #endif /* (USBTMC_DMA8_ACTIVE) */


#else
    #if (USBTMC_EP_DMA_AUTO_OPT == 0u)
        /******************************************************************************
        * Function Name: USBTMC_EP_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        CY_ISR(USBTMC_EP_DMA_DONE_ISR)
        {
            uint8 int8Status;
            uint8 int17Status;
            uint8 ep_status;
            uint8 ep = USBTMC_EP1;

        #ifdef USBTMC_EP_DMA_DONE_ISR_ENTRY_CALLBACK
            USBTMC_EP_DMA_DONE_ISR_EntryCallback();
        #endif /* (USBTMC_EP_DMA_DONE_ISR_ENTRY_CALLBACK) */

            /* `#START EP_DMA_DONE_BEGIN_USER_CODE` Place your code here */

            /* `#END` */

            /* Read clear on read status register with EP source of interrupt. */
            int17Status = USBTMC_EP17_DMA_Done_SR_Read() & USBTMC_EP17_SR_MASK;
            int8Status  = USBTMC_EP8_DMA_Done_SR_Read()  & USBTMC_EP8_SR_MASK;

            while (int8Status != 0u)
            {
                while (int17Status != 0u)
                {
                    if ((int17Status & 1u) != 0u)  /* If EpX interrupt present. */
                    {
                        /* Read Endpoint Status Register. */
                        ep_status = USBTMC_ARB_EP_BASE.arbEp[ep].epSr;

                        if ((0u == (ep_status & USBTMC_ARB_EPX_SR_IN_BUF_FULL)) &&
                            (0u ==USBTMC_inBufFull[ep]))
                        {
                            /* `#START EP_DMA_DONE_USER_CODE` Place your code here */

                            /* `#END` */

                        #ifdef USBTMC_EP_DMA_DONE_ISR_CALLBACK
                            USBTMC_EP_DMA_DONE_ISR_Callback();
                        #endif /* (USBTMC_EP_DMA_DONE_ISR_CALLBACK) */

                            /* Transfer again 2 last bytes into pre-fetch endpoint area. */
                            USBTMC_ARB_EP_BASE.arbEp[ep].rwWaMsb = 0u;
                            USBTMC_ARB_EP_BASE.arbEp[ep].rwWa = (USBTMC_DMA_BYTES_PER_BURST * ep) - USBTMC_DMA_BYTES_REPEAT;
                            USBTMC_LoadNextInEP(ep, 1u);

                            /* Set Data ready status to generate DMA request. */
                            USBTMC_ARB_EP_BASE.arbEp[ep].epCfg |= USBTMC_ARB_EPX_CFG_IN_DATA_RDY;
                        }
                    }

                    ep++;
                    int17Status >>= 1u;
                }

                int8Status >>= 1u;

                if (int8Status != 0u)
                {
                    /* Prepare pointer for EP8. */
                    ep = USBTMC_EP8;
                    int17Status = int8Status & 0x01u;
                }
            }

            /* `#START EP_DMA_DONE_END_USER_CODE` Place your code here */

            /* `#END` */

    #ifdef USBTMC_EP_DMA_DONE_ISR_EXIT_CALLBACK
        USBTMC_EP_DMA_DONE_ISR_ExitCallback();
    #endif /* (USBTMC_EP_DMA_DONE_ISR_EXIT_CALLBACK) */
        }
    #endif /* (USBTMC_EP_DMA_AUTO_OPT == 0u) */
#endif /* (CY_PSOC4) */
#endif /* (USBTMC_EP_MANAGEMENT_DMA_AUTO) */


#if (CY_PSOC4)
    /***************************************************************************
    * Function Name: USBTMC_IntrHandler
    ************************************************************************//**
    *
    *   Interrupt handler for Hi/Mid/Low ISRs.
    *
    *  regCause - The cause register of interrupt. One of the three variants:
    *       USBTMC_INTR_CAUSE_LO_REG - Low interrupts.
    *       USBTMC_INTR_CAUSE_MED_REG - Med interrupts.
    *       USBTMC_INTR_CAUSE_HI_REG - - High interrupts.
    *
    *
    ***************************************************************************/
    CY_INLINE static void USBTMC_IntrHandler(uint32 intrCause)
    {
        /* Array of pointers to component interrupt handlers. */
        static const cyisraddress USBTMC_isrCallbacks[] =
        {
            NULL,
            &USBTMC_BUS_RESET_ISR,
            &USBTMC_EP_0_ISR,
            NULL,
            NULL,
            &USBTMC_EP_1_ISR,
            &USBTMC_EP_2_ISR,
            &USBTMC_EP_3_ISR,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
        };

        uint32 cbIdx = 0u;

        /* Check arbiter interrupt source first. */
        if (0u != (intrCause & USBTMC_INTR_CAUSE_ARB_INTR))
        {
            USBTMC_isrCallbacks[USBTMC_ARB_EP_INTR_NUM]();
        }

        /* Check all other interrupt sources (except arbiter and resume). */
        intrCause = (intrCause  & USBTMC_INTR_CAUSE_CTRL_INTR_MASK) |
                    ((intrCause & USBTMC_INTR_CAUSE_EP1_8_INTR_MASK) >>
                                  USBTMC_INTR_CAUSE_EP_INTR_SHIFT);

        /* Call interrupt handlers for active interrupt sources. */
        while (0u != intrCause)
        {
            if (0u != (intrCause & 0x1u))
            {
                 USBTMC_isrCallbacks[cbIdx]();
            }

            intrCause >>= 1u;
            ++cbIdx;
        }
    }


    /***************************************************************************
    * Function Name: USBTMC_INTR_HI_ISR
    ************************************************************************//**
    *
    *   Interrupt Service Routine for the high group of the interrupt sources.
    *
    *
    ***************************************************************************/
    CY_ISR(USBTMC_INTR_HI_ISR)
    {
        USBTMC_IntrHandler(USBTMC_INTR_CAUSE_HI_REG);
    }

    /***************************************************************************
    * Function Name: USBTMC_INTR_MED_ISR
    ************************************************************************//**
    *
    *   Interrupt Service Routine for the medium group of the interrupt sources.
    *
    *
    ***************************************************************************/
    CY_ISR(USBTMC_INTR_MED_ISR)
    {
       USBTMC_IntrHandler(USBTMC_INTR_CAUSE_MED_REG);
    }

    /***************************************************************************
    * Function Name: USBTMC_INTR_LO_ISR
    ************************************************************************//**
    *
    *   Interrupt Service Routine for the low group of the interrupt sources.
    *
    *
    ***************************************************************************/
    CY_ISR(USBTMC_INTR_LO_ISR)
    {
        USBTMC_IntrHandler(USBTMC_INTR_CAUSE_LO_REG);
    }
#endif /* (CY_PSOC4) */


/* [] END OF FILE */
