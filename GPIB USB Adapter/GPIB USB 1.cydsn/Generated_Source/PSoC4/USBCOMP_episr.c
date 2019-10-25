/***************************************************************************//**
* \file USBCOMP_episr.c
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

#include "USBCOMP_pvt.h"
#include "USBCOMP_cydmac.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


#if (USBCOMP_EP1_ISR_ACTIVE)
    /******************************************************************************
    * Function Name: USBCOMP_EP_1_ISR
    ***************************************************************************//**
    *
    *  Endpoint 1 Interrupt Service Routine
    *
    ******************************************************************************/
    CY_ISR(USBCOMP_EP_1_ISR)
    {

    #ifdef USBCOMP_EP_1_ISR_ENTRY_CALLBACK
        USBCOMP_EP_1_ISR_EntryCallback();
    #endif /* (USBCOMP_EP_1_ISR_ENTRY_CALLBACK) */

        /* `#START EP1_USER_CODE` Place your code here */

        /* `#END` */

    #if (CY_PSOC3 && defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
        !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
        {
            uint8 intEn = EA;
            CyGlobalIntEnable;  /* Enable nested interrupts. */
    #endif /* (CY_PSOC3 && USBCOMP_ISR_SERVICE_MIDI_OUT) */
    
        USBCOMP_ClearSieEpInterruptSource(USBCOMP_SIE_INT_EP1_INTR);
            
        /* Notifies user that transfer IN or OUT transfer is completed.
        * IN endpoint: endpoint buffer can be reloaded, Host is read data.
        * OUT endpoint: data is ready to be read from endpoint buffer. 
        */
    #if (CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)
        if (0u != (USBCOMP_EP[USBCOMP_EP1].addr & USBCOMP_DIR_IN))
    #endif /* (CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO) */
        {
            /* Read CR0 register to clear SIE lock. */
            (void) USBCOMP_SIE_EP_BASE.sieEp[USBCOMP_EP1].epCr0;
            
            /* Toggle all endpoint types except ISOC. */
            if (USBCOMP_GET_EP_TYPE(USBCOMP_EP1) != USBCOMP_EP_TYPE_ISOC)
            {
                USBCOMP_EP[USBCOMP_EP1].epToggle ^= USBCOMP_EPX_CNT_DATA_TOGGLE;
            }

            /* EP_MANAGEMENT_DMA_AUTO (Ticket ID# 214187): For OUT endpoint this event is used to notify
            * user that DMA has completed copying data from OUT endpoint which is not completely true.
            * Because last chunk of data is being copied.
            * For CY_PSOC 3/5LP: it is acceptable as DMA is really fast.
            * For CY_PSOC4: this event is set in Arbiter interrupt (source is DMA_TERMIN).
            */
            USBCOMP_EP[USBCOMP_EP1].apiEpState = USBCOMP_EVENT_PENDING;
        }

    #if (!(CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO))
        #if (defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
            !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
            if (USBCOMP_midi_out_ep == USBCOMP_EP1)
            {
                USBCOMP_MIDI_OUT_Service();
            }
        #endif /* (USBCOMP_ISR_SERVICE_MIDI_OUT) */
    #endif /* (!(CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)) */
    
        /* `#START EP1_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBCOMP_EP_1_ISR_EXIT_CALLBACK
        USBCOMP_EP_1_ISR_ExitCallback();
    #endif /* (USBCOMP_EP_1_ISR_EXIT_CALLBACK) */

    #if (CY_PSOC3 && defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
        !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
        
            EA = intEn; /* Restore nested interrupt configuration. */
        }
    #endif /* (CY_PSOC3 && USBCOMP_ISR_SERVICE_MIDI_OUT) */
    }

#endif /* (USBCOMP_EP1_ISR_ACTIVE) */


#if (USBCOMP_EP2_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBCOMP_EP_2_ISR
    ****************************************************************************//**
    *
    *  Endpoint 2 Interrupt Service Routine.
    *
    *******************************************************************************/
    CY_ISR(USBCOMP_EP_2_ISR)
    {
    #ifdef USBCOMP_EP_2_ISR_ENTRY_CALLBACK
        USBCOMP_EP_2_ISR_EntryCallback();
    #endif /* (USBCOMP_EP_2_ISR_ENTRY_CALLBACK) */

        /* `#START EP2_USER_CODE` Place your code here */

        /* `#END` */

    #if (CY_PSOC3 && defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
        !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
        {
            uint8 intEn = EA;
            CyGlobalIntEnable;  /* Enable nested interrupts. */
    #endif /* (CY_PSOC3 && USBCOMP_ISR_SERVICE_MIDI_OUT) */

        USBCOMP_ClearSieEpInterruptSource(USBCOMP_SIE_INT_EP2_INTR);

        /* Notifies user that transfer IN or OUT transfer is completed.
        * IN endpoint: endpoint buffer can be reloaded, Host is read data.
        * OUT endpoint: data is ready to be read from endpoint buffer. 
        */
    #if (CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)
        if (0u != (USBCOMP_EP[USBCOMP_EP2].addr & USBCOMP_DIR_IN))
    #endif /* (CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO) */
        {            
            /* Read CR0 register to clear SIE lock. */
            (void) USBCOMP_SIE_EP_BASE.sieEp[USBCOMP_EP2].epCr0;
            
            /* Toggle all endpoint types except ISOC. */
            if (USBCOMP_GET_EP_TYPE(USBCOMP_EP2) != USBCOMP_EP_TYPE_ISOC)
            {
                USBCOMP_EP[USBCOMP_EP2].epToggle ^= USBCOMP_EPX_CNT_DATA_TOGGLE;
            }

            /* EP_MANAGEMENT_DMA_AUTO (Ticket ID# 214187): For OUT endpoint this event is used to notify
            * user that DMA has completed copying data from OUT endpoint which is not completely true.
            * Because last chunk of data is being copied.
            * For CY_PSOC 3/5LP: it is acceptable as DMA is really fast.
            * For CY_PSOC4: this event is set in Arbiter interrupt (source is DMA_TERMIN).
            */
            USBCOMP_EP[USBCOMP_EP2].apiEpState = USBCOMP_EVENT_PENDING;
        }

    #if (!(CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO))
        #if (defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
            !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
            if (USBCOMP_midi_out_ep == USBCOMP_EP2)
            {
                USBCOMP_MIDI_OUT_Service();
            }
        #endif /* (USBCOMP_ISR_SERVICE_MIDI_OUT) */
    #endif /* (!(CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)) */        
    
        /* `#START EP2_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBCOMP_EP_2_ISR_EXIT_CALLBACK
        USBCOMP_EP_2_ISR_ExitCallback();
    #endif /* (USBCOMP_EP_2_ISR_EXIT_CALLBACK) */

    #if (CY_PSOC3 && defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
        !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
        
            EA = intEn; /* Restore nested interrupt configuration. */
        }
    #endif /* (CY_PSOC3 && USBCOMP_ISR_SERVICE_MIDI_OUT) */
    }
