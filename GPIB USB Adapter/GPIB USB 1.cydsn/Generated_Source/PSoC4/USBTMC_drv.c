/***************************************************************************//**
* \file USBTMC_drv.c
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

#include "USBTMC_pvt.h"
#include "cyapicallbacks.h"


/***************************************
* Global data allocation
***************************************/

volatile T_USBTMC_EP_CTL_BLOCK USBTMC_EP[USBTMC_MAX_EP];

/** Contains the current configuration number, which is set by the host using a 
 * SET_CONFIGURATION request. This variable is initialized to zero in 
 * USBFS_InitComponent() API and can be read by the USBFS_GetConfiguration() 
 * API.*/
volatile uint8 USBTMC_configuration;

/** Contains the current interface number.*/
volatile uint8 USBTMC_interfaceNumber;

/** This variable is set to one after SET_CONFIGURATION and SET_INTERFACE 
 *requests. It can be read by the USBFS_IsConfigurationChanged() API */
volatile uint8 USBTMC_configurationChanged;

/** Contains the current device address.*/
volatile uint8 USBTMC_deviceAddress;

/** This is a two-bit variable that contains power status in the bit 0 
 * (DEVICE_STATUS_BUS_POWERED or DEVICE_STATUS_SELF_POWERED) and remote wakeup 
 * status (DEVICE_STATUS_REMOTE_WAKEUP) in the bit 1. This variable is 
 * initialized to zero in USBFS_InitComponent() API, configured by the 
 * USBFS_SetPowerStatus() API. The remote wakeup status cannot be set using the 
 * API SetPowerStatus(). */
volatile uint8 USBTMC_deviceStatus;

volatile uint8 USBTMC_interfaceSetting[USBTMC_MAX_INTERFACES_NUMBER];
volatile uint8 USBTMC_interfaceSetting_last[USBTMC_MAX_INTERFACES_NUMBER];
volatile uint8 USBTMC_interfaceStatus[USBTMC_MAX_INTERFACES_NUMBER];

/** Contains the started device number. This variable is set by the 
 * USBFS_Start() or USBFS_InitComponent() APIs.*/
volatile uint8 USBTMC_device;

/** Initialized class array for each interface. It is used for handling Class 
 * specific requests depend on interface class. Different classes in multiple 
 * alternate settings are not supported.*/
const uint8 CYCODE *USBTMC_interfaceClass;


/***************************************
* Local data allocation
***************************************/

volatile uint8  USBTMC_ep0Toggle;
volatile uint8  USBTMC_lastPacketSize;

/** This variable is used by the communication functions to handle the current 
* transfer state.
* Initialized to TRANS_STATE_IDLE in the USBFS_InitComponent() API and after a 
* complete transfer in the status stage.
* Changed to the TRANS_STATE_CONTROL_READ or TRANS_STATE_CONTROL_WRITE in setup 
* transaction depending on the request type.
*/
volatile uint8  USBTMC_transferState;
volatile T_USBTMC_TD USBTMC_currentTD;
volatile uint8  USBTMC_ep0Mode;
volatile uint8  USBTMC_ep0Count;
volatile uint16 USBTMC_transferByteCount;


