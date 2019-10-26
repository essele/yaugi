/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "project.h"
#include <stdio.h>

// We only include this file if we have a TMC device configured...
#if defined(USBTMC_TRUE) || defined(USBCOMP_TRUE)

    
#if defined(USBCOMP_TRUE)
    
// Map functions...
#define USBTMC_Start(args...)               USBCOMP_Start(args)   
#define USBTMC_InitControlRead(args...)     USBCOMP_InitControlRead(args)   
#define USBTMC_GetEPState(args...)          USBCOMP_GetEPState(args)   
#define USBTMC_ReadOutEP(args...)           USBCOMP_ReadOutEP(args)   
#define USBTMC_LoadInEP(args...)            USBCOMP_LoadInEP(args) 
#define USBTMC_EnableOutEP(args...)         USBCOMP_EnableOutEP(args) 
    
// Defines...
#define USBTMC_bRequestReg                  USBCOMP_bRequestReg   
#define USBTMC_currentTD                    USBCOMP_currentTD  
#define USBTMC_FALSE                        USBCOMP_FALSE  
#define USBTMC_EP                           USBCOMP_EP  
#define USBTMC_EP1                          USBCOMP_EP1  
#define USBTMC_EP2                          USBCOMP_EP2  
#define USBTMC_OUT_BUFFER_FULL              USBCOMP_OUT_BUFFER_FULL  
#define USBTMC_IN_BUFFER_EMPTY              USBCOMP_IN_BUFFER_EMPTY  

#endif 
    
    
#include "usb_tmc.h"
#include "gpib.h" 

// Types of BULK message requests
#define DEV_DEP_MSG_OUT             1
#define REQUEST_DEV_DEP_MSG_IN      2
#define DEV_DEP_MSG_IN              2

// States for the sender/receive engine
#define WAITING                     0
#define FROM_HOST                   1
#define TO_HOST_FIRST               2
#define TO_HOST_CONT                3

// The main buffer we use for USB transfers
#define USB_BUF_SIZE                256
uint8_t     usb_buf[USB_BUF_SIZE];

// Helper macro...
#define MIN(a,b)                    (a < b ? a : b)

// Response for the class request GET_CAPABILITIES
struct get_cap_response {
    uint8   status;
    uint8   zero;
    uint16  bcdUSBTMC;
    uint8   intcap;
    uint8   devcap;
    uint8   zero6[6];
    uint8   zero12[12];   
};

struct get_cap_response gcr;



/**
 * Process class requests...
 */
uint8 USBTMC_DispatchClassRqst() {
    switch (USBTMC_bRequestReg) {
        case 3:         // INITIATE ABORT BULK IN
   //         break;
        
        case 4:         // CHECK ABORT BULK IN STATUS
    //        break;
        
        case 6:         // CHECK CLEAR STATUS
   //         break;

        case 7:      // GET CAPABILITIES
            memset(&gcr, 0, sizeof(struct get_cap_response));
            gcr.status = 1;
            gcr.bcdUSBTMC = 0x0100;
            gcr.intcap = 0;
            gcr.devcap = 1;
        
            USBTMC_currentTD.count = sizeof(struct get_cap_response);
            USBTMC_currentTD.pData = (uint8 *)&gcr;
            return USBTMC_InitControlRead();
            
        
        default:
            return USBTMC_FALSE;          
    }
}

struct dev_dep_msg_out {
    uint8_t     msg_id;
    uint8_t     bTag;
    uint8_t     bTagInverse;
    uint8_t     zero;
    uint32_t    transferSize;
    uint8_t     transferAttributes;
    uint8_t     zero1;
    uint8_t     zero2;
    uint8_t     zero3;
    uint8_t     data[0];
};

struct dev_dep_msg_in {
    uint8_t     msg_id;
    uint8_t     bTag;
    uint8_t     bTagInverse;
    uint8_t     zero;
    uint32_t    transferSize;
    uint8_t     transferAttributes;
    uint8_t     termChar;
    uint8_t     zero1;
    uint8_t     zero2;
    uint8_t     data[0];
};

// We need to keep state for multi-packet transfers...
uint8_t     tag = 0;                // for matching request to reply
uint32_t    size = 0;               // track how much still to send/receive
uint8_t     last = 0;               // does this contain the last char
uint8_t     *ptr;                   // for sending large buffers
uint8_t     state = WAITING;        // the state machine

// Hack for addressing talker - TODO: gpib.c should keep state
int ttt = 0;

/**
 * Process incoming data from USB...
 *
 * (call if WAITING or FROM_HOST)
 */
