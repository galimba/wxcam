global negative_asm ;negative_asm(unsigned char* frame, int size);
section .data

allF: db 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;      Negative filter       ;
; Autores:                   ;
;      galimba@gmail.com     ;
; adrian.bonaccini@gmail.com ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
section .text

negative_asm:
        ; los parametros son pasados asi:
        ;	rdi= char* frame
        ;	rsi= int size
        ;
	push rbp	;     reservo
	mov rbp,rsp	;       la
	push rbx	;      pila
	push r12	;        y
	push r13	;      guardo
	push r14	;    registros
	push r15	;       (...)

	;;;;;;;;;; Aca comienza la funcion ;;;;;;;;
	xor rcx, rcx	; limpio el reg ;;;;;;;;;;;;;;;;;; CREO Q NO HACE FALTA
	mov rcx, rsi	; rcx <- size
	sar rcx, 4	    ; dividimos xq procesamos de a 16 bytes/canales
        
.ciclo:
    ; cargo los pixels en el registro
    movdqu xmm1, [rdi]	; xmm1 = aR|aG|aB|bR|...|eB|fR = (5 pixels abcde + 1 byte de f)	
    movdqu xmm2, [allF]
    ;pxor xmm2, xmm2     ; xmm2 <- 0
    psubsb xmm2, xmm1	; calculo el negativo complemento a dos
    movdqu [rdi], xmm2	; lo devuelvo al buffer

    ;movdqu [rdi], xmm1	; devuelvo a memoria los pixels cambiados
    add rdi, 16 	; avanzo el puntero
  loop .ciclo

	pop r15		;   restauro
	pop r14		;     los
	pop r13		;   registros
	pop r12		;     por
	pop rbx		; convencion C
	pop rbp		;

ret


