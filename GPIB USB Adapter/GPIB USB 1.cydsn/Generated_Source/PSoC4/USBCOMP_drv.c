/***************************************************************************//**
* \file USBCOMP_drv.c
* \version 3.20
*
* \brief
*  This file contains the Endpoint 0 Driver for the USBFS Component.  
*
********************************************************************************
* \copyright
* Copyright 2008-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USBCOMP_pvt.h"
#include "cyapicallbacks.h"


/***************************************
* Global data allocation
***************************************/

volatile T_USBCOMP_EP_CTL_BLOCK USBCOMP_EP[USBCOMP_MAX_EP];

/** Contains the current configuration number, which is set by the host using a 
 * SET_CONFIGURATION request. This variable is initialized to zero in 
 * USBFS_InitComponent() API and can be read by the USBFS_GetConfiguration() 
 * API.*/
volatile uint8 USBCOMP_configuration;

/** Contains the current interface number.*/
volatile uint8 USBCOMP_interfaceNumber;

/** This variable is set to one after SET_CONFIGURATION and SET_INTERFACE 
 *requests. It can be read by the USBFS_IsConfigurationChanged() API */
volatile uint8 USBCOMP_configurationChanged;

/** Contains the current device address.*/
volatile uint8 USBCOMP_deviceAddress;

/** This is a two-bit variable that contains power status in the bit 0 
 * (DEVICE_STATUS_BUS_POWERED or DEVICE_STATUS_SELF_POWERED) and remote wakeup 
 * status (DEVICE_STATUS_REMOTE_WAKEUP) in the bit 1. This variable is 
 * initialized to zero in USBFS_InitComponent() API, configured by the 
 * USBFS_SetPowerStatus() API. The remote wakeup status cannot be set using the 
 * API SetPowerStatus(). */
volatile uint8 USBCOMP_deviceStatus;

volatile uint8 USBCOMP_interfaceSetting[USBCOMP_MAX_INTERFACES_NUMBER];
volatile uint8 USBCOMP_interfaceSetting_last[USBCOMP_MAX_INTERFACES_NUMBER];
volatile uint8 USBCOMP_interfaceStatus[USBCOMP_MAX_INTERFACES_NUMBER];

/** Contains the started device number. This variable is set by the 
 * USBFS_Start() or USBFS_InitComponent() APIs.*/
volatile uint8 USBCOMP_device;

/** Initialized class array for each interface. It is used for handling Class 
 * specific requests depend on interface class. Different classes in multiple 
 * alternate settings are not supported.*/
const uint8 CYCODE *USBCOMP_interfaceClass;


/***************************************
* Local data allocation
***************************************/

volatile uint8  USBCOMP_ep0Toggle;
volatile uint8  USBCOMP_lastPacketSize;

/** This variable is used by the communication functions to handle the current 
* transfer state.
* Initialized to TRANS_STATE_IDLE in the USBFS_InitComponent() API and after a 
* complete transfer in the status stage.
* Changed to the TRANS_STATE_CONTROL_READ or TRANS_STATE_CONTROL_WRITE in setup 
* transaction depending on the request type.
*/
volatile uint8  USBCOMP_transferState;
volatile T_USBCOMP_TD USBCOMP_currentTD;
volatile uint8  USBCOMP_ep0Mode;
volatile uint8  USBCOMP_ep0Count;
volatile uint16 USBCOMP_transferByteCount;


