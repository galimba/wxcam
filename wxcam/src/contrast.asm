;void contrast_asm(unsigned char* src, int size);
%include "src/globales.mac";
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;      contrast filter       ;
; Autores:                   ;
;      galimba@gmail.com     ;
; adrian.bonaccini@gmail.com ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
global contrast_asm

section .text

section .rodata

replic:   dd 0x00000000, 0x00000000, 0x00000000, 0x00000000
cleandiv: dd 0xFF3FFF3F, 0xFF3FFF3F, 0xFF3FFF3F, 0xFF3FFF3F

;Parameters
;rdi = char* source
;rsi = int size

contrast_asm:

    begin_convencion_C

    initialize:

    mov rcx, rsi        ; contador de bytes
    mov rax, 16         ; bytes que proceso (1 xmm)

        byte_cycle:

        movdqu xmm0, [rdi]

        percentageup:

        ; divido por cuatro y limpio residuo
        movdqu xmm1, xmm0
        psrlw xmm1, 2
        movdqu xmm2, [cleandiv]
        pand xmm1, xmm2
        paddusb xmm0, xmm1

        continue_cycle:

        movdqu [rdi], xmm0       ; bajo 16 bytes a memoria
        add rdi, rax             ; sincronizo el puntero
        sub rcx, rax             ; decremento el contador
        cmp rcx, rax             ; me quedan bytes?
        jl end

        jmp byte_cycle

    end:

    end_convencion_C

ret