#endif /* (USBCOMP_EP2_ISR_ACTIVE) */


#if (USBCOMP_EP3_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBCOMP_EP_3_ISR
    ****************************************************************************//**
    *
    *  Endpoint 3 Interrupt Service Routine.
    *
    *******************************************************************************/
    CY_ISR(USBCOMP_EP_3_ISR)
    {
    #ifdef USBCOMP_EP_3_ISR_ENTRY_CALLBACK
        USBCOMP_EP_3_ISR_EntryCallback();
    #endif /* (USBCOMP_EP_3_ISR_ENTRY_CALLBACK) */

        /* `#START EP3_USER_CODE` Place your code here */

        /* `#END` */

    #if (CY_PSOC3 && defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
        !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
        {
            uint8 intEn = EA;
            CyGlobalIntEnable;  /* Enable nested interrupts. */
    #endif /* (CY_PSOC3 && USBCOMP_ISR_SERVICE_MIDI_OUT) */

        USBCOMP_ClearSieEpInterruptSource(USBCOMP_SIE_INT_EP3_INTR);    

        /* Notifies user that transfer IN or OUT transfer is completed.
        * IN endpoint: endpoint buffer can be reloaded, Host is read data.
        * OUT endpoint: data is ready to be read from endpoint buffer. 
        */
    #if (CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)
        if (0u != (USBCOMP_EP[USBCOMP_EP3].addr & USBCOMP_DIR_IN))
    #endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO) */
        {            
            /* Read CR0 register to clear SIE lock. */
            (void) USBCOMP_SIE_EP_BASE.sieEp[USBCOMP_EP3].epCr0;

            /* Toggle all endpoint types except ISOC. */
            if (USBCOMP_GET_EP_TYPE(USBCOMP_EP3) != USBCOMP_EP_TYPE_ISOC)
            {
                USBCOMP_EP[USBCOMP_EP3].epToggle ^= USBCOMP_EPX_CNT_DATA_TOGGLE;
            }

            /* EP_MANAGEMENT_DMA_AUTO (Ticket ID# 214187): For OUT endpoint this event is used to notify
            * user that DMA has completed copying data from OUT endpoint which is not completely true.
            * Because last chunk of data is being copied.
            * For CY_PSOC 3/5LP: it is acceptable as DMA is really fast.
            * For CY_PSOC4: this event is set in Arbiter interrupt (source is DMA_TERMIN).
            */
            USBCOMP_EP[USBCOMP_EP3].apiEpState = USBCOMP_EVENT_PENDING;
        }

    #if (!(CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO))
        #if (defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
            !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
            if (USBCOMP_midi_out_ep == USBCOMP_EP3)
            {
                USBCOMP_MIDI_OUT_Service();
            }
        #endif /* (USBCOMP_ISR_SERVICE_MIDI_OUT) */
    #endif /* (!(CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)) */        

        /* `#START EP3_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBCOMP_EP_3_ISR_EXIT_CALLBACK
        USBCOMP_EP_3_ISR_ExitCallback();
    #endif /* (USBCOMP_EP_3_ISR_EXIT_CALLBACK) */

    #if (CY_PSOC3 && defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
        !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
        
            EA = intEn; /* Restore nested interrupt configuration. */
        }
    #endif /* (CY_PSOC3 && USBCOMP_ISR_SERVICE_MIDI_OUT) */
    }
#endif /* (USBCOMP_EP3_ISR_ACTIVE) */


#if (USBCOMP_EP4_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBCOMP_EP_4_ISR
    ****************************************************************************//**
    *
    *  Endpoint 4 Interrupt Service Routine.
    *
    *******************************************************************************/
    CY_ISR(USBCOMP_EP_4_ISR)
    {
    #ifdef USBCOMP_EP_4_ISR_ENTRY_CALLBACK
        USBCOMP_EP_4_ISR_EntryCallback();
    #endif /* (USBCOMP_EP_4_ISR_ENTRY_CALLBACK) */

        /* `#START EP4_USER_CODE` Place your code here */

        /* `#END` */

    #if (CY_PSOC3 && defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
        !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
        {
            uint8 intEn = EA;
            CyGlobalIntEnable;  /* Enable nested interrupts. */
    #endif /* (CY_PSOC3 && USBCOMP_ISR_SERVICE_MIDI_OUT) */

        USBCOMP_ClearSieEpInterruptSource(USBCOMP_SIE_INT_EP4_INTR);
        
        /* Notifies user that transfer IN or OUT transfer is completed.
        * IN endpoint: endpoint buffer can be reloaded, Host is read data.
        * OUT endpoint: data is ready to read from endpoint buffer. 
        */
    #if (CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)
        if (0u != (USBCOMP_EP[USBCOMP_EP4].addr & USBCOMP_DIR_IN))
    #endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO) */
        {
            /* Read CR0 register to clear SIE lock. */
            (void) USBCOMP_SIE_EP_BASE.sieEp[USBCOMP_EP4].epCr0;

            /* Toggle all endpoint types except ISOC. */
            if (USBCOMP_GET_EP_TYPE(USBCOMP_EP4) != USBCOMP_EP_TYPE_ISOC)
            {
                USBCOMP_EP[USBCOMP_EP4].epToggle ^= USBCOMP_EPX_CNT_DATA_TOGGLE;
            }

            /* EP_MANAGEMENT_DMA_AUTO (Ticket ID# 214187): For OUT endpoint this event is used to notify
            * user that DMA has completed copying data from OUT endpoint which is not completely true.
            * Because last chunk of data is being copied.
            * For CY_PSOC 3/5LP: it is acceptable as DMA is really fast.
            * For CY_PSOC4: this event is set in Arbiter interrupt (source is DMA_TERMIN).
            */
            USBCOMP_EP[USBCOMP_EP4].apiEpState = USBCOMP_EVENT_PENDING;
        }

    #if (!(CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO))
        #if (defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
            !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
            if(USBCOMP_midi_out_ep == USBCOMP_EP4)
            {
                USBCOMP_MIDI_OUT_Service();
            }
        #endif /* (USBCOMP_ISR_SERVICE_MIDI_OUT) */
    #endif /* (!(CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)) */        

        /* `#START EP4_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBCOMP_EP_4_ISR_EXIT_CALLBACK
        USBCOMP_EP_4_ISR_ExitCallback();
    #endif /* (USBCOMP_EP_4_ISR_EXIT_CALLBACK) */

    #if (CY_PSOC3 && defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
        !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
        
            EA = intEn; /* Restore nested interrupt configuration. */
        }
    #endif /* (CY_PSOC3 && USBCOMP_ISR_SERVICE_MIDI_OUT) */
    }
