#pragma once

#include <lcom/lcf.h>

/** @defgroup RTC rtc.h
 * @{
 */

#define RTC_IRQ             8 		/**< @brief RTC IRQ */
#define RTC_REG             10		/**< @brief RTC register */
#define RTC_WAIT            244 	/**< @brief RTC Wait */
#define RTC_SECONDS         0		/**< @brief To read seconds */
#define RTC_MINUTES         2		/**< @brief To read minutes */	
#define RTC_HOURS           4		/**< @brief To read hours */
#define RTC_WRITE_REG       0x70 	/**< @brief RTC write register */
#define RTC_READ_REG        0x71	/**< @brief RTC read register */

/**
	* @brief Date struct
	*/
typedef struct {
	uint16_t seconds; /**< seconds Seconds */
	uint16_t minutes; /**< minutes Minutes */
	uint16_t hours; /**< hours Hours */
} date_struct;

/**
	* @brief Subscribes rtc interrupts.
    * @param bit_no Bit_no argument
	* @return Return 0 upon success and non-zero otherwise
	*/
int (rtc_subscribe_int)(uint8_t *bit_no);

/**
	* @brief Unsubscribes rtc interrupts.
	* @return Return 0 upon success and non-zero otherwise
	*/
int (rtc_unsubscribe_int)();

/**
	* @brief Gets current time in minutes, hours and seconds.
	* @return Return a struct with the informations
	*/
date_struct get_date();

/**
	* @brief Converts to a decimal number.
	* @param from What needs to be converted
	* @return Return the decimal number
	*/
int convert (uint16_t from);

/**
	* @brief Reads information from RTC.
	* @param toRead Write in RTC
	* @param read Read from RTC
	* @return Return 0 upon success and non-zero otherwise
	*/
int read_from_rtc(uint32_t toRead, uint32_t* read);

/**
	* @brief Converts read hour to be from 1 to 12.
	* @param hour Read hour
	* @return Return the new number
	*/
int treatHour(int hour);

/** @} end of rtc.h */
