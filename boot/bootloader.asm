KERNEL_OFFSET equ 0x8000
VIDEO_MEMORY  equ 0xB8000
CODE_SEG      equ 0x08
DATA_SEG      equ 0x10
PAGE_PRESENT  equ (1 << 0)
PAGE_WRITE    equ (1 << 1)
[bits 16]
[org 0x7C00]
	mov byte [BOOT_DISK], dl



	mov al, 25        ; amount of sectors we wanna load
	call load_kernel
	mov ax, 0x13
	int 0x10
	call enter_32_bit_protected_mode
	jmp $

enter_32_bit_protected_mode:
  call enableA20                      ; We need to do this so we can access more memory, im not
                                      ; very sure about details
  cli                                 ; Disable interrupts, after this point we don't have a 
                                      ; Valid IDT (Interrupt Descriptor Table)

  lgdt [gdt_descriptor]               ; Load our GDT
  mov eax, cr0
  or eax, 0x01                        ; Set the 32bit Protected mode bit
  mov cr0, eax                        ; Actually enable 32bit Protected mode
  jmp CODE_SEG:start_of_32_bit_code   ; Flush the CPU pipeline with a far jump
                                      ; If we don't do a far jump bad things
                                      ; Happen
enableA20:
  in al, 0x92
  or al, 00000010b
  out 0x92, al
  ret

; GDT
gdt_start:               ; Null Desctiptor
  dq 0x0000000000000000
gdt_code:                ; Code Selector
  dw 0xFFFF
  dw 0x0000
  db 0x00
  db 10011010b
  db 11001111b
  db 0x00
gdt_data:                ; Data Selector
  dw 0xFFFF
  dw 0x0000
  db 0x00
  db 10010010b
  db 11001111b
  db 0x00
gdt_descriptor:
  dw 23         ; Size
  dq gdt_start  ; Address

load_kernel:
	mov bx, 0x00
	mov es, bx
	mov bx, KERNEL_OFFSET ; Where we want to load our kernel

	mov ah, 0x02 ; BIOS read sector function
	mov dl, [BOOT_DISK]
	mov ch, 0x00 ; Cylinder 0
	mov dh, 0x00 ; Head 0
	mov cl, 0x02 ; Second Sector (After boot sector)
	int 0x13
	jnc .return
	mov bx, 0xb000
	mov es, bx
	mov word [es:0x8000], 0x1f35
	jmp $
.return:
	ret


BOOT_DISK: db 0

[bits 32]
start_of_32_bit_code:
  mov ax, DATA_SEG   ; Load our data segment into all of the Segment Registers
                     ; Except for the code segment
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
	call enable_SSE
  call enter_64_bit_long_mode
  jmp CODE_SEG:start_of_64_bit_code

enable_SSE:
	mov eax, cr0
	and ax, 0xFFFB
	or ax, 0x2
	mov cr0, eax
	mov eax, cr4
	or ax, 3 << 9
	mov cr4, eax
	ret

enable_AVX:
	push eax
	push ecx
	push edx

	xor ecx, ecx
	xgetbv
	or eax, 7
	xsetbv
	
	pop edx
	pop ecx
	pop eax
	ret

enter_64_bit_long_mode:
  mov edi, 0x1000
  push di
  mov ecx, 0x1000
  xor eax, eax
  cld
  rep stosd

  pop di
  lea eax, [es:di + 0x1000]         ; Address of PDP
  or eax, PAGE_PRESENT | PAGE_WRITE
  mov [es:di], eax

  lea eax, [es:di + 0x2000]         ; Address of PD
  or eax, PAGE_PRESENT | PAGE_WRITE
  mov [es:di + 0x1000], eax

  lea eax, [es:di + 0x3000]         ; Address of PT
  or eax, PAGE_PRESENT | PAGE_WRITE
  mov [es:di + 0x2000], eax

  push di
  lea di, [di + 0x3000]
  mov eax, PAGE_PRESENT | PAGE_WRITE

.loop_page_table:
  mov [es:di], eax
  add eax, 0x1000
  add di, 8
  cmp eax, 0x200000
  jb .loop_page_table
  
  pop di

  mov eax, cr4
  or eax, 10100000b
  mov cr4, eax

  mov edx, edi
  mov cr3, edx

  mov ecx, 0xC0000080
  rdmsr

  or eax, 0x00000100
  wrmsr

  mov ebx, cr0
  or ebx, 0x80000000
  mov cr0, ebx

  mov byte [gdt_code + 6], 10101111b
  mov byte [gdt_data + 6], 10001111b
  ret

[bits 64]
start_of_64_bit_code:
  jmp KERNEL_OFFSET
times 510-($-$$) db 0x00          ; Padding
dw 0xAA55                         ; Magic Number

