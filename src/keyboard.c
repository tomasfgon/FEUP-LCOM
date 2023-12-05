#include "keyboard.h"

static int hook_id = 1;
uint32_t temp;
uint32_t scancode;
uint32_t temp2;
int cnt_sysinb = 0;


int (kbc_subscribe_int)(uint8_t *bit_no) {

    if(bit_no == NULL){
      printf("error in bit_no value");
      return 1;
    }

    *bit_no = (uint8_t)hook_id;
    if(sys_irqsetpolicy(KBC_IRQ,IRQ_REENABLE | IRQ_EXCLUSIVE,&hook_id)!=0){
        printf("error subscribing keyboard");
        return 1;
    }

    return 0;

}

int (kbc_unsubscribe_int)() {

    if(sys_irqrmpolicy(&hook_id) != 0){
        printf("error unsubscribing keyboard");
        return 1;
    };
    return 0;
}

void (kbc_ih)() {
  
  if(kbc_read_scancode() != 0){
    printf("error in kbc_read_scancode");
  }
  
}

int kbc_read_scancode() {

  if(counter_sysinb(KBC_REG, &temp) != 0){
    printf("error reading from KBC status reg");
      return 1;
  }

  uint8_t errors = (PAR_ERR | TO_ERR);

  if((temp & errors) != 0){
      printf("par or to error \n");
      return 1;
  }
  if(((temp & AUX)!=0) || (temp & OB_FULL)==0){
      printf("aux or out_buf error \n");
      return 1;
  }


  if(counter_sysinb(OUT_BUF, &scancode) != 0){
      printf("error reading from KBC status reg \n");
      return 1;
    }

  return 0;

}

int counter_sysinb(port_t p, uint32_t *value){
    cnt_sysinb++;
    if(sys_inb(p,value)!=0){
        return 1;
    }
    return 0;
}

int scancode_handler(){
    uint8_t arr[2];
    bool make = true;
    if(scancode == 0xE0){

        arr[1] = scancode;
        if(counter_sysinb(OUT_BUF, &scancode) != 0){
            printf("error reading from KBC status reg \n");
            return 1;
        }
        arr[0] = scancode;

    }
    else{
        arr[1] = 0x00;
        arr[0] = scancode;
    }
    if(arr[0]>=0x80){
        make = false;
    }
    if(arr[1] == 0x00){
        uint8_t arr2[1];
        arr2[0] = arr[0];
    }
    else{
    }
    return 0;
}

int issue_cmd(uint8_t cmd){

    while(true){

        if(counter_sysinb(KBC_REG, &temp2)!=0){
            return 1;
        }
        if((temp2 & IB_FULL)!=0){
            return 1;
        }

        tickdelay(micros_to_ticks(DELAY_US));
    }
}