#endif /* (USBCOMP_EP4_ISR_ACTIVE) */


#if (USBCOMP_EP5_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBCOMP_EP_5_ISR
    ****************************************************************************//**
    *
    *  Endpoint 5 Interrupt Service Routine
    *
    *
    *******************************************************************************/
    CY_ISR(USBCOMP_EP_5_ISR)
    {
    #ifdef USBCOMP_EP_5_ISR_ENTRY_CALLBACK
        USBCOMP_EP_5_ISR_EntryCallback();
    #endif /* (USBCOMP_EP_5_ISR_ENTRY_CALLBACK) */

        /* `#START EP5_USER_CODE` Place your code here */

        /* `#END` */

    #if (defined(USBCOMP_ENABLE_MIDI_STREAMING) && !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && \
                 USBCOMP_ISR_SERVICE_MIDI_OUT && CY_PSOC3)
        {
            uint8 intEn = EA;
            CyGlobalIntEnable;  /* Enable nested interrupts. */
    #endif /* (CY_PSOC3 && USBCOMP_ISR_SERVICE_MIDI_OUT) */

        USBCOMP_ClearSieEpInterruptSource(USBCOMP_SIE_INT_EP5_INTR);
    
        /* Notifies user that transfer IN or OUT transfer is completed.
        * IN endpoint: endpoint buffer can be reloaded, Host is read data.
        * OUT endpoint: data is ready to read from endpoint buffer. 
        */
    #if (CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)
        if (0u != (USBCOMP_EP[USBCOMP_EP5].addr & USBCOMP_DIR_IN))
    #endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO) */
        {            
            /* Read CR0 register to clear SIE lock. */
            (void) USBCOMP_SIE_EP_BASE.sieEp[USBCOMP_EP5].epCr0;

            /* Toggle all endpoint types except ISOC. */
            if (USBCOMP_GET_EP_TYPE(USBCOMP_EP5) != USBCOMP_EP_TYPE_ISOC)
            {
                USBCOMP_EP[USBCOMP_EP5].epToggle ^= USBCOMP_EPX_CNT_DATA_TOGGLE;
            }

            /* EP_MANAGEMENT_DMA_AUTO (Ticket ID# 214187): For OUT endpoint this event is used to notify
            * user that DMA has completed copying data from OUT endpoint which is not completely true.
            * Because last chunk of data is being copied.
            * For CY_PSOC 3/5LP: it is acceptable as DMA is really fast.
            * For CY_PSOC4: this event is set in Arbiter interrupt (source is DMA_TERMIN).
            */
            USBCOMP_EP[USBCOMP_EP5].apiEpState = USBCOMP_EVENT_PENDING;
        }

    #if (!(CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO))        
        #if (defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
            !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
            if (USBCOMP_midi_out_ep == USBCOMP_EP5)
            {
                USBCOMP_MIDI_OUT_Service();
            }
        #endif /* (USBCOMP_ISR_SERVICE_MIDI_OUT) */
    #endif /* (!(CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)) */

        /* `#START EP5_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBCOMP_EP_5_ISR_EXIT_CALLBACK
        USBCOMP_EP_5_ISR_ExitCallback();
    #endif /* (USBCOMP_EP_5_ISR_EXIT_CALLBACK) */

    #if (CY_PSOC3 && defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
        !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
        
            EA = intEn; /* Restore nested interrupt configuration. */
        }
    #endif /* (CY_PSOC3 && USBCOMP_ISR_SERVICE_MIDI_OUT) */
    }
#endif /* (USBCOMP_EP5_ISR_ACTIVE) */


#if (USBCOMP_EP6_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBCOMP_EP_6_ISR
    ****************************************************************************//**
    *
    *  Endpoint 6 Interrupt Service Routine.
    *
    *
    *******************************************************************************/
    CY_ISR(USBCOMP_EP_6_ISR)
    {
    #ifdef USBCOMP_EP_6_ISR_ENTRY_CALLBACK
        USBCOMP_EP_6_ISR_EntryCallback();
    #endif /* (USBCOMP_EP_6_ISR_ENTRY_CALLBACK) */

        /* `#START EP6_USER_CODE` Place your code here */

        /* `#END` */

    #if (CY_PSOC3 && defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
        !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
        {
            uint8 intEn = EA;
            CyGlobalIntEnable;  /* Enable nested interrupts. */
    #endif /* (CY_PSOC3 && USBCOMP_ISR_SERVICE_MIDI_OUT) */

        USBCOMP_ClearSieEpInterruptSource(USBCOMP_SIE_INT_EP6_INTR);
        
        /* Notifies user that transfer IN or OUT transfer is completed.
        * IN endpoint: endpoint buffer can be reloaded, Host is read data.
        * OUT endpoint: data is ready to read from endpoint buffer. 
        */
    #if (CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)
        if (0u != (USBCOMP_EP[USBCOMP_EP6].addr & USBCOMP_DIR_IN))
    #endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO) */
        {
            /* Read CR0 register to clear SIE lock. */
            (void) USBCOMP_SIE_EP_BASE.sieEp[USBCOMP_EP6].epCr0;

            /* Toggle all endpoint types except ISOC. */
            if (USBCOMP_GET_EP_TYPE(USBCOMP_EP6) != USBCOMP_EP_TYPE_ISOC)
            {
                USBCOMP_EP[USBCOMP_EP6].epToggle ^= USBCOMP_EPX_CNT_DATA_TOGGLE;
            }
            
            /* EP_MANAGEMENT_DMA_AUTO (Ticket ID# 214187): For OUT endpoint this event is used to notify
            * user that DMA has completed copying data from OUT endpoint which is not completely true.
            * Because last chunk of data is being copied.
            * For CY_PSOC 3/5LP: it is acceptable as DMA is really fast.
            * For CY_PSOC4: this event is set in Arbiter interrupt (source is DMA_TERMIN).
            */
            USBCOMP_EP[USBCOMP_EP6].apiEpState = USBCOMP_EVENT_PENDING;
        }

    #if (!(CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO))
        #if (defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
            !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
            if (USBCOMP_midi_out_ep == USBCOMP_EP6)
            {
                USBCOMP_MIDI_OUT_Service();
            }
        #endif /* (USBCOMP_ISR_SERVICE_MIDI_OUT) */
    #endif /* (!(CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)) */

        /* `#START EP6_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBCOMP_EP_6_ISR_EXIT_CALLBACK
        USBCOMP_EP_6_ISR_ExitCallback();
    #endif /* (USBCOMP_EP_6_ISR_EXIT_CALLBACK) */

    #if (CY_PSOC3 && defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
        !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
        
            EA = intEn; /* Restore nested interrupt configuration. */
        }
    #endif /* (CY_PSOC3 && USBCOMP_ISR_SERVICE_MIDI_OUT) */
    }
