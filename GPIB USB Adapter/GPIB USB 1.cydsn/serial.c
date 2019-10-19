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

#include "serial.h"
#include <stdio.h>

#define MAX_BUF          64
#define MAX(a,b)        (a > b ? a : b)
#define MIN(a,b)        (a < b ? a : b)

uint8_t output_buffer[MAX_BUF];
int     output_free = MAX_BUF;

uint8_t input_buffer[MAX_BUF];
int     input_avail = 0;
uint8_t *input_p = input_buffer;


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





/* [] END OF FILE */
