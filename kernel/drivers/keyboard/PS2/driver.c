#include "../../../include/stdint.h"
#include "../../../include/sys/io.h"
#include "../../../include/sys/IDT.h"
#include "../../../include/sys/PIC.h"

extern void* _start;


struct interrupt_snapshot
{
  uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsp, rbp, rsi, rdi, edx, rcx, rbx, rax;
  uint64_t rip, cs, rflags, useresp, ss;
} __attribute__((packed));

typedef struct interrupt_snapshot registers;

void keyboard_interrupt_handler(registers regs)
{
  uint8_t volatile scan_code = inb(0x60);
  SEND_EOI();
}
