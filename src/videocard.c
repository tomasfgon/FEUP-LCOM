#include "videocard.h"

void* buffer_base = NULL;
uint8_t* video_mem = NULL;

uint16_t x_res, y_res;
uint8_t bits_per_pixel, bytes_per_pixel, vg_mode;
uint32_t red_mask, green_mask, blue_mask;
uint8_t red_field_position, green_field_position, blue_field_position;
uint8_t red_mask_size, green_mask_size, blue_mask_size;

void privctl(phys_bytes mr_base, size_t size) {
  	int r;
	struct minix_mem_range mr;

	mr.mr_base = mr_base;
	mr.mr_limit = mr_base + size;

	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);
}

void* (vg_init)(uint16_t mode) {


	vbe_mode_info_t info;
	if (vbe_get_mode_info(mode, &info)) {
		return NULL;
	}

	if ((info.ModeAttributes & BIT(0)) == 0)
		return NULL;

	x_res = info.XResolution;
	y_res = info.YResolution;
	vg_mode = info.MemoryModel;

	// If in direct mode, calculate the masks
	if (info.MemoryModel == 4) {

		red_mask = 0;
		red_field_position = info.RedFieldPosition;
		red_mask_size = info.RedMaskSize;
		for (uint32_t i = info.RedFieldPosition; i < info.RedFieldPosition + info.RedMaskSize; ++i)
			red_mask |= BIT(i);

		green_mask = 0;
		green_field_position = info.GreenFieldPosition;
		green_mask_size = info.GreenMaskSize;
		for (uint32_t i = info.GreenFieldPosition; i < info.GreenFieldPosition + info.GreenMaskSize; ++i)
			green_mask |= BIT(i);

		blue_mask = 0;
		blue_field_position = info.BlueFieldPosition;
		blue_mask_size = info.BlueMaskSize;
		for (uint32_t i = info.BlueFieldPosition; i < info.BlueFieldPosition + info.BlueMaskSize; ++i)
			blue_mask |= BIT(i);
	}

	// +7 para garantir q se for necessário reservamos espaço para o final do byte
	bits_per_pixel = info.BitsPerPixel;
	bytes_per_pixel = (bits_per_pixel + 7) >> 3;
  size_t size = x_res * y_res * bytes_per_pixel;

	privctl((phys_bytes) info.PhysBasePtr, size);
	buffer_base = vm_map_phys(SELF, (void *) info.PhysBasePtr, size);

	if (buffer_base == MAP_FAILED)
		printf("Mapping failed\n");

	  reg86_t r;

	/* Specify the appropriate register values */

	memset(&r, 0, sizeof(r)); /* zero the structure */

	r.ax = 0x4F02;    /* Set Video Mode function */
	r.bx = BIT(14) | mode;
	r.intno = 0x10; /* BIOS video services */

	/* Make the BIOS call */

	if (sys_int86(&r) != OK) {
		printf("\tvg_init(): sys_	int86() failed \n");
		return NULL;
	}
  video_mem = (uint8_t*) malloc(x_res*y_res);
	return buffer_base;
}

int (vg_draw_hline_proj)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, void* buffer) {

	uint8_t *base = (uint8_t *) buffer + (y * x_res + x) * bytes_per_pixel;
	for (uint16_t i = 0; i < len; ++i) {

		for (uint8_t j = 0; j < bytes_per_pixel; ++j) {
			*base = color >> (j * 8);
			++base;
		}

	}

	return 0;
}

int (vg_draw_rectangle_proj)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, void* buffer) {

	if (x > x_res || y > y_res)
		return 0;

	if (x + width >= x_res) {
		width = x_res - x;
	}

	if (y + height >= y_res) {
		height = y_res - y;
	}

	for (uint16_t i = y; i < y + height; ++i) {
		vg_draw_hline_proj(x, i, width, color, buffer);
	}

  return 0;

}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, void* buffer) {

	uint8_t *ptr = (uint8_t *) buffer + (y * x_res + x) * bytes_per_pixel;

	for (uint8_t j = 0; j < bytes_per_pixel; ++j) {
		*ptr = color >> (j * 8);
		++ptr;
	}

	return 0;
}

int vg_draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y, void* buffer) {

	xpm_image_t img;
	uint8_t* sprite = NULL;

	sprite = xpm_load(xpm, XPM_INDEXED, &img);

	uint counter = 0;
	uint16_t extra_x = 0;

	uint16_t width, height;
	if (x + img.width <= x_res)
		width = x + img.width;
	else {
		width = x_res;
		extra_x = x + img.width - width;
	}
	if (y + img.height <= y_res)
		height = y + img.height;
	else
		height = y_res;


	for (uint16_t j = y; j < height; j++) {
		 for (uint16_t i = x; i < width; i++) {
       if(sprite[counter]!=65){
         vg_draw_pixel(i, j, (sprite[counter]), buffer);
       }

			counter++;
		}
		counter += extra_x;
	}

	return 0;
}

uint8_t vg_update_xpm(xpm_map_t xpm, uint16_t old_x, uint16_t old_y, uint16_t new_x, uint16_t new_y, void* buffer) {

	xpm_image_t img;
	uint8_t* map = NULL;

	map = xpm_load(xpm, XPM_INDEXED, &img);

	// Paint over the old one
	if (vg_draw_rectangle_proj(old_x, old_y, img.width, img.height, 0, buffer_base))
		return 1;

	if (vg_draw_xpm(xpm, new_x, new_y, buffer))
		return 1;

	return 0;
}

void vg_fill_black(void* buffer) {
	memset(buffer, 11, x_res * y_res * bytes_per_pixel);
}

void free_video_mem(){
  free(video_mem);
  free(buffer_base);
}
