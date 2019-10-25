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

// We only include this file if we have a UART device configured...
#if defined(USBUART_TRUE) || defined(USBCOMP_TRUE)

#include "serial.h"
#include "input.h"
#include "gpib.h"
#include "command.h"
#include "settings.h"
#include "utils.h"
    
#if defined(USBCOMP_TRUE)
    
// Map functions...
#define USBUART_CDCIsReady(args...)         USBCOMP_CDCIsReady(args)   
#define USBUART_PutData(args...)            USBCOMP_PutData(args)   
#define USBUART_DataIsReady(args...)        USBCOMP_DataIsReady(args)   
#define USBUART_GetAll(args...)             USBCOMP_GetAll(args)   
#define USBUART_CDC_Init(args...)           USBCOMP_CDC_Init(args) 

#endif 


#define MAX_BUF          64
#define MAX(a,b)        (a > b ? a : b)
#define MIN(a,b)        (a < b ? a : b)

uint8_t output_buffer[MAX_BUF];
int     output_free = MAX_BUF;

uint8_t input_buffer[MAX_BUF];
int     input_avail = 0;
uint8_t *input_p = input_buffer;

// Persist autoread from query to reply
int autoread = 0;    



#define GEN_BUF_SIZE          512
uint8_t general_buffer[GEN_BUF_SIZE];

void serial_printf(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf((char *)general_buffer, GEN_BUF_SIZE-1, fmt, args);
    serial_add_string((char *)general_buffer);
    serial_flush();
}
void serial_vprintf(char *fmt, va_list args) {
//    va_list args;
//    va_start(args, fmt);
    vsnprintf((char *)general_buffer, GEN_BUF_SIZE-1, fmt, args);
    serial_add_string((char *)general_buffer);
    serial_flush();
}




// TODO: some kind of serial_error("fmt", v, v, v); //



/**
 * Add some data to the serial output buffer, if we have enough to fill the
 * buffer then we will send, otherwise we'll save for the call to flush.
 * (this will automatically handle sending a zero if a full prior buffer
 * has been sent.)
 */
void serial_add(uint8_t *buf, int len) {
    
    while (len > 0) {
        int         copy_size = MIN(len, output_free);
        uint8_t     *out_dest = output_buffer + (MAX_BUF - output_free);
        
        // Copy the data and update the counters
        memcpy(out_dest, buf, copy_size);
        buf += copy_size;
        len -= copy_size;
        output_free -= copy_size;
            
        // If the buffer is full then we need to send it
        if (output_free == 0) {
            serial_flush();
        }
    }
}
void serial_add_string(char *string) {
    serial_add((uint8_t *)string, strlen(string));
}

/**
 * Flush whatever is in the output buffer, we'll do this even if there is nothing
 * since that likely means we've just sent a full buffer, so need to send a zero
 * length packet.
 */
void serial_flush() {
    while (USBUART_CDCIsReady() == 0) {}
    USBUART_PutData(output_buffer, MAX_BUF - output_free);    
    output_free = MAX_BUF;
}


bool serial_available() {
    if (input_avail > 0) {
        return true;
    }
    return (USBUART_DataIsReady() != 0);
}

uint8_t serial_getbyte() {
    // Needs to be a while, just in case we get a zero size input??
    while (input_avail == 0) {
        // Block for more data...
        while(USBUART_DataIsReady() == 0) {};
        input_avail = USBUART_GetAll(input_buffer);
        input_p = input_buffer;
    }
    
    input_avail--;
    return *input_p++;
}




/**
 * Handle USB configuration changes
 */
void usbuart_reconfig() {
    USBUART_CDC_Init();
}    

/**
 * Main poll function for uart support
 */
void usbuart_poll() {
    // Process all the incoming serial data...
    while (serial_available()) {
        // Do we have a full line of input?
        if (input_data() == 1) {
            uint8_t *buf = input_getbuffer();
            unsigned int len = input_getlength();
                    
            if (len == 0) {
                // Empty ... just reprompt
            } else if (len >= 2 && buf[0] == '+' && buf[1] == '+') {
                cmd_process(buf, len);
                        
                // TODO: if this was a ++auto command we need to change autoread????
            } else {
                
                // We've already stripped CR/NL from the input, so now we can just add
                // whatever is needed...
                switch (settings.eos) {
                    case 0:     // add CR + LF
                                buf[len++] = 13;
                                buf[len++] = 10;
                                break;
                                
                    case 1:     // add CR
                                buf[len++] = 13;
                                break;
                                
                    case 2:     // add LF
                                buf[len++] = 10;
                                break;
                                
                    case 3:     // do nothing
                                break;
                }               
                
                if (gpib_get_mode() == GPIB_RUNNING) {
                    //buf[len] = 0;
                    gpib_send(settings.address, buf, len);
                    
                    // Only auto-query if we should
                    autoread = 0;
                    if (settings.autoread) {
                        if (settings.autoread == 1 || (settings.autoread == 2 && contains_byte(buf, len, '?'))) {
                            gpib_address_talker(settings.address);
                            autoread = 1;
                        }
                    }                            
                } else {
                    serial_printf("<no GPIB device connected>\r\n");
                }
            }
            input_start();
        }    
    }
            
    if (autoread && gpib_get_mode() == GPIB_RUNNING && gpib_talking()) {                
        uint8_t *buf;
        int     len;
        int     ended;
        
        buf = gpib_get_buffer();
        
        do {
            len = gpib_read(GPIB_EOI, &ended);

            // Remove CR/LF's...
            len = chomp(buf, len);
                              
            input_show_output(buf, len);
            
        } while(ended == GPIB_NOT_ENDED);   
    }        
}


#endif

/* [] END OF FILE */
