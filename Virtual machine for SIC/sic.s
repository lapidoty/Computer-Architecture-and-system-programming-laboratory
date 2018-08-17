section .data
	inputFirstFormat:
		dq "%d" , 0
	outputFirstFormat:
		dq "%d " , 0
	
section .bss
	index: resq 1
	pointed_index: resq 1
	ptrArray: resq 1
	word_Counter: resq 1
	
section .text
	global main
	extern printf,scanf
	extern malloc
	extern realloc
	extern free

%macro SET_POINTER_TO_RDX 3
    mov rdx , %1
	add rdx , %2
	add rdx , %3
  %endmacro

main:
	nop
	enter 0, 0		; prepare a frame
	mov qword [index], 0

Allocate_Array:
	mov rax, 1024 	
	inc rax
	sal rax, 2		; Each element will be treated as an int. (eg. count * sizeof(float))
	mov rdi , rax 	
	call malloc 	; We allocate our elements on the heap.
	mov qword [ptrArray] , rax 	; And we save a pointer to the first element in our array.
	mov rax, 1
Reading_Loop:

  .get_offset:

	cmp eax, -1
	je Calculation_Loop

	mov eax, 4
	mov edx, [index] ; index
	mul edx

	SET_POINTER_TO_RDX [ptrArray] , rax , 4

  .call_scanf:
	lea rdi , [inputFirstFormat] 	
	mov rsi , rdx
	call scanf

	mov rbx , [ptrArray]
	inc qword [index]
	inc qword [word_Counter]
	
	
	jmp Reading_Loop

	
	
Calculation_Loop:
dec qword [word_Counter]
mov qword [index] , 0
	.while_loop:
		.check_condition:

				
				; *** while (M[i] || M[i + 1] || M[i + 2]) ***
				mov eax, 4
				mov edx, [index] ; index
				mul edx
				SET_POINTER_TO_RDX [ptrArray] , rax , 4
				mov r11d , [index]
				cmp dword [rdx] , 0
				
				jne .inside_loop

				inc qword [index]
				mov eax, 4
				mov edx, [index] ; index
				mul edx
				SET_POINTER_TO_RDX [ptrArray] , rax , 4
				dec qword [index]
				cmp dword [rdx] , 0
			
				jne .inside_loop
				inc qword [index]
				inc qword [index]
				mov eax, 4
				mov edx, [index] ; index
				mul edx
				SET_POINTER_TO_RDX [ptrArray] , rax , 4
				dec qword [index]
				dec qword [index]
				cmp dword [rdx] , 0
				
				jne .inside_loop
				jmp Print_Memory

		.inside_loop:
			

			; *** if ((M[M[i]] -= M[M[i + 1]]) < 0) ***

				.if_condition:

						; *** r8 = M[M[i]] ***

						mov eax,4
						mov edx, [index] ; index
						mul edx
						SET_POINTER_TO_RDX [ptrArray] , rax , 4

						xor rbx , rbx
						mov ebx , [rdx]
						mov [pointed_index] , rbx 

						mov eax, 4
						mov edx, [pointed_index] ; pointed_index
						mul edx
						SET_POINTER_TO_RDX [ptrArray] , rax , 4

						mov  r8d , [rdx]  

						; *** r9 = M[M[i+1]] ***

						inc qword [index]
						mov eax, 4
						mov edx, [index] ; index
						mul edx
						SET_POINTER_TO_RDX [ptrArray] , rax , 4

						xor rbx , rbx
						mov ebx , [rdx]
						mov [pointed_index] , rbx 

						mov eax, 4
						mov edx, [pointed_index] ; pointed_index
						mul edx
						SET_POINTER_TO_RDX [ptrArray] , rax , 4

						mov  r9d , [rdx] 

						; *** M[M[i]] = M[M[i]] - M[M[i+1]]  ***

						dec qword [index]
						mov eax, 4
						mov edx, [index] ; index
						mul edx
						SET_POINTER_TO_RDX [ptrArray] , rax , 4

						xor rbx , rbx
						mov ebx , [rdx]
						mov [pointed_index] , rbx 

						mov eax, 4
						mov edx, [pointed_index] ; pointed_index
						mul edx
						SET_POINTER_TO_RDX [ptrArray] , rax , 4

						mov r10 , r8
						sub r8 , r9
						mov [rdx] , r8d

						cmp r8 , 0
						jge .else_condition

						; *** i=M[i+2] ***

						.jump_memory: 
							inc qword [index]
							inc qword [index]
							mov eax, 4
							mov edx, [index] ; pointed_index
							mul edx
							SET_POINTER_TO_RDX [ptrArray] , rax , 4
							xor rbx , rbx
							mov ebx , [rdx]
							mov qword [index] , rbx
							jmp .while_loop

						; *** i=i+3 ***

						.else_condition: 
							inc qword [index]
							inc qword [index]
							inc qword [index]
							jmp .while_loop

;*** for (i = 0; i < MAX_PHYSICAL_MEMORY; ++i) { printf("%d ", M[i]); } ***

Print_Memory:

		mov rbx , [ptrArray]
		mov qword [index] , 0

		.print_Loop:

			mov qword r12 , [word_Counter] 
			cmp qword [index] , r12
			je End_Program

			mov eax, 4
			mov edx, [index] ; index
			mul edx
			SET_POINTER_TO_RDX [ptrArray] , rax , 4

			lea rdi , [outputFirstFormat]
			mov rsi , [rdx]
			mov rax , 1
			call printf

			inc qword [index]
			jmp .print_Loop

End_Program:
	leave			
	ret							
					



	