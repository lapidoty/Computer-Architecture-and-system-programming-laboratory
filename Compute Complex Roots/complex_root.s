section .data
	inputFirstFormat:
		dq "epsilon = %lf order = %ld \n" ,0
	inputSecondFormat:
		dq "coeff %ld = %lf %lf \n" , 0
	inputThirdFormat:
		dq "initial = %lf %lf" 
	outputFormat4:
		dq "root = %.16e %.16e " , 10 , 0

section .bss
	;Constants globals:
	epsilonStore: resq 1
	orderStore: resq 1
	index: resq 1
	currOrder: resq 1
	currCoeff: resb COMPLEX_size

	;Temporery intertaion globals:
	initial: resb COMPLEX_size


	;Temporery results globals:
	result: resb COMPLEX_size
	f_result: resb COMPLEX_size
	add_result: resb COMPLEX_size
	sub_result: resb COMPLEX_size
	mult_result: resb COMPLEX_size
	div_result: resb COMPLEX_size
	der_result: resb COMPLEX_size

	;Array pointers:
	ptrArray: resq 1
	ptrDerivativeArray: resq 1
	
section .text
	global main
	extern printf,scanf
	extern malloc
	extern realloc
	extern free

struc COMPLEX
  .a  resq 1
  .b  resq 1
	endstruc

%macro SET_POINTER_TO_RDX 3
    mov rdx , %1
	add rdx , %2
	add rdx , %3
  %endmacro

main:
	nop
	enter 0, 0		; prepare a frame
	mov qword [index], 0

store_epsilon_and_order:

  .call_scanf:
	lea rdi , [inputFirstFormat] 	; call scanf for epsilon and order:
	lea rsi , [epsilonStore]
	lea rdx , [orderStore]
	call scanf

  .allocate_polynom_array:
	mov rax, [orderStore] 	; Our arrays will contain orderStore+1 elements:
	inc rax
	sal rax, 4		; Each element will be treated as an float. (eg. count * sizeof(float))
	mov rdi , rax 	
	call malloc 	; We allocate our elements on the heap.
	mov qword [ptrArray] , rax 	; And we save a pointer to the first element in our array.
	mov qword rbx , [ptrArray] ;****
	
	mov qword rbx , [ptrArray] ;****

	mov rbx , [orderStore]
	mov [index] , rbx
store_coeffs:

	

  .call_scanf:
	lea rdi , [inputSecondFormat] 	; And we store the current coeff in it.
	lea rsi , [currOrder]
	lea rdx , [currCoeff+8]
	lea rcx , [currCoeff+16]
	call scanf

  .get_offset:
	mov eax, COMPLEX_size
	mov edx, [currOrder] ; index
	mul edx

	movsd xmm0 , [currCoeff+8]
	SET_POINTER_TO_RDX [ptrArray] , rax , 8
	movsd [rdx] , xmm0
	movsd xmm1 , [currCoeff+16]
	SET_POINTER_TO_RDX [ptrArray] , rax , 16
	movsd [rdx] , xmm1
	

  .end:
	dec qword [index] 	; inc index.
	mov r9 , [index]
	cmp r9 , 0
	jge store_coeffs

	mov qword rbx , [ptrArray] ;****
store_initial:

	.call_scanf:
	lea rdi , [inputThirdFormat] 	; call scanf for epsilon and order:
	lea rsi , [initial+8]
	lea rdx, [initial+16]
	call scanf

	mov qword rbx , [ptrArray] ;****
	.allocate_derivative_array:
	mov rax, [orderStore] 	; Our arrays will contain orderStore elements:
	sal rax, 4		; Each element will be treated as an float. (eg. count * sizeof(float))
	mov rdi , rax 	
	call malloc 	; We allocate our elements on the heap.
	mov qword [ptrDerivativeArray] , rax 	; And we save a pointer to the first element in our array.

;******Local vars prepartion*****

struc mainLocals
.old_rbp: resq 1
.Loc0: resq 1
.Loc1: resq 1
.Loc2: resq 1
.Loc3: resq 1
.Loc4: resq 1
.Loc5: resq 1
.Loc6: resq 1
.Loc7: resq 1
.Loc8: resq 1
.Loc9: resq 1
endstruc

%define local_var_main(offset) qword [rbp - offset]

