#pragma once

#include <lcom/lcf.h>

void privctl();
int vg_draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y, void* buffer);
uint8_t vg_update_xpm(xpm_map_t xpm, uint16_t old_x, uint16_t old_y, uint16_t new_x, uint16_t new_y, void* buffer);
//int (vg_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step);
void vg_fill_black(void* buffer);
int (vg_draw_hline_proj)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, void* buffer);
int (vg_draw_rectangle_proj)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, void* buffer);
void free_video_mem();
