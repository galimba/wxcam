;void mono_asm(unsigned char* src, int size);
%include "src/globales.mac";
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;      Monochrome filter     ;
; Autores:                   ;
;      galimba@gmail.com     ;
; adrian.bonaccini@gmail.com ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
global mono_asm

section .text

section .rodata

    clean_div_4: dd 0xFF3FFF3F, 0xFF3FFF3F, 0xFF3FFF3F, 0xFF3FFF3F
    last_on:     dd 0x00000000, 0x00000000, 0x00000000, 0xFF000000
    last_off:    dd 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00FFFFFF
    shuffle:     dd 0x03000000, 0x06060303, 0x09090906, 0xFF0C0C0C

    ;Parameters
    ;rdi = char* source
    ;rsi = int size

mono_asm:

    begin_convencion_C

    initialize:

;rdtsc_:
;cpuid           ; para prevenir out-of-order execution del read tsc
;rdtsc           ; EDX:EAX tienen los 64 bits del tsc
;sal rdx, 32     ; muevo de EDX a la parte alta de RDX
;add rax, rdx    ; 
;mov r15,rax     ; r15 mantiene el valor del counter al ppio.



    mov rcx, rsi        ; contador de bytes
    mov rax, 15         ; bytes que proceso (5 píxeles)

    byte_cycle:

    movdqu xmm1, [rdi]

    ; rescato el byte que no voy a procesar
    movdqu xmm0, [last_on]
    pand xmm0, xmm1

    ; divido por cuatro y limpio el residuo
	psrlw xmm1, 2
    movdqu xmm8, [clean_div_4]
    pand xmm1, xmm8

	; xmm2 tiene G/2
    movdqu xmm2, xmm1
    psrldq xmm2, 1

	; xmm3 tiene B/4
    movdqu xmm3, xmm2
    psrldq xmm3, 1

	; en xmm1 queda (R+2G+B)/4
    paddb xmm1, xmm2
    paddb xmm1, xmm2
    paddb xmm1, xmm3

    ; replico el resultado en cada byte
    movdqu xmm8, [shuffle]
    pshufb xmm1, xmm8

    ; quito el último byte
    movdqu xmm8, [last_off]
    pand xmm1, xmm8

    ; vuelvo a poner el byte de back up
    por xmm1, xmm0

    movdqu [rdi], xmm1       ; bajo 16 bytes a memoria
    add rdi, rax             ; sincronizo el puntero
    sub rcx, rax             ; decremento el contador
    cmp rcx, rax             ; tengo aún 15 bytes delante?
    jl end

    jmp byte_cycle

    end:
;cpuid
;rdtsc
;sal rdx, 32
;add rax, rdx
;sub rax, r15            ; time_end - time_start


    end_convencion_C

ret