void process_incoming_usb() {
    uint8_t     *ptr;
    int         len;
    uint16_t    wMaxPacketSize = USBTMC_EP[USBTMC_EP2].bufferSize;
    
    
    struct dev_dep_msg_out  *mop = (struct dev_dep_msg_out *)usb_buf;
    
    while(USBTMC_GetEPState(USBTMC_EP2) == USBTMC_OUT_BUFFER_FULL) {
        // We have some data to process...
        uint16 usblen = USBTMC_ReadOutEP(USBTMC_EP2, usb_buf, wMaxPacketSize);
        if (usblen == 0) {
            return;
        }
        
        // Waiting means this will be a new transfer from the host...
        if (state == WAITING) {

            switch(mop->msg_id) {
                case DEV_DEP_MSG_OUT:
                    tag = mop->bTag;
                    size = mop->transferSize;
                    last = mop->transferAttributes;         // last when size = 0
                    state = FROM_HOST;                      // switch to read mode
                
                    // Work out how much data is in the packet...
                    ptr = mop->data;
                    len = MIN(usblen - sizeof(struct dev_dep_msg_out), size);
                
                    // Prepare to talk to GPIB... 
                    gpib_address_listener(1); // TODO
                    ttt = 0;
                    break;
                
                case REQUEST_DEV_DEP_MSG_IN:
                    tag = mop->bTag;
                    size = mop->transferSize;
                    // todo term char stuff
                    state = TO_HOST_FIRST;
                    if (ttt == 0) {
                        gpib_address_talker(1);
                        ttt = 1;
                    }
                    break;
                default:
                    // TODO: error condition
                    return;
                    break;
            }
        } else if (state == FROM_HOST) {
            // This is a continuation packet...
            ptr = usb_buf;
            len = MIN(usblen, size);
        }
              
        // So we now have some data to deal with... 
        if (state == FROM_HOST) {
            size -= len;
            
            int reallylast = last && (size == 0);
            
            gpib_send_bytes(ptr, len, reallylast);
            
            // Go back to waiting for as new incoming packet...
            if (reallylast) {
                tag = 0;
                state = WAITING;
            }
        }
    }
}

/**
 * Call when we need to be sending a reply back to the host, should only be
 * called when STATE == TO_HOST_[FIRST|CONT]
 */
void process_outgoing_usb() {
    uint16_t    wMaxPacketSize = USBTMC_EP[USBTMC_EP1].bufferSize;
    int         ended;


    // If this is the first call for this packet, we will need to read the data
    // and setup a full header based on the result.
    if (state == TO_HOST_FIRST) {
        // Work out how much space we have in this buffer (but can never be bigger than request size)
        struct dev_dep_msg_in *mip = (struct dev_dep_msg_in *)usb_buf;
        uint32_t maxdata = USB_BUF_SIZE - sizeof(struct dev_dep_msg_in);            
        maxdata = MIN(maxdata, size);

        // Read data from GPIB...
        int gpiblen = gpib_read_bytes(mip->data, maxdata, &ended);

        // Now create the msg_in structure, zero everything that should be zero
        mip->msg_id = DEV_DEP_MSG_IN;
        mip->bTag = tag;
        mip->bTagInverse = ~tag;
        mip->transferSize = gpiblen;
        mip->transferAttributes = (ended == GPIB_NOT_ENDED ? 0 : 1);
        mip->zero = 0;
        mip->zero1 = 0;
        mip->zero2 = 0;
        mip->termChar = 0;  // not used in the reply

        // Now we can use "size" to track the total amount to send (data+header)
        // this also pads to four bytes
        size = (gpiblen + sizeof(struct dev_dep_msg_in) + 3) & ~3;
        ptr = usb_buf;
        
        state = TO_HOST_CONT;
    }
    
    while (state == TO_HOST_CONT &&
        USBTMC_GetEPState(USBTMC_EP1) == USBTMC_IN_BUFFER_EMPTY) {
            
        // Work out the size of the packet...
        int len = MIN(size, wMaxPacketSize);

        // Send packet, and mover pointer along...
        USBTMC_LoadInEP(USBTMC_EP1, ptr, len);
        ptr += len;
        
        // If we have finished, we may need to send a zero size packet, so we only actually
        // end if this packet wasn't max length.
        size -= len;
        if (size == 0 && len != wMaxPacketSize) {
            tag = 0;
            state = WAITING;
        }        
    }
}

/**
 * Initialisation function for tmc support
 */
void usbtmc_init() {
}

/**
 * Handle USB configuration changes
 */
void usbtmc_reconfig() {
    USBTMC_EnableOutEP(USBTMC_EP2);
}    

/**
 * Main poll function for tmc support
 */
void usbtmc_poll() {
    process_incoming_usb();
    process_outgoing_usb();
}


#endif

/* [] END OF FILE */