%define f_y0_a local_var_main(operationLocals.Loc0)
%define f_y0_b local_var_main(operationLocals.Loc1)
%define f_der_y0_a local_var_main(operationLocals.Loc2)
%define f_der_y0_b local_var_main(operationLocals.Loc3)
%define y0_a local_var_main(operationLocals.Loc4)
%define y0_b local_var_main(operationLocals.Loc5)
%define y1_a local_var_main(operationLocals.Loc6)
%define y1_b local_var_main(operationLocals.Loc7)
%define root_a local_var_main(operationLocals.Loc8)
%define root_b local_var_main(operationLocals.Loc9)
Main_loop:

	.initialize: 
		movsd xmm0, [initial+8]
		movsd qword y0_a, xmm0
		movsd xmm0, [initial+16]
		movsd qword y0_b, xmm0
	.loop:
	  .calc_f:	
		finit
		fld qword y0_a         
		fld qword y0_b

		mov rdi , [ptrArray]
		movsd xmm0 , y0_a
		movsd xmm1 , y0_b
		mov rsi , [f_result+8]
		mov rdx , [f_result+16]
		mov rcx , [orderStore]
		sub rsp , 88
		call _Eval_poly ;***1***
		add rsp , 88
		movsd xmm0 , [f_result+8]
		movsd xmm1 , [f_result+16]

		finit
		fld qword [f_result+8]         
		fld qword [f_result+16] 


		movsd f_y0_a , xmm0
		movsd f_y0_b , xmm1
		mov qword [f_result+8] , 0
		mov qword [f_result+16] , 0
	

		finit
		fld qword f_y0_a         
		fld qword f_y0_b 

	  .calc_f_der:	
		sub rsp , 88
		call _Cmplx_der
		add rsp , 88
		mov rdi , [ptrDerivativeArray]
		movsd xmm0 , y0_a
		movsd xmm1 , y0_b
		mov rsi , [f_result+8]
		mov rdx , [f_result+16]
		mov rcx , [orderStore]
		dec rcx
		cmp rcx , 0
		je .Zero
		sub rsp , 88
		call _Eval_poly ;***2***
		add rsp , 88
		movsd xmm0 , [f_result+8]
		movsd xmm1 , [f_result+16]
		movsd f_der_y0_a , xmm0
		movsd f_der_y0_b , xmm1
		jmp .notZero
		.Zero:
		mov r15 , rcx
		mov eax, COMPLEX_size      
		mov edx, r15d ; index
		mul edx
		SET_POINTER_TO_RDX rdi , rax , 8 
		movsd xmm2, [rdx]
		SET_POINTER_TO_RDX rdi , rax , 16 
		movsd xmm3, [rdx]
		movsd f_der_y0_a , xmm2
		movsd f_der_y0_b , xmm3

		.notZero:
		finit
		fld qword f_der_y0_a         
		fld qword f_der_y0_b         

		mov qword [f_result+8] , 0
		mov qword [f_result+16] , 0

		.calc_divison:
		movsd xmm0 , f_y0_a
		movsd xmm1 , f_y0_b
		movsd xmm2 , f_der_y0_a
		movsd xmm3 , f_der_y0_b
		sub rsp , 88
		call _Cmplx_div
		add rsp , 88
		movsd xmm0 , [div_result+8]
		movsd xmm1 , [div_result+16]
		movsd [f_result+8] , xmm0
		movsd [f_result+16] , xmm1

		finit
		fld qword [f_result+8]         
		fld qword [f_result+16] 

	  .calc_y0_sub_div_result:
		movsd xmm0 , y0_a
		movsd xmm1 , y0_b
		movsd xmm2 , [f_result+8]
		movsd xmm3 , [f_result+16]
		sub rsp , 88
		call _Cmplx_sub
		add rsp , 88
		movsd xmm0 , [sub_result+8]
		movsd xmm1 , [sub_result+16]
		movsd y1_a , xmm0
		movsd y1_b , xmm1
		movsd root_a , xmm0
		movsd root_b , xmm1

		finit
		fld qword y1_a        
		fld qword y1_b


	  .compare_distacne_y1_y0:
		mov rdi , [ptrArray]
		movsd xmm0 , y1_a
		movsd xmm1 , y1_b
		mov rsi , [f_result+8]
		mov rdx , [f_result+16]
		mov rcx , [orderStore]
		sub rsp , 88
		call _Eval_poly ;***3***
		add rsp , 88
	

		finit
		fld qword [f_result+8]         ;st0 = a
		fld qword [f_result+8]         ;st0 = a, st1 = a
		fmul st0 , st1                  ;st0 = a*a, st1 = a
		mov qword [f_result+8] , 0
		fst qword [f_result+8]

		finit
		fld qword [f_result+16]         ;st0 = b
		fld qword [f_result+16]         ;st0 = b, st1 = b
		fmul st0 , st1                    ;st0 = b*b, st1 = b
		mov qword [f_result+8] , 0
		fst qword [f_result+16]

		finit
		fld qword [f_result+8]         ;st0 = a
		fld qword [f_result+16]         ;st0 = b, st1 = a
		fadd st0 , st1                 ;st0 = b+a, st1 = a
		mov qword [f_result+8] , 0
		fst qword [f_result+8]

		finit
		fld qword [f_result+8]         
		fsqrt     
		mov qword [f_result+8] , 0                        
		fst qword [f_result+8]

		finit
		fld qword [epsilonStore] 
		fld qword [f_result+8]          
		fcomi                             
		;fst qword [f_result+8]
		jb .end
		movsd xmm0 , y1_a
		movsd y0_a , xmm0
		movsd xmm0 , y1_b
		movsd y0_b , xmm0
		jmp .loop

	  .end:
		finit
		fld qword y1_a        
		fld qword y1_b 
		lea rdi , [outputFormat4]
		movsd xmm0 , y1_a
		movsd xmm1 , y1_b
		mov rax , 2
		call printf

	leave			; dump the top frame
	ret			; return from main


