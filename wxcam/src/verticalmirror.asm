global verticalMirror_asm ;verticalMirror_asm(unsigned char* frame, int size, int width, height);
section .data

shuffle: dd 0x0E0D0C0F, 0x060B0A09, 0x04030807, 0x02010005
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   Vertical Mirror filter   ;
; Autores:                   ;
;      galimba@gmail.com     ;
; adrian.bonaccini@gmail.com ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
section .text

verticalMirror_asm:
        ; los parametros son pasados asi:
        ;	rdi= char* frame
        ;	rsi= int size
        ;       rdx= int width
        ;       rcx= int height !!!
        ;
	push rbp	;     reservo
	mov rbp,rsp	;       la
	push rbx	;      pila
	push r12	;        y
	push r13	;      guardo
	push r14	;    registros
	push r15	;       (...)

	;;;;;;;;;; Aca comienza la funcion ;;;;;;;;
        mov r12, rdx
        sar r12, 1      ; r12 = ancho/2
        ;lea r15, [rdi + rsi - 1] ; r15 = (img+size) para controlar el final
        lea r15, [rdi + rsi]; r15 = (img+size) para controlar el final
        xor rbx, rbx
        
.ciclo_largo:
        
        .ciclo_corto:
            pxor xmm1, xmm1
            lea r14, [rdi + rbx] ; cargo la dir. de memoria que quiero levantar
            movdqu xmm1, [r14]	; xmm1 = aR|aG|aB|bR|...|eB|fR = (5 pixels abcde + 1 byte de f)	
            ;lea r13, [r14 + r12] ; cargo el destino
            lea r13, [rdi+rdx]  ; r13 = primer byte de la siguiente linea
            sub r13, rbx        ; r13 = lugar donde termina el cacho de imagen a mover
            sub r13, 16         ; r13 = destino a mover la imagen
            ; ahora tengo que dar vuelta los pixels
            ; uso la mascara
            ; NOTA: acomodar para usar 15 bytes, por el orden inverso de pixels
            movdqu xmm2, [shuffle]
            pshufb xmm1, xmm2
                       
            
            ; devuelvo a memoria la imagen espejada
            movdqu [r13], xmm1  ; guardo los datos
            add rbx, 15 ; aumento el contador
            cmp rbx, r12    ; comparo si llegue a media pantalla levantando
            jne .ciclo_corto
            
            add rdi, rdx    ; paso a la siguiente fila
            xor rbx, rbx    ; limpio el contador
        cmp rdi, r15    ; comparo si llegue al final de la imagen
        jl .ciclo_largo

	pop r15		;   restauro
	pop r14		;     los
	pop r13		;   registros
	pop r12		;     por
	pop rbx		; convencion C
	pop rbp		;

ret





