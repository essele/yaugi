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
#include <stdbool.h>

#define GPIB_NODEVICE       0
#define GPIB_INIT           1
#define GPIB_RUNNING        2

// "end" codes for gpib_read 
#define GPIB_NOT_ENDED      0
#define GPIB_ENDED          1
#define GPIB_EOI            0x100
#define GPIB_TIMEOUT        0x101

// Return values for gpib_read (if not a length)
#define GPIB_ERROR          -1


int gpib_poll();
int gpib_get_mode();


void gpib_init_pins();

uint16_t gpib_readall();

//void gpib_send(uint8_t address, const char *buf);
void gpib_send(uint8_t address, const uint8_t *buf, int len);
int gpib_read(int until, int *end);

uint8_t *gpib_get_buffer();


bool gpib_talking();
void gpib_address_talker(uint8_t address);

void gpib_interface_clear();
void gpib_send_SDC();
void gpib_send_LLO();
void gpib_send_GTL();

/* [] END OF FILE */