/*******************************************************************************
* Function Name: USBTMC_ep_0_Interrupt
****************************************************************************//**
*
*  This Interrupt Service Routine handles Endpoint 0 (Control Pipe) traffic.
*  It dispatches setup requests and handles the data and status stages.
*
*
*******************************************************************************/
CY_ISR(USBTMC_EP_0_ISR)
{
    uint32 tempReg;
    uint32 modifyReg;

#ifdef USBTMC_EP_0_ISR_ENTRY_CALLBACK
    USBTMC_EP_0_ISR_EntryCallback();
#endif /* (USBTMC_EP_0_ISR_ENTRY_CALLBACK) */
    
    tempReg = USBTMC_EP0_CR_REG;
    if ((tempReg & USBTMC_MODE_ACKD) != 0u)
    {
        modifyReg = 1u;
        if ((tempReg & USBTMC_MODE_SETUP_RCVD) != 0u)
        {
            if ((tempReg & USBTMC_MODE_MASK) != USBTMC_MODE_NAK_IN_OUT)
            {
                /* Mode not equal to NAK_IN_OUT: invalid setup */
                modifyReg = 0u;
            }
            else
            {
                USBTMC_HandleSetup();
                
                if ((USBTMC_ep0Mode & USBTMC_MODE_SETUP_RCVD) != 0u)
                {
                    /* SETUP bit set: exit without mode modificaiton */
                    modifyReg = 0u;
                }
            }
        }
        else if ((tempReg & USBTMC_MODE_IN_RCVD) != 0u)
        {
            USBTMC_HandleIN();
        }
        else if ((tempReg & USBTMC_MODE_OUT_RCVD) != 0u)
        {
            USBTMC_HandleOUT();
        }
        else
        {
            modifyReg = 0u;
        }
        
        /* Modify the EP0_CR register */
        if (modifyReg != 0u)
        {
            
            tempReg = USBTMC_EP0_CR_REG;
            
            /* Make sure that SETUP bit is cleared before modification */
            if ((tempReg & USBTMC_MODE_SETUP_RCVD) == 0u)
            {
                /* Update count register */
                tempReg = (uint32) USBTMC_ep0Toggle | USBTMC_ep0Count;
                USBTMC_EP0_CNT_REG = tempReg;
               
                /* Make sure that previous write operaiton was successful */
                if (tempReg == USBTMC_EP0_CNT_REG)
                {
                    /* Repeat until next successful write operation */
                    do
                    {
                        /* Init temporary variable */
                        modifyReg = USBTMC_ep0Mode;
                        
                        /* Unlock register */
                        tempReg = (uint8) (USBTMC_EP0_CR_REG & USBTMC_MODE_SETUP_RCVD);
                        
                        /* Check if SETUP bit is not set */
                        if (0u == tempReg)
                        {
                            /* Set the Mode Register  */
                            USBTMC_EP0_CR_REG = USBTMC_ep0Mode;
                            
                            /* Writing check */
                            modifyReg = USBTMC_EP0_CR_REG & USBTMC_MODE_MASK;
                        }
                    }
                    while (modifyReg != USBTMC_ep0Mode);
                }
            }
        }
    }

    USBTMC_ClearSieInterruptSource(USBTMC_INTR_SIE_EP0_INTR);
	
#ifdef USBTMC_EP_0_ISR_EXIT_CALLBACK
    USBTMC_EP_0_ISR_ExitCallback();
#endif /* (USBTMC_EP_0_ISR_EXIT_CALLBACK) */
}


/*******************************************************************************
* Function Name: USBTMC_HandleSetup
****************************************************************************//**
*
*  This Routine dispatches requests for the four USB request types
*
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_HandleSetup(void) 
{
    uint8 requestHandled;
    
    /* Clear register lock by SIE (read register) and clear setup bit 
    * (write any value in register).
    */
    requestHandled = (uint8) USBTMC_EP0_CR_REG;
    USBTMC_EP0_CR_REG = (uint32) requestHandled;
    requestHandled = (uint8) USBTMC_EP0_CR_REG;

    if ((requestHandled & USBTMC_MODE_SETUP_RCVD) != 0u)
    {
        /* SETUP bit is set: exit without mode modification. */
        USBTMC_ep0Mode = requestHandled;
    }
    else
    {
        /* In case the previous transfer did not complete, close it out */
        USBTMC_UpdateStatusBlock(USBTMC_XFER_PREMATURE);

        /* Check request type. */
        switch (USBTMC_bmRequestTypeReg & USBTMC_RQST_TYPE_MASK)
        {
            case USBTMC_RQST_TYPE_STD:
                requestHandled = USBTMC_HandleStandardRqst();
                break;
                
            case USBTMC_RQST_TYPE_CLS:
                requestHandled = USBTMC_DispatchClassRqst();
                break;
                
            case USBTMC_RQST_TYPE_VND:
                requestHandled = USBTMC_HandleVendorRqst();
                break;
                
            default:
                requestHandled = USBTMC_FALSE;
                break;
        }
        
        /* If request is not recognized. Stall endpoint 0 IN and OUT. */
        if (requestHandled == USBTMC_FALSE)
        {
            USBTMC_ep0Mode = USBTMC_MODE_STALL_IN_OUT;
        }
    }
}


