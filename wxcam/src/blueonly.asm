;blueonly_asm(unsigned char* frame, int size, int color);
global blueonly_asm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;      channel filter        ;
; Autores:                   ;
;      galimba@gmail.com     ;
; adrian.bonaccini@gmail.com ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
section .rodata

mask_r: db 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF
mask_g: db 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00
mask_b: db 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00

section .text

blueonly_asm:
        ; los parametros son pasados asi:
        ;	rdi= char* frame
        ;	rsi= int size
        ;       rdx= color(0=red, 2=green, 4=blue... otra cosa y la cagamos)
        
	push rbp	;     reservo
	mov rbp,rsp	;       la
	push rbx	;      pila
	push r12	;        y
	push r13	;      guardo
	push r14	;    registros
	push r15	;       (...)

	;;;;;;;;;; Aca comienza la funcion ;;;;;;;;
        mov r12, rdx    ; salvo el parametro del color
	xor rdx, rdx	; limpio dividendo:high
	mov rax, rsi	; el dividendo:low = size
	mov rcx, 0xF	; la constante 15 por la que divido
	div rcx         ; divido
	mov rcx, rax	; el resultado deberia estar en rdx:rax, lo muevo al rcx (contador)
                        ; rax ya esta libre, lo uso como indice
        mov rdx, 4      ; 0=red, 2=green, 4=blue
	lea rbx, [mask_r + 8*r12]
        movdqu xmm0, [rbx]
.ciclo:
	; cargo los pixels en el registro
	movdqu xmm1, [rdi]	; xmm0 = aR|aG|aB|bR | bG|bB|cR|cG | cB|dR|dG|dB | eR|eG|eB|fR = (5 pixels abcde + 1 byte de f)
	pand xmm1, xmm0

        movdqu [rdi], xmm1  ; devuelvo a memoria los pixels cambiados
	add rdi, 15         ; avanzo el puntero 15, porque cambie 5 pixels
	dec rcx
	cmp qword rcx, 0
    jnz .ciclo

;;;;;;;;; -------------------- ;;;;;;;;;;

	pop r15		;   restauro
	pop r14		;     los
	pop r13		;   registros
	pop r12		;     por
	pop rbx		; convencion C
	pop rbp		;

ret






