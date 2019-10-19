/****
 * YAUGI -- Yet Another USB GPIB Interface
 * 
 * Lee Essen, 2019
 *
 * This work is licensed under a Creative Commons 
 * Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 */

#include "settings.h"

const uint8 eeprom[EEPROM_PHYSICAL_SIZE]   __ALIGNED(CY_FLASH_SIZEOF_ROW) = {0u}; 

#define SIGNATURE   0xBADDAD49
#define VERSION     1

// The main settings structure
struct settings settings;

/**
 * Set the default settings...
 */
void defaults() {
    settings.address = 1;
    settings.autoread = 2;
    settings.eoi = 1;
    settings.eos = 0;
    settings.eot_enable = 1;
    settings.eot_char = '*';
    settings.read_tmo_ms = 1000;
 }

/**
 * Read existing settings from the EEPROM, and setup some defaults if
 * we don't have anything sensible
 */
int settings_init() {
    cy_en_em_eeprom_status_t rc; 

    // Initialise the EEPROM
    rc = EEPROM_Init((uint32_t)&eeprom[0]);
    if (rc != CY_EM_EEPROM_SUCCESS) {
        return -1;
    }

    // Read the current values into settings...
    rc = EEPROM_Read(0, &settings, sizeof(struct settings));
    if (rc != CY_EM_EEPROM_SUCCESS) {
        //TODO: defaults
        defaults();
        return -1;
    }

    // See if we have some valid settings... if not create some...
    if (settings.signature != SIGNATURE) {
        settings.signature = SIGNATURE;
        settings.version = VERSION;

        defaults();    
    }
    // Force defaults for testing (REMOVE!)
    // REMOVE
    // REMOVE
    // REMOVE
    defaults();
    return 0;
}

int settings_save() {
    cy_en_em_eeprom_status_t rc; 

    rc = EEPROM_Write(0, &settings, sizeof(struct settings));
    if (rc != CY_EM_EEPROM_SUCCESS) {
        // TODO: error handling
        return -1;
    }
    return 0;
}

/* [] END OF FILE */