/*******************************************************************************
* Function Name: USBTMC_HandleIN
****************************************************************************//**
*
*  This routine handles EP0 IN transfers.
*
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_HandleIN(void) 
{
    switch (USBTMC_transferState)
    {
        case USBTMC_TRANS_STATE_IDLE:
            break;
        
        case USBTMC_TRANS_STATE_CONTROL_READ:
            USBTMC_ControlReadDataStage();
            break;
            
        case USBTMC_TRANS_STATE_CONTROL_WRITE:
            USBTMC_ControlWriteStatusStage();
            break;
            
        case USBTMC_TRANS_STATE_NO_DATA_CONTROL:
            USBTMC_NoDataControlStatusStage();
            break;
            
        default:    /* there are no more states */
            break;
    }
}


/*******************************************************************************
* Function Name: USBTMC_HandleOUT
****************************************************************************//**
*
*  This routine handles EP0 OUT transfers.
*
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_HandleOUT(void) 
{
    switch (USBTMC_transferState)
    {
        case USBTMC_TRANS_STATE_IDLE:
            break;
        
        case USBTMC_TRANS_STATE_CONTROL_READ:
            USBTMC_ControlReadStatusStage();
            break;
            
        case USBTMC_TRANS_STATE_CONTROL_WRITE:
            USBTMC_ControlWriteDataStage();
            break;
            
        case USBTMC_TRANS_STATE_NO_DATA_CONTROL:
            /* Update the completion block */
            USBTMC_UpdateStatusBlock(USBTMC_XFER_ERROR);
            
            /* We expect no more data, so stall INs and OUTs */
            USBTMC_ep0Mode = USBTMC_MODE_STALL_IN_OUT;
            break;
            
        default:    
            /* There are no more states */
            break;
    }
}


/*******************************************************************************
* Function Name: USBTMC_LoadEP0
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
*  USBTMC_transferByteCount - Update the transfer byte count from the
*     last transaction.
*  USBTMC_ep0Count - counts the data loaded to the SIE memory in
*     current packet.
*  USBTMC_lastPacketSize - remembers the USBFS_ep0Count value for the
*     next packet.
*  USBTMC_transferByteCount - sum of the previous bytes transferred
*     on previous packets(sum of USBFS_lastPacketSize)
*  USBTMC_ep0Toggle - inverted
*  USBTMC_ep0Mode  - prepare for mode register content.
*  USBTMC_transferState - set to TRANS_STATE_CONTROL_READ
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_LoadEP0(void) 
{
    uint32 ep0Count = 0u;

    /* Update the transfer byte count from the last transaction */
    USBTMC_transferByteCount += USBTMC_lastPacketSize;

    /* Now load the next transaction */
    while ((USBTMC_currentTD.count > 0u) && (ep0Count < 8u))
    {
        USBTMC_EP0_DR_BASE.epData[ep0Count] = (uint32) *USBTMC_currentTD.pData;
        USBTMC_currentTD.pData = &USBTMC_currentTD.pData[1u];
        ep0Count++;
        USBTMC_currentTD.count--;
    }

    /* Support zero-length packet */
    if ((USBTMC_lastPacketSize == 8u) || (ep0Count > 0u))
    {
        /* Update the data toggle */
        USBTMC_ep0Toggle ^= USBTMC_EP0_CNT_DATA_TOGGLE;
        /* Set the Mode Register  */
        USBTMC_ep0Mode = USBTMC_MODE_ACK_IN_STATUS_OUT;
        /* Update the state (or stay the same) */
        USBTMC_transferState = USBTMC_TRANS_STATE_CONTROL_READ;
    }
    else
    {
        /* Expect Status Stage Out */
        USBTMC_ep0Mode = USBTMC_MODE_STATUS_OUT_ONLY;
        /* Update the state (or stay the same) */
        USBTMC_transferState = USBTMC_TRANS_STATE_CONTROL_READ;
    }

    /* Save the packet size for next time */
    USBTMC_ep0Count =       (uint8) ep0Count;
    USBTMC_lastPacketSize = (uint8) ep0Count;
}


