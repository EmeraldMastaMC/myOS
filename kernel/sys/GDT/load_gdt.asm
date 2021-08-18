global load_GDT
extern GDTDescriptor
load_GDT:
	mov rax, rdi           ; First Argument, Pointer To GDT
	mov bx, si             ; Second Argument, Code Segment
	mov cx, dx             ; Third Argument,  Data Segment

	mov word [jumpseg], bx ; Change the code segment that we are jumping to the
	                       ; One provided

	mov r8, jump_buffer    ; Our far jump buffer
	lgdt [rax]             ; Load GDT
	;rex.w jmp far [r8]
	jmp far [r8]

jump_buffer:
	dq return
jumpseg:
	dw 0x00 ; Will be filled with a valid Code Segment

return:
	mov ds, cx
	mov es, cx
	mov fs, cx
	mov gs, cx
	mov ss, cx
	ret
