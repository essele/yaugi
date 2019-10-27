/****
 * YAUGI -- Yet Another USB GPIB Interface
 * 
 * Lee Essen, 2019
 *
 * This work is licensed under a Creative Commons 
 * Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 */


#include "command.h"
#include "serial.h"
#include "settings.h"
#include "gpib.h"
#include "utils.h"
#include <stdio.h>

#define MAJOR_VERSION 1
#define MINOR_VERSION 2


#define MAX_TOKS 20

char    *toks[MAX_TOKS];
int     tok_count = 0;




/**
 * A set or view value of uint16_t
 */
struct uint_based {
    int         min;
    int         max;
    uint16_t    *set;
};

/**
 * A straight command that doesn't take any arguments
 */
struct no_args {
};

/**
 * If we have specific itemised options we can display the help and status
 * information
 */
struct item_meaning {
    int     value;
    char    *meaning;
};

struct cmd {
    char                        *name;
    int                         (*func)(char *argv[], int argc, const struct cmd *cmd);
    uint16_t                    *setting;
    uint16_t                    min;
    uint16_t                    max;
//
//    union {
//        struct uint_based       uint;
//        struct no_args          noargs;
//    } u;
    const char                  *help;
    const char                  **usage;
    const struct item_meaning   *meanings;
    int                         (*post_func)(char *argv[], int argc);
};


const struct cmd commands[];

/**
 * Split an input string on whitespace, null in between tokens and populate the list
 * (up to a max number of arguments)
 */
int tokenise(uint8_t *buf, int len) {
    int i = 0;
    
    tok_count = 0;
    
    while(i < len) {
        
        // First get past any leading whitespace...
        while(i < len) {
            if (strchr(" \t", *buf) == NULL) {
                break;
            }
            i++;
            buf++;
        }
        // Could be the end of the string?
        if (i == len) {
            return tok_count;
        }
        
        // Setup the token
        toks[tok_count] = (char *)buf;
        
        // Now get past any non-whitespace...
        while (i < len) {
            if (strchr(" \t", *buf) != NULL) {
                break;
            }
            i++;
            buf++;
        }
        // Null terminate the token
        *buf++ = 0;
        i++;
        tok_count++;
        
        if (tok_count == MAX_TOKS) {
            serial_printf("<error: too many tokens>\r\n");
            return 0;
        }
    }
    return tok_count;
}

/**
 * is_int() and to_int()
 */
int is_int(char *v) {
    while(*v) {
        if (strchr("0123456789", *v) == NULL) {
            return 0;
        }
        v++;
    }
    return 1;
}

int to_int(char *v) {
    int i = 0;
    while(*v) {
        i *= 10;
        i += (*v++ - '0');
    }
    return i;
}

/**
 * Take the command line and call the appropriate command function, we will be
 * called only when the first two characters are ++, so we can skip those here
 */
void cmd_process(uint8_t *buf, int len) {
    int argc = tokenise(buf, len);

    // We will always have the command a minimum, so return if 0
    if (argc == 0) {
        return;
    }
    
    // Find the command entry in the list...
    int i = 0;
    while (commands[i].name != NULL) {
        if (strcmp(toks[0], commands[i].name) == 0) {
            commands[i].func(toks, argc, &commands[i]);
            return;
        }
        i++;
    }
    serial_printf("<error: unknown ++ command>\r\n");
}



void cmd_usage(char *argv[], const struct cmd *cmd) {
    const char **p = cmd->usage;
    serial_printf("<usage: ++%s", argv[0]);
    if (**p) {
        serial_printf(" %s", *p++);
    }
    serial_printf(">\r\n");
    
    // First print any additional usage instructions...
    while(*p) {
        serial_printf("%s\r\n", (char *)*p++);
    }
    
    // Now handle specific value meanings if they are there
    if (cmd->meanings) {
        const struct item_meaning *m = cmd->meanings;
        while (m->meaning != NULL) {
            serial_printf("\t%d - %s\r\n", m->value, m->meaning);
            m++;
        }
    }
    
    // If it's a setting then we can show the current value
    if (cmd->setting) {
        uint16_t value = *cmd->setting;
        serial_printf("\t(current setting: %d)\r\n", value);
    }
   
    // Send it...
    serial_flush();
}

/**
 * Process a command that sets/gets an integer argument
 */
int cmd_uint(char *argv[], int argc, const struct cmd *cmd) {
    if (argc == 1) {
        // Get the value...
        serial_printf("%d\r\n", *(cmd->setting));
        return 0;
    }
    if (argc != 2 || !is_int(argv[1])) {
        cmd_usage(argv, cmd);
        return 1;
    }
    int byte = to_int(argv[1]);
    if (byte < cmd->min || byte > cmd->max) {
        cmd_usage(argv, cmd);
        return 1;
    }
    
    *(cmd->setting) = (uint16_t)byte;
    
    if (cmd->post_func != NULL) {
        return cmd->post_func(argv, argc);
    }
    return 0;
}