/*******************************************************************************
* Function Name: USBCOMP_ep_0_Interrupt
****************************************************************************//**
*
*  This Interrupt Service Routine handles Endpoint 0 (Control Pipe) traffic.
*  It dispatches setup requests and handles the data and status stages.
*
*
*******************************************************************************/
CY_ISR(USBCOMP_EP_0_ISR)
{
    uint32 tempReg;
    uint32 modifyReg;

#ifdef USBCOMP_EP_0_ISR_ENTRY_CALLBACK
    USBCOMP_EP_0_ISR_EntryCallback();
#endif /* (USBCOMP_EP_0_ISR_ENTRY_CALLBACK) */
    
    tempReg = USBCOMP_EP0_CR_REG;
    if ((tempReg & USBCOMP_MODE_ACKD) != 0u)
    {
        modifyReg = 1u;
        if ((tempReg & USBCOMP_MODE_SETUP_RCVD) != 0u)
        {
            if ((tempReg & USBCOMP_MODE_MASK) != USBCOMP_MODE_NAK_IN_OUT)
            {
                /* Mode not equal to NAK_IN_OUT: invalid setup */
                modifyReg = 0u;
            }
            else
            {
                USBCOMP_HandleSetup();
                
                if ((USBCOMP_ep0Mode & USBCOMP_MODE_SETUP_RCVD) != 0u)
                {
                    /* SETUP bit set: exit without mode modificaiton */
                    modifyReg = 0u;
                }
            }
        }
        else if ((tempReg & USBCOMP_MODE_IN_RCVD) != 0u)
        {
            USBCOMP_HandleIN();
        }
        else if ((tempReg & USBCOMP_MODE_OUT_RCVD) != 0u)
        {
            USBCOMP_HandleOUT();
        }
        else
        {
            modifyReg = 0u;
        }
        
        /* Modify the EP0_CR register */
        if (modifyReg != 0u)
        {
            
            tempReg = USBCOMP_EP0_CR_REG;
            
            /* Make sure that SETUP bit is cleared before modification */
            if ((tempReg & USBCOMP_MODE_SETUP_RCVD) == 0u)
            {
                /* Update count register */
                tempReg = (uint32) USBCOMP_ep0Toggle | USBCOMP_ep0Count;
                USBCOMP_EP0_CNT_REG = tempReg;
               
                /* Make sure that previous write operaiton was successful */
                if (tempReg == USBCOMP_EP0_CNT_REG)
                {
                    /* Repeat until next successful write operation */
                    do
                    {
                        /* Init temporary variable */
                        modifyReg = USBCOMP_ep0Mode;
                        
                        /* Unlock register */
                        tempReg = (uint8) (USBCOMP_EP0_CR_REG & USBCOMP_MODE_SETUP_RCVD);
                        
                        /* Check if SETUP bit is not set */
                        if (0u == tempReg)
                        {
                            /* Set the Mode Register  */
                            USBCOMP_EP0_CR_REG = USBCOMP_ep0Mode;
                            
                            /* Writing check */
                            modifyReg = USBCOMP_EP0_CR_REG & USBCOMP_MODE_MASK;
                        }
                    }
                    while (modifyReg != USBCOMP_ep0Mode);
                }
            }
        }
    }

    USBCOMP_ClearSieInterruptSource(USBCOMP_INTR_SIE_EP0_INTR);
	
#ifdef USBCOMP_EP_0_ISR_EXIT_CALLBACK
    USBCOMP_EP_0_ISR_ExitCallback();
#endif /* (USBCOMP_EP_0_ISR_EXIT_CALLBACK) */
}


/*******************************************************************************
* Function Name: USBCOMP_HandleSetup
****************************************************************************//**
*
*  This Routine dispatches requests for the four USB request types
*
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_HandleSetup(void) 
{
    uint8 requestHandled;
    
    /* Clear register lock by SIE (read register) and clear setup bit 
    * (write any value in register).
    */
    requestHandled = (uint8) USBCOMP_EP0_CR_REG;
    USBCOMP_EP0_CR_REG = (uint32) requestHandled;
    requestHandled = (uint8) USBCOMP_EP0_CR_REG;

    if ((requestHandled & USBCOMP_MODE_SETUP_RCVD) != 0u)
    {
        /* SETUP bit is set: exit without mode modification. */
        USBCOMP_ep0Mode = requestHandled;
    }
    else
    {
        /* In case the previous transfer did not complete, close it out */
        USBCOMP_UpdateStatusBlock(USBCOMP_XFER_PREMATURE);

        /* Check request type. */
        switch (USBCOMP_bmRequestTypeReg & USBCOMP_RQST_TYPE_MASK)
        {
            case USBCOMP_RQST_TYPE_STD:
                requestHandled = USBCOMP_HandleStandardRqst();
                break;
                
            case USBCOMP_RQST_TYPE_CLS:
                requestHandled = USBCOMP_DispatchClassRqst();
                break;
                
            case USBCOMP_RQST_TYPE_VND:
                requestHandled = USBCOMP_HandleVendorRqst();
                break;
                
            default:
                requestHandled = USBCOMP_FALSE;
                break;
        }
        
        /* If request is not recognized. Stall endpoint 0 IN and OUT. */
        if (requestHandled == USBCOMP_FALSE)
        {
            USBCOMP_ep0Mode = USBCOMP_MODE_STALL_IN_OUT;
        }
    }
}


