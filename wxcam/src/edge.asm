;void edge_asm(unsigned char* src, unsigned char* dst, int height, int width);

%include "src/globales.mac";
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;         Edge filter        ;
; Autores:                   ;
;      galimba@gmail.com     ;
; adrian.bonaccini@gmail.com ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
global edge_asm

section .data

section .text

section .rodata

    unos:       dd 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
    ceros:      dd 0x00000000, 0x00000000, 0x00000000, 0x00000000
    mask_255:   dd 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF

    ;Parameters
    ;rdi = char* source
    ;rsi = char* destiny
    ;rdx = int height
    ;rcx = int width

edge_asm:

    begin_convencion_C

	;;;;;;;;;; Aca comienza la funcion ;;;;;;;;

    inicializo_variables:

    mov r8, rdi         ; r8 = fuente
    mov r9, rsi         ; r9 = destino

    mov r10, rdx        ; r10 = alto
    mov r11, rcx        ; r11 = ancho en px
    add r11, rcx        ; http://goo.gl/XSAlJ
    add r11, rcx        ; r11 = ancho en bytes
    

    mov rcx, r10        ; ecx = contador de filas / alto

    ciclo_filas:

    mov rdx, r11        ; edx = contador de columnas / ancho

    ciclo_columnas:

    cmp rcx, r10
    jne sigue_1
    jmp pintar_negro    ; si estoy en la primera fila

    sigue_1:

    mov rax, r11
    sub rax, 3
    cmp rdx, rax
    jle sigue_2
    jmp pintar_negro    ; si estoy en la primera columna

    sigue_2:

    cmp rcx, 1
    jne sigue_3
    jmp pintar_negro    ; si estoy en la última fila

    sigue_3:

    cmp rdx, 1
    jne simd_sobel_X
    jmp pintar_negro    ; si estoy en la última columna

    simd_sobel_X:

    cmp rdx, 10         ; si tengo aún 10 bytes por levantar
    jg continuar_2
    jmp pintar_negro

    continuar_2:

    pxor XMM0, XMM0         ; limpio XMM0
    pxor XMM5, XMM5         ; resultados parciales parte ALTA
    pxor XMM6, XMM6         ; resultados parciales parte BAJA
    movdqu XMM7, [ceros]

    ; 1ra FILA
    sub r8, r11             ; subo el puntero una fila (lectura horizontal)
    movdqu XMM1, [r8-3]     ; leo desde el pixel anterior
    movdqu XMM2, [r8+3]     ; leo desde el pixel siguiente

    movdqu XMM3, XMM1
    movdqu XMM4, XMM2
    punpckhbw XMM3, XMM7    ; desempaqueto parte ALTA de XMM1
    punpckhbw XMM4, XMM7    ; desempaqueto parte ALTA de XMM2
    psubsw XMM5, XMM3       ; XMM5 parte alta (-1)
    paddsw XMM5, XMM4       ; XMM5 parte alta (+1)

    movdqu XMM3, XMM1
    movdqu XMM4, XMM2
    punpcklbw XMM3, XMM7    ; desempaqueto parte BAJA de XMM1
    punpcklbw XMM4, XMM7    ; desempaqueto parte BAJA de XMM2
    psubsw XMM6, XMM3       ; XMM6 parte baja (-1)
    paddsw XMM6, XMM4       ; XMM6 parte baja (+1)

    add r8, r11             ; vuelvo el puntero a su posición

    ; 2da FILA
    movdqu XMM1, [r8-3]     ; leo desde el pixel anterior
    movdqu XMM2, [r8+3]     ; leo desde el pixel siguiente

    movdqu XMM3, XMM1
    movdqu XMM4, XMM2
    punpckhbw XMM3, XMM7
    punpckhbw XMM4, XMM7
    psubsw XMM5, XMM3
    psubsw XMM5, XMM3
    paddsw XMM5, XMM4
    paddsw XMM5, XMM4

    movdqu XMM3, XMM1
    movdqu XMM4, XMM2
    punpcklbw XMM3, XMM7
    punpcklbw XMM4, XMM7
    psubsw XMM6, XMM3
    psubsw XMM6, XMM3
    paddsw XMM6, XMM4
    paddsw XMM6, XMM4

    ; 3ra FILA
    add r8, r11             ; bajo el puntero una fila (lectura horizontal)

    movdqu XMM1, [r8-3]     ; leo desde el pixel anterior
    movdqu XMM2, [r8+3]     ; leo desde el pixel siguiente

    movdqu XMM3, XMM1
    movdqu XMM4, XMM2
    punpckhbw XMM3, XMM7
    punpckhbw XMM4, XMM7
    psubsw XMM5, XMM3
    paddsw XMM5, XMM4

    movdqu XMM3, XMM1
    movdqu XMM4, XMM2
    punpcklbw XMM3, XMM7
    punpcklbw XMM4, XMM7
    psubsw XMM6, XMM3
    paddsw XMM6, XMM4

    sub r8, r11             ; vuelvo el puntero a su posición

    simd_absoluto_X:

    pxor XMM3, XMM3         ; limpio el registro
    pcmpgtw XMM3, XMM5      ; los negativos de XMM5
    movdqu XMM4, [unos]     ; mascara de UNOS
    pxor XMM4, XMM3         ; los positivos de XMM5
    pand XMM3, XMM5         ; los negativos de XMM5
    pand XMM4, XMM5         ; los postivos de XMM5
    pxor XMM5, XMM5         ; limpio el registro
    psubsw XMM5, XMM3       ; obtengo el absoluto de cada byte
    por XMM5, XMM4          ; joinneo los resultados

    pxor XMM3, XMM3         ; limpio el registro
    pcmpgtw XMM3, XMM6      ; los negativos de XMM6
    movdqu XMM4, [unos]     ; mascara de UNOS
    pxor XMM4, XMM3         ; los positivos de XMM6
    pand XMM3, XMM6         ; los negativos de XMM6
    pand XMM4, XMM6         ; los postivos de XMM6
    pxor XMM6, XMM6         ; limpio el registro
    psubsw XMM6, XMM3       ; obtengo el absoluto de cada byte
    por XMM6, XMM4          ; joinneo los resultados

    simd_saturar_X:

    movdqu XMM4, [mask_255]
    movdqu XMM3, XMM5
    pcmpgtw XMM3, XMM4      ; mask words mayores a 255
    movdqu XMM4, [unos]
    pxor XMM4, XMM3         ; mask words menores a 255
    pand XMM4, XMM5         ; words de XMM5 menores a 255
    movdqu XMM7, [mask_255]
    pand XMM3, XMM7
    por XMM4, XMM3
    movdqu XMM5, XMM4

    movdqu XMM4, [mask_255]
    movdqu XMM3, XMM6
    pcmpgtw XMM3, XMM4      ; mask words mayores a 255
    movdqu XMM4, [unos]
    pxor XMM4, XMM3         ; mask words menores a 255
    pand XMM4, XMM6         ; words de XMM6 menores a 255
    movdqu XMM7, [mask_255]
    pand XMM3, XMM7
    por XMM4, XMM3
    movdqu XMM6, XMM4

    simd_empaquetar_X:

    packuswb XMM6, XMM5
    movdqu XMM0, XMM6       ; dejo en XMM0 el resultado

    simd_sobel_Y:

    pxor XMM1, XMM1

    pxor XMM5, XMM5         ; resultados parciales parte ALTA
    pxor XMM6, XMM6         ; resultados parciales parte BAJA
    movdqu XMM7, [ceros]

    ; 1ra FILA
    sub r8, r11             ; subo el puntero una fila

    movdqu XMM1, [r8-3]     ; leo desde el pixel anterior
    punpckhbw XMM1, XMM7    ; desempaqueto parte ALTA de XMM1
    psubsw XMM5, XMM1       ; XMM5 parte alta (-1)
    movdqu XMM1, [r8-3]     ; repito la misma lectura
    punpcklbw XMM1, XMM7    ; desempaqueto parte BAJA de XMM1
    psubsw XMM6, XMM1       ; XMM6 parte baja (-1)

    movdqu XMM1, [r8+0]     ; leo desde el pixel actual
    punpckhbw XMM1, XMM7    ; desempaqueto parte ALTA de XMM1
    psubsw XMM5, XMM1       ; XMM5 parte alta (-2)
    psubsw XMM5, XMM1       ; XMM5 parte alta (-2)
    movdqu XMM1, [r8+0]     ; repito la misma lectura
    punpcklbw XMM1, XMM7    ; desempaqueto parte BAJA de XMM1
    psubsw XMM6, XMM1       ; XMM6 parte baja (-2)
    psubsw XMM6, XMM1       ; XMM6 parte baja (-2)

    movdqu XMM1, [r8+3]     ; leo desde el pixel siguiente
    punpckhbw XMM1, XMM7    ; desempaqueto parte ALTA de XMM1
    psubsw XMM5, XMM1       ; XMM5 parte alta (-1)
    movdqu XMM1, [r8+3]     ; repito la lectura
    punpcklbw XMM1, XMM7    ; desempaqueto parte BAJA de XMM1
    psubsw XMM6, XMM1       ; XMM6 parte baja (-1)

    add r8, r11             ; vuelvo el puntero a su posición

    ; 3ra FILA
    add r8, r11             ; avanzo el puntero una fila

    movdqu XMM1, [r8-3]     ; leo desde el pixel anterior
    punpckhbw XMM1, XMM7    ; desempaqueto parte ALTA de XMM1
    paddsw XMM5, XMM1       ; XMM5 parte alta (+1)
    movdqu XMM1, [r8-3]     ; repito la lectura
    punpcklbw XMM1, XMM7    ; desempaqueto parte BAJA de XMM1
    paddsw XMM6, XMM1       ; XMM6 parte baja (+1)

    movdqu XMM1, [r8+0]     ; leo desde el pixel actual
    punpckhbw XMM1, XMM7    ; desempaqueto parte ALTA de XMM1
    paddsw XMM5, XMM1       ; XMM5 parte alta (+2)
    paddsw XMM5, XMM1       ; XMM5 parte alta (+2)
    movdqu XMM1, [r8+0]     ; repito la lectura
    punpcklbw XMM1, XMM7    ; desempaqueto parte BAJA de XMM1
    paddsw XMM6, XMM1       ; XMM6 parte baja (+2)
    paddsw XMM6, XMM1       ; XMM6 parte baja (+2)

    movdqu XMM1, [r8+3]     ; leo desde el pixel siguiente
    punpckhbw XMM1, XMM7    ; desempaqueto parte ALTA de XMM1
    paddsw XMM5, XMM1       ; XMM5 parte alta (+1)
    movdqu XMM1, [r8+3]     ; repito la lectura
    punpcklbw XMM1, XMM7    ; desempaqueto parte BAJA de XMM1
    paddsw XMM6, XMM1       ; XMM6 parte baja (+1)

    sub r8, r11             ; vuelvo el puntero a su posición

    simd_absoluto_Y:    

    pxor XMM3, XMM3         ; limpio el registro
    pcmpgtw XMM3, XMM5      ; los negativos de XMM5
    movdqu XMM4, [unos]     ; mascara de UNOS
    pxor XMM4, XMM3         ; los positivos de XMM5
    pand XMM3, XMM5         ; los negativos de XMM5
    pand XMM4, XMM5         ; los postivos de XMM5
    pxor XMM5, XMM5         ; limpio el registro
    psubsw XMM5, XMM3       ; obtengo el absoluto de cada byte
    por XMM5, XMM4          ; joinneo los resultados

    pxor XMM3, XMM3         ; limpio el registro
    pcmpgtw XMM3, XMM6      ; los negativos de XMM6
    movdqu XMM4, [unos]     ; mascara de UNOS
    pxor XMM4, XMM3         ; los positivos de XMM6
    pand XMM3, XMM6         ; los negativos de XMM6
    pand XMM4, XMM6         ; los postivos de XMM6
    pxor XMM6, XMM6         ; limpio el registro
    psubsw XMM6, XMM3       ; obtengo el absoluto de cada byte
    por XMM6, XMM4          ; joinneo los resultados

    simd_saturar_Y:

    movdqu XMM4, [mask_255]
    movdqu XMM3, XMM5
    pcmpgtw XMM3, XMM4      ; mask words mayores a 255
    movdqu XMM4, [unos]
    pxor XMM4, XMM3         ; mask words menores a 255
    pand XMM4, XMM5         ; words de XMM5 menores a 255
    movdqu XMM7, [mask_255]
    pand XMM3, XMM7
    por XMM4, XMM3
    movdqu XMM5, XMM4

    movdqu XMM4, [mask_255]
    movdqu XMM3, XMM6
    pcmpgtw XMM3, XMM4      ; mask words mayores a 255
    movdqu XMM4, [unos]
    pxor XMM4, XMM3         ; mask words menores a 255
    pand XMM4, XMM6         ; words de XMM6 menores a 255
    movdqu XMM7, [mask_255]
    pand XMM3, XMM7
    por XMM4, XMM3
    movdqu XMM6, XMM4

    simd_empaquetar_Y:

    packuswb XMM6, XMM5
    movdqu XMM1, XMM6       ; dejo en XMM1 el resultado

    simd_sobel_XY:

    ;XMM0 tiene la info de X
    ;XMM1 tiene la info de Y

    movdqu XMM2, [ceros]
    movdqu XMM3, [ceros]

    movdqu XMM2, [unos]
    movdqu XMM3, [unos]

    pand XMM0, XMM2
    pand XMM1, XMM3
    paddusb XMM0, XMM1

    simd_ciclo:

    ;movdqu XMM0, [r8]
    movdqu [r9], XMM0
    add r8, 10
    add r9, 10

    sub rdx, 10        ; decremento el índice de columnas
    cmp rdx, 0         ; si no hay mas columnas, cambio de fila
    je siguiente_fila
    jmp ciclo_columnas

    pre_loop:	

    jmp ciclo_filas

    pintar_negro:

    xor rax, rax         ; pinto de negro o ceros

    seguir_ciclo:

    ; pinto 1 pixel
    mov byte [r9], al

    ; sincronizo punteros
    add r8, 1
    add r9, 1

    ; decremento contador columnas
    sub rdx, 1
    cmp rdx, 0
    je siguiente_fila
    jmp ciclo_columnas

    siguiente_fila:

    ; decremento RCX contador de filas (loop lo hace)
    loop pre_loop

    fin:

    ;;;;;;;;;; ACA termina la funcion  ;;;;;;;;

    end_convencion_C

ret