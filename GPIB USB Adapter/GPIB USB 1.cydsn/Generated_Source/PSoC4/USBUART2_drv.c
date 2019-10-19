/***************************************************************************//**
* \file USBUART2_drv.c
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

#include "USBUART2_pvt.h"
#include "cyapicallbacks.h"


/***************************************
* Global data allocation
***************************************/

volatile T_USBUART2_EP_CTL_BLOCK USBUART2_EP[USBUART2_MAX_EP];

/** Contains the current configuration number, which is set by the host using a 
 * SET_CONFIGURATION request. This variable is initialized to zero in 
 * USBFS_InitComponent() API and can be read by the USBFS_GetConfiguration() 
 * API.*/
volatile uint8 USBUART2_configuration;

/** Contains the current interface number.*/
volatile uint8 USBUART2_interfaceNumber;

/** This variable is set to one after SET_CONFIGURATION and SET_INTERFACE 
 *requests. It can be read by the USBFS_IsConfigurationChanged() API */
volatile uint8 USBUART2_configurationChanged;

/** Contains the current device address.*/
volatile uint8 USBUART2_deviceAddress;

/** This is a two-bit variable that contains power status in the bit 0 
 * (DEVICE_STATUS_BUS_POWERED or DEVICE_STATUS_SELF_POWERED) and remote wakeup 
 * status (DEVICE_STATUS_REMOTE_WAKEUP) in the bit 1. This variable is 
 * initialized to zero in USBFS_InitComponent() API, configured by the 
 * USBFS_SetPowerStatus() API. The remote wakeup status cannot be set using the 
 * API SetPowerStatus(). */
volatile uint8 USBUART2_deviceStatus;

volatile uint8 USBUART2_interfaceSetting[USBUART2_MAX_INTERFACES_NUMBER];
volatile uint8 USBUART2_interfaceSetting_last[USBUART2_MAX_INTERFACES_NUMBER];
volatile uint8 USBUART2_interfaceStatus[USBUART2_MAX_INTERFACES_NUMBER];

/** Contains the started device number. This variable is set by the 
 * USBFS_Start() or USBFS_InitComponent() APIs.*/
volatile uint8 USBUART2_device;

/** Initialized class array for each interface. It is used for handling Class 
 * specific requests depend on interface class. Different classes in multiple 
 * alternate settings are not supported.*/
const uint8 CYCODE *USBUART2_interfaceClass;


/***************************************
* Local data allocation
***************************************/

volatile uint8  USBUART2_ep0Toggle;
volatile uint8  USBUART2_lastPacketSize;

/** This variable is used by the communication functions to handle the current 
* transfer state.
* Initialized to TRANS_STATE_IDLE in the USBFS_InitComponent() API and after a 
* complete transfer in the status stage.
* Changed to the TRANS_STATE_CONTROL_READ or TRANS_STATE_CONTROL_WRITE in setup 
* transaction depending on the request type.
*/
volatile uint8  USBUART2_transferState;
volatile T_USBUART2_TD USBUART2_currentTD;
volatile uint8  USBUART2_ep0Mode;
volatile uint8  USBUART2_ep0Count;
volatile uint16 USBUART2_transferByteCount;


