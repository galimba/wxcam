;void blushing_asm(unsigned char* src, int size, int threshold);
%include "src/globales.mac";
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;      Instagram Filter      ;
; Autores:                   ;
;      galimba@gmail.com     ;
; adrian.bonaccini@gmail.com ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
global blushing_asm

section .text

section .rodata

first_on:  dd 0x00FFFFFF, 0x00000000, 0x00000000, 0x00000000
first_off: dd 0xFF000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF

unpack:   dd 0xFF01FF00, 0xFF03FF02, 0xFF05FF04, 0xFF07FF06
mul_low:  dd 0xFFFF0100, 0xFFFF0302, 0xFFFF0504, 0xFFFFFFFF
mul_high: dd 0x0100FFFF, 0x0302FFFF, 0x0504FFFF, 0xFFFFFFFF

correte:  dd 0x07060504, 0x0B0A0908, 0x0F0E0D0C, 0xFFFFFFFF
replic:   dd 0xFF000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF

blush_1:  dd 0x00000010, 0x00000000, 0x00000000, 0x00000000
blush_2:  dd 0x00000016, 0x00000000, 0x00000000, 0x00000000
firstdw:  dd 0x03020100, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF

;Parameters
;rdi = char* source
;rsi = int size
;rdx = int threshold

blushing_asm:

    begin_convencion_C

    initialize:

    mov rcx, rsi        ; contador de bytes
    mov rax, 3          ; bytes que proceso (1 pixel)

        byte_cycle:

        movdqu xmm0, [rdi]
        ;XMM0 -> la papa

        movdqu xmm15, xmm0
        ;XMM15 -> la papa

        movdqu xmm14, xmm15
        movdqu xmm13, [unpack]
        pshufb xmm14, xmm13
        ;XMM14 -> XMM15 spliteado en words

        movdqu xmm13, xmm14
        pmullw xmm13, xmm13
        movdqu xmm12, [mul_low]
        pshufb xmm13, xmm12
        ;XMM13 -> XMM14^2 parte baja

        movdqu xmm12, xmm14
        pmulhuw xmm12, xmm12
        movdqu xmm11, [mul_high]
        pshufb xmm12, xmm11
        ;XMM12 -> XMM14^2 parte alta

        por xmm13, xmm12
        movdqu xmm14, xmm13
        ;XMM14 -> (255-R)^2, (0-G)^2, (0-B)^2, 0x0

        movdqu xmm11, [correte]
        pshufb xmm13, xmm11
        paddd xmm14, xmm13
        pshufb xmm13, xmm11
        paddd xmm14, xmm13
        movdqu xmm11, [firstdw]
        pshufb xmm14, xmm11
        ;XMM14 -> (255-R)^2 + (0-G)^2 + (0-B)^2

        movq r12, xmm14
        cmp r12,rdx
        jle blusher

        movdqu xmm8, [blush_1]
        jmp continue_cycle

        blusher:
        movdqu xmm8, [blush_2]

        continue_cycle:

        movdqu xmm1, xmm0
        paddb xmm1, xmm8

        movdqu xmm8, [first_on]
        pand xmm1, xmm8

        movdqu xmm8, [first_off]
        pand xmm0, xmm8
        por xmm0, xmm1
        ;XMM0 -> tiene la mezcla

        movdqu [rdi], xmm0       ; bajo 16 bytes a memoria
        add rdi, rax             ; sincronizo el puntero
        sub rcx, rax             ; decremento el contador
        cmp rcx, rax             ; me quedan bytes?
        jl end

        jmp byte_cycle

    end:

    end_convencion_C

ret