#endif /* (USBCOMP_EP6_ISR_ACTIVE) */


#if (USBCOMP_EP7_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBCOMP_EP_7_ISR
    ****************************************************************************//**
    *
    *  Endpoint 7 Interrupt Service Routine.
    *
    *
    *******************************************************************************/
    CY_ISR(USBCOMP_EP_7_ISR)
    {
    #ifdef USBCOMP_EP_7_ISR_ENTRY_CALLBACK
        USBCOMP_EP_7_ISR_EntryCallback();
    #endif /* (USBCOMP_EP_7_ISR_ENTRY_CALLBACK) */

        /* `#START EP7_USER_CODE` Place your code here */

        /* `#END` */

    #if (CY_PSOC3 && defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
        !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
        {
            uint8 intEn = EA;
            CyGlobalIntEnable;  /* Enable nested interrupts. */
    #endif /* (CY_PSOC3 && USBCOMP_ISR_SERVICE_MIDI_OUT) */
    
        USBCOMP_ClearSieEpInterruptSource(USBCOMP_SIE_INT_EP7_INTR);
        
        /* Notifies user that transfer IN or OUT transfer is completed.
        * IN endpoint: endpoint buffer can be reloaded, Host is read data.
        * OUT endpoint: data is ready to read from endpoint buffer. 
        */
    #if (CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)
        if (0u != (USBCOMP_EP[USBCOMP_EP7].addr & USBCOMP_DIR_IN))
    #endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO) */
        {           
            /* Read CR0 register to clear SIE lock. */
            (void) USBCOMP_SIE_EP_BASE.sieEp[USBCOMP_EP7].epCr0;

            /* Toggle all endpoint types except ISOC. */
            if (USBCOMP_GET_EP_TYPE(USBCOMP_EP7) != USBCOMP_EP_TYPE_ISOC)
            {
                USBCOMP_EP[USBCOMP_EP7].epToggle ^= USBCOMP_EPX_CNT_DATA_TOGGLE;
            }
            
            /* EP_MANAGEMENT_DMA_AUTO (Ticket ID# 214187): For OUT endpoint this event is used to notify
            * user that DMA has completed copying data from OUT endpoint which is not completely true.
            * Because last chunk of data is being copied.
            * For CY_PSOC 3/5LP: it is acceptable as DMA is really fast.
            * For CY_PSOC4: this event is set in Arbiter interrupt (source is DMA_TERMIN).
            */
            USBCOMP_EP[USBCOMP_EP7].apiEpState = USBCOMP_EVENT_PENDING;
        }


    #if (!(CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO))
        #if (defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
            !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
            if(USBCOMP_midi_out_ep == USBCOMP_EP7)
            {
                USBCOMP_MIDI_OUT_Service();
            }
        #endif /* (USBCOMP_ISR_SERVICE_MIDI_OUT) */
    #endif /* (!(CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)) */

        /* `#START EP7_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBCOMP_EP_7_ISR_EXIT_CALLBACK
        USBCOMP_EP_7_ISR_ExitCallback();
    #endif /* (USBCOMP_EP_7_ISR_EXIT_CALLBACK) */

    #if (CY_PSOC3 && defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
        !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
        
            EA = intEn; /* Restore nested interrupt configuration. */
        }
    #endif /* (CY_PSOC3 && USBCOMP_ISR_SERVICE_MIDI_OUT) */
    }
#endif /* (USBCOMP_EP7_ISR_ACTIVE) */


#if (USBCOMP_EP8_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBCOMP_EP_8_ISR
    ****************************************************************************//**
    *
    *  Endpoint 8 Interrupt Service Routine
    *
    *
    *******************************************************************************/
    CY_ISR(USBCOMP_EP_8_ISR)
    {
    #ifdef USBCOMP_EP_8_ISR_ENTRY_CALLBACK
        USBCOMP_EP_8_ISR_EntryCallback();
    #endif /* (USBCOMP_EP_8_ISR_ENTRY_CALLBACK) */

        /* `#START EP8_USER_CODE` Place your code here */

        /* `#END` */

    #if (CY_PSOC3 && defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
        !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
        {
            uint8 intEn = EA;
            CyGlobalIntEnable;  /* Enable nested interrupts. */
    #endif /* (CY_PSOC3 && USBCOMP_ISR_SERVICE_MIDI_OUT) */

        USBCOMP_ClearSieEpInterruptSource(USBCOMP_SIE_INT_EP8_INTR);
        
        /* Notifies user that transfer IN or OUT transfer is completed.
        * IN endpoint: endpoint buffer can be reloaded, Host is read data.
        * OUT endpoint: data is ready to read from endpoint buffer. 
        */
    #if (CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)
        if (0u != (USBCOMP_EP[USBCOMP_EP8].addr & USBCOMP_DIR_IN))
    #endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO) */
        {
            /* Read CR0 register to clear SIE lock. */
            (void) USBCOMP_SIE_EP_BASE.sieEp[USBCOMP_EP8].epCr0;

            /* Toggle all endpoint types except ISOC. */
            if (USBCOMP_GET_EP_TYPE(USBCOMP_EP8) != USBCOMP_EP_TYPE_ISOC)
            {
                USBCOMP_EP[USBCOMP_EP8].epToggle ^= USBCOMP_EPX_CNT_DATA_TOGGLE;
            }

            /* EP_MANAGEMENT_DMA_AUTO (Ticket ID# 214187): For OUT endpoint this event is used to notify
            * user that DMA has completed copying data from OUT endpoint which is not completely true.
            * Because last chunk of data is being copied.
            * For CY_PSOC 3/5LP: it is acceptable as DMA is really fast.
            * For CY_PSOC4: this event is set in Arbiter interrupt (source is DMA_TERMIN).
            */
            USBCOMP_EP[USBCOMP_EP8].apiEpState = USBCOMP_EVENT_PENDING;
        }

    #if (!(CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO))
        #if (defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
            !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
            if (USBCOMP_midi_out_ep == USBCOMP_EP8)
            {
                USBCOMP_MIDI_OUT_Service();
            }
        #endif /* (USBCOMP_ISR_SERVICE_MIDI_OUT) */
    #endif /* (!(CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)) */

        /* `#START EP8_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBCOMP_EP_8_ISR_EXIT_CALLBACK
        USBCOMP_EP_8_ISR_ExitCallback();
    #endif /* (USBCOMP_EP_8_ISR_EXIT_CALLBACK) */

    #if (CY_PSOC3 && defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
        !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
        
            EA = intEn; /* Restore nested interrupt configuration. */
        }
    #endif /* (CY_PSOC3 && USBCOMP_ISR_SERVICE_MIDI_OUT) */
    }
