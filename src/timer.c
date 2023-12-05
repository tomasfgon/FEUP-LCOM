#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "timer.h"

static int timer0_hook_id;
unsigned long int global_timer0_counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  uint8_t st;

  if(timer_get_conf(timer, &st) != 0){
    printf("error in timer_get_conf");
    return 1;
  }

  st = st & (0x0F);

  uint8_t control;

  switch(timer){
    case 0:{
      control = (st | TIMER_SEL0 | TIMER_LSB_MSB);
      break;
    }
    case 1:{
      control = (st | TIMER_SEL1 | TIMER_LSB_MSB);
      break;
    }
    case 2:{
      control = (st | TIMER_SEL2 | TIMER_LSB_MSB);
      break;
    }
  }

  if(sys_outb(TIMER_CTRL, control) != 0){
    printf("error sys_outb");
    return 1;
  };

  uint16_t div = (TIMER_FREQ / freq);

  uint8_t div_lsb;
  uint8_t div_msb;

  if(util_get_LSB(div, &div_lsb) != 0){
    printf("error util_get_LSB");
    return 1;
  };

  if(util_get_MSB(div, &div_msb) != 0){
    printf("error util_get_MSB");
    return 1;
  };

  switch(timer){
    case 0:{
      if(sys_outb(TIMER_0, div_lsb) != 0){
      printf("error sys_outb");
      return 1;
      };
      if(sys_outb(TIMER_0, div_msb) != 0){
      printf("error sys_outb");
      return 1;
      };
      break;
    }
    case 1:{
      if(sys_outb(TIMER_1, div_lsb) != 0){
      printf("error sys_outb");
      return 1;
      };
      if(sys_outb(TIMER_1, div_msb) != 0){
      printf("error sys_outb");
      return 1;
      };
      break;
    }
    case 2:{
      if(sys_outb(TIMER_2, div_lsb) != 0){
      printf("error sys_outb");
      return 1;
      };
      if(sys_outb(TIMER_2, div_msb) != 0){
      printf("error sys_outb");
      return 1;
      };
      break;
    }
  }

  return 0;

}

int (timer_subscribe_int)(uint8_t *bit_no) {
    *bit_no = (uint8_t)timer0_hook_id;
    if(sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&timer0_hook_id)!=0){
        printf("error subscribing");
        return 1;
    }

    return 0;
}

int (timer_unsubscribe_int)() {
    if(sys_irqrmpolicy(&timer0_hook_id)!=0){
        printf("error unsubscribing");
        return 1;
    };
    return 0;
}

void (timer_int_handler)() {
    global_timer0_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  u32_t byte = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if(sys_outb(TIMER_CTRL, byte)!=0){
      printf("error sys_outb");
      return 1;
   }

   switch(timer){
   case 0:{
      if(util_sys_inb(TIMER_0, st)!=0){
      printf("error util_sys_inb");
      return 1;
      }
      break;
      }
      case 1:{
      if(util_sys_inb(TIMER_1, st)!=0){
          printf("error util_sys_inb");
          return 1;
      }
      break;
      }
      case 2:{
      if(util_sys_inb(TIMER_2, st)!=0){
          printf("error util_sys_inb");
          return 1;
      }
      break;
      }

      }

      return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val un;

  if(timer_get_conf(timer, &st)!=0){
  printf("erro timer_get_conf");
  return 1;
  }

  switch(field){
  case tsf_all:{
    un.byte = st;
    break;
  }
  case tsf_initial:{
    st = st & (BIT(5)|BIT(4));
    st = st>>4;
    un.in_mode = st;
    break;
  }
  case tsf_mode:{
  st = st & (BIT(3) | BIT(2) | BIT(1));
  st = st>>1;
    if(st==0x00){
    un.count_mode = 0;
    }
    else if(st==0x01){
    un.count_mode = 1;}
    else if((st == 0x02) || (st == 0x06)){
    un.count_mode = 2;}
    else if((st == 0x03) || (st == 0x07)){
    un.count_mode = 3;}
    else if(st == 0x04){
    un.count_mode = 4;}
    else if(st == 0x05){
    un.count_mode = 5;}
    break;
  }
  case tsf_base:{
    un.bcd = (st & BIT(0));
    break;
  }
  }

  if(timer_print_config(timer, field, un)!=0){
  printf("error timer_print_config");
  return 1;}
  return 0;
}