/*******************************************************************************
* Function Name: USBCOMP_HandleIN
****************************************************************************//**
*
*  This routine handles EP0 IN transfers.
*
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_HandleIN(void) 
{
    switch (USBCOMP_transferState)
    {
        case USBCOMP_TRANS_STATE_IDLE:
            break;
        
        case USBCOMP_TRANS_STATE_CONTROL_READ:
            USBCOMP_ControlReadDataStage();
            break;
            
        case USBCOMP_TRANS_STATE_CONTROL_WRITE:
            USBCOMP_ControlWriteStatusStage();
            break;
            
        case USBCOMP_TRANS_STATE_NO_DATA_CONTROL:
            USBCOMP_NoDataControlStatusStage();
            break;
            
        default:    /* there are no more states */
            break;
    }
}


/*******************************************************************************
* Function Name: USBCOMP_HandleOUT
****************************************************************************//**
*
*  This routine handles EP0 OUT transfers.
*
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_HandleOUT(void) 
{
    switch (USBCOMP_transferState)
    {
        case USBCOMP_TRANS_STATE_IDLE:
            break;
        
        case USBCOMP_TRANS_STATE_CONTROL_READ:
            USBCOMP_ControlReadStatusStage();
            break;
            
        case USBCOMP_TRANS_STATE_CONTROL_WRITE:
            USBCOMP_ControlWriteDataStage();
            break;
            
        case USBCOMP_TRANS_STATE_NO_DATA_CONTROL:
            /* Update the completion block */
            USBCOMP_UpdateStatusBlock(USBCOMP_XFER_ERROR);
            
            /* We expect no more data, so stall INs and OUTs */
            USBCOMP_ep0Mode = USBCOMP_MODE_STALL_IN_OUT;
            break;
            
        default:    
            /* There are no more states */
            break;
    }
}


/*******************************************************************************
* Function Name: USBCOMP_LoadEP0
****************************************************************************//**
*
*  This routine loads the EP0 data registers for OUT transfers. It uses the
*  currentTD (previously initialized by the _InitControlWrite function and
*  updated for each OUT transfer, and the bLastPacketSize) to determine how
*  many uint8s to transfer on the current OUT.
*
*  If the number of uint8s remaining is zero and the last transfer was full,
*  we need to send a zero length packet.  Otherwise we send the minimum
*  of the control endpoint size (8) or remaining number of uint8s for the
*  transaction.
*
*
* \globalvars
*  USBCOMP_transferByteCount - Update the transfer byte count from the
*     last transaction.
*  USBCOMP_ep0Count - counts the data loaded to the SIE memory in
*     current packet.
*  USBCOMP_lastPacketSize - remembers the USBFS_ep0Count value for the
*     next packet.
*  USBCOMP_transferByteCount - sum of the previous bytes transferred
*     on previous packets(sum of USBFS_lastPacketSize)
*  USBCOMP_ep0Toggle - inverted
*  USBCOMP_ep0Mode  - prepare for mode register content.
*  USBCOMP_transferState - set to TRANS_STATE_CONTROL_READ
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_LoadEP0(void) 
{
    uint32 ep0Count = 0u;

    /* Update the transfer byte count from the last transaction */
    USBCOMP_transferByteCount += USBCOMP_lastPacketSize;

    /* Now load the next transaction */
    while ((USBCOMP_currentTD.count > 0u) && (ep0Count < 8u))
    {
        USBCOMP_EP0_DR_BASE.epData[ep0Count] = (uint32) *USBCOMP_currentTD.pData;
        USBCOMP_currentTD.pData = &USBCOMP_currentTD.pData[1u];
        ep0Count++;
        USBCOMP_currentTD.count--;
    }

    /* Support zero-length packet */
    if ((USBCOMP_lastPacketSize == 8u) || (ep0Count > 0u))
    {
        /* Update the data toggle */
        USBCOMP_ep0Toggle ^= USBCOMP_EP0_CNT_DATA_TOGGLE;
        /* Set the Mode Register  */
        USBCOMP_ep0Mode = USBCOMP_MODE_ACK_IN_STATUS_OUT;
        /* Update the state (or stay the same) */
        USBCOMP_transferState = USBCOMP_TRANS_STATE_CONTROL_READ;
    }
    else
    {
        /* Expect Status Stage Out */
        USBCOMP_ep0Mode = USBCOMP_MODE_STATUS_OUT_ONLY;
        /* Update the state (or stay the same) */
        USBCOMP_transferState = USBCOMP_TRANS_STATE_CONTROL_READ;
    }

    /* Save the packet size for next time */
    USBCOMP_ep0Count =       (uint8) ep0Count;
    USBCOMP_lastPacketSize = (uint8) ep0Count;
}