#endif /* (USBCOMP_EP8_ISR_ACTIVE) */


#if (USBCOMP_SOF_ISR_ACTIVE)
    /*******************************************************************************
    * Function Name: USBCOMP_SOF_ISR
    ****************************************************************************//**
    *
    *  Start of Frame Interrupt Service Routine.
    *
    *
    *******************************************************************************/
    CY_ISR(USBCOMP_SOF_ISR)
    {
    #ifdef USBCOMP_SOF_ISR_ENTRY_CALLBACK
        USBCOMP_SOF_ISR_EntryCallback();
    #endif /* (USBCOMP_SOF_ISR_ENTRY_CALLBACK) */

        /* `#START SOF_USER_CODE` Place your code here */

        /* `#END` */

        USBCOMP_ClearSieInterruptSource(USBCOMP_INTR_SIE_SOF_INTR);

    #ifdef USBCOMP_SOF_ISR_EXIT_CALLBACK
        USBCOMP_SOF_ISR_ExitCallback();
    #endif /* (USBCOMP_SOF_ISR_EXIT_CALLBACK) */
    }
#endif /* (USBCOMP_SOF_ISR_ACTIVE) */


#if (USBCOMP_BUS_RESET_ISR_ACTIVE)
/*******************************************************************************
* Function Name: USBCOMP_BUS_RESET_ISR
****************************************************************************//**
*
*  USB Bus Reset Interrupt Service Routine.  Calls _Start with the same
*  parameters as the last USER call to _Start
*
*
*******************************************************************************/
CY_ISR(USBCOMP_BUS_RESET_ISR)
{
#ifdef USBCOMP_BUS_RESET_ISR_ENTRY_CALLBACK
    USBCOMP_BUS_RESET_ISR_EntryCallback();
#endif /* (USBCOMP_BUS_RESET_ISR_ENTRY_CALLBACK) */

    /* `#START BUS_RESET_USER_CODE` Place your code here */

    /* `#END` */

    USBCOMP_ClearSieInterruptSource(USBCOMP_INTR_SIE_BUS_RESET_INTR);

    USBCOMP_ReInitComponent();

#ifdef USBCOMP_BUS_RESET_ISR_EXIT_CALLBACK
    USBCOMP_BUS_RESET_ISR_ExitCallback();
#endif /* (USBCOMP_BUS_RESET_ISR_EXIT_CALLBACK) */
}
#endif /* (USBCOMP_BUS_RESET_ISR_ACTIVE) */


#if (USBCOMP_LPM_ACTIVE)
/***************************************************************************
* Function Name: USBCOMP_INTR_LPM_ISR
************************************************************************//**
*
*   Interrupt Service Routine for LPM of the interrupt sources.
*
*
***************************************************************************/
CY_ISR(USBCOMP_LPM_ISR)
{
#ifdef USBCOMP_LPM_ISR_ENTRY_CALLBACK
    USBCOMP_LPM_ISR_EntryCallback();
#endif /* (USBCOMP_LPM_ISR_ENTRY_CALLBACK) */

    /* `#START LPM_BEGIN_USER_CODE` Place your code here */

    /* `#END` */

    USBCOMP_ClearSieInterruptSource(USBCOMP_INTR_SIE_LPM_INTR);

    /* `#START LPM_END_USER_CODE` Place your code here */

    /* `#END` */

#ifdef USBCOMP_LPM_ISR_EXIT_CALLBACK
    USBCOMP_LPM_ISR_ExitCallback();
#endif /* (USBCOMP_LPM_ISR_EXIT_CALLBACK) */
}
#endif /* (USBCOMP_LPM_ACTIVE) */


