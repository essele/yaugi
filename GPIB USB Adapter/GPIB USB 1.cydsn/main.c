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
#include <stdio.h>

#include "gpib.h"
#include "serial.h"
#include "input.h"
#include "command.h"
#include "settings.h"
#include "utils.h"
#include "usb_tmc.h"


#define USBFS_DEVICE    (0u)

#define GPIB_BUF_SIZE     (512)

uint8_t gpib_buf[GPIB_BUF_SIZE];

const uint8_t read_err[] = "<error reading from gpib>";
const unsigned int read_err_len = sizeof(read_err);


int main(void)
{
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    settings_init();
    // TODO: keep the eeprom state in memory so we can show it with a ++status type call
   
    PWM_1_Start();
    
//    Timer1_Init();
//    Timer1_Enable();
    // Free running counter...
    Timer1_Start();
    
    #ifdef USBTMC_TRUE
        #define USB_GetConfiguration(args...)           USBTMC_GetConfiguration(args)
        #define USB_IsConfigurationChanged(args...)     USBTMC_IsConfigurationChanged(args)
        #define USB_GetConfiguration(args...)           USBTMC_GetConfiguration(args)

        USBTMC_Start(USBFS_DEVICE, USBTMC_5V_OPERATION);
        USBTMC_SetPowerStatus(USBTMC_DEVICE_STATUS_BUS_POWERED );    
    #endif

    #ifdef USBUART_TRUE
        #define USB_GetConfiguration(args...)           USBUART_GetConfiguration(args)
        #define USB_IsConfigurationChanged(args...)     USBUART_IsConfigurationChanged(args)
        #define USB_GetConfiguration(args...)           USBUART_GetConfiguration(args)

        USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);
        USBUART_SetPowerStatus(USBUART_DEVICE_STATUS_BUS_POWERED );        
    #endif
    
    #ifdef USBCOMP_TRUE
        #define USB_GetConfiguration(args...)           USBCOMP_GetConfiguration(args)
        #define USB_IsConfigurationChanged(args...)     USBCOMP_IsConfigurationChanged(args)
        #define USB_GetConfiguration(args...)           USBCOMP_GetConfiguration(args)

        USBCOMP_Start(USBFS_DEVICE, USBCOMP_5V_OPERATION);
        USBCOMP_SetPowerStatus(USBCOMP_DEVICE_STATUS_BUS_POWERED );
    #endif
    

//    CyDelay(500);
    
    /* Wait until device is enumerated by host. */

    while (0u == USB_GetConfiguration()) {}
    
    
     /* Enable OUT endpoints to receive data from host. */
#if defined(USBTMC_TRUE) || defined(USBCOMP_TRUE)
    usbtmc_reconfig();
#endif 
#if defined(USBUART_TRUE) || defined(USBCOMP_TRUE)
    usbuart_reconfig();
#endif 
    
//    USBUART_EnableOutEP(USBUART_EP2);
    
 //   input_start();
    
    for(;;)
    {
        // Handle reconfiguration of the device...
        if (0u != USB_IsConfigurationChanged())
        {
            // Initialize IN endpoints when device is configured...
            if (0u != USB_GetConfiguration())
            {
#if defined(USBTMC_TRUE) || defined(USBCOMP_TRUE)
                usbtmc_reconfig();
#endif 
#if defined(USBUART_TRUE) || defined(USBCOMP_TRUE)
                usbuart_reconfig();
#endif                
            }
        }
        
        // Check for state changes with GPIB
        if (gpib_poll() == 1) {
            // TODO: make this user friendly (what does state mean)
//            serial_printf("<gpib state change: state is now %d\r\n", gpib_get_mode());
        }
        
        // Main processing (if we are configured
        if (USB_GetConfiguration() != 0) {

            // Run TMC related processing...
#if defined(USBTMC_TRUE) || defined(USBCOMP_TRUE)
            usbtmc_poll();
#endif
            // Run UART related processing...
#if defined(USBUART_TRUE) || defined(USBCOMP_TRUE)
            //process_incoming_usb();
            //process_outgoing_usb();
            usbuart_poll();
#endif
        }
    }

   /*     
               // Service USB CDC when device is configured. 
        if (0u != USBUART_GetConfiguration())
        {
            // See if we can read some data...

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
            
            // Check for state changes with GPIB
            if (gpib_poll() == 1) {
                // TODO: make this user friendly (what does state mean)
                serial_printf("<gpib state change: state is now %d\r\n", gpib_get_mode());
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
    }
    */
}
            



/* [] END OF FILE */