/**
 * A command that takes no arguments is really just a call to the helper function, but
 * we will check the syntax first
 */
int cmd_noargs(char *argv[], int argc, const struct cmd *cmd) {
    if (argc != 1) {
        cmd_usage(argv, cmd);
        return 1;
    }
    if (cmd->post_func != NULL) {
        return cmd->post_func(argv, argc);
    }
    return 0;    
}

/**
 * The help command can take zero or one arguments
 */
int cmd_help(char *argv[], int argc, const struct cmd *cmd) {
    // Did we even get the help command wrong?
    if (argc > 2) {
        cmd_usage(argv, cmd);
        return 1;
    }
    // No args, means just show the list...
    if (argc == 1) {
        int i = 0;
        while (commands[i].name != NULL) {
            serial_printf("++%-12.12s\t%s\r\n", commands[i].name, commands[i].help);
            i++;
        }
        return 0;
    }
    // One arg, means show usage of the given command
    int i = 0;
    while (commands[i].name != NULL) {
        if (strcmp(argv[1], commands[i].name) == 0) {
            cmd_usage(&argv[1], &commands[i]);
            return 0;
        }
        i++;
    }
    serial_printf("<error: unknown command: %s>\r\n", argv[1]);
    return 0;
}

/**
 * Read from the GPIB bus
 */
int cmd_read(char *argv[], int argc, const struct cmd *cmd) {
    int until = GPIB_TIMEOUT;
//    uint8_t *buf = gpib_get_buffer();
//    int     len;
    uint8_t buf[16];
    int     ended;
    
    if (argc > 2) {
        cmd_usage(argv, cmd);
        return 1;
    }
    if (argc == 2) {
        if (strcmp(argv[1], "eoi") == 0) {
            until = GPIB_EOI;
        } else if (!is_int(argv[1])) {
            cmd_usage(argv, cmd);
            return 1;
        } else {
            int ch = to_int(argv[1]);
            if (ch < 0 || ch > 255) {
                cmd_usage(argv, cmd);
                return 1;
            }
            until = ch;
        }
    }
    
    gpib_address_talker(settings.address);
    
    do {
        int len = gpib_read_bytes(buf, 16, &ended);
        if (settings.interact) {
            len = chomp(buf, len);   
        }
        serial_add(buf, len);
    } while (ended == GPIB_NOT_ENDED);
    if (settings.interact) {
        serial_add_string("\r\n");
    }
    serial_flush();
    return 0;
}

/**
 * List out all items of settings that can be set by commands
 */
int cmd_config(char *argv[], int argc, const struct cmd *cmd) {
    if (argc != 1) {
        cmd_usage(argv, cmd);
        return 1;
    }
    // Now look through the commands, finding ones referencing the config
    int i = 0;
    while (commands[i].name != NULL) {
        if (commands[i].setting != NULL) {
            uint16_t value = *(commands[i].setting);
            char    *meaning = NULL;
            
            if (commands[i].meanings != NULL) {
                const struct item_meaning *m = commands[i].meanings;
                while (m->meaning != NULL) {
                    if (m->value == value) {
                        meaning = m->meaning;
                        break;
                    }
                    m++;
                }
            }
            serial_printf("%-10.10s = %d", commands[i].name, value);
            if (meaning) {
                serial_printf(" (%s)", meaning);
            }
            serial_printf("\r\n");
        }
        i++;
    }   
    return 0;
}


int helper_version() {
    serial_printf("yaugi %d.%d by Lee Essen\r\n", MAJOR_VERSION, MINOR_VERSION);
    return 0;
}
 

int helper_gpiball() {
    uint16_t v = gpib_readall();
    serial_printf("pin status = %04x\r\n", v);
    return 0;
}

/**
 * Send the selected device clear command (needs to be tested)
 */
int helper_clr() {
    gpib_send_SDC();
    return 0;
}
int helper_ifc() {
    gpib_interface_clear();
    return 0;
}
int helper_llo() {
    gpib_send_LLO();
    return 0;
}
int helper_loc() {
    gpib_send_GTL();
    return 0;
}
int helper_spoll() {
    uint8_t v = gpib_serial_poll();
    serial_printf("got poll=%d\r\n", v);
    return 0;
}



/**
 *
 */

/**
 * Supported list of ++ commands
 */




const char *no_usage_args[] = { "", NULL };

