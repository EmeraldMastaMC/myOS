//#include "include/stdint.h"
#include "include/stdlib.h"
#include "include/sys/GDT.h"
void puts(char* buffer);
void utoa(char* dest, const unsigned int buf, uint64_t num);
unsigned int utoalen(unsigned int num);

void _start()
{
	//initGDT();
	char amy[60];

	void* tina = pmalloc(1);
	utoa(amy, 60, (uint64_t)tina);
	puts(amy);

	tina = pmalloc(512);
	void* second = tina;
	utoa(amy, 60, (uint64_t)tina);
	puts(amy);

	tina = pmalloc(1);
	void* third = tina;
	utoa(amy, 60, (uint64_t)tina);
	puts(amy);
	
	pfree(second, 1);

	tina = pmalloc(1);
	utoa(amy, 60, (uint64_t)tina);
	puts(amy);
}

void puts(char* buffer)
{
	int16_t* VIDEO = (int16_t*)0xB8000;
	for(int i = 0;; i++, VIDEO++)
	{
		if(!buffer[i]) break;
		*VIDEO = 0x1F00 | buffer[i];
	}
}

unsigned int utoalen(unsigned int num)
{
  for(unsigned int i = 1;; i++)
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