#if (USBCOMP_EP_MANAGEMENT_DMA && USBCOMP_ARB_ISR_ACTIVE)
    /***************************************************************************
    * Function Name: USBCOMP_ARB_ISR
    ************************************************************************//**
    *
    *  Arbiter Interrupt Service Routine.
    *
    *
    ***************************************************************************/
    CY_ISR(USBCOMP_ARB_ISR)
    {
        uint32 arbIntrStatus;
        uint32 epStatus;
        uint32 ep = USBCOMP_EP1;

    #ifdef USBCOMP_ARB_ISR_ENTRY_CALLBACK
        USBCOMP_ARB_ISR_EntryCallback();
    #endif /* (USBCOMP_ARB_ISR_ENTRY_CALLBACK) */

        /* `#START ARB_BEGIN_USER_CODE` Place your code here */

        /* `#END` */

        /* Get pending ARB interrupt sources. */
        arbIntrStatus = USBCOMP_ARB_INT_SR_REG;

        while (0u != arbIntrStatus)
        {
            /* Check which EP is interrupt source. */
            if (0u != (arbIntrStatus & 0x01u))
            {
                /* Get endpoint enable interrupt sources. */
                epStatus = (USBCOMP_ARB_EP_BASE.arbEp[ep].epSr & USBCOMP_ARB_EP_BASE.arbEp[ep].epIntEn);

                /* Handle IN endpoint buffer full event: happens only once when endpoint buffer is loaded. */
                if (0u != (epStatus & USBCOMP_ARB_EPX_INT_IN_BUF_FULL))
                {
                    if (0u != (USBCOMP_EP[ep].addr & USBCOMP_DIR_IN))
                    {
                        /* Clear data ready status. */
                        USBCOMP_ARB_EP_BASE.arbEp[ep].epCfg &= (uint32) ~USBCOMP_ARB_EPX_CFG_IN_DATA_RDY;

                    #if (CY_PSOC3 || CY_PSOC5LP)
                        #if (USBCOMP_EP_MANAGEMENT_DMA_AUTO && (USBCOMP_EP_DMA_AUTO_OPT == 0u))
                            /* Set up common area DMA with rest of data. */
                            if(USBCOMP_inLength[ep] > USBCOMP_DMA_BYTES_PER_BURST)
                            {
                                USBCOMP_LoadNextInEP(ep, 0u);
                            }
                            else
                            {
                                USBCOMP_inBufFull[ep] = 1u;
                            }
                        #endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO && (USBCOMP_EP_DMA_AUTO_OPT == 0u)) */
                    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

                        /* Arm IN endpoint. */
                        USBCOMP_SIE_EP_BASE.sieEp[ep].epCr0 = USBCOMP_EP[ep].epMode;

                    #if (defined(USBCOMP_ENABLE_MIDI_STREAMING) && USBCOMP_ISR_SERVICE_MIDI_IN)
                        if (ep == USBCOMP_midi_in_ep)
                        {
                            /* Clear MIDI input pointer. */
                            USBCOMP_midiInPointer = 0u;
                        }
                    #endif /* (USBCOMP_ENABLE_MIDI_STREAMING) */
                    }
                }

            #if (USBCOMP_EP_MANAGEMENT_DMA_MANUAL)
                /* Handle DMA completion event for OUT endpoints. */
                if (0u != (epStatus & USBCOMP_ARB_EPX_SR_DMA_GNT))
                {
                    if (0u == (USBCOMP_EP[ep].addr & USBCOMP_DIR_IN))
                    {
                        /* Notify user that data has been copied from endpoint buffer. */
                        USBCOMP_EP[ep].apiEpState = USBCOMP_NO_EVENT_PENDING;

                        /* DMA done coping data: OUT endpoint has to be re-armed by user. */
                    }
                }
            #endif /* (USBCOMP_EP_MANAGEMENT_DMA_MANUAL) */

            #if (CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO)
                /* Handle DMA completion event for OUT endpoints. */
                if (0u != (epStatus & USBCOMP_ARB_EPX_INT_DMA_TERMIN))
                {
                    uint32 channelNum = USBCOMP_DmaChan[ep];

                    /* Restore burst counter for endpoint. */
                    USBCOMP_DmaEpBurstCnt[ep] = USBCOMP_DMA_GET_BURST_CNT(USBCOMP_DmaEpBurstCntBackup[ep]);

                    /* Disable DMA channel to restore descriptor configuration. The on-going transfer is aborted. */
                    USBCOMP_CyDmaChDisable(channelNum);

                    /* Generate DMA tr_out signal to notify USB IP that DMA is done. This signal is not generated
                    * when transfer was aborted (it occurs when host writes less bytes than buffer size).
                    */
                    USBCOMP_CyDmaTriggerOut(USBCOMP_DmaBurstEndOut[ep]);

                    /* Restore destination address for output endpoint. */
                    USBCOMP_CyDmaSetDstAddress(channelNum, USBCOMP_DMA_DESCR0, (void*) ((uint32) USBCOMP_DmaEpBufferAddrBackup[ep]));
                    USBCOMP_CyDmaSetDstAddress(channelNum, USBCOMP_DMA_DESCR1, (void*) ((uint32) USBCOMP_DmaEpBufferAddrBackup[ep] +
                                                                                                                   USBCOMP_DMA_BYTES_PER_BURST));

                    /* Restore number of data elements to transfer which was adjusted for last burst. */
                    if (0u != (USBCOMP_DmaEpLastBurstEl[ep] & USBCOMP_DMA_DESCR_REVERT))
                    {
                        USBCOMP_CyDmaSetNumDataElements(channelNum, (USBCOMP_DmaEpLastBurstEl[ep] >> USBCOMP_DMA_DESCR_SHIFT),
                                                                             USBCOMP_DMA_GET_MAX_ELEM_PER_BURST(USBCOMP_DmaEpLastBurstEl[ep]));
                    }

                    /* Validate descriptor 0 and 1 (also reset current state). Command to start with descriptor 0. */
                    USBCOMP_CyDmaValidateDescriptor(channelNum, USBCOMP_DMA_DESCR0);
                    if (USBCOMP_DmaEpBurstCntBackup[ep] > 1u)
                    {
                        USBCOMP_CyDmaValidateDescriptor(channelNum, USBCOMP_DMA_DESCR1);
                    }
                    USBCOMP_CyDmaSetDescriptor0Next(channelNum);

                    /* Enable DMA channel: configuration complete. */
                    USBCOMP_CyDmaChEnable(channelNum);
                    
                    
                    /* Read CR0 register to clear SIE lock. */
                    (void) USBCOMP_SIE_EP_BASE.sieEp[ep].epCr0;
                    
                    /* Toggle all endpoint types except ISOC. */
                    if (USBCOMP_GET_EP_TYPE(ep) != USBCOMP_EP_TYPE_ISOC)
                    {
                        USBCOMP_EP[ep].epToggle ^= USBCOMP_EPX_CNT_DATA_TOGGLE;
                    }
            
                    /* Notify user that data has been copied from endpoint buffer. */
                    USBCOMP_EP[ep].apiEpState = USBCOMP_EVENT_PENDING;
                    
                #if (defined(USBCOMP_ENABLE_MIDI_STREAMING) && \
                    !defined(USBCOMP_MAIN_SERVICE_MIDI_OUT) && USBCOMP_ISR_SERVICE_MIDI_OUT)
                    if (USBCOMP_midi_out_ep == ep)
                    {
                        USBCOMP_MIDI_OUT_Service();
                    }
                #endif /* (USBCOMP_ISR_SERVICE_MIDI_OUT) */
                }
            #endif /* (CY_PSOC4 && USBCOMP_EP_MANAGEMENT_DMA_AUTO) */


                /* `#START ARB_USER_CODE` Place your code here for handle Buffer Underflow/Overflow */

                /* `#END` */

            #ifdef USBCOMP_ARB_ISR_CALLBACK
                USBCOMP_ARB_ISR_Callback(ep, epStatus);
            #endif /* (USBCOMP_ARB_ISR_CALLBACK) */

                /* Clear serviced endpoint interrupt sources. */
                USBCOMP_ARB_EP_BASE.arbEp[ep].epSr = epStatus;
            }

            ++ep;
            arbIntrStatus >>= 1u;
        }

        /* `#START ARB_END_USER_CODE` Place your code here */

        /* `#END` */

    #ifdef USBCOMP_ARB_ISR_EXIT_CALLBACK
        USBCOMP_ARB_ISR_ExitCallback();
    #endif /* (USBCOMP_ARB_ISR_EXIT_CALLBACK) */
    }

#endif /*  (USBCOMP_ARB_ISR_ACTIVE && USBCOMP_EP_MANAGEMENT_DMA) */