/*******************************************************************************
* Function Name: USBUART2_ep_0_Interrupt
****************************************************************************//**
*
*  This Interrupt Service Routine handles Endpoint 0 (Control Pipe) traffic.
*  It dispatches setup requests and handles the data and status stages.
*
*
*******************************************************************************/
CY_ISR(USBUART2_EP_0_ISR)
{
    uint32 tempReg;
    uint32 modifyReg;

#ifdef USBUART2_EP_0_ISR_ENTRY_CALLBACK
    USBUART2_EP_0_ISR_EntryCallback();
#endif /* (USBUART2_EP_0_ISR_ENTRY_CALLBACK) */
    
    tempReg = USBUART2_EP0_CR_REG;
    if ((tempReg & USBUART2_MODE_ACKD) != 0u)
    {
        modifyReg = 1u;
        if ((tempReg & USBUART2_MODE_SETUP_RCVD) != 0u)
        {
            if ((tempReg & USBUART2_MODE_MASK) != USBUART2_MODE_NAK_IN_OUT)
            {
                /* Mode not equal to NAK_IN_OUT: invalid setup */
                modifyReg = 0u;
            }
            else
            {
                USBUART2_HandleSetup();
                
                if ((USBUART2_ep0Mode & USBUART2_MODE_SETUP_RCVD) != 0u)
                {
                    /* SETUP bit set: exit without mode modificaiton */
                    modifyReg = 0u;
                }
            }
        }
        else if ((tempReg & USBUART2_MODE_IN_RCVD) != 0u)
        {
            USBUART2_HandleIN();
        }
        else if ((tempReg & USBUART2_MODE_OUT_RCVD) != 0u)
        {
            USBUART2_HandleOUT();
        }
        else
        {
            modifyReg = 0u;
        }
        
        /* Modify the EP0_CR register */
        if (modifyReg != 0u)
        {
            
            tempReg = USBUART2_EP0_CR_REG;
            
            /* Make sure that SETUP bit is cleared before modification */
            if ((tempReg & USBUART2_MODE_SETUP_RCVD) == 0u)
            {
                /* Update count register */
                tempReg = (uint32) USBUART2_ep0Toggle | USBUART2_ep0Count;
                USBUART2_EP0_CNT_REG = tempReg;
               
                /* Make sure that previous write operaiton was successful */
                if (tempReg == USBUART2_EP0_CNT_REG)
                {
                    /* Repeat until next successful write operation */
                    do
                    {
                        /* Init temporary variable */
                        modifyReg = USBUART2_ep0Mode;
                        
                        /* Unlock register */
                        tempReg = (uint8) (USBUART2_EP0_CR_REG & USBUART2_MODE_SETUP_RCVD);
                        
                        /* Check if SETUP bit is not set */
                        if (0u == tempReg)
                        {
                            /* Set the Mode Register  */
                            USBUART2_EP0_CR_REG = USBUART2_ep0Mode;
                            
                            /* Writing check */
                            modifyReg = USBUART2_EP0_CR_REG & USBUART2_MODE_MASK;
                        }
                    }
                    while (modifyReg != USBUART2_ep0Mode);
                }
            }
        }
    }

    USBUART2_ClearSieInterruptSource(USBUART2_INTR_SIE_EP0_INTR);
	
#ifdef USBUART2_EP_0_ISR_EXIT_CALLBACK
    USBUART2_EP_0_ISR_ExitCallback();
#endif /* (USBUART2_EP_0_ISR_EXIT_CALLBACK) */
}


/*******************************************************************************
* Function Name: USBUART2_HandleSetup
****************************************************************************//**
*
*  This Routine dispatches requests for the four USB request types
*
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBUART2_HandleSetup(void) 
{
    uint8 requestHandled;
    
    /* Clear register lock by SIE (read register) and clear setup bit 
    * (write any value in register).
    */
    requestHandled = (uint8) USBUART2_EP0_CR_REG;
    USBUART2_EP0_CR_REG = (uint32) requestHandled;
    requestHandled = (uint8) USBUART2_EP0_CR_REG;

    if ((requestHandled & USBUART2_MODE_SETUP_RCVD) != 0u)
    {
        /* SETUP bit is set: exit without mode modification. */
        USBUART2_ep0Mode = requestHandled;
    }
    else
    {
        /* In case the previous transfer did not complete, close it out */
        USBUART2_UpdateStatusBlock(USBUART2_XFER_PREMATURE);

        /* Check request type. */
        switch (USBUART2_bmRequestTypeReg & USBUART2_RQST_TYPE_MASK)
        {
            case USBUART2_RQST_TYPE_STD:
                requestHandled = USBUART2_HandleStandardRqst();
                break;
                
            case USBUART2_RQST_TYPE_CLS:
                requestHandled = USBUART2_DispatchClassRqst();
                break;
                
            case USBUART2_RQST_TYPE_VND:
                requestHandled = USBUART2_HandleVendorRqst();
                break;
                
            default:
                requestHandled = USBUART2_FALSE;
                break;
        }
        
        /* If request is not recognized. Stall endpoint 0 IN and OUT. */
        if (requestHandled == USBUART2_FALSE)
        {
            USBUART2_ep0Mode = USBUART2_MODE_STALL_IN_OUT;
        }
    }
}


