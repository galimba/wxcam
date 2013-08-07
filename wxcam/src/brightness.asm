;void brightness_asm(unsigned char* src, int size, int value);
%include "src/globales.mac";
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;      Brightness filter     ;
; Autores:                   ;
;      galimba@gmail.com     ;
; adrian.bonaccini@gmail.com ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
global brightness_asm

section .text

section .rodata

replic:   dd 0x00000000, 0x00000000, 0x00000000, 0x00000000

;Parameters
;rdi = char* source
;rsi = int size
;rdx = int value

brightness_asm:

    begin_convencion_C

    initialize:

    mov rcx, rsi        ; contador de bytes
    mov rax, 16         ; bytes que proceso (1 xmm)

    movq xmm1, rdx
    movdqu xmm0, [replic]
    pshufb xmm1, xmm0   ; xmm1 tiene el value replicado

        byte_cycle:

        movdqu xmm0, [rdi]

        subtract:

        psubusb xmm0, xmm1

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