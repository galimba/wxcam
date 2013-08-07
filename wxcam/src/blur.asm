;~ ;void blur_asm(unsigned char *src, unsigned char *dst, int h, int w, int src row size, int dst row size)
;~ ;los registros:
;~ ;unsigned char *src -> rdi 
;~ ;unsigned char *dst -> rsi
;~ ;int h -> edx 
;~ ;int w -> ecx
;~ ;int src row size -> R8d  
;~ ;int dst row size -> R9d
;~ 
section .data
const13 dd 13.0,13.0,13.0,13.0

DEFAULT REL

section .text
global blur_asm
blur_asm:

	push rbp
	mov rbp, rsp
	push rbx
	push r12
	push r13
	push r14
	push r15
	
	;acomodar los registros
	movsxd rcx, ecx
	movsxd rdx, edx
	movsxd r8, r8d
	movsxd r9, r9d
	;~ ; la idea es procesar por partes para no quedarme sin registros.
	;~ ; suponiendo que la imagen es la siguiente:
	;~ ;        | a |
	;~ ;    | b | c | d |
	;~ ;| e | f | g | h | i |
	;~ ;    | j | k | l |
	;~ ;        | m |
	;~ ; primera partes proceso 
	;~ ; f1:       | a |
	;~ ; f2:   | b | c | d |
	;~ ; segunda parte procesos f3: | e | f | g | h | i |
	;~ ; tercera parte: 
	;~ ; f4 :  | j | k | l |
	;~ ; f5 : 		| m |
	;~ 
	;Voy a procesar de a dos porque quiero operar con floats. 
	mov r15, rcx;preservo r15 para usar el rcx en el loop
	mov r13, rcx
	;mov r13, r15
	sar r13, 1 ; porque quiero procesar de a 2
	mov r14, r13;me guardo la #iteraciones a procesar.
	;~ 
	sal r14,1;(#iteraciones)*2 cantidad de pixeles procesados.
	;sub rcx,2 ; le resto cuatro porque los dos de cada borde
	sub rcx,r14;en rcx tenemos (w - (#iteraciones)*2)
	;ahora en rcx tengo la cantidad de pixeles que faltan procesar
	
	sal rcx,2 
	;Ahora los multiplico por 4 porque necesito la cantidad de bytes
	
	xor r14,r14
	mov r14,8 ; como proceso de a dos pixeles tengo 8 bytes entonces
	sub r14,rcx ; 8 - cantidad de bytes que falta procesar = cuanto tengo que retroceder 
	
	mov r13,r14; 8-(w - (#iteraciones)*4) en r13 vamos a guardar cuanto vamos a tener que retroceder
	;en todas la filas vamos a tener que procesar la misma cantidad de pixeles por fuera del cicloColumnas.
	;y esa cantidad la vamos a guardar en r13
	;vamos a retroceder el puntero entonces	
		
	;acomodo los registros
	pxor xmm6, xmm6 ; lo uso para limpiar los registros 
	pxor xmm0, xmm0 ; lo uso de acumulador de la suma
	;~ 
	movdqu xmm7, [const13] ; lo uso de acumulador de la suma
	;cvtdq2ps xmm7, xmm7 ; la idea es que xmm7 -> 13 | 13
	
	;acomodo el alto:
	sub rdx,4 ;2 pixeles x 2 bordes  = 4
	
	;acomodo el ancho	
	sub r15,4 ;le resto cuatro por los 2 pixeles del borde 2 pixeles x 2 bordes  = 4
	sar r15,1 ;divido dos por que proceso de a dos pixeles 
	
.cicloFilas:
		;~ 
		;~ ;bajo los datos
			mov rcx, r15	
			;~ sar rcx, 1 ; divido dos porque proceso de a dos pixeles
			;mov rax,rdi
			mov r11,rdi ; r11-> tengo el puntero del source
			mov r12,rsi	; r12-> tengo el puntero del destino
			lea r12, [r12 + 8]
			lea r12, [r12 + 2*r9]
			
		.cicloColumnas:
			pxor xmm0, xmm0 ; lo uso de acumulador de la suma
			movq xmm1, [r11+8] 		; xmm1 -> f1: | a |
			movq xmm2, [r11+r8+4] 	;xmm2 -> f2: | b |
			movq xmm3, [r11+r8+8] 	;xmm3 -> f2: | c |
			movq xmm4, [r11+r8+12] 	;xmm4 -> f2: | d |
			;ya tengo la primer parte para sumar es decir:
			;f1:        | a |
			;f2:    | b | c | d |
			;los voy a extender y pasarlos a word para no perder precision en la suma:
			punpcklbw xmm1,xmm6
			punpcklbw xmm2,xmm6
			punpcklbw xmm3,xmm6
			punpcklbw xmm4,xmm6
			;sumo	
			paddw xmm0, xmm1
			paddw xmm0, xmm2
			paddw xmm0, xmm3
			paddw xmm0, xmm4 ;ahora en xmm0 tengo a + b + c + d
			
			;voy por la segunda parte de la suma:
			movq xmm1, [r11+r8*2] 		; xmm1 -> f3: | e |
			movq xmm2, [r11+r8*2+4] 	; xmm1 -> f3: | f |
			movq xmm3, [r11+r8*2+8] 	; xmm1 -> f3: | g |
			movq xmm4, [r11+r8*2+12] 	; xmm1 -> f3: | h |
			movq xmm5, [r11+r8*2+16] 	; xmm1 -> f3: | i |
			;ya tengo la segunda parte para sumar es decir:
			;f3: | e | f | g | h | i |
			;los voy a extender y pasarlos a word para no perder precision en la suma:
			punpcklbw xmm1,xmm6
			punpcklbw xmm2,xmm6
			punpcklbw xmm3,xmm6
			punpcklbw xmm4,xmm6
			punpcklbw xmm5,xmm6
			;sumo	
			paddw xmm0, xmm1 ;ahora en xmm0 tengo a + b + c + d + e
			paddw xmm0, xmm2 ;ahora en xmm0 tengo a + b + c + d + e + f
			paddw xmm0, xmm3 ;ahora en xmm0 tengo a + b + c + d + e + f + g
			paddw xmm0, xmm4 ;ahora en xmm0 tengo a + b + c + d + e + f + g + h
			paddw xmm0, xmm5 ;ahora en xmm0 tengo a + b + c + d + e + f + g + h + i
			
			;OPERO PARA CONSEGUIR LOS INDICES DE LA TERCER FILA.
			mov rax, r8
			add rax, r8
			add rax, r8
			;voy por la tercer parte de la suma:
			movq xmm1, [r11+rax+4]		; xmm1 -> f4: | j |
			movq xmm2, [r11+rax+8]	 	; xmm2 -> f4: | k |
			movq xmm3, [r11+rax+12] 	; xmm3 -> f4: | l |
			movq xmm4, [r11+r8*4+8] 	; xmm4 -> f5: | m |
			;ya tengo la tercera parte para sumar es decir:
			; f4 :  | j | k | l |
			; f5 : 		| m |

			;los voy a extender y pasarlos a word para no perder precision en la suma:
			punpcklbw xmm1,xmm6
			punpcklbw xmm2,xmm6
			punpcklbw xmm3,xmm6
			punpcklbw xmm4,xmm6
			
			;sumo	
			paddw xmm0, xmm1 ;ahora en xmm0 tengo a + b + c + d + e + f + g + h + i + j
			paddw xmm0, xmm2 ;ahora en xmm0 tengo a + b + c + d + e + f + g + h + i + j + k 
			paddw xmm0, xmm3 ;ahora en xmm0 tengo a + b + c + d + e + f + g + h + i + j + k + l 
			paddw xmm0, xmm4 ;ahora en xmm0 tengo a + b + c + d + e + f + g + h + i + j + k + l + m
			; ahora en xmm0 tengo la suma de todos los subindices.
			
			movdqu xmm8, xmm0     ; | (a+...+m)r1 | (a+...+m)g1 | (a+...+m)b1 | (a+...+m)@1 | (a+...+m)r2 | (a+...+m)g2 | (a+...+m)b2 | (a+...+m)@2 |
			punpckhwd xmm8,xmm6   ; | (a+...+m)r1 | (a+...+m)g1 | (a+...+m)b1 | (a+...+m)@1 |
			punpcklwd xmm0, xmm6  ; | (a+...+m)r2 | (a+...+m)g2 | (a+...+m)b2 | (a+...+m)@2 | 
			
			;convierto a floats
			cvtdq2ps xmm0, xmm0
			cvtdq2ps xmm8, xmm8
				
			;ahora divido
			divps xmm0, xmm7
			divps xmm8, xmm7
			
			;convierto a enteros
			cvtps2dq xmm0, xmm0
			cvtps2dq xmm8, xmm8
			
			;empaqueto de nuevo paso a word
			packusdw xmm0, xmm8
			
			;convierto a byte
			packuswb xmm0, xmm0	
			movq [r12],xmm0
			
		;avanzo punteros	
		add r11, 8
		add r12, 8
		dec rcx
		cmp rcx,0
		jne .cicloColumnas
		;~ 
		
		;acomodo el  puntero.
		sub r11,r13
		sub r12,r13
		
		; los pixeles por fuera del ciclo :)
			pxor xmm0, xmm0 ; lo uso de acumulador de la suma
			movq xmm1, [r11+8] 		; xmm1 -> f1: | a |
			movq xmm2, [r11+r8+4] 	;xmm2 -> f2: | b |
			movq xmm3, [r11+r8+8] 	;xmm3 -> f2: | c |
			movq xmm4, [r11+r8+12] 	;xmm4 -> f2: | d |
			;ya tengo la primer parte para sumar es decir:
			;f1:        | a |
			;f2:    | b | c | d |
			;los voy a extender y pasarlos a word para no perder precision en la suma:
			punpcklbw xmm1,xmm6
			punpcklbw xmm2,xmm6
			punpcklbw xmm3,xmm6
			punpcklbw xmm4,xmm6
			;sumo	
			paddw xmm0, xmm1
			paddw xmm0, xmm2
			paddw xmm0, xmm3
			paddw xmm0, xmm4 ;ahora en xmm0 tengo a + b + c + d
			
			;voy por la segunda parte de la suma:
			movq xmm1, [r11+r8*2] 		; xmm1 -> f3: | e |
			movq xmm2, [r11+r8*2+4] 	; xmm1 -> f3: | f |
			movq xmm3, [r11+r8*2+8] 	; xmm1 -> f3: | g |
			movq xmm4, [r11+r8*2+12] 	; xmm1 -> f3: | h |
			movq xmm5, [r11+r8*2+16] 	; xmm1 -> f3: | i |
			;ya tengo la segunda parte para sumar es decir:
			;f3: | e | f | g | h | i |
			;los voy a extender y pasarlos a word para no perder precision en la suma:
			punpcklbw xmm1,xmm6
			punpcklbw xmm2,xmm6
			punpcklbw xmm3,xmm6
			punpcklbw xmm4,xmm6
			punpcklbw xmm5,xmm6
			;sumo	
			paddw xmm0, xmm1 ;ahora en xmm0 tengo a + b + c + d + e
			paddw xmm0, xmm2 ;ahora en xmm0 tengo a + b + c + d + e + f
			paddw xmm0, xmm3 ;ahora en xmm0 tengo a + b + c + d + e + f + g
			paddw xmm0, xmm4 ;ahora en xmm0 tengo a + b + c + d + e + f + g + h
			paddw xmm0, xmm5 ;ahora en xmm0 tengo a + b + c + d + e + f + g + h + i
			
			;OPERO PARA CONSEGUIR LOS INDICES DE LA TERCER FILA.
			mov rax, r8
			add rax, r8
			add rax, r8
			;voy por la tercer parte de la suma:
			movq xmm1, [r11+rax+4]		; xmm1 -> f4: | j |
			movq xmm2, [r11+rax+8]	 	; xmm2 -> f4: | k |
			movq xmm3, [r11+rax+12] 	; xmm3 -> f4: | l |
			movq xmm4, [r11+r8*4+8] 	; xmm4 -> f5: | m |
			;ya tengo la tercera parte para sumar es decir:
			; f4 :  | j | k | l |
			; f5 : 		| m |

			;los voy a extender y pasarlos a word para no perder precision en la suma:
			punpcklbw xmm1,xmm6
			punpcklbw xmm2,xmm6
			punpcklbw xmm3,xmm6
			punpcklbw xmm4,xmm6
			
			;sumo	
			paddw xmm0, xmm1 ;ahora en xmm0 tengo a + b + c + d + e + f + g + h + i + j
			paddw xmm0, xmm2 ;ahora en xmm0 tengo a + b + c + d + e + f + g + h + i + j + k 
			paddw xmm0, xmm3 ;ahora en xmm0 tengo a + b + c + d + e + f + g + h + i + j + k + l 
			paddw xmm0, xmm4 ;ahora en xmm0 tengo a + b + c + d + e + f + g + h + i + j + k + l + m
			; ahora en xmm0 tengo la suma de todos los subindices.
			
			movdqu xmm8, xmm0     ; | (a+...+m)r1 | (a+...+m)g1 | (a+...+m)b1 | (a+...+m)@1 | (a+...+m)r2 | (a+...+m)g2 | (a+...+m)b2 | (a+...+m)@2 |
			punpckhwd xmm8,xmm6   ; | (a+...+m)r1 | (a+...+m)g1 | (a+...+m)b1 | (a+...+m)@1 |
			punpcklwd xmm0, xmm6  ; | (a+...+m)r2 | (a+...+m)g2 | (a+...+m)b2 | (a+...+m)@2 | 
			
			;convierto a floats
			cvtdq2ps xmm0, xmm0
			cvtdq2ps xmm8, xmm8
				
			;ahora divido
			divps xmm0, xmm7
			divps xmm8, xmm7
			
			;convierto a enteros
			cvtps2dq xmm0, xmm0
			cvtps2dq xmm8, xmm8
			
			;empaqueto de nuevo paso a word
			packusdw xmm0, xmm8
			
			;convierto a byte
			packuswb xmm0, xmm0	
			movq [r12],xmm0
			;terminé la fila
			
			
	add rdi,r8
	add rsi,r9
 
	dec rdx
	cmp rdx,0
	jne .cicloFilas
    

.fin:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rbp		
	ret