/*******************************************************************************
* Function Name: USBTMC_InitControlRead
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
*  USBTMC_currentTD.count - counts of data to be sent.
*  USBTMC_currentTD.pData - data pointer.
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBTMC_InitControlRead(void) 
{
    uint16 xferCount;

    if (USBTMC_currentTD.count == 0u)
    {
        (void) USBTMC_InitZeroLengthControlTransfer();
    }
    else
    {
        /* Set up the state machine */
        USBTMC_transferState = USBTMC_TRANS_STATE_CONTROL_READ;
        
        /* Set the toggle, it gets updated in LoadEP */
        USBTMC_ep0Toggle = 0u;
        
        /* Initialize the Status Block */
        USBTMC_InitializeStatusBlock();
        
        xferCount = ((uint16)((uint16) USBTMC_lengthHiReg << 8u) | ((uint16) USBTMC_lengthLoReg));

        if (USBTMC_currentTD.count > xferCount)
        {
            USBTMC_currentTD.count = xferCount;
        }
        
        USBTMC_LoadEP0();
    }

    return (USBTMC_TRUE);
}


/*******************************************************************************
* Function Name: USBTMC_InitZeroLengthControlTransfer
****************************************************************************//**
*
*  Initialize a zero length data IN transfer.
*
* \return
*  requestHandled state.
*
* \globalvars
*  USBTMC_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USBTMC_ep0Mode  - prepare for mode register content.
*  USBTMC_transferState - set to TRANS_STATE_CONTROL_READ
*  USBTMC_ep0Count - cleared, means the zero-length packet.
*  USBTMC_lastPacketSize - cleared.
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBTMC_InitZeroLengthControlTransfer(void)
                                                
{
    /* Update the state */
    USBTMC_transferState = USBTMC_TRANS_STATE_CONTROL_READ;
    
    /* Set the data toggle */
    USBTMC_ep0Toggle = USBTMC_EP0_CNT_DATA_TOGGLE;
    
    /* Set the Mode Register  */
    USBTMC_ep0Mode = USBTMC_MODE_ACK_IN_STATUS_OUT;
    
    /* Save the packet size for next time */
    USBTMC_lastPacketSize = 0u;
    
    USBTMC_ep0Count = 0u;

    return (USBTMC_TRUE);
}


/*******************************************************************************
* Function Name: USBTMC_ControlReadDataStage
****************************************************************************//**
*
*  Handle the Data Stage of a control read transfer.
*
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_ControlReadDataStage(void) 

{
    USBTMC_LoadEP0();
}


/*******************************************************************************
* Function Name: USBTMC_ControlReadStatusStage
****************************************************************************//**
*
*  Handle the Status Stage of a control read transfer.
*
*
* \globalvars
*  USBTMC_USBFS_transferByteCount - updated with last packet size.
*  USBTMC_transferState - set to TRANS_STATE_IDLE.
*  USBTMC_ep0Mode  - set to MODE_STALL_IN_OUT.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_ControlReadStatusStage(void) 
{
    /* Update the transfer byte count */
    USBTMC_transferByteCount += USBTMC_lastPacketSize;
    
    /* Go Idle */
    USBTMC_transferState = USBTMC_TRANS_STATE_IDLE;
    
    /* Update the completion block */
    USBTMC_UpdateStatusBlock(USBTMC_XFER_STATUS_ACK);
    
    /* We expect no more data, so stall INs and OUTs */
    USBTMC_ep0Mode = USBTMC_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBTMC_InitControlWrite
