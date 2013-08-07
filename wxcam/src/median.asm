;void median_asm(unsigned char* frame, unsigned char* dst, int height, int width, int size);

global median_asm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;      Median filter         ;
; Autores:                   ;
;      galimba@gmail.com     ;
; adrian.bonaccini@gmail.com ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
section .rodata
lastOff:     dd 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00FFFFFF
onlyRed: db 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF
onlyGreen: db 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00
onlyBlue: db 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00
sinMedio: db 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
soloMedio: db 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

section .text

median_asm:
        ; los parametros son pasados asi:
        ;	rdi= char* src
        ;	rsi= char* dst
        ;       rdx= int height
        ;       rcx= int width en pixels
        ;       r8= int size en bytes
        ;
	push rbp	;     reservo
	mov rbp,rsp	;       la
	push rbx	;      pila
	push r12	;        y
	push r13	;      guardo
	push r14	;    registros
	push r15	;       (...)

start:
        ;
        mov rax, r8     ; rax = size
        mov r8, rdi     ; r8 = frame
        mov r9, rsi     ; r9 = dest
        mov r10, rdx    ; r10 = alto en lineas
        mov r11, rcx    ; r11 = contador ancho en pixels
        mov r15, rcx
        add r15, rcx
        add r15, rcx    ; r15 = ancho en bytes
        
cicloLasFilas:
        ;sub r10, 5      ; r10= cantidad de filas de ventanas que me quedan
        dec r10        ; mientras testeo, estoy levantando ventanas de una sola linea
    cicloLasColumnas:        
        sub r11, 1      ; r11= cantidad de ventanas restantes en la fila actual
        ;
        movdqu xmm7, [lastOff]
        mov r12, r8     ; puntero a la ventana actual en src
        mov r13, r9     ; puntero a la ventana actual en dst
;;;;;;;;
        ;  la primera lina de la ventana la copio xq queda igual
        movdqu xmm1, [r12]
        pand xmm1, xmm7
        ;movdqu [r13], xmm1  ; muevo el resultado a donde corresponde

        ; calculo donde arranca la segunda linea de la ventana
        add r12, r15    ; sumandole el ancho de imagen
        ; idem para dst
        add r13, r15    ; sumandole el ancho de imagen
;;;;;;;;
        ; la segunda linea de la ventana la copio xq queda igual
        movdqu xmm2, [r12]
        pand xmm2, xmm7
        ;movdqu [r13], xmm2
        pavgb xmm1, xmm2 ;;;;;;;;;;;;;;;;;;; xmm1= avg xmm1-2
;;;;;;;;; calculo donde arranca la tercera linea
        add r12, r15
        ; idem para dst
        add r13, r15
        ;movdqu xmm3, [r12]
        pand xmm3, xmm7
        pavgb xmm1, xmm3;;;;;;;;;;;; xmm1= avg xmm1-3
        ;movdqu [r13], xmm3
;;;;;;;;
        ; calculo donde arranca la cuarta linea
        add r12, r15
        ; idem para dst
        add r13, r15
        movdqu xmm4, [r12]
        pand xmm4, xmm7
        pavgb xmm1, xmm4;;;;;;;;;;;;;; xmm1= avg xmm1-4
        ;movdqu [r13], xmm4
;;;;;;;;
        ; calculo donde arranca la quinta linea
        add r12, r15
        ; idem para dst
        add r13, r15
        movdqu xmm5, [r12]
        pand xmm5, xmm7
        pavgb xmm1, xmm5;;;;;;;;; xmm1= avg xmm1-5
        ;movdqu [r13], xmm5
        ;
        movdqu xmm2, xmm1   ; copio los avg parciales
        movdqu xmm4, xmm1   ; copio denuevo
        pslldq xmm2, 3      ; muevo un pixel a izq
        pavgb xmm1, xmm3    ; calculo avg contra ese
        pslldq xmm2, 3      ; muevo otro pixel a izq
        pavgb xmm1, xmm2    ; vuelvo a calcular avg
        
        psrldq xmm4, 3      ; muevo un pixel a der
        pavgb xmm1, xmm4    ; calculo avg con ese
        psrldq xmm4, 3      ; muevo otro pixel a der
        pavgb xmm1, xmm4    ; vuelvo a calcular avg
        
        movdqu xmm2, [soloMedio]
        pand xmm1, xmm2
        movdqu xmm0, [sinMedio]
        pand xmm3, xmm0
        por xmm1, xmm3      ; xmm1 = xmm3...avg...xmm3
        psrldq xmm1, 6
        movq r12, xmm1
        sub r13, r15
        sub r13, r15        ; r13 = puntero a la tercera linea de la ventana
        mov [r13], r12d
        
        ;psadbw xmm1, xmm2   ; xmm1 = suma de promedios

        ; aca deberia usar xmm2, 3, 4 y 5 para hacer los calculos
        ; de la ventana, y guardar la media en: r8 + 2*wide + 2
        ;
        ; luego me muevo 5 pixels en src y dst
        add r8, 3
        add r9, 3
    ; comparo si llegue al final (ultima ventana de esta linea)
    cmp r11, 5
    jge cicloLasColumnas
        ;
        ; si llegue, paso a la siguiente linea de ventanas
        ;add r8, 3
        ;add r9, 3
        mov r11, rcx    ; restauro el contador de columnas
        
    cmp r10, 0      ; si todavia me faltan filas por recorrer
    jg cicloLasFilas
;; termine de pintar la imagen, estoy en la ultima fila
    ;
    
    xor rcx, rcx
ultima_negro:
    mov [r13], rcx
    add r13, 4
    cmp r13, rax
jl ultima_negro

end:
        ;;;;;;;;;; Aca finaliza la funcion ;;;;;;;;
	pop r15		;   restauro
	pop r14		;     los
	pop r13		;   registros
	pop r12		;     por
	pop rbx		; convencion C
	pop rbp		;

ret