Operations:
;******Local vars prepartion*****
struc operationLocals
.old_rbp: resq 1
.Loc0: resq 1
.Loc1: resq 1
.Loc2: resq 1
.Loc3: resq 1
.Loc4: resq 1
.Loc5: resq 1
.Loc6: resq 1
.Loc7: resq 1
.Loc8: resq 1
.Loc9: resq 1
endstruc

%define local_var_operations(offset) qword [rbp - offset]

%define op_a local_var_operations(operationLocals.Loc0)
%define op_b local_var_operations(operationLocals.Loc1)
%define op_c local_var_operations(operationLocals.Loc2)
%define op_d local_var_operations(operationLocals.Loc3)
%define result_R local_var_operations(operationLocals.Loc4)
%define result_I local_var_operations(operationLocals.Loc5)

;Local vars for divion only:
%define numerator_R local_var_operations(operationLocals.Loc6)
%define numerator_I local_var_operations(operationLocals.Loc7)
%define denominator_R local_var_operations(operationLocals.Loc8)
%define denominator_I local_var_operations(operationLocals.Loc9)


;******Addition*****
;Name:
;	_Cmplx_add
;Parameters:
;   1) xmm0 - a (real part of oprand1)
;   2) xmm1 - b (imginary part of oprand1)
;   3) xmm2 - c (real part of oprand2)
;   4) xmm3 - d (imginary part of oprand2)
;   5) rdx - add_result pointer
;Returns:
;      add_result

_Cmplx_add:
	nop
	enter 0, 0		; prepare a frame
	mov qword [add_result+8] , 0
	mov qword [add_result+16] , 0
	movsd op_a , xmm0
	movsd op_b , xmm1
	movsd op_c , xmm2
	movsd op_d , xmm3
	
	finit
	fld qword op_a          ; st0 = a
	fld qword op_c          ; st0 = c, st1 = a
	fadd                    ; st0 = a+c
	fst qword [add_result+8] ; put in the add_result real part the a+c
	
	fld qword op_b            ;st0 = b
	fld qword op_d            ;st0 = d, st1 = b
	fadd                      ;st0 = d+b
	fst qword [add_result+16] ; put in the add_result imaginary part the b+d

	;implement here

	leave			; dump the top frame
	ret			; return from main


;******Substraction*****
;Name:
;	_Cmplx_sub
;Parameters:
;   1) xmm0 - a (real part of oprand1)
;   2) xmm1 - b (imginary part of oprand1)
;   3) xmm2 - c (real part of oprand2)
;   4) xmm3 - d (imginary part of oprand2)
;   5) rdx - sub_result pointer
;Returns:
;      sub_result

_Cmplx_sub:
	nop
	enter 0, 0		; prepare a frame
	mov qword [sub_result+8] , 0
	mov qword [sub_result+16] , 0
	movsd op_a , xmm0
	movsd op_b , xmm1
	movsd op_c , xmm2
	movsd op_d , xmm3

	finit
	fld qword op_a				; st0 = a
	fld qword op_c  			; st0 = c
	fsub			; st0 = a - c
	fst qword [sub_result+8]

	fld qword op_b				; st0 = b
	fld qword op_d  			; st0 = d
	fsub			; st0 = b - d
	fst qword [sub_result+16]
	leave			; dump the top frame
	ret			; return from main