/*******************************************************************************
* Function Name: USBUART2_HandleIN
****************************************************************************//**
*
*  This routine handles EP0 IN transfers.
*
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBUART2_HandleIN(void) 
{
    switch (USBUART2_transferState)
    {
        case USBUART2_TRANS_STATE_IDLE:
            break;
        
        case USBUART2_TRANS_STATE_CONTROL_READ:
            USBUART2_ControlReadDataStage();
            break;
            
        case USBUART2_TRANS_STATE_CONTROL_WRITE:
            USBUART2_ControlWriteStatusStage();
            break;
            
        case USBUART2_TRANS_STATE_NO_DATA_CONTROL:
            USBUART2_NoDataControlStatusStage();
            break;
            
        default:    /* there are no more states */
            break;
    }
}


/*******************************************************************************
* Function Name: USBUART2_HandleOUT
****************************************************************************//**
*
*  This routine handles EP0 OUT transfers.
*
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBUART2_HandleOUT(void) 
{
    switch (USBUART2_transferState)
    {
        case USBUART2_TRANS_STATE_IDLE:
            break;
        
        case USBUART2_TRANS_STATE_CONTROL_READ:
            USBUART2_ControlReadStatusStage();
            break;
            
        case USBUART2_TRANS_STATE_CONTROL_WRITE:
            USBUART2_ControlWriteDataStage();
            break;
            
        case USBUART2_TRANS_STATE_NO_DATA_CONTROL:
            /* Update the completion block */
            USBUART2_UpdateStatusBlock(USBUART2_XFER_ERROR);
            
            /* We expect no more data, so stall INs and OUTs */
            USBUART2_ep0Mode = USBUART2_MODE_STALL_IN_OUT;
            break;
            
        default:    
            /* There are no more states */
            break;
    }
}


/*******************************************************************************
* Function Name: USBUART2_LoadEP0
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
*  USBUART2_transferByteCount - Update the transfer byte count from the
*     last transaction.
*  USBUART2_ep0Count - counts the data loaded to the SIE memory in
*     current packet.
*  USBUART2_lastPacketSize - remembers the USBFS_ep0Count value for the
*     next packet.
*  USBUART2_transferByteCount - sum of the previous bytes transferred
*     on previous packets(sum of USBFS_lastPacketSize)
*  USBUART2_ep0Toggle - inverted
*  USBUART2_ep0Mode  - prepare for mode register content.
*  USBUART2_transferState - set to TRANS_STATE_CONTROL_READ
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBUART2_LoadEP0(void) 
{
    uint32 ep0Count = 0u;

    /* Update the transfer byte count from the last transaction */
    USBUART2_transferByteCount += USBUART2_lastPacketSize;

    /* Now load the next transaction */
    while ((USBUART2_currentTD.count > 0u) && (ep0Count < 8u))
    {
        USBUART2_EP0_DR_BASE.epData[ep0Count] = (uint32) *USBUART2_currentTD.pData;
        USBUART2_currentTD.pData = &USBUART2_currentTD.pData[1u];
        ep0Count++;
        USBUART2_currentTD.count--;
    }

    /* Support zero-length packet */
    if ((USBUART2_lastPacketSize == 8u) || (ep0Count > 0u))
    {
        /* Update the data toggle */
        USBUART2_ep0Toggle ^= USBUART2_EP0_CNT_DATA_TOGGLE;
        /* Set the Mode Register  */
        USBUART2_ep0Mode = USBUART2_MODE_ACK_IN_STATUS_OUT;
        /* Update the state (or stay the same) */
        USBUART2_transferState = USBUART2_TRANS_STATE_CONTROL_READ;
    }
    else
    {
        /* Expect Status Stage Out */
        USBUART2_ep0Mode = USBUART2_MODE_STATUS_OUT_ONLY;
        /* Update the state (or stay the same) */
        USBUART2_transferState = USBUART2_TRANS_STATE_CONTROL_READ;
    }

    /* Save the packet size for next time */
    USBUART2_ep0Count =       (uint8) ep0Count;
    USBUART2_lastPacketSize = (uint8) ep0Count;
}


