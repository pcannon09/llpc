BITS 64

section .text

global llpc_sound_outb
global llpc_sound_inb

; Function Definitions:
; void llpc_sound_outb(UINT16 port, UINT8 value);
; UINT8 llpc_sound_inb(UINT16 port);

llpc_sound_outb:
	mov dx, di
	mov al, sil
	out dx, al

	ret

llpc_sound_inb:
	mov dx, di
	in al, dx
	movzx eax, al

	ret

