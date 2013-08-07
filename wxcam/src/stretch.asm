;stretch_asm(unsigned char* frame, unsigned char* dst, int height, int width, int size);
global stretch_asm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;      X2 Zoom filter        ;
; Autores:                   ;
;      galimba@gmail.com     ;
; adrian.bonaccini@gmail.com ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section .rodata
ALIGN 16
lastOff:     dd 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00FFFFFF
sinMedio: db 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
soloMedio: db 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
shuffle_1:   dd 0x00020100, 0x04030201, 0x05040305, 0xFFFFFFFF 

section .text

stretch_asm:
        ; los parametros son pasados asi:
        ;	rdi= char* src
        ;	rsi= char* dst
        ;       rdx= int height
        ;       rcx= int width en pixels
        ;       r8 = int size
	push rbp	;     reservo
	mov rbp,rsp	;       la
	push rbx	;      pila
	push r12	;        y
	push r13	;      guardo
	push r14	;    registros
	push r15	;       (...)

	;;;;;;;;;; Aca comienza la funcion ;;;;;;;;
        mov r8, rdi     ; r8 = frame
        mov r9, rsi     ; r9 = dest
        
        
        mov r14, rcx    ; voy a calcular el ancho de src en bytes
        add r14, rcx
        add r14, rcx    ; r14 = 3 * rcx
        mov rcx, r14    ; rcx = width en bytes
        sar r14, 1      ; r14 = ancho inicial de src en bytes
        mov r11, r14
        
        mov r10, rdx    ; r10 = alto de src en lineas
        sar r10, 1
        movdqa xmm0, [shuffle_1]
cicloLasFilas:
        sub r10, 1        ; correccion de emi, tengo que estirar copiando dos lineas
    cicloLasColumnas:        
        sub r11, 6      ; r11= cantidad de bytes restantes en la fila actual
        mov r12, r8     ; puntero a la linea actual en src
        mov r13, r9     ; puntero a la linea actual en dst
        ;;;;;
        movdqu xmm1, [r12]
        pshufb xmm1, xmm0
        movdqu [r13], xmm1
        ;mov dword [r13 + r14*2], 0h    ; probando con lineas negras
        movdqu [r13 + rcx], xmm1
        ;;;;;;
        add r8, 6
        add r9, 12
    cmp r11, 0
    jg cicloLasColumnas
        ;
        add r8, r14; media fila en src
        add r9, rcx


        mov r11, r14    ; restauro el contador de columnas
    cmp r10, 0      ; si todavia me faltan filas por recorrer
    jg cicloLasFilas

.end:

	pop r15		;   restauro
	pop r14		;     los
	pop r13		;   registros
	pop r12		;     por
	pop rbx		; convencion C
	pop rbp		;

ret