/*******************************************************************************
* Function Name: USBUART2_InitControlRead
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
*  USBUART2_currentTD.count - counts of data to be sent.
*  USBUART2_currentTD.pData - data pointer.
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBUART2_InitControlRead(void) 
{
    uint16 xferCount;

    if (USBUART2_currentTD.count == 0u)
    {
        (void) USBUART2_InitZeroLengthControlTransfer();
    }
    else
    {
        /* Set up the state machine */
        USBUART2_transferState = USBUART2_TRANS_STATE_CONTROL_READ;
        
        /* Set the toggle, it gets updated in LoadEP */
        USBUART2_ep0Toggle = 0u;
        
        /* Initialize the Status Block */
        USBUART2_InitializeStatusBlock();
        
        xferCount = ((uint16)((uint16) USBUART2_lengthHiReg << 8u) | ((uint16) USBUART2_lengthLoReg));

        if (USBUART2_currentTD.count > xferCount)
        {
            USBUART2_currentTD.count = xferCount;
        }
        
        USBUART2_LoadEP0();
    }

    return (USBUART2_TRUE);
}


/*******************************************************************************
* Function Name: USBUART2_InitZeroLengthControlTransfer
****************************************************************************//**
*
*  Initialize a zero length data IN transfer.
*
* \return
*  requestHandled state.
*
* \globalvars
*  USBUART2_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USBUART2_ep0Mode  - prepare for mode register content.
*  USBUART2_transferState - set to TRANS_STATE_CONTROL_READ
*  USBUART2_ep0Count - cleared, means the zero-length packet.
*  USBUART2_lastPacketSize - cleared.
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBUART2_InitZeroLengthControlTransfer(void)
                                                
{
    /* Update the state */
    USBUART2_transferState = USBUART2_TRANS_STATE_CONTROL_READ;
    
    /* Set the data toggle */
    USBUART2_ep0Toggle = USBUART2_EP0_CNT_DATA_TOGGLE;
    
    /* Set the Mode Register  */
    USBUART2_ep0Mode = USBUART2_MODE_ACK_IN_STATUS_OUT;
    
    /* Save the packet size for next time */
    USBUART2_lastPacketSize = 0u;
    
    USBUART2_ep0Count = 0u;

    return (USBUART2_TRUE);
}


/*******************************************************************************
* Function Name: USBUART2_ControlReadDataStage
****************************************************************************//**
*
*  Handle the Data Stage of a control read transfer.
*
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBUART2_ControlReadDataStage(void) 

{
    USBUART2_LoadEP0();
}


/*******************************************************************************
* Function Name: USBUART2_ControlReadStatusStage
****************************************************************************//**
*
*  Handle the Status Stage of a control read transfer.
*
*
* \globalvars
*  USBUART2_USBFS_transferByteCount - updated with last packet size.
*  USBUART2_transferState - set to TRANS_STATE_IDLE.
*  USBUART2_ep0Mode  - set to MODE_STALL_IN_OUT.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBUART2_ControlReadStatusStage(void) 
{
    /* Update the transfer byte count */
    USBUART2_transferByteCount += USBUART2_lastPacketSize;
    
    /* Go Idle */
    USBUART2_transferState = USBUART2_TRANS_STATE_IDLE;
    
    /* Update the completion block */
    USBUART2_UpdateStatusBlock(USBUART2_XFER_STATUS_ACK);
    
    /* We expect no more data, so stall INs and OUTs */
    USBUART2_ep0Mode = USBUART2_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBUART2_InitControlWrite