#if (USBCOMP_EP_MANAGEMENT_DMA_AUTO)
#if (CY_PSOC4)

    /******************************************************************************
    * Function Name: USBCOMP_EPxDmaDone
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
    CY_INLINE static void USBCOMP_EPxDmaDone(uint8 dmaCh, uint8 ep)
    {
        uint32 nextAddr;

        /* Manage data elements which remain to transfer. */
        if (0u != USBCOMP_DmaEpBurstCnt[ep])
        {
            if(USBCOMP_DmaEpBurstCnt[ep] <= 2u)
            {
                /* Adjust length of last burst. */
                USBCOMP_CyDmaSetNumDataElements(dmaCh,
                                                    ((uint32) USBCOMP_DmaEpLastBurstEl[ep] >> USBCOMP_DMA_DESCR_SHIFT),
                                                    ((uint32) USBCOMP_DmaEpLastBurstEl[ep] &  USBCOMP_DMA_BURST_BYTES_MASK));
            }
            

            /* Advance source for input endpoint or destination for output endpoint. */
            if (0u != (USBCOMP_EP[ep].addr & USBCOMP_DIR_IN))
            {
                /* Change source for descriptor 0. */
                nextAddr = (uint32) USBCOMP_CyDmaGetSrcAddress(dmaCh, USBCOMP_DMA_DESCR0);
                nextAddr += (2u * USBCOMP_DMA_BYTES_PER_BURST);
                USBCOMP_CyDmaSetSrcAddress(dmaCh, USBCOMP_DMA_DESCR0, (void *) nextAddr);

                /* Change source for descriptor 1. */
                nextAddr += USBCOMP_DMA_BYTES_PER_BURST;
                USBCOMP_CyDmaSetSrcAddress(dmaCh, USBCOMP_DMA_DESCR1, (void *) nextAddr);
            }
            else
            {
                /* Change destination for descriptor 0. */
                nextAddr  = (uint32) USBCOMP_CyDmaGetDstAddress(dmaCh, USBCOMP_DMA_DESCR0);
                nextAddr += (2u * USBCOMP_DMA_BYTES_PER_BURST);
                USBCOMP_CyDmaSetDstAddress(dmaCh, USBCOMP_DMA_DESCR0, (void *) nextAddr);

                /* Change destination for descriptor 1. */
                nextAddr += USBCOMP_DMA_BYTES_PER_BURST;
                USBCOMP_CyDmaSetDstAddress(dmaCh, USBCOMP_DMA_DESCR1, (void *) nextAddr);
            }

            /* Enable DMA to execute transfer as it was disabled because there were no valid descriptor. */
            USBCOMP_CyDmaValidateDescriptor(dmaCh, USBCOMP_DMA_DESCR0);
            
            --USBCOMP_DmaEpBurstCnt[ep];
            if (0u != USBCOMP_DmaEpBurstCnt[ep])
            {
                USBCOMP_CyDmaValidateDescriptor(dmaCh, USBCOMP_DMA_DESCR1);
                --USBCOMP_DmaEpBurstCnt[ep];
            }
            
            USBCOMP_CyDmaChEnable (dmaCh);
            USBCOMP_CyDmaTriggerIn(USBCOMP_DmaReqOut[ep]);
        }
        else
        {
            /* No data to transfer. False DMA trig. Ignore.  */
        }

    }

    #if (USBCOMP_DMA1_ACTIVE)
        /******************************************************************************
        * Function Name: USBCOMP_EP1_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  Endpoint 1 DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        void USBCOMP_EP1_DMA_DONE_ISR(void)
        {

            USBCOMP_EPxDmaDone((uint8)USBCOMP_EP1_DMA_CH,
                                                  USBCOMP_EP1);
                
        }
    #endif /* (USBCOMP_DMA1_ACTIVE) */


    #if (USBCOMP_DMA2_ACTIVE)
        /******************************************************************************
        * Function Name: USBCOMP_EP2_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  Endpoint 2 DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        void USBCOMP_EP2_DMA_DONE_ISR(void)
        {

            USBCOMP_EPxDmaDone((uint8)USBCOMP_EP2_DMA_CH,
                                                  USBCOMP_EP2);
        }
    #endif /* (USBCOMP_DMA2_ACTIVE) */


    #if (USBCOMP_DMA3_ACTIVE)
        /******************************************************************************
        * Function Name: USBCOMP_EP3_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  Endpoint 3 DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        void USBCOMP_EP3_DMA_DONE_ISR(void)
        {

            USBCOMP_EPxDmaDone((uint8)USBCOMP_EP3_DMA_CH,
                                                  USBCOMP_EP3);
        }
    #endif /* (USBCOMP_DMA3_ACTIVE) */


    #if (USBCOMP_DMA4_ACTIVE)
        /******************************************************************************
        * Function Name: USBCOMP_EP4_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  Endpoint 4 DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        void USBCOMP_EP4_DMA_DONE_ISR(void)
        {

            USBCOMP_EPxDmaDone((uint8)USBCOMP_EP4_DMA_CH,
                                                  USBCOMP_EP4);
        }
    #endif /* (USBCOMP_DMA4_ACTIVE) */


    #if (USBCOMP_DMA5_ACTIVE)
        /******************************************************************************
        * Function Name: USBCOMP_EP5_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  Endpoint 5 DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        void USBCOMP_EP5_DMA_DONE_ISR(void)
        {

            USBCOMP_EPxDmaDone((uint8)USBCOMP_EP5_DMA_CH,
                                                  USBCOMP_EP5);
        }
    #endif /* (USBCOMP_DMA5_ACTIVE) */


    #if (USBCOMP_DMA6_ACTIVE)
        /******************************************************************************
        * Function Name: USBCOMP_EP6_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  Endpoint 6 DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        void USBCOMP_EP6_DMA_DONE_ISR(void)
        {

            USBCOMP_EPxDmaDone((uint8)USBCOMP_EP6_DMA_CH,
                                                  USBCOMP_EP6);
        }
    #endif /* (USBCOMP_DMA6_ACTIVE) */


    #if (USBCOMP_DMA7_ACTIVE)
        /******************************************************************************
        * Function Name: USBCOMP_EP7_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  Endpoint 7 DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        void USBCOMP_EP7_DMA_DONE_ISR(void)
        {

            USBCOMP_EPxDmaDone((uint8)USBCOMP_EP7_DMA_CH,
                                                  USBCOMP_EP7);
        }
    #endif /* (USBCOMP_DMA7_ACTIVE) */


    #if (USBCOMP_DMA8_ACTIVE)
        /******************************************************************************
        * Function Name: USBCOMP_EP8_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  Endpoint 8 DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        void USBCOMP_EP8_DMA_DONE_ISR(void)
        {

            USBCOMP_EPxDmaDone((uint8)USBCOMP_EP8_DMA_CH,
                                                  USBCOMP_EP8);
        }
    #endif /* (USBCOMP_DMA8_ACTIVE) */


