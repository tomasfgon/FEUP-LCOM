#include "mouse.h"

int hook_id = MOUSE_IRQ;
uint8_t scancode_mouse;
uint8_t stat;

int (check_in_buff)() {
  uint8_t stat;

  if (util_sys_inb(KBC_REG, &stat) != 0) {return 1;}

  if (stat & IB_FULL)
    return 1;
  else
    return 0;
}

int (check_out_buff)() {
  uint8_t stat;

  if (util_sys_inb(KBC_REG, &stat) != 0) {
    printf("error in utils_sys_inb\n");
    return 1;
  }

  if (stat & OB_FULL) {
    if ((stat & (PAR_ERR | TO_ERR)) != 0)
      return 1;
    else
      return 0;
  }
  else
    return 1;

}

int (mouse_subscribe_int)(uint8_t *bit_no) {

    if(bit_no == NULL){
      printf("error in bit_no value");
      return 1;
    }

    *bit_no = (uint8_t)hook_id;

    if(sys_irqsetpolicy(MOUSE_IRQ,IRQ_REENABLE | IRQ_EXCLUSIVE,&hook_id)!=0){
        printf("error subscribing mouse");
        return 1;
    }

    return 0;
}

int (mouse_unsubscribe_int)() {

    if(sys_irqrmpolicy(&hook_id) != 0){
        printf("error unsubscribing keyboard");
        return 1;
    };
    return 0;
}

void (mouse_ih)(){

  while (1) {
    if (check_out_buff()==0) {
      if (util_sys_inb(OUT_BUF, &scancode_mouse) != 0) {
        printf("error in utils_sys_inb\n");
      }
      break;
    }
    else {
      break;
    }
  }
}

void (parse_packet)(struct packet *pp) {

  pp->lb = pp->bytes[0] & LB;

  pp->rb = pp->bytes[0] & RB;

  pp->mb = pp->bytes[0] & MB;

  if(pp->bytes[0] & X_DELTA)
    pp->delta_x = pp->bytes[1] - 256;
  else
    pp->delta_x = pp->bytes[1];

  if(pp->bytes[0] & Y_DELTA)
    pp->delta_y = pp->bytes[2] - 256;
  else
    pp->delta_y = pp->bytes[2];

  pp->x_ov = pp->bytes[0] & X_OVF;

  pp->y_ov = pp->bytes[0] & Y_OVF;

}

int (mouse_en_data_rep)() {
  uint8_t resp;
  uint8_t nack_count = 0;

  while(nack_count < 3) {

    if(sys_outb(KBC_REG, WRITE_TO_MOUSE)!=0){
      printf("error writing 0xD4");
      return 1;
    }

    if (mouse_write_cmd(ENA_DATA_REP, &resp) != 0) {
      printf("error disabling data report");
      return 1;
    }

    if (resp == ACK) {return 0;}
    else if (resp == NACK)
      nack_count++;
    else if (resp == ERROR)
      return 1;

  }

  return 0;
}


int (mouse_dis_data_rep)() {
  uint8_t resp;
  uint8_t nack_count = 0;

  while(nack_count < 3){

    if(sys_outb(KBC_REG, WRITE_TO_MOUSE)!=0){
      printf("error writing 0xD4");
      return 1;
    }

    if (mouse_write_cmd(DIS_DATA_REP, &resp) != 0) {
      printf("error disabling data report");
      return 1;
    }

    if (resp == ACK) {return 0;}
    else if (resp == NACK)
      nack_count++;
    else if (resp == ERROR)
      return 1;

  }

  return 0;
}

int(mouse_write_cmd)(uint32_t cmd, uint8_t *resp) {
  int counter = 0;

  while (counter< 3) {

    if (check_in_buff()==0) {

      if (sys_outb(OUT_BUF, cmd) != 0) {
        printf("\nerror in sys_outb\n");
        return 1;
      }

      if (util_sys_inb(OUT_BUF, resp) != 0) {
        printf("\nerror in utils_sys_inb\n");
        return 1;
      }

      return 0;
    }

    counter++;
  }
  return 1;
}
