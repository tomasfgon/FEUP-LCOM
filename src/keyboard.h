#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_
#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"
#include "i8254.h"

/** @defgroup Keyboard Keyboard.h
 * @{ Keyboard.h
 */

 /**
     * @brief holds keyboard scancode or breakcode value.
     */
extern uint32_t scancode;

/**
    * @brief subscribes kbc.
    */
int (kbc_subscribe_int)(uint8_t *bit_no);

/**
    * @brief unsubscribes kbc.
    * @return Returns 0 on success and 1 otherwise.
    */
int (kbc_unsubscribe_int)();

/**
    * @brief calls read scancode function.
    */
void (kbc_ih)();

/**
    * @brief Retrieves the scancode and verifies for errors.
    * @return Returns 0 on success and 1 otherwise.
    */
int kbc_read_scancode();

/**
    * @brief counts number of sys_inb calls.
    * @return Returns 0 on success and 1 otherwise.
    */
int counter_sysinb(port_t p, uint32_t *value);

/**
    * @brief Handles scancodes and saves it to the scancode variable.
    * @return Returns 0 on success and 1 otherwise.
    */
int scancode_handler();

/**
    * @brief Verifies for errors on input buffer
    * @return Returns 0 on success and 1 otherwise.
    */
int issue_cmd(uint8_t cmd);


/** @} end of Keyboard */
#endif /*_LCOM_KEYBOARD_H_*/
