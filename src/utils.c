#include <lcom/lcf.h>

#include <stdint.h>

#include "i8042.h"


int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
    uint16_t temp = val & (0x00FF);
    *lsb = (uint8_t)temp;
    return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
    uint16_t temp = val & (0xFF00);
    temp = temp >> 8;
    *msb = (uint8_t)temp;
    return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  u32_t temp;

  if(sys_inb(port, &temp)!=0){
    printf("error in sys_inb");
  }
  *value = (uint8_t)temp;
  return 0;
}

phys_bytes far_ptr_to_linear(uint32_t far_ptr) {
	// já vem com shift de 4 para a direita (16 - 4 = 12)
	return (phys_bytes) (((far_ptr & 0xFFFF0000) >> 12) + (far_ptr & 0x0000FFFF));
}

void* linear_to_virt(uint32_t linear_ptr, mmap_t *map) {
	return (void*) (((uint32_t) map->virt) + (linear_ptr - map->phys));
}
