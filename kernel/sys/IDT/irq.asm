%macro pushaq 0
  push rax
  push rbx
  push rcx
  push rdx
  push rdi
  push rsi
  push rbp
  push rsp
  push r8
  push r9
  push r10
  push r11
  push r12
  push r13
  push r14
  push r15
%endmacro

%macro popaq 0
 pop r15
 pop r14
 pop r13
 pop r12
 pop r11
 pop r10
 pop r9
 pop r8
 pop rsp
 pop rbp
 pop rsi
 pop rdi
 pop rdx
 pop rcx
 pop rbx
 pop rax
%endmacro

global IRQ0
global IRQ1
global load_IDT

IRQ0:
  pushaq
  popaq
  iretq

extern keyboard_interrupt_handler
IRQ1:
  pushaq
  call keyboard_interrupt_handler
  popaq
  iretq


load_IDT:
  lidt [rdi]
  ret