;******Multiply*****
;Name:
;	_Cmplx_mult
;Parameters:
;   1) xmm0 - a (real part of oprand1)
;   2) xmm1 - b (imginary part of oprand1)
;   3) xmm2 - c (real part of oprand2)
;   4) xmm3 - d (imginary part of oprand2)
;   5) rdx - mult_result pointer
;Returns:
;      mult_result

_Cmplx_mult:
	nop
	enter 0, 0		; prepare a frame
	mov qword [mult_result+8] , 0
	mov qword [mult_result+16] , 0
    movsd op_a , xmm0
	movsd op_b , xmm1
	movsd op_c , xmm2
	movsd op_d , xmm3
	
	finit

	
	fld qword op_a         ;st0 = a
	fld qword op_c         ;st0 = c, st1 = a
	fmul                   ;st0 = a*c, st1=a
	fld qword op_b         ;st0 = b, st1= a*c, st2 = a
	fld qword op_d         ;st0 = d, st1 = b, st2 = a*c, st3= a
	fmul                   ;st0 = b*d, st1 = b, st2 = a*c, st3= a
	fsubr st0, st1             ;st0 = ac-bd, st1=b, st2=a*c, st3 = a
	fst qword [mult_result+8] ;puts in the real part of result the ac-bd
	
	fld qword op_a         ;st0 = a
	fld qword op_d         ;st0 = d, st1 = a
	fmul                   ;st0 = a*d, st1 = a
	fld qword op_b         ;st0 = b, st1 = a*d, st2 = a
	fld qword op_c         ;st0 = c, st1 = b, st2 = a*d, st3 = a
	fmul                   ;st0 = b*c, st1 = b, st2 = a*d, st3 = a
	fadd st0, st1              ;st0 = bc+ad, st1 = b, st2 = a*d, st3 = a
	fst qword [mult_result+16] ;puts in the imaginary part of result the bc+ad

	leave			; dump the top frame
	ret			; return from main


;******Divison*****
;Name:
;	_Cmplx_div
;Parameters:
;   1) xmm0 - a (real part of oprand1)
;   2) xmm1 - b (imginary part of oprand1)
;   3) xmm2 - c (real part of oprand2)
;   4) xmm3 - d (imginary part of oprand2)
;   5) rdx - div_result pointer
;Returns:
;      div_result

_Cmplx_div:
	nop
	enter 0, 0		; prepare a frame
	finit

		.prepare_memory:
		mov qword [div_result+8] , 0
		mov qword [div_result+16] , 0
		movsd op_a , xmm0
		movsd op_b , xmm1
		movsd op_c , xmm2
		movsd op_d , xmm3

		fld qword op_a         ;st0 = a
		fld qword op_c         ;st0 = c, st1 = a
		fmul                   ;st0 = a*c, st1=a
		fld qword op_b         ;st0 = b, st1= a*c, st2 = a
		fld qword op_d         ;st0 = d, st1 = b, st2 = a*c, st3= a
		fmul                   ;st0 = b*d, st1 = b, st2 = a*c, st3= a
		fadd st0, st1          ;st0 = bc+ad, st1 = b, st2 = a*d, st3 = a
		fst qword numerator_R

		fld qword op_b         ;st0 = b
		fld qword op_c         ;st0 = c, st1 = b
		fmul                   ;st0 = b*c, st1=b
		fld qword op_a         ;st0 = a, st1= b*c, st2 = b
		fld qword op_d         ;st0 = d, st1 = a, st2 = b*c, st3= b
		fmul                   ;st0 = a*d, st1 = a, st2 = b*c, st3= b
		fsubr st0, st1          ;st0 = bc-ad, st1 = a, st2 = b*d, st3 = b
		fst qword numerator_I

		fld qword op_c         ;st0 = a
		fld qword op_c         ;st0 = c, st1 = c
		fmul                   ;st0 = c*c, st1=c
		fld qword op_d         ;st0 = d, st1= c*c, st2 = c
		fld qword op_d         ;st0 = d, st1 = d, st2 = c*c, st3= c
		fmul                   ;st0 = d*d, st1 = d, st2 = c*c, st3= c
		fadd st0, st1          ;st0 = c*c+d*d, st1 = d, st2 = c*c, st3 = c
		fst qword denominator_R

		fld qword numerator_R       
		fld qword denominator_R       
		fdiv              
		fst qword [div_result+8]

	
		fld qword numerator_I               
		fdiv st0, st2                  
		fst qword [div_result+16]

	leave			; dump the top frame
	ret			; return from main