#else
    #if (USBCOMP_EP_DMA_AUTO_OPT == 0u)
        /******************************************************************************
        * Function Name: USBCOMP_EP_DMA_DONE_ISR
        ***************************************************************************//**
        *
        *  DMA Done Interrupt Service Routine.
        *
        *
        ******************************************************************************/
        CY_ISR(USBCOMP_EP_DMA_DONE_ISR)
        {
            uint8 int8Status;
            uint8 int17Status;
            uint8 ep_status;
            uint8 ep = USBCOMP_EP1;

        #ifdef USBCOMP_EP_DMA_DONE_ISR_ENTRY_CALLBACK
            USBCOMP_EP_DMA_DONE_ISR_EntryCallback();
        #endif /* (USBCOMP_EP_DMA_DONE_ISR_ENTRY_CALLBACK) */

            /* `#START EP_DMA_DONE_BEGIN_USER_CODE` Place your code here */

            /* `#END` */

            /* Read clear on read status register with EP source of interrupt. */
            int17Status = USBCOMP_EP17_DMA_Done_SR_Read() & USBCOMP_EP17_SR_MASK;
            int8Status  = USBCOMP_EP8_DMA_Done_SR_Read()  & USBCOMP_EP8_SR_MASK;

            while (int8Status != 0u)
            {
                while (int17Status != 0u)
                {
                    if ((int17Status & 1u) != 0u)  /* If EpX interrupt present. */
                    {
                        /* Read Endpoint Status Register. */
                        ep_status = USBCOMP_ARB_EP_BASE.arbEp[ep].epSr;

                        if ((0u == (ep_status & USBCOMP_ARB_EPX_SR_IN_BUF_FULL)) &&
                            (0u ==USBCOMP_inBufFull[ep]))
                        {
                            /* `#START EP_DMA_DONE_USER_CODE` Place your code here */

                            /* `#END` */

                        #ifdef USBCOMP_EP_DMA_DONE_ISR_CALLBACK
                            USBCOMP_EP_DMA_DONE_ISR_Callback();
                        #endif /* (USBCOMP_EP_DMA_DONE_ISR_CALLBACK) */

                            /* Transfer again 2 last bytes into pre-fetch endpoint area. */
                            USBCOMP_ARB_EP_BASE.arbEp[ep].rwWaMsb = 0u;
                            USBCOMP_ARB_EP_BASE.arbEp[ep].rwWa = (USBCOMP_DMA_BYTES_PER_BURST * ep) - USBCOMP_DMA_BYTES_REPEAT;
                            USBCOMP_LoadNextInEP(ep, 1u);

                            /* Set Data ready status to generate DMA request. */
                            USBCOMP_ARB_EP_BASE.arbEp[ep].epCfg |= USBCOMP_ARB_EPX_CFG_IN_DATA_RDY;
                        }
                    }

                    ep++;
                    int17Status >>= 1u;
                }

                int8Status >>= 1u;

                if (int8Status != 0u)
                {
                    /* Prepare pointer for EP8. */
                    ep = USBCOMP_EP8;
                    int17Status = int8Status & 0x01u;
                }
            }

            /* `#START EP_DMA_DONE_END_USER_CODE` Place your code here */

            /* `#END` */

    #ifdef USBCOMP_EP_DMA_DONE_ISR_EXIT_CALLBACK
        USBCOMP_EP_DMA_DONE_ISR_ExitCallback();
    #endif /* (USBCOMP_EP_DMA_DONE_ISR_EXIT_CALLBACK) */
        }
    #endif /* (USBCOMP_EP_DMA_AUTO_OPT == 0u) */
#endif /* (CY_PSOC4) */
#endif /* (USBCOMP_EP_MANAGEMENT_DMA_AUTO) */


#if (CY_PSOC4)
    /***************************************************************************
    * Function Name: USBCOMP_IntrHandler
    ************************************************************************//**
    *
    *   Interrupt handler for Hi/Mid/Low ISRs.
    *
    *  regCause - The cause register of interrupt. One of the three variants:
    *       USBCOMP_INTR_CAUSE_LO_REG - Low interrupts.
    *       USBCOMP_INTR_CAUSE_MED_REG - Med interrupts.
    *       USBCOMP_INTR_CAUSE_HI_REG - - High interrupts.
    *
    *
    ***************************************************************************/
    CY_INLINE static void USBCOMP_IntrHandler(uint32 intrCause)
    {
        /* Array of pointers to component interrupt handlers. */
        static const cyisraddress USBCOMP_isrCallbacks[] =
        {
            NULL,
            &USBCOMP_BUS_RESET_ISR,
            &USBCOMP_EP_0_ISR,
            NULL,
            NULL,
            &USBCOMP_EP_1_ISR,
            &USBCOMP_EP_2_ISR,
            &USBCOMP_EP_3_ISR,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
        };

        uint32 cbIdx = 0u;

        /* Check arbiter interrupt source first. */
        if (0u != (intrCause & USBCOMP_INTR_CAUSE_ARB_INTR))
        {
            USBCOMP_isrCallbacks[USBCOMP_ARB_EP_INTR_NUM]();
        }

        /* Check all other interrupt sources (except arbiter and resume). */
        intrCause = (intrCause  & USBCOMP_INTR_CAUSE_CTRL_INTR_MASK) |
                    ((intrCause & USBCOMP_INTR_CAUSE_EP1_8_INTR_MASK) >>
                                  USBCOMP_INTR_CAUSE_EP_INTR_SHIFT);

        /* Call interrupt handlers for active interrupt sources. */
        while (0u != intrCause)
        {
            if (0u != (intrCause & 0x1u))
            {
                 USBCOMP_isrCallbacks[cbIdx]();
            }

            intrCause >>= 1u;
            ++cbIdx;
        }
    }


    /***************************************************************************
    * Function Name: USBCOMP_INTR_HI_ISR
    ************************************************************************//**
    *
    *   Interrupt Service Routine for the high group of the interrupt sources.
    *
    *
    ***************************************************************************/
    CY_ISR(USBCOMP_INTR_HI_ISR)
    {
        USBCOMP_IntrHandler(USBCOMP_INTR_CAUSE_HI_REG);
    }

    /***************************************************************************
    * Function Name: USBCOMP_INTR_MED_ISR
    ************************************************************************//**
    *
    *   Interrupt Service Routine for the medium group of the interrupt sources.
    *
    *
    ***************************************************************************/
    CY_ISR(USBCOMP_INTR_MED_ISR)
    {
       USBCOMP_IntrHandler(USBCOMP_INTR_CAUSE_MED_REG);
    }

    /***************************************************************************
    * Function Name: USBCOMP_INTR_LO_ISR
    ************************************************************************//**
    *
    *   Interrupt Service Routine for the low group of the interrupt sources.
    *
    *
    ***************************************************************************/
    CY_ISR(USBCOMP_INTR_LO_ISR)
    {
        USBCOMP_IntrHandler(USBCOMP_INTR_CAUSE_LO_REG);
    }
#endif /* (CY_PSOC4) */


/* [] END OF FILE */
