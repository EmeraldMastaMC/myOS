[org 0x7E00]
; IN AX, WIDTH
; IN BX, HEIGHT
; IN CL, Bits Per Pixel
vbe_set_mode:
  mov   dword [vbe_info_block], VBE2_SIGN

  mov     [width], ax
  mov     [height], bx
  mov     [bpp], cl
 
  sti
 
  push    es          ; some VESA BIOSes destroy ES, or so I read
  mov     ax, 0x4F00        ; get VBE BIOS info
  mov     di, vbe_info_block
  int     0x10
  pop     es
 
  cmp     ax, 0x4F        ; BIOS doesn't support VBE?
  jne     NoVBE
 
  mov     ax, word [vbe_info_block + 14]
  mov     [offset], ax
  mov     ax, word [vbe_info_block + 20]
  mov     [segm], ax
 
  mov     ax, [segm]
  mov     fs, ax
  mov     si, [offset]
 
find_mode:
  mov     dx, [fs:si]
  add     si, 2
  mov     [offset], si
  mov     [mode], dx
  mov     ax, 0
  mov     fs, ax
 
  cmp     word [mode], 0xFFFF      ; end of list?
  je      EndOfList
 
  push    es
  mov     ax, 0x4F01        ; get VBE mode info
  mov     cx, [mode]
  mov     di, mode_info_block
  int     0x10
  pop     es
 
  cmp     ax, 0x4F
  jne     VBEGetError

  mov     ax, [width]
  cmp     ax, [mode_info_block + 18]
  jne     next_mode
 
  mov     ax, [height]
  cmp     ax, [mode_info_block + 20]
  jne     next_mode
 
  mov     al, [bpp]
  cmp     al, [mode_info_block + 25]
  jne     next_mode
 
  ; If we make it here, we've found the correct mode!
  mov     ax, [width]
  mov     word [vbe_final_info_block + 0], ax
  mov     ax, [height]
  mov     word [vbe_final_info_block + 2], ax
  mov     eax, [mode_info_block + 40]
  mov     dword [vbe_final_info_block + 4], eax
  mov     ax, [mode_info_block + 16]
  mov     word [vbe_final_info_block + 8], ax
  mov     eax, 0
  mov     al, [bpp]
  mov     byte [vbe_final_info_block + 10], al
  shr     eax, 3
  mov     dword [vbe_final_info_block + 11], eax
 
  mov     ax, [width]
  shr     ax, 3
  dec     ax
  mov     word[vbe_final_info_block + 15], ax
 
  mov     ax, [height]
  shr     ax, 4
  dec     ax
  mov     word[vbe_final_info_block + 17], ax


  ; Set the mode
  push    es
  mov     ax, 0x4F02
  mov     bx, [mode]
  or      bx, 0x4000      ; enable LFB
  mov     di, 0
  int     0x10
  pop     es
 
  cmp     ax, 0x4F
  jne     SetError
  
  stc

  ret
 
next_mode:
  mov     ax, [segm]
  mov     fs, ax
  mov     si, [offset]
  jmp     find_mode
 
NoVBE:
  clc
  ret
 
EndOfList:
  clc
  ret
 
VBEGetError:
  clc
  ret
 
SetError:
  clc
  ret

;vbe_info_block:   times 128 dd 0xDEC0ADDE
vbe_info_block:   times 512 db 0xEE
mode_info_block:  times 256 db 0xFF

;mode_info_block:  times 64  dd 0xAFBEADDE
;vbe_info_block:   times 128 dd 0x00
;mode_info_block:  times 64  dd 0x00

vbe_final_info_block: times 19 db 0x00


width:        dw 0
height:        dw 0
bpp:          db 0
segm:          dw 0
offset:        dw 0
mode:          dw 0

VBE2_SIGN:       db "VBE2",0
times 1536-($-$$) db 0
