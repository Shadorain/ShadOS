;;kernel.asm

bits 32
section .text
        ;multiboot spec
        align 4
        dd 0x1BADB002            ; magic
        dd 0x00                  ; flags
        dd - (0x1BADB002 + 0x00) ; checksum == 0

global start
global kb_handler
global read_port
global write_port
global load_idt

extern kmain
extern kb_handler_main

; ---------------------------------------------
read_port:
    mov edx, [esp + 4]
    in al, dx
    ret

write_port:
    mov edx, [esp + 4]
    mov al, [esp + 4 + 4];
    out dx, al
    ret

load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret

kb_handler:
    call kb_handler_main
    iretd
; ---------------------------------------------

start:
    cli
    mov esp, stack_space	; sets stack pointer
    call kmain
    hlt		 	; halts CPU

section .bss
resb 8192
stack_space:
; vim:ft=nasm
