/****
 * YAUGI -- Yet Another USB GPIB Interface
 * 
 * Lee Essen, 2019
 *
 * This work is licensed under a Creative Commons 
 * Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 */

#include "project.h"

struct settings {
    uint32_t    signature;
    uint16_t    version;
    uint16_t    address;            // target GPIB address
    uint16_t    autoread;           // automatic reading
    uint16_t    eoi;                // use EOI signal when sending
    uint16_t    eos;                // send terminators after each GPIB output
    uint16_t    eot_char;
    uint16_t    eot_enable;
    uint16_t    read_tmo_ms;        // read timeout in ms
};


extern struct settings settings;

int settings_init();

/* [] END OF FILE */
