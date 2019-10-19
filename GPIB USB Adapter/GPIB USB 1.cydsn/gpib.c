/****
 * YAUGI -- Yet Another USB GPIB Interface
 * 
 * Lee Essen, 2019
 *
 * This work is licensed under a Creative Commons 
 * Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 */

#include "gpib.h"
#include "settings.h"

/**
 * How long after we start to see a device do we consider it ok to talk to
 * (in ms)
 */
#define GPIB_INIT_TIME      3000

/**
 * The internal gpib buffer
 */
#define GPIB_BUF_SIZE       256
uint8_t gpib_buffer[GPIB_BUF_SIZE];

uint8_t *gpib_get_buffer() {
    return gpib_buffer;
}


/**
 * Definitions of the various lines (for bitbanging)
 */
typedef enum {
    ATN=0, SRQ, IFC, NDAC, NRFD, DAV, EOI, REN } gpib_line;

/**
 * My internal GPIB address (default controller = 21)
 */
uint8_t my_address = 21;

/**
 * Ensure the pin settings are correct for all of the pins. This should be
 * open drain, with a drive low. (This will be done by the design)
 *
 */
void gpib_init_pins() {
    
}

inline void gpib_settle() {
    CyDelayUs(2);
}

/**
 * We can detect if a device is not present since all the pins will be low, if
 * that is the case then we don't want to proceed. We'll just check the SIG pins
 * since all shouldn't be low in normal operation.
 *
 * TODO: we should only really do this check when we are not driving anything, otherwise
 * we might get strange results.
 */

int _gpib_mode = GPIB_NODEVICE;





/**
 * Assert means pulling a line low
 */
inline void gpib_assert_line(gpib_line line) {
    (*(reg32 *)SIG__DR_CLR) = (1 << line);
}

/**
 * Unasserting means letting it float high
 */
inline void gpib_unassert_line(gpib_line line) {
    (*(reg32 *)SIG__DR_SET) = (1 << line);
}

uint16_t gpib_readall() {
    uint16_t rc = ((*(reg32 *)SIG__PS) & 0xff) << 8;
    rc |= ((*(reg32 *)DIO__PS) & 0xff);
    return rc;
}


/**
 * Read the status of a line ... true is low! false is high!
 */
inline bool gpib_read_line(gpib_line line) {
    int v = ((*(reg32 *)SIG__PS) >> line) & 0x0001;
    return v == 0 ? true : false;
}


/**
 * Called periodically, will announce state changes, return value of 1 indicates we have
 * had a state change.
 *
 * We'll just keep an eye on IFC and ATN ... if either are low then (since we are the controller) this
 * must be a power fail/missing device.
 *
 * Once a device looks like it's back, we start a timer so we don't jump the gun
 */
int gpib_poll() {
    //uint8_t sig = (*(reg32 *)SIG__PS & 0xff);
    int old_state = _gpib_mode;
   
    bool atn = gpib_read_line(ATN);
    bool ifc = gpib_read_line(IFC);
    
    // Used to disable state checking (if a scope is on the lines!)
    //    atn = ifc = false;
    
    // If either ATN or IFC are zero, then we must be missing a device ;-)
    if (atn || ifc) {
        _gpib_mode = GPIB_NODEVICE;
    } else if (_gpib_mode == GPIB_NODEVICE) {
        // device has potentially come back ... zero the timer..
        Timer1_WriteCounter(0);
        _gpib_mode = GPIB_INIT;
    } else if (_gpib_mode == GPIB_INIT) {
        // after a period we need to switch
        if (Timer1_ReadCounter() > GPIB_INIT_TIME) {
            _gpib_mode = GPIB_RUNNING;
        }
    }    
    return (_gpib_mode != old_state ? 1 : 0);
}

int gpib_get_mode() {
    return _gpib_mode;
}

/**
 * Wait for a line to go low, if it doesn't within read_tmo_ms then timeout
 */
inline bool gpib_wait_for_line_to_assert(gpib_line line) {
    // Prepare the counter...
    Timer1_WriteCounter(0);
    
    while (1) {
        if (gpib_read_line(line) == true) {
            return true;
        }
        if (Timer1_ReadCounter() > settings.read_tmo_ms) {
            return false;
        }
    }
}

