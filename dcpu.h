#ifndef DCPU_H
#define DCPU_H

#include <stdint.h>
#include <stdlib.h>

/* put all forward-declaration here */

#define MAX_HARDWARE 0xFFFF

struct dcpu;
typedef struct dcpu dcpu16_t;

/* memory is 16 bit words */
typedef uint16_t  dcpu_reg_t;
typedef  int16_t  dcpu_reg_sign_t;

/* CPU states */
typedef enum {
    NORMAL, SKIPPING, HALTING, ON_FIRE
} dcpu_state_t;

extern const char *state_strs[];

/* makeup of an instruction */
typedef union {
    uint16_t all;
    struct {
        uint16_t o : 5;
        uint16_t b : 5;
        uint16_t a : 6;
    };
} dcpu_inst_t;

/* then include... */
#include "dcpu_ops.h"
#include "hardware_device.h"
#include "hardware/printer.h"
#include "hardware/cpucontrol.h"

/* cpu registers */
struct dcpu {
    /* Registers */
    uint16_t A, B, C, X, Y, Z, I, J;
    /* program counter */
    uint16_t PC;
    /* stack pointer */
    uint16_t SP;
    /* extra/excess */
    uint16_t EX;
    /* interrupt addr */
    uint16_t IA;

    /* memory */
    uint16_t *memory;

    /* hardware */
    dcpu_hardware_t **hardware;
    unsigned int hardware_count;

    /* extra stuff */
    unsigned long clocks;
    dcpu_state_t state;
    unsigned long frequency;

    // interrupts
    void (*interrupt)(struct dcpu *dcpu, dcpu_reg_t msg);
    uint8_t IAQ;
    uint16_t int_buffer[256];
    uint16_t  ib_start, ib_end, ib_size;

};

/* Create a new dcpu16 with the specified program memory */
void dcpu_create( dcpu16_t *dcpu, uint16_t *prog );

/* Attach the provided hardware device to the DCPU */
int dcpu_add_hardware( dcpu16_t *dcpu, dcpu_hardware_t *hardware );

/* cleanup after this dcpu16 */
void dcpu_free( dcpu16_t *dcpu );

/* Execute a clock tick */
int dcpu_tick( dcpu16_t *dcpu );

/* Returns zero when SUB PC,1 has not be encountered, nonzero otherwise */
int dcpu_complete( dcpu16_t *dcpu );

#endif