#define no_item_meanings    NULL
#define no_setting          NULL, 0, 0
#define no_helper           NULL

const struct item_meaning auto_means[] = {
    { 0, "read-after-write disabled" },
    { 1, "read-after-write always" },
    { 2, "read-after-write if write contained ?" },
    { 0, NULL }
};

const struct item_meaning enable_disable_means[] = {
    { 0, "disabled" },
    { 1, "enabled" },
    { 0, NULL },
};

const struct item_meaning eos_means[] = {
    { 0, "append CR+LF to GPIB output" },
    { 1, "append CR to GPIB output" },
    { 2, "append LF to GPIB output" },
    { 3, "do not append to GPIB output" },
    { 0, NULL }
};


const char *enable_disable_usage[] = {
    "[0|1]",
    NULL
};


const struct cmd commands[] = {
    { "addr", cmd_uint,
        &settings.address, 0, 30,
        "Configure or query the GPIB device address",
        (const char *[]) { 
            "[PAD]", 
            "\twhere PAD is between 0 and 30", 
            NULL 
        },
        no_item_meanings,
        no_helper,
    },
    { "auto", cmd_uint,
        &settings.autoread, 0, 2,
        "Configure automatic read after write",
        (const char *[]){
            "[0|1|2]",
            NULL
        },
        auto_means,
        no_helper,
    },
    { "clr", cmd_noargs,
        no_setting,
        "Send a selected device clear (SDC) message",
        no_usage_args,
        no_item_meanings,
        helper_clr
    },
    { "eoi", cmd_uint,
        &settings.autoread, 0, 1,
        "Assertion of EOI line with the last transmitted character",
        enable_disable_usage,
        enable_disable_means,
        no_helper,
    },
    { "eos", cmd_uint,
        &settings.eos, 0, 3,
        "Append specific terminator to GPIB output",
        (const char *[]){ "[0|1|2|3]", NULL },
        eos_means,
        no_helper,
    },    
    { "eot_char", cmd_uint,
        &settings.eot_char, 0, 255,
        "Character to add for EOI if eot_enable",
        (const char *[]) { 
            "[char]", 
            "\twhere char is between 0 and 255", 
            NULL 
        },
        no_item_meanings,
        no_helper,
    },
    { "eot_enable", cmd_uint,
        &settings.eot_enable, 0, 1,
        "Append eot_char when EOI detected",
        enable_disable_usage,
        enable_disable_means,
        no_helper,
    },

    { "help", cmd_help,
        no_setting,
        "Display command list",
        (const char *[]){
            "[command]",
            NULL
        },
        no_item_meanings,
        no_helper,
    },
    { "ifc", cmd_noargs,
        no_setting,
        "Send an interface clear (IFC) message",
        no_usage_args,
        no_item_meanings,
        helper_ifc
    },
    { "interact", cmd_uint,
        &settings.interact, 0, 1,
        "Use interactive input mode",
        enable_disable_usage,
        enable_disable_means,
        no_helper,
    },
    { "llo", cmd_noargs,
        no_setting,
        "Send a local lock out (LLO) message",
        no_usage_args,
        no_item_meanings,
        helper_llo
    },
    { "loc", cmd_noargs,
        no_setting,
        "Send a go to local (GTL) message",
        no_usage_args,
        no_item_meanings,
        helper_loc
    },    
    { "read", cmd_read,
        no_setting,
        "Read from the device",
        (const char *[]) {
            "[EOI | char]",
            "\twhere char is between 0 and 255",
            NULL
        },
        no_item_meanings,
        no_helper
    },
    { "read_tmo_ms", cmd_uint,
        &settings.read_tmo_ms, 1, 3000,
        "Read timeout value in ms",
        (const char *[]) { 
            "[time]", 
            "\twhere time is between 1 and 3000", 
            NULL 
        },
        no_item_meanings,
        no_helper,
    },
    { "spoll", cmd_noargs,
        no_setting,
        "Send a serial poll request to the current address",
        no_usage_args,
        no_item_meanings,
        helper_spoll
    },
    { "ver", cmd_noargs,
        no_setting,
        "Return the version of the yaugi firmware",
        no_usage_args,
        no_item_meanings,
        helper_version
    },
    { "config", cmd_config,
        no_setting,
        "Show current configuration settings",
        no_usage_args,
        no_item_meanings,
        no_helper
    },
    
    { "xx", cmd_noargs,
        no_setting,
        "Read all pins",
        no_usage_args,
        no_item_meanings,
        helper_gpiball
    },
  
    // End of list
    { NULL, NULL, no_setting, NULL, no_usage_args, no_item_meanings, no_helper },
};


/* [] END OF FILE */