/*******************************************************************************
* Function Name: USBCOMP_InitControlRead
****************************************************************************//**
*
*  Initialize a control read transaction. It is used to send data to the host.
*  The following global variables should be initialized before this function
*  called. To send zero length packet use InitZeroLengthControlTransfer
*  function.
*
*
* \return
*  requestHandled state.
*
* \globalvars
*  USBCOMP_currentTD.count - counts of data to be sent.
*  USBCOMP_currentTD.pData - data pointer.
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBCOMP_InitControlRead(void) 
{
    uint16 xferCount;

    if (USBCOMP_currentTD.count == 0u)
    {
        (void) USBCOMP_InitZeroLengthControlTransfer();
    }
    else
    {
        /* Set up the state machine */
        USBCOMP_transferState = USBCOMP_TRANS_STATE_CONTROL_READ;
        
        /* Set the toggle, it gets updated in LoadEP */
        USBCOMP_ep0Toggle = 0u;
        
        /* Initialize the Status Block */
        USBCOMP_InitializeStatusBlock();
        
        xferCount = ((uint16)((uint16) USBCOMP_lengthHiReg << 8u) | ((uint16) USBCOMP_lengthLoReg));

        if (USBCOMP_currentTD.count > xferCount)
        {
            USBCOMP_currentTD.count = xferCount;
        }
        
        USBCOMP_LoadEP0();
    }

    return (USBCOMP_TRUE);
}


/*******************************************************************************
* Function Name: USBCOMP_InitZeroLengthControlTransfer
****************************************************************************//**
*
*  Initialize a zero length data IN transfer.
*
* \return
*  requestHandled state.
*
* \globalvars
*  USBCOMP_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USBCOMP_ep0Mode  - prepare for mode register content.
*  USBCOMP_transferState - set to TRANS_STATE_CONTROL_READ
*  USBCOMP_ep0Count - cleared, means the zero-length packet.
*  USBCOMP_lastPacketSize - cleared.
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBCOMP_InitZeroLengthControlTransfer(void)
                                                
{
    /* Update the state */
    USBCOMP_transferState = USBCOMP_TRANS_STATE_CONTROL_READ;
    
    /* Set the data toggle */
    USBCOMP_ep0Toggle = USBCOMP_EP0_CNT_DATA_TOGGLE;
    
    /* Set the Mode Register  */
    USBCOMP_ep0Mode = USBCOMP_MODE_ACK_IN_STATUS_OUT;
    
    /* Save the packet size for next time */
    USBCOMP_lastPacketSize = 0u;
    
    USBCOMP_ep0Count = 0u;

    return (USBCOMP_TRUE);
}


/*******************************************************************************
* Function Name: USBCOMP_ControlReadDataStage
****************************************************************************//**
*
*  Handle the Data Stage of a control read transfer.
*
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_ControlReadDataStage(void) 

{
    USBCOMP_LoadEP0();
}


/*******************************************************************************
* Function Name: USBCOMP_ControlReadStatusStage
****************************************************************************//**
*
*  Handle the Status Stage of a control read transfer.
*
*
* \globalvars
*  USBCOMP_USBFS_transferByteCount - updated with last packet size.
*  USBCOMP_transferState - set to TRANS_STATE_IDLE.
*  USBCOMP_ep0Mode  - set to MODE_STALL_IN_OUT.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_ControlReadStatusStage(void) 
{
    /* Update the transfer byte count */
    USBCOMP_transferByteCount += USBCOMP_lastPacketSize;
    
    /* Go Idle */
    USBCOMP_transferState = USBCOMP_TRANS_STATE_IDLE;
    
    /* Update the completion block */
    USBCOMP_UpdateStatusBlock(USBCOMP_XFER_STATUS_ACK);
    
    /* We expect no more data, so stall INs and OUTs */
    USBCOMP_ep0Mode = USBCOMP_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBCOMP_InitControlWrite