****************************************************************************//**
*
*  Initialize a control write transaction
*
* \return
*  requestHandled state.
*
* \globalvars
*  USBUART2_USBFS_transferState - set to TRANS_STATE_CONTROL_WRITE
*  USBUART2_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USBUART2_ep0Mode  - set to MODE_ACK_OUT_STATUS_IN
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBUART2_InitControlWrite(void) 
{
    uint16 xferCount;

    /* Set up the state machine */
    USBUART2_transferState = USBUART2_TRANS_STATE_CONTROL_WRITE;
    
    /* This might not be necessary */
    USBUART2_ep0Toggle = USBUART2_EP0_CNT_DATA_TOGGLE;
    
    /* Initialize the Status Block */
    USBUART2_InitializeStatusBlock();

    xferCount = ((uint16)((uint16) USBUART2_lengthHiReg << 8u) | ((uint16) USBUART2_lengthLoReg));

    if (USBUART2_currentTD.count > xferCount)
    {
        USBUART2_currentTD.count = xferCount;
    }

    /* Expect Data or Status Stage */
    USBUART2_ep0Mode = USBUART2_MODE_ACK_OUT_STATUS_IN;

    return(USBUART2_TRUE);
}


/*******************************************************************************
* Function Name: USBUART2_ControlWriteDataStage
****************************************************************************//**
*
*  Handle the Data Stage of a control write transfer
*       1. Get the data (We assume the destination was validated previously)
*       2. Update the count and data toggle
*       3. Update the mode register for the next transaction
*
*
* \globalvars
*  USBUART2_transferByteCount - Update the transfer byte count from the
*    last transaction.
*  USBUART2_ep0Count - counts the data loaded from the SIE memory
*    in current packet.
*  USBUART2_transferByteCount - sum of the previous bytes transferred
*    on previous packets(sum of USBFS_lastPacketSize)
*  USBUART2_ep0Toggle - inverted
*  USBUART2_ep0Mode  - set to MODE_ACK_OUT_STATUS_IN.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBUART2_ControlWriteDataStage(void) 
{
    uint32 ep0Count;
    uint32 regIndex = 0u;

    ep0Count = (USBUART2_EP0_CNT_REG & USBUART2_EPX_CNT0_MASK) - USBUART2_EPX_CNTX_CRC_COUNT;

    USBUART2_transferByteCount += (uint8)ep0Count;

    while ((USBUART2_currentTD.count > 0u) && (ep0Count > 0u))
    {
        *USBUART2_currentTD.pData = (uint8) USBUART2_EP0_DR_BASE.epData[regIndex];
        USBUART2_currentTD.pData = &USBUART2_currentTD.pData[1u];
        regIndex++;
        ep0Count--;
        USBUART2_currentTD.count--;
    }
    
    USBUART2_ep0Count = (uint8)ep0Count;
    
    /* Update the data toggle */
    USBUART2_ep0Toggle ^= USBUART2_EP0_CNT_DATA_TOGGLE;
    
    /* Expect Data or Status Stage */
    USBUART2_ep0Mode = USBUART2_MODE_ACK_OUT_STATUS_IN;
}


/*******************************************************************************
* Function Name: USBUART2_ControlWriteStatusStage
****************************************************************************//**
*
*  Handle the Status Stage of a control write transfer
*
* \globalvars
*  USBUART2_transferState - set to TRANS_STATE_IDLE.
*  USBUART2_USBFS_ep0Mode  - set to MODE_STALL_IN_OUT.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBUART2_ControlWriteStatusStage(void) 
{
    /* Go Idle */
    USBUART2_transferState = USBUART2_TRANS_STATE_IDLE;
    
    /* Update the completion block */    
    USBUART2_UpdateStatusBlock(USBUART2_XFER_STATUS_ACK);
    
    /* We expect no more data, so stall INs and OUTs */
    USBUART2_ep0Mode = USBUART2_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBUART2_InitNoDataControlTransfer
