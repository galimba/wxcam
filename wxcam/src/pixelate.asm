;void pixelate_asm(unsigned char* src, unsigned char* dst, int height, int width);

%include "src/globales.mac";
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;      Pixelate filter       ;
; Autores:                   ;
;      galimba@gmail.com     ;
; adrian.bonaccini@gmail.com ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
global pixelate_asm

section .data

section .text

section .rodata

    bring_last_3:  dd 0xFF0F0E0D, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
    first_3_off:   dd 0x03FFFFFF, 0x07060504, 0x0B0A0908, 0x0F0E0D0C

    shuffle_xmm: 
                    dd 0x00020100, 0x01000201, 0x02010002, 0x0C0E0D0C ;grade 4 / 1
                    dd 0x00020100, 0x01000201, 0x080A0908, 0x09080A09 ;grade 4 / 2
                    dd 0x00020100, 0x04060504, 0x05040605, 0x06050406 ;grade 4 / 3

                    dd 0x00020100, 0x01000201, 0x02010002, 0x00020100 ;grade 8 / 1
                    dd 0x00020100, 0x01000201, 0x080A0908, 0x09080A09 ;grade 8 / 2
                    dd 0x00020100, 0x01000201, 0x02010002, 0x00020100 ;grade 8 / 3

                    dd 0x00020100, 0x01000201, 0x02010002, 0x00020100 ;grade 16 / 1
                    dd 0x00020100, 0x01000201, 0x02010002, 0x00020100 ;grade 16 / 2
                    dd 0x00020100, 0x01000201, 0x02010002, 0x00020100 ;grade 16 / 3

    ;Parameters
    ;rdi = char* source
    ;rsi = int grade
    ;rdx = int height
    ;rcx = int width

pixelate_asm:

    begin_convencion_C

    initialize:

    push rdi            ; preservo los registros con variables
    push rsi            ; preservo los registros con variables
    push rdx            ; preservo los registros con variables
    push rcx            ; preservo los registros con variables

    mov r8, rdi         ; r8 = fuente
    mov r9, rsi         ; r9 = grade
    mov r10, rdx        ; r10 = alto
    mov r11, rcx        ; r11 = ancho en px
    add r11, rcx        ; http://goo.gl/XSAlJ
    add r11, rcx        ; r11 = ancho en bytes

    cmp r9, 0
    je grade_4
    cmp r9, 1
    je grade_8
    cmp r9, 2
    je grade_16

    grade_4:
        mov r9, 4
        mov r12, 0
        jmp ugly_shit
    grade_8:
        mov r9, 8
        mov r12, 48
        jmp ugly_shit
    grade_16:
        mov r9, 16
        mov r12, 96
        jmp ugly_shit

    ugly_shit:

    mov rcx, r10        ; rcx es contador de alto
    rows_cycle:

        mov rdx, r11        ; edx = contador de ancho
        columns_cycle:

            movdqu xmm1, [r8+00]
            movdqu xmm2, [r8+16]
            movdqu xmm3, [r8+32]

            movdqu xmm4, [shuffle_xmm + r12 + 00]
            pshufb xmm1, xmm4
            movdqu [r8+00], xmm1

            ;copiar bytes 14, 15 y 16 de xmm1 a xmm2
            movdqu xmm4, [bring_last_3]
            pshufb xmm1, xmm4
            movdqu xmm5, [first_3_off]
            pshufb xmm2, xmm5
            pxor xmm2, xmm1

            movdqu xmm5, [shuffle_xmm + r12 + 16]
            pshufb xmm2, xmm5
            movdqu [r8+16], xmm2

            ;copiar bytes 14, 15 y 16 de xmm2 a xmm3
            movdqu xmm5, [bring_last_3]
            pshufb xmm2, xmm5
            movdqu xmm6, [first_3_off]
            pshufb xmm3, xmm6
            pxor xmm3, xmm2

            movdqu xmm6, [shuffle_xmm + r12 + 32]
            pshufb xmm3, xmm6
            movdqu [r8+32], xmm3

        add r8, 48          ; incremento puntero al buffer
        sub rdx, 48         ; decremento el índice de columnas
        cmp rdx, 0          ; si no hay mas columnas, cambio de fila
        jne columns_cycle

        ;_;memcpy_begin;_;
        push rsi            ;parámetro de movsq
        push rdi            ;parámetro de movsq
        push rcx            ;parámetro de rep

        mov rsi, r8         ;mov rsi, source + ancho
        sub rsi, r11        ;acomodo el source

        mov rdi, r8         ;mov edi, dest
        mov r13, r9        ;r13 es el contador

        ciclo_memcpy:

            mov rcx, r11        ;actualizo el contador
            shr rcx, 3          ;divido por 8 (trabajo con words)
            rep movsq

        dec r13
        cmp r13, 1
        jne ciclo_memcpy

        fin_memcpy:

        mov r8, rdi        ; incremento el puntero (grade - 1) veces

        pop rcx
        pop rdi
        pop rsi
        ;_;memcpy_end;_;

    sub rcx, r9
    cmp rcx, 0
    jne rows_cycle


    end_cycle:

    fin:

    pop rcx    ;restauro los registros con variables
    pop rdx    ;restauro los registros con variables
    pop rsi    ;restauro los registros con variables
    pop rdi    ;restauro los registros con variables

    end_convencion_C

ret