****************************************************************************//**
*
*  Initialize a control write transaction
*
* \return
*  requestHandled state.
*
* \globalvars
*  USBCOMP_USBFS_transferState - set to TRANS_STATE_CONTROL_WRITE
*  USBCOMP_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USBCOMP_ep0Mode  - set to MODE_ACK_OUT_STATUS_IN
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBCOMP_InitControlWrite(void) 
{
    uint16 xferCount;

    /* Set up the state machine */
    USBCOMP_transferState = USBCOMP_TRANS_STATE_CONTROL_WRITE;
    
    /* This might not be necessary */
    USBCOMP_ep0Toggle = USBCOMP_EP0_CNT_DATA_TOGGLE;
    
    /* Initialize the Status Block */
    USBCOMP_InitializeStatusBlock();

    xferCount = ((uint16)((uint16) USBCOMP_lengthHiReg << 8u) | ((uint16) USBCOMP_lengthLoReg));

    if (USBCOMP_currentTD.count > xferCount)
    {
        USBCOMP_currentTD.count = xferCount;
    }

    /* Expect Data or Status Stage */
    USBCOMP_ep0Mode = USBCOMP_MODE_ACK_OUT_STATUS_IN;

    return(USBCOMP_TRUE);
}


/*******************************************************************************
* Function Name: USBCOMP_ControlWriteDataStage
****************************************************************************//**
*
*  Handle the Data Stage of a control write transfer
*       1. Get the data (We assume the destination was validated previously)
*       2. Update the count and data toggle
*       3. Update the mode register for the next transaction
*
*
* \globalvars
*  USBCOMP_transferByteCount - Update the transfer byte count from the
*    last transaction.
*  USBCOMP_ep0Count - counts the data loaded from the SIE memory
*    in current packet.
*  USBCOMP_transferByteCount - sum of the previous bytes transferred
*    on previous packets(sum of USBFS_lastPacketSize)
*  USBCOMP_ep0Toggle - inverted
*  USBCOMP_ep0Mode  - set to MODE_ACK_OUT_STATUS_IN.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_ControlWriteDataStage(void) 
{
    uint32 ep0Count;
    uint32 regIndex = 0u;

    ep0Count = (USBCOMP_EP0_CNT_REG & USBCOMP_EPX_CNT0_MASK) - USBCOMP_EPX_CNTX_CRC_COUNT;

    USBCOMP_transferByteCount += (uint8)ep0Count;

    while ((USBCOMP_currentTD.count > 0u) && (ep0Count > 0u))
    {
        *USBCOMP_currentTD.pData = (uint8) USBCOMP_EP0_DR_BASE.epData[regIndex];
        USBCOMP_currentTD.pData = &USBCOMP_currentTD.pData[1u];
        regIndex++;
        ep0Count--;
        USBCOMP_currentTD.count--;
    }
    
    USBCOMP_ep0Count = (uint8)ep0Count;
    
    /* Update the data toggle */
    USBCOMP_ep0Toggle ^= USBCOMP_EP0_CNT_DATA_TOGGLE;
    
    /* Expect Data or Status Stage */
    USBCOMP_ep0Mode = USBCOMP_MODE_ACK_OUT_STATUS_IN;
}


/*******************************************************************************
* Function Name: USBCOMP_ControlWriteStatusStage
****************************************************************************//**
*
*  Handle the Status Stage of a control write transfer
*
* \globalvars
*  USBCOMP_transferState - set to TRANS_STATE_IDLE.
*  USBCOMP_USBFS_ep0Mode  - set to MODE_STALL_IN_OUT.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_ControlWriteStatusStage(void) 
{
    /* Go Idle */
    USBCOMP_transferState = USBCOMP_TRANS_STATE_IDLE;
    
    /* Update the completion block */    
    USBCOMP_UpdateStatusBlock(USBCOMP_XFER_STATUS_ACK);
    
    /* We expect no more data, so stall INs and OUTs */
    USBCOMP_ep0Mode = USBCOMP_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBCOMP_InitNoDataControlTransfer