/**
 * Wait for a line to go high, if it doesn't within read_tmo_ms then timeout
 */
inline bool gpib_wait_for_line_to_unassert(gpib_line line) {
    // Prepare the counter...
    Timer1_WriteCounter(0);
 
    while (1) {
        if (gpib_read_line(line) == false) {
            return true;
        }
        if (Timer1_ReadCounter() > settings.read_tmo_ms) {
            return false;
        }
    }
}

/**
 * Transmit a byte of data on the bus, post this call nothing should be
 * asserted. An error returns false.
 */
bool gpib_send_byte(uint8_t byte, bool last) {
    bool ok;
    
    if (last) {
       gpib_assert_line(EOI);
    }
    
    // Wait for NRFD to be high...
    ok = gpib_wait_for_line_to_unassert(NRFD);
    if (!ok) goto cleanup;
    
    // Write data onto the bus... (reverse logic!)
    (*(reg32 *)DIO__DR) = ~byte;
    
    // Allow settling time...
    gpib_settle();     // we may need to adjust this later
    
    // Signal data is valie...
    gpib_assert_line(DAV);
    
    // Wait until everyone has received it...
    ok = gpib_wait_for_line_to_unassert(NDAC);

cleanup:    
    // Stop transmitting and clear the data
    gpib_unassert_line(DAV);
    (*(reg32 *)DIO__DR) = 0xff;

    // Unassert anyway...
    gpib_unassert_line(EOI);
    
    return ok;
 }

/**
 * Read a character from the GPIB bus, return 0 if we got a timeout
 * or 1 on success.
 */
int gpib_receive_byte(uint8_t *byte, int *eoi) {
    int ok;
    
    gpib_assert_line(NDAC);
    
    /*
    // Prepare to receive...
    gpib_assert_line(NRFD);
    
    // Why do they unassert ATN if it's set??
    */
    
    // Signal our readiness... (should alreadt be done)
    gpib_unassert_line(NRFD);
    
    
    // Wait for data to be available...
    ok = gpib_wait_for_line_to_assert(DAV);
    if (!ok) {
        gpib_unassert_line(NRFD);
        gpib_unassert_line(NDAC);
        return 0;
    }
    // Not ready for more data at this point
    gpib_assert_line(NRFD);
    
    // Read the data...
    *byte = ~(*(reg32 *)DIO__PS);
    *eoi = gpib_read_line(EOI);
    
    // If byte is NL (10 decimal) then it's the same as EOI
    if (*byte == 10) {
        *eoi = true;    // TODO
    }
    
    
    // We've read it...
    gpib_unassert_line(NDAC);
 //   gpib_settle();                  // why would we do this?
    
    // Wait for the talker to finish this byte
    ok = gpib_wait_for_line_to_unassert(DAV);    

//cleanup:
    gpib_assert_line(NDAC);
    gpib_unassert_line(NRFD);   // we are still ready for data
 
    //gpib_settle();
    
    return ok;
}

void gpib_interface_clear() {
    gpib_assert_line(IFC);
    gpib_settle();              // TODO ... proper delay
    gpib_unassert_line(IFC);
    gpib_settle();              // TODO ... proper delay
}

void gpib_LAD(uint8_t address) {
    gpib_send_byte(0x20 + address, false);
}

void gpib_TAD(uint8_t address) {
    gpib_send_byte(0x40 + address, false);
}

void gpib_UNL() {
    gpib_LAD(31);
}

void gpib_UNT() {
    gpib_TAD(31);
}

void gpib_address_listener(uint8_t address) {
    // Need to enable "REMOTE" so devices will enter remote mode
    // TODO: this should probably be an option??
    // TODO: do we need to undo this for a talker??
    
    gpib_unassert_line(NRFD);
    gpib_unassert_line(NDAC);
    
    gpib_assert_line(REN);
    gpib_settle();                  // do we need this?

    gpib_assert_line(ATN);
    gpib_settle();                  // do we need this?
    
    gpib_UNL();     // stop everyone listening
    gpib_UNT();     // stop everyone talking
    gpib_LAD(address);
    gpib_TAD(my_address);
    
    gpib_unassert_line(ATN);
    gpib_settle();              // do we need this?
}