****************************************************************************//**
*
*  Initialize a no data control transfer
*
* \return
*  requestHandled state.
*
* \globalvars
*  USBUART2_transferState - set to TRANS_STATE_NO_DATA_CONTROL.
*  USBUART2_ep0Mode  - set to MODE_STATUS_IN_ONLY.
*  USBUART2_ep0Count - cleared.
*  USBUART2_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBUART2_InitNoDataControlTransfer(void) 
{
    USBUART2_transferState = USBUART2_TRANS_STATE_NO_DATA_CONTROL;
    USBUART2_ep0Mode       = USBUART2_MODE_STATUS_IN_ONLY;
    USBUART2_ep0Toggle     = USBUART2_EP0_CNT_DATA_TOGGLE;
    USBUART2_ep0Count      = 0u;

    return (USBUART2_TRUE);
}


/*******************************************************************************
* Function Name: USBUART2_NoDataControlStatusStage
****************************************************************************//**
*  Handle the Status Stage of a no data control transfer.
*
*  SET_ADDRESS is special, since we need to receive the status stage with
*  the old address.
*
* \globalvars
*  USBUART2_transferState - set to TRANS_STATE_IDLE.
*  USBUART2_ep0Mode  - set to MODE_STALL_IN_OUT.
*  USBUART2_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USBUART2_deviceAddress - used to set new address and cleared
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBUART2_NoDataControlStatusStage(void) 
{
    if (0u != USBUART2_deviceAddress)
    {
        /* Update device address if we got new address. */
        USBUART2_CR0_REG = (uint32) USBUART2_deviceAddress | USBUART2_CR0_ENABLE;
        USBUART2_deviceAddress = 0u;
    }

    USBUART2_transferState = USBUART2_TRANS_STATE_IDLE;
    
    /* Update the completion block. */
    USBUART2_UpdateStatusBlock(USBUART2_XFER_STATUS_ACK);
    
    /* Stall IN and OUT, no more data is expected. */
    USBUART2_ep0Mode = USBUART2_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBUART2_UpdateStatusBlock
****************************************************************************//**
*
*  Update the Completion Status Block for a Request.  The block is updated
*  with the completion code the USBUART2_transferByteCount.  The
*  StatusBlock Pointer is set to NULL.
*
*  completionCode - status.
*
*
* \globalvars
*  USBUART2_currentTD.pStatusBlock->status - updated by the
*    completionCode parameter.
*  USBUART2_currentTD.pStatusBlock->length - updated.
*  USBUART2_currentTD.pStatusBlock - cleared.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBUART2_UpdateStatusBlock(uint8 completionCode) 
{
    if (USBUART2_currentTD.pStatusBlock != NULL)
    {
        USBUART2_currentTD.pStatusBlock->status = completionCode;
        USBUART2_currentTD.pStatusBlock->length = USBUART2_transferByteCount;
        USBUART2_currentTD.pStatusBlock = NULL;
    }
}


/*******************************************************************************
* Function Name: USBUART2_InitializeStatusBlock
****************************************************************************//**
*
*  Initialize the Completion Status Block for a Request.  The completion
*  code is set to USB_XFER_IDLE.
*
*  Also, initializes USBUART2_transferByteCount.  Save some space,
*  this is the only consumer.
*
* \globalvars
*  USBUART2_currentTD.pStatusBlock->status - set to XFER_IDLE.
*  USBUART2_currentTD.pStatusBlock->length - cleared.
*  USBUART2_transferByteCount - cleared.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBUART2_InitializeStatusBlock(void) 
{
    USBUART2_transferByteCount = 0u;
    
    if (USBUART2_currentTD.pStatusBlock != NULL)
    {
        USBUART2_currentTD.pStatusBlock->status = USBUART2_XFER_IDLE;
        USBUART2_currentTD.pStatusBlock->length = 0u;
    }
}


/* [] END OF FILE */