****************************************************************************//**
*
*  Initialize a control write transaction
*
* \return
*  requestHandled state.
*
* \globalvars
*  USBTMC_USBFS_transferState - set to TRANS_STATE_CONTROL_WRITE
*  USBTMC_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USBTMC_ep0Mode  - set to MODE_ACK_OUT_STATUS_IN
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBTMC_InitControlWrite(void) 
{
    uint16 xferCount;

    /* Set up the state machine */
    USBTMC_transferState = USBTMC_TRANS_STATE_CONTROL_WRITE;
    
    /* This might not be necessary */
    USBTMC_ep0Toggle = USBTMC_EP0_CNT_DATA_TOGGLE;
    
    /* Initialize the Status Block */
    USBTMC_InitializeStatusBlock();

    xferCount = ((uint16)((uint16) USBTMC_lengthHiReg << 8u) | ((uint16) USBTMC_lengthLoReg));

    if (USBTMC_currentTD.count > xferCount)
    {
        USBTMC_currentTD.count = xferCount;
    }

    /* Expect Data or Status Stage */
    USBTMC_ep0Mode = USBTMC_MODE_ACK_OUT_STATUS_IN;

    return(USBTMC_TRUE);
}


/*******************************************************************************
* Function Name: USBTMC_ControlWriteDataStage
****************************************************************************//**
*
*  Handle the Data Stage of a control write transfer
*       1. Get the data (We assume the destination was validated previously)
*       2. Update the count and data toggle
*       3. Update the mode register for the next transaction
*
*
* \globalvars
*  USBTMC_transferByteCount - Update the transfer byte count from the
*    last transaction.
*  USBTMC_ep0Count - counts the data loaded from the SIE memory
*    in current packet.
*  USBTMC_transferByteCount - sum of the previous bytes transferred
*    on previous packets(sum of USBFS_lastPacketSize)
*  USBTMC_ep0Toggle - inverted
*  USBTMC_ep0Mode  - set to MODE_ACK_OUT_STATUS_IN.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_ControlWriteDataStage(void) 
{
    uint32 ep0Count;
    uint32 regIndex = 0u;

    ep0Count = (USBTMC_EP0_CNT_REG & USBTMC_EPX_CNT0_MASK) - USBTMC_EPX_CNTX_CRC_COUNT;

    USBTMC_transferByteCount += (uint8)ep0Count;

    while ((USBTMC_currentTD.count > 0u) && (ep0Count > 0u))
    {
        *USBTMC_currentTD.pData = (uint8) USBTMC_EP0_DR_BASE.epData[regIndex];
        USBTMC_currentTD.pData = &USBTMC_currentTD.pData[1u];
        regIndex++;
        ep0Count--;
        USBTMC_currentTD.count--;
    }
    
    USBTMC_ep0Count = (uint8)ep0Count;
    
    /* Update the data toggle */
    USBTMC_ep0Toggle ^= USBTMC_EP0_CNT_DATA_TOGGLE;
    
    /* Expect Data or Status Stage */
    USBTMC_ep0Mode = USBTMC_MODE_ACK_OUT_STATUS_IN;
}


/*******************************************************************************
* Function Name: USBTMC_ControlWriteStatusStage
****************************************************************************//**
*
*  Handle the Status Stage of a control write transfer
*
* \globalvars
*  USBTMC_transferState - set to TRANS_STATE_IDLE.
*  USBTMC_USBFS_ep0Mode  - set to MODE_STALL_IN_OUT.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_ControlWriteStatusStage(void) 
{
    /* Go Idle */
    USBTMC_transferState = USBTMC_TRANS_STATE_IDLE;
    
    /* Update the completion block */    
    USBTMC_UpdateStatusBlock(USBTMC_XFER_STATUS_ACK);
    
    /* We expect no more data, so stall INs and OUTs */
    USBTMC_ep0Mode = USBTMC_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBTMC_InitNoDataControlTransfer
