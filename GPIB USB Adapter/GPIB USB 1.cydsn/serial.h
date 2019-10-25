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
#include <stdbool.h>
#include <stdarg.h>

void serial_add(uint8_t *buf, int len);
void serial_add_string(char *string);
void serial_flush();

bool serial_available();
uint8_t serial_getbyte();

void serial_printf(char *fmt, ...);
void serial_vprintf(char *fmt, va_list args);

void usbuart_reconfig();
void usbuart_poll();

/* [] END OF FILE */
