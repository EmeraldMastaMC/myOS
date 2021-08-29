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

void slope(int m)
{
	for(int i = -160; i < 160; i++)
	{
		int y = i * m;
		if((100 <= y) || (y <= -100))
		{
			continue;
		}
		math_put_pixel(i, y, 0x24);
	}
	if(m != 0)
	{
		for(int i = -100; i < 100; i++)
		{
			int x = i / m;
			if((160 <= x) || (x <= -160))
			{
				continue;
			}
			math_put_pixel(x, i, 0x24);
		}
	}
}

void _start()
{
  init_GDT();
  init_IDT();
  init_PIC();
	asm volatile("sti");
	clear_mask_PIC(2);
	clear_mask_PIC(12);

	outb(0x60, 0xF4);


	for(int i = -100; i <= 100;)
	{
		static bool forward = true;

		asm volatile("hlt");
		asm volatile("hlt");

		clear_screen();
		slope(i);
		if(i == 100)
		{
			forward = false;
			i--;
			continue;
		}
		else if((i == -100) && (forward == false))
		{
			forward = true;
			i++;
			continue;
		}
		if(forward)
		{
			i++;
			continue;
		} else 
		{
			i--;
			continue;
		}

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

void put_pixel(unsigned int x, unsigned int y, uint8_t color)
{
	uint8_t* vm = (uint8_t*)0xA0000 + (y * 320) + x;
	*vm = color;
}

void math_put_pixel(int x, int y, uint8_t color)
{
	put_pixel(x + 160, (~y + 1) + 100, color);
}

void clear_screen()
{
	uint8_t* vm = (uint8_t*)0xA0000;
	for(unsigned int i = 0; i < (320 * 200); i++)
		vm[i] = 0;
}
