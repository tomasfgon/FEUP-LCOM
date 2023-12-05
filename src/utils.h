#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"

/** @defgroup Utils Utils.h
 * @{ Utils.h
 */

extern unsigned long int no_of_calls;

/**
    * @brief Gets least significant byte from value and saves it to lsb variable.
    * @return Returns 0 on success and 1 otherwise.
    * @param val Original value.
    * @param lsb Variable to store.
    */
int (util_get_LSB) (uint16_t val, uint8_t *lsb);
/**
    * @brief Gets most significant byte from value and saves it to msb variable.
    * @return Returns 0 on success and 1 otherwise.
    * @param val Original value.
    * @param msb Variable to store.
    */
int (util_get_MSB) (uint16_t val, uint8_t *msb);
/**
    * @brief Allows sys_inb calls to uint8_t variables
    * @return Returns 0 on success and 1 otherwise.
    * @param port port.
    * @param value value to save in.
    */
int (util_sys_inb) (int port, uint8_t *value);

/**
    * @brief Sends command to port.
    * @return Returns 0 on success and 1 otherwise.
    * @param port port.
    * @param cmd command
    */
uint8_t (kbc_send_cmd) (uint8_t port, uint8_t cmd);
/**
    * @brief Reads output buffer
    * @return Returns 0 on success and 1 otherwise.
    * @param port port.
    * @param content saves content to this variable.
    * @param isMouse verifies if it is Mouse.
    */
uint8_t (kbc_read_outbf) (uint8_t port, uint8_t *content, bool isMouse);


uint8_t restore_kbc_byte();

phys_bytes far_ptr_to_linear(uint32_t far_ptr);

void* linear_to_virt(uint32_t linear_ptr, mmap_t *map);

/** @} end of Utils */
