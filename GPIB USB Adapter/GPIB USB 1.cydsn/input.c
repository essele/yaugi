/****
 * YAUGI -- Yet Another USB GPIB Interface
 * 
 * Lee Essen, 2019
 *
 * This work is licensed under a Creative Commons 
 * Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 */


#include "input.h"
#include "serial.h"
#include <stdarg.h>
#include <stdio.h>


#define INPUT_BUF_SIZE 128
#define PROMPT "yaugi> "

uint8_t last_buf[INPUT_BUF_SIZE];           // keep the last command for recall
int     last_buf_len = 0;

uint8_t input_buf[INPUT_BUF_SIZE];
int     input_buf_len = 0;
int     input_cursor_pos = 0;

uint8_t esc_buf[5];
int     escape = 0;

#define ESC_UP      0x1001
#define ESC_DOWN    0x1002
#define ESC_LEFT    0x1003
#define ESC_RIGHT   0x1004

struct escape_sequence {
    char *seq;
    int  val;
};

const struct escape_sequence escapes[] = {
    { "\x1b[A", ESC_UP },
    { "\x1b[B", ESC_DOWN },
    { "\x1b[C", ESC_LEFT },
    { "\x1b[D", ESC_RIGHT },
    { NULL, 0 }
};

/**
 * See if the current escape sequence is valid, and potentially complete
 * we return 0 for valid but not complete, -1 for invalid, or the value
 * if valid.
 */

int valid_escape(uint8_t *buf, unsigned int len) {
    unsigned int i = 0;
    
    while(escapes[i].val != 0) {
        if (memcmp(escapes[i].seq, buf, len) == 0) {
            if (strlen(escapes[i].seq) == len) {
                return escapes[i].val;
            } else {
                return 0;
            }
        }
        i++;
    }
    // No match at all
    return -1;
}

/**
 * Called when we need to start the input of a new line, just cleans up
 * and shows the prompt
 */
void input_start() {
    input_buf[0] = 0;
    input_buf_len = 0;
    serial_add_string(PROMPT);
    serial_flush();
}

/**
 * Allow access to the input buffer
 */
uint8_t *input_getbuffer() {
    return input_buf;
}

unsigned int input_getlength() {
    return input_buf_len;
}

/**
 * Handles incoming serial data, processes interactive stuff and then returns
 * 1 when we are ready with the line, or 0 otherwise.
 */
int input_data() {
    int c = serial_getbyte();
    
    // First check if we are in the middle of an escape sequence...
    if (escape) {
        esc_buf[escape++] = c;
        
        c = valid_escape(esc_buf, escape);
        if (c == -1) {
            // Not valid... we'll translate into a '?'
            c = '?';
            escape = 0;
        } else if (c != 0) {
            // Must be a complete one...
            escape = 0;
        } else {
            // Partial match ... do nothing more
            return 0;
        }
    }
    
    // Now we can process incoming keys/data...
    switch(c) {
        case 0x1b:      // Escape
                        esc_buf[0] = 0x1b;
                        escape = 1;
                        break;
                        
        case 0x7f:      // Backspace
                        if (input_buf_len > 0) {
                            input_buf_len--;
                            serial_add_string("\x7f");
                            serial_flush();
                        }
                        break;

        case 0x08:      // Delete
                        if (input_buf_len > 0) {
                            input_buf_len--;
                            serial_add_string("\x08 \x08");
                            serial_flush();
                        }
                        break;
                        
        case '\n':      // Newline, just ignore
                        break;
                        
        case '\r':      // Carriage return, end of line...
                        input_buf[input_buf_len] = 0;
                        serial_add_string("\r\n");
                        serial_flush();
                        
                        // Keep the last command (if there was one)
                        if (input_buf_len > 0) {
                            memcpy(last_buf, input_buf, input_buf_len);
                            last_buf_len = input_buf_len;
                        }
                        return 1;
                        break;
                        
        case ESC_UP:    // Recall the last buffer
                        memcpy(input_buf, last_buf, last_buf_len);
                        input_buf_len = last_buf_len;
                        
                        // Clear the line, redraw the prompt and the line
                        serial_add_string("\r\x1b[K");
                        serial_add_string(PROMPT);
                        serial_add(input_buf, input_buf_len);
                        serial_flush();
                        break;
                        
        case ESC_DOWN:
        case ESC_LEFT:
        case ESC_RIGHT:
                        break;
    
        default:        // Normal character
                        input_buf[input_buf_len++] = c;
                        serial_add((uint8_t *)&c, 1);
                        serial_flush();
                        break;

    }
    return 0;
}

/**
 * If we need to show some output then we need to clear the line, send the output and
 * then recreate the state on the following line.
 *
 * Two ways of doing this...
 */
void input_printf(char *fmt, ...) {
    
    va_list args;
    va_start(args, fmt);

    serial_add_string("\r\x1b[K");
    serial_vprintf(fmt, args);
    serial_add_string("\r\n");
    serial_add_string(PROMPT);
    serial_add(input_buf, input_buf_len);
    serial_flush();
    serial_flush();
}

void input_show_output(uint8_t *output, int len) {
    serial_add_string("\r\x1b[K");
    serial_add(output, len);
    serial_add_string("\r\n");
    serial_add_string(PROMPT);
    serial_add(input_buf, input_buf_len);
    serial_flush();
}

/* [] END OF FILE */
