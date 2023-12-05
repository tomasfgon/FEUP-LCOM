
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8254.h"
#include "i8042.h"
#include "utils.h"

/** @defgroup Mouse mouse.h
 * @{
 */

/**
	* @brief Checks mouse input buffer.
	* @return Return 0 upon success and non-zero otherwise
	*/
int (check_in_buff)();

/**
	* @brief Checks mouse output buffer.
	* @return Return 0 upon success and non-zero otherwise
	*/
int (check_out_buff)();

/**
	* @brief Subscribes mouse interrupts.
    * @param bit_no Bit_no argument
	* @return Return 0 upon success and non-zero otherwise
	*/
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
	* @brief Unsubscribes mouse interrupts.
	* @return Return 0 upon success and non-zero otherwise
	*/
int (mouse_unsubscribe_int)();

/**
	* @brief Deals with mouse interrupts.
	*/
void (mouse_ih)();

/**
	* @brief Parses a mouse packet.
    * @param pp Mouse packet
	*/
void (parse_packet)(struct packet * pp);

/**
	* @brief Enables data reporting.
	* @return Return 0 upon success and non-zero otherwise
	*/
int (mouse_en_data_rep)();

/**
	* @brief Disables data reporting.
	* @return Return 0 upon success and non-zero otherwise
	*/
int (mouse_dis_data_rep)();

/**
	* @brief Write a command to the mouse.
    * @param cmd Command to write.
    * @param resp Buffer response
	* @return Return 0 upon success and non-zero otherwise
	*/
int(mouse_write_cmd)(uint32_t cmd, uint8_t *resp);

/** @} end of mouse.h */