;******Derivative*****
;Name:
;	_Cmplx_der
;Returns:
;      Derivative array

_Cmplx_der:
	nop
	enter 0, 0		; prepare a frame

	mov rbx , [orderStore]
	mov [index] , rbx
	mov r10 , [orderStore]
	dec r10

	.loop:
	.get_regular_offset:
		mov eax, COMPLEX_size
		mov edx, [index] ; index
		mul edx

	.mult_call:

		finit
		SET_POINTER_TO_RDX [ptrArray] , rax , 8 
		movsd xmm0 , [rdx]
		fld qword [rdx]  
		SET_POINTER_TO_RDX [ptrArray] , rax , 16 
		movsd xmm1 , [rdx]
        fld qword [rdx] 
		

		movsd op_a , xmm0
		movsd op_b , xmm1

		mov eax, COMPLEX_size
		mov edx, r10d ; index
		mul edx
		

		finit
		fld qword op_a         ;st0 = a
		fild qword [index]      ;st0 = index, st1 = a
		fmul                   ;st0 = a*index, st1=a
		SET_POINTER_TO_RDX [ptrDerivativeArray] , rax , 8 
		fst qword [rdx]
		fld qword [rdx]
		finit
		fld qword op_b         ;st0 = b
		fild qword [index]      ;st0 = index, st1 = b
		fmul                   ;st0 = b*index, st1=b
		SET_POINTER_TO_RDX [ptrDerivativeArray] , rax , 16 
		fst qword [rdx]

		dec qword [index] 	; inc index.
		dec r10
		mov r9 , [index]
		cmp r9 , 0
		je .end
		jmp .loop

	.end:
	leave			; dump the top frame
	ret			; return from main



;******Polynom calc*****
;Name:
;	_Eval_poly
;Parameters:
;   1) rdi - array pointer to be calculated ( Polynom/Derivative ),
;   2) xmm0 - y0.a/x0.a value as param to the polynom
;   3) xmm1 - y0.b/x0.b value as param to the polynom
;	4) rcx - order of array
;Returns:
;      f_result

_Eval_poly: 
	enter 0, 0		; prepare a frame

	.checks1:
		finit
		movsd [initial+8] , xmm0
		fld qword [initial+8] 
		movsd [initial+16] , xmm1
		fld qword [initial+8]  

	.prepare_first_offset:
		mov r15, rcx
		mov eax, COMPLEX_size      
		mov edx, r15d ; index
		mul edx
	
		SET_POINTER_TO_RDX rdi , rax , 8 
		movsd xmm2, [rdx]
		SET_POINTER_TO_RDX rdi , rax , 16 
		movsd xmm3, [rdx]

		cmp r15 , 0
		je .checks3
	.checks2.2:   
    	finit
		movsd [f_result+16] , xmm2
		fld qword [f_result+16] 
		movsd [f_result+16] , xmm3
		fld qword [f_result+16]    
	
  .horner_loop:                    
    
    .call_mult:
        movsd xmm0 , [initial+8]               
        movsd xmm1 , [initial+16]     
		
        sub rsp, 88
		call _Cmplx_mult                        ;multiplying y0 with the coeff
		add rsp, 88

	.checks2:
		finit
		fld qword [mult_result+8] 
		fld qword [mult_result+16]   

	.prepare_second_offset:
		dec r15                    
        mov eax, COMPLEX_size      
        mov edx, r15d ; index
        mul edx

    .call_add:
		movsd xmm2 ,[mult_result+8]          
		movsd xmm3 ,[mult_result+16]  
		SET_POINTER_TO_RDX rdi , rax , 8 
		movsd xmm0, [rdx]
		SET_POINTER_TO_RDX rdi , rax , 16 
		movsd xmm1, [rdx]      
		sub rsp, 88
		call _Cmplx_add                            ; adding (Cn*y0)+Cn-1
		add rsp, 88

	.checks3:
		finit
		fld qword [mult_result+8] 
		fld qword [mult_result+16]  

		movsd xmm2, [add_result+8]                                    
		movsd xmm3, [add_result+16]  

    .check_condition:                    
        cmp r15, 0					
		ja .horner_loop	

	.end:		
        movsd [f_result+16], xmm3                   
        movsd xmm3, [add_result+8]
        movsd [f_result+8], xmm3

    .checks4:
        finit
		fld qword [f_result+8]         
		fld qword [f_result+16] 
                
        leave			; dump the top frame
        ret			; return from main