****************************************************************************//**
*
*  Initialize a no data control transfer
*
* \return
*  requestHandled state.
*
* \globalvars
*  USBTMC_transferState - set to TRANS_STATE_NO_DATA_CONTROL.
*  USBTMC_ep0Mode  - set to MODE_STATUS_IN_ONLY.
*  USBTMC_ep0Count - cleared.
*  USBTMC_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 USBTMC_InitNoDataControlTransfer(void) 
{
    USBTMC_transferState = USBTMC_TRANS_STATE_NO_DATA_CONTROL;
    USBTMC_ep0Mode       = USBTMC_MODE_STATUS_IN_ONLY;
    USBTMC_ep0Toggle     = USBTMC_EP0_CNT_DATA_TOGGLE;
    USBTMC_ep0Count      = 0u;

    return (USBTMC_TRUE);
}


/*******************************************************************************
* Function Name: USBTMC_NoDataControlStatusStage
****************************************************************************//**
*  Handle the Status Stage of a no data control transfer.
*
*  SET_ADDRESS is special, since we need to receive the status stage with
*  the old address.
*
* \globalvars
*  USBTMC_transferState - set to TRANS_STATE_IDLE.
*  USBTMC_ep0Mode  - set to MODE_STALL_IN_OUT.
*  USBTMC_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USBTMC_deviceAddress - used to set new address and cleared
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_NoDataControlStatusStage(void) 
{
    if (0u != USBTMC_deviceAddress)
    {
        /* Update device address if we got new address. */
        USBTMC_CR0_REG = (uint32) USBTMC_deviceAddress | USBTMC_CR0_ENABLE;
        USBTMC_deviceAddress = 0u;
    }

    USBTMC_transferState = USBTMC_TRANS_STATE_IDLE;
    
    /* Update the completion block. */
    USBTMC_UpdateStatusBlock(USBTMC_XFER_STATUS_ACK);
    
    /* Stall IN and OUT, no more data is expected. */
    USBTMC_ep0Mode = USBTMC_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBTMC_UpdateStatusBlock
****************************************************************************//**
*
*  Update the Completion Status Block for a Request.  The block is updated
*  with the completion code the USBTMC_transferByteCount.  The
*  StatusBlock Pointer is set to NULL.
*
*  completionCode - status.
*
*
* \globalvars
*  USBTMC_currentTD.pStatusBlock->status - updated by the
*    completionCode parameter.
*  USBTMC_currentTD.pStatusBlock->length - updated.
*  USBTMC_currentTD.pStatusBlock - cleared.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_UpdateStatusBlock(uint8 completionCode) 
{
    if (USBTMC_currentTD.pStatusBlock != NULL)
    {
        USBTMC_currentTD.pStatusBlock->status = completionCode;
        USBTMC_currentTD.pStatusBlock->length = USBTMC_transferByteCount;
        USBTMC_currentTD.pStatusBlock = NULL;
    }
}


/*******************************************************************************
* Function Name: USBTMC_InitializeStatusBlock
****************************************************************************//**
*
*  Initialize the Completion Status Block for a Request.  The completion
*  code is set to USB_XFER_IDLE.
*
*  Also, initializes USBTMC_transferByteCount.  Save some space,
*  this is the only consumer.
*
* \globalvars
*  USBTMC_currentTD.pStatusBlock->status - set to XFER_IDLE.
*  USBTMC_currentTD.pStatusBlock->length - cleared.
*  USBTMC_transferByteCount - cleared.
*
* \reentrant
*  No.
*
*******************************************************************************/
void USBTMC_InitializeStatusBlock(void) 
{
    USBTMC_transferByteCount = 0u;
    
    if (USBTMC_currentTD.pStatusBlock != NULL)
    {
        USBTMC_currentTD.pStatusBlock->status = USBTMC_XFER_IDLE;
        USBTMC_currentTD.pStatusBlock->length = 0u;
    }
}


/* [] END OF FILE */
