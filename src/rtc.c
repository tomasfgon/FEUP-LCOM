#include "rtc.h"
#include "utils.h"


static int rtc_hook_id;

int (rtc_subscribe_int)(uint8_t *bit_no) {

    if(bit_no == NULL){
      printf("error in bit_no value");
      return 1;
    }

    *bit_no = (uint8_t)rtc_hook_id;

    if(sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE , &rtc_hook_id)!=0){
        printf("error subscribing mouse");
        return 1;
    }

    return 0;
}

int (rtc_unsubscribe_int)() {

	if(sys_irqrmpolicy(&rtc_hook_id) != 0){
        printf("error unsubscribing rtc");
        return 1;
    };
    return 0;
} 

date_struct get_date() {
	
    date_struct date;
	uint32_t info;

	read_from_rtc(RTC_REG, &info);

	if (info & RTC_WAIT)
		tickdelay(micros_to_ticks(RTC_WAIT)); 
	
	read_from_rtc(RTC_SECONDS, &info);
	date.seconds = convert(info);

	read_from_rtc(RTC_MINUTES, &info);
	date.minutes = convert(info);

	read_from_rtc(RTC_HOURS, &info);
	date.hours = convert(info);

	return date;
}


int convert (uint16_t from) {

	unsigned short a, b;
	a = (from >> 4);
	b = (from & 0x0f);

	if (!a) 
		return b;
	
	else if (a < 8) 
		return 10 * a + b;
	
	else if (a == 8) 
		return 12 + b;
	
	else 
		return 22 + b;
}

int read_from_rtc(uint32_t toRead, uint32_t* read) {

	if(sys_outb(RTC_WRITE_REG, toRead)) {
        printf("error in sys_outb\n");
    	return 1;
    }

  	if(sys_inb(RTC_READ_REG, read)) {
        printf("error in sys_inb\n");
    	return 1;
    }
  
  	return 0;
}

int treatHour(int hour){
	if(hour > 12)
		hour = hour - 12;

	return hour;
}