void gpib_address_talker(uint8_t address) {
    // Make sure we are not asserting any handshake stuff...
    gpib_unassert_line(NRFD);
    gpib_unassert_line(NDAC);
 
    // 
    gpib_assert_line(ATN);
    gpib_settle();                  // do we need this?
    
    gpib_UNL();     // stop everyone listening
    gpib_UNT();     // stop everyone talking
    gpib_LAD(my_address);
    gpib_TAD(address);
    
    gpib_unassert_line(ATN);
    gpib_settle();              // do we need this?
    
    // Prepare to receive... should NDAC be earlier to avoid both high errors
    gpib_assert_line(NRFD);
    gpib_assert_line(NDAC);
    
    // Signal our readiness...
    gpib_unassert_line(NRFD);
}

/**
 * See if the talker has put some data on the bus...
 */
bool gpib_talking() {
    return gpib_read_line(DAV);
}

void gpib_send(uint8_t address, const char *buf) {
    char *p = (char *)buf;
    
    gpib_address_listener(address);
    while(*p) {
        gpib_send_byte(*p, (*(p+1) == 0));
        p++;
    }
}

/**
 * Read data from the currently addressed talker
 *
 * TODO: needs to return a buffer (from this file) with a length, but also
 *       and indication of whether we are done or not.
 *
 */
int gpib_read(uint8_t *buf, int maxsize) {
    int i = 0;
    bool ok;
    bool eoi;
    
    //gpib_address_talker(address);
    while(i < maxsize) {
        ok = gpib_receive_byte(buf+i, &eoi);
        if (!ok) {
            buf[i] = 0;
            return -1;
        }
        if (eoi) {
            buf[i++] = 0;
            return i;
        }
        i++;
    }
    return -1;   // TODO!!
}

/**
 * Read from the GPIB interface, look for EOI or the end character, we
 * return a buffer and the length used, and also a flag to say whether we
 * got an "end" status
 *
 * rc = <len> valid bytes
 * rc = -1 = error
 *
 * need a buffer, a length, and a return code
 *
 *
 *
 *
 * rc = length
 *  end = 0 (GPIB_NOT_ENDED)
 *  end = 0x100 (GPIB_EOI)
 *  end = (char) (end cause)
 *  end = -1 (ERROR)
 *  end = -2 (TIMEOUT)

 * timeout with len, ended with len, not ended

 *
 * timeout
 * ended
 * not ended
 *
 * 
 */

int gpib_read2(int until, int *end) {
    int         i = 0;
    int         ok;
    int         eoi;
    uint8_t     *buf;
    
    buf = gpib_buffer;
    
    while (i < (GPIB_BUF_SIZE-1)) {
        // First case is a potential timeout (no errors on read)
        if (!gpib_receive_byte(buf, &eoi)) {
            *end = GPIB_TIMEOUT;
            return i;
        }
               
        // We assume is a valid character...
        i++;

        // If we get an EOI then we might need to add an extra char and
        // potentially end this read
        if (eoi) {
            if (settings.eot_enable) {
                *++buf = settings.eot_char;
                i++;
            }
            if (until == GPIB_EOI) {
                *end = GPIB_ENDED;
                return i;
            }
        }        
        
        // If we are only waiting for timeout, then loop...
        if (until == GPIB_TIMEOUT) {    
            buf++;
            continue;
        }
        
        // If we match the given end char
        if (until == *buf) {
            *end = GPIB_ENDED;
            return i;
        }     
        buf++;
    }
    *end = GPIB_NOT_ENDED;
    return i;
}


/*
void gpib_test() {
    uint8_t buf[256];
 
    gpib_send(1, "*IDN?");
//    gpib_readfrom(1, buf, 256);
    CyDelay(500);
    
//    gpib_send(1, "DEEVBLOG@@@X");
//    CyDelay(2000);
}
*/



/* [] END OF FILE */