****************************************************************************//**
*
*  Initialize a no data control transfer
*
* \return
*  requestHandled state.
*
* \globalvars
*  USBCOMP_transferState - set to TRANS_STATE_NO_DATA_CONTROL.
*  USBCOMP_ep0Mode  - set to MODE_STATUS_IN_ONLY.
*  USBCOMP_ep0Count - cleared.
*  USBCOMP_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBCOMP_InitNoDataControlTransfer(void) 
{
    USBCOMP_transferState = USBCOMP_TRANS_STATE_NO_DATA_CONTROL;
    USBCOMP_ep0Mode       = USBCOMP_MODE_STATUS_IN_ONLY;
    USBCOMP_ep0Toggle     = USBCOMP_EP0_CNT_DATA_TOGGLE;
    USBCOMP_ep0Count      = 0u;

    return (USBCOMP_TRUE);
}


/*******************************************************************************
* Function Name: USBCOMP_NoDataControlStatusStage
****************************************************************************//**
*  Handle the Status Stage of a no data control transfer.
*
*  SET_ADDRESS is special, since we need to receive the status stage with
*  the old address.
*
* \globalvars
*  USBCOMP_transferState - set to TRANS_STATE_IDLE.
*  USBCOMP_ep0Mode  - set to MODE_STALL_IN_OUT.
*  USBCOMP_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USBCOMP_deviceAddress - used to set new address and cleared
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_NoDataControlStatusStage(void) 
{
    if (0u != USBCOMP_deviceAddress)
    {
        /* Update device address if we got new address. */
        USBCOMP_CR0_REG = (uint32) USBCOMP_deviceAddress | USBCOMP_CR0_ENABLE;
        USBCOMP_deviceAddress = 0u;
    }

    USBCOMP_transferState = USBCOMP_TRANS_STATE_IDLE;
    
    /* Update the completion block. */
    USBCOMP_UpdateStatusBlock(USBCOMP_XFER_STATUS_ACK);
    
    /* Stall IN and OUT, no more data is expected. */
    USBCOMP_ep0Mode = USBCOMP_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBCOMP_UpdateStatusBlock
****************************************************************************//**
*
*  Update the Completion Status Block for a Request.  The block is updated
*  with the completion code the USBCOMP_transferByteCount.  The
*  StatusBlock Pointer is set to NULL.
*
*  completionCode - status.
*
*
* \globalvars
*  USBCOMP_currentTD.pStatusBlock->status - updated by the
*    completionCode parameter.
*  USBCOMP_currentTD.pStatusBlock->length - updated.
*  USBCOMP_currentTD.pStatusBlock - cleared.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_UpdateStatusBlock(uint8 completionCode) 
{
    if (USBCOMP_currentTD.pStatusBlock != NULL)
    {
        USBCOMP_currentTD.pStatusBlock->status = completionCode;
        USBCOMP_currentTD.pStatusBlock->length = USBCOMP_transferByteCount;
        USBCOMP_currentTD.pStatusBlock = NULL;
    }
}


/*******************************************************************************
* Function Name: USBCOMP_InitializeStatusBlock
****************************************************************************//**
*
*  Initialize the Completion Status Block for a Request.  The completion
*  code is set to USB_XFER_IDLE.
*
*  Also, initializes USBCOMP_transferByteCount.  Save some space,
*  this is the only consumer.
*
* \globalvars
*  USBCOMP_currentTD.pStatusBlock->status - set to XFER_IDLE.
*  USBCOMP_currentTD.pStatusBlock->length - cleared.
*  USBCOMP_transferByteCount - cleared.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBCOMP_InitializeStatusBlock(void) 
{
    USBCOMP_transferByteCount = 0u;
    
    if (USBCOMP_currentTD.pStatusBlock != NULL)
    {
        USBCOMP_currentTD.pStatusBlock->status = USBCOMP_XFER_IDLE;
        USBCOMP_currentTD.pStatusBlock->length = 0u;
    }
}


/* [] END OF FILE */
