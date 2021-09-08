#include "include/stdint.h"
#include "include/stdlib.h"
#include "include/sys/GDT.h"
#include "include/sys/IDT.h"
#include "include/sys/PIC.h"
#include "include/sys/io.h"
#include "include/sys/memory/page_management.h"

void utoa(char* dest, const unsigned int buf, uint64_t num);

void put_pixel(unsigned int x, unsigned int y, uint8_t color);
void math_put_pixel(int x, int y, uint8_t color);
void clear_screen();

unsigned int utoalen(size_t num);

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

void _start()
{
	uint16_t* vm = (uint16_t*)0xb8000;
	vm[8] = 0x1F00 | '0';
  struct vbe_mode_info_structure* vbe_mode_info = (struct  vbe_mode_info_structure*)(0x4f6 + 0x7C00);
	char hex_lookup[16] = "0123456789ABCDEF";
	uint32_t* framebuffer   = (uint32_t*)(size_t)(vbe_mode_info->framebuffer);
	vm[0] = 0x1F00 | hex_lookup[(vbe_mode_info->framebuffer & 0xF0000000) >> 28];
	vm[1] = 0x1F00 | hex_lookup[(vbe_mode_info->framebuffer & 0x0F000000) >> 24];
	vm[2] = 0x1F00 | hex_lookup[(vbe_mode_info->framebuffer & 0x00F00000) >> 20];
	vm[3] = 0x1F00 | hex_lookup[(vbe_mode_info->framebuffer & 0x000F0000) >> 16];
	vm[4] = 0x1F00 | hex_lookup[(vbe_mode_info->framebuffer & 0x0000F000) >> 12];
	vm[5] = 0x1F00 | hex_lookup[(vbe_mode_info->framebuffer & 0x00000F00) >> 8 ];
	vm[6] = 0x1F00 | hex_lookup[(vbe_mode_info->framebuffer & 0x000000F0) >> 4 ];
	vm[7] = 0x1F00 | hex_lookup[(vbe_mode_info->framebuffer & 0x0000000F) >> 0 ];
	//framebuffer[10000] = 0x00FFFFFF;
  init_GDT();
  init_IDT();
  init_PIC();
  asm volatile("sti");
  clear_mask_PIC(2);
  clear_mask_PIC(12);


	for(;;)
	{
		asm volatile("hlt");
	}

	

}

unsigned int utoalen(size_t num)
{
  for(size_t i = 1;; i++)
  {
    num /= 10;
    if(!num)
    {
     return i;
    }
  }
}

void utoa(char* dest, const unsigned int buf, uint64_t num)
{
  if(utoalen(num) > buf) return;
  for(unsigned int i = (utoalen(num) - 1);; i--)
  {
    unsigned int tmp = num % 10;

    dest[i] = (char)(tmp + 48);

    if((num /= 10) == 0) break;
  }
}

