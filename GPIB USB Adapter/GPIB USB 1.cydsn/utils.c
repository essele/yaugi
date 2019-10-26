/****
 * YAUGI -- Yet Another USB GPIB Interface
 * 
 * Lee Essen, 2019
 *
 * This work is licensed under a Creative Commons 
 * Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 */

#include "utils.h"

int contains_byte(uint8_t *buf, int len, uint8_t byte) {
    for (int i=0; i < len; i++) {
        if (*buf++ == byte) {
            return 1;
        }
    }
    return 0;
}

/**
 * Remove CR's and LF's from the buffer, return the new length
 */
int chomp(uint8_t *buf, int len) {
    uint8_t *s = buf;
    uint8_t *d = buf;
    int     newlen = 0;
    
    for (int i=0; i < len; i++) {
        if (*s == 10 || *s == 13) {
            s++;
            continue;
        }
        *d++ = *s++;
        newlen++;
    }
    return newlen;
}


/* [] END OF FILE */
