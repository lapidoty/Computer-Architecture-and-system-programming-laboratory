section .data                       ; data section, read-write

        
section .text                       ; our code is always in the .text section
        global  _add 
        global  _sub 
        global  _mult
        global  _div  
        global _compare                    ; makes the function appear in global scope
        extern printf               ; tell linker that printf is defined elsewhere                          ; (not used in the program)


struc locals_for_add
.old_rbp: resq 1
.Loc0: resq 1
.Loc1: resq 1
endstruc

%define local_var(offset) qword [rbp - offset]

%define A_Vector_Size_add local_var(locals_for_add.Loc0)
%define B_Vector_Size_add local_var(locals_for_add.Loc1)

;add two vectors of digits
;
;;int _add(void * v1, void * v2, void ** result)
;
;Parameters:
;   1) rdx - first vector
;   2) rdi - second vector
;   3) r8 - pointer to the result vector ( rbx holds the result address )
;   4) rbx - flag , if 1 goes to r8 no pointer , if -1 goes to r8 is pointer
;Returns:
;      void

_add:                      ; functions are defined as labels
        push rbp             ; save Base Pointer (bp) original value
        mov rbp, rsp        ; use base pointer to access stack contents

        mov A_Vector_Size_add , rsi ; Initilaize ==> A_Vector_Size
        mov byte [rbp-4] , 0
        mov B_Vector_Size_add , rcx ; Initilaize ==> A_Vector_Size
        mov byte [rbp-12] , 0

        cmp r12 , 1
        je .r8_no_pointer
        mov rbx, [r8]
        jmp .r8_is_pointer
        .r8_no_pointer:
             mov rbx , r8
        .r8_is_pointer: 
        mov r12 , 0 

        mov r15, 0x0            ; sets the carry for first time
        mov r14, 0x0            ; sets the progress for first time
        .loop:
            mov r11b, [rdi+r14]         ; puts the LSB of operand1 to LSB of r10
            mov r10b, [rdx+r14]         ; puts the LSB of operand2 to LSB of r10
            cmp B_Vector_Size_add, r14            ; checks whether we finished all the shorter number digits
            jbe .skipline           ; if we did, we skip the next line
            add r11b, r10b          ; added the LSB to r11b
            .skipline:
                add r11b, r15b          ; add the result with the carry
                cmp r11b, 0x09          ; checks if we passed '9'
                ja .carry               ; jump to make a carry
                jmp .notcarry           ; jump in case there isnt a carry
                .carry:
                    sub r11b, 10        ; reduces by 10
                    mov r15b, 0x1       ; saves carry
                    jmp .rest
                .notcarry:  
                    mov r15b, 0x0       ; sets carry to 0
                .rest:
                    mov [rbx+r14], r11b     ; puts answer in the memory of "result" from c
                    inc r14             ; increasing the counter to move forward
                    cmp A_Vector_Size_add, r14       ; checks wheter we finished all the shorter number digits
                    mov r9, [rbx]       ; for testing
                    ja .loop            ; if we didnt, we go do everything again
                    mov [rbx+r14], r15b  ; putting the carry in the last slot of the result
        
         mov     rsp, rbp
         pop     rbp
         ret

struc locals_for_sub
.old_rbp: resq 1
.Loc0: resq 1
.Loc1: resq 1
endstruc

%define local_var(offset) qword [rbp - offset]

%define A_Vector_Size_sub local_var(locals_for_sub.Loc0)
%define B_Vector_Size_sub local_var(locals_for_sub.Loc1)

;sub two vectors of digits
;
;;int _sub(void * v1, void * v2, void ** result)
;
;Parameters:
;   1) rdx - first vector
;   2) rdi - second vector
;   3) r8 - pointer to the result vector ( rbx holds the result address )
;   4) rbx - flag , if 1 goes to r8 no pointer , if -1 goes to r8 is pointer
;Returns:
;      void
_sub:                              ; functions are defined as labels
        push    rbp                 ; save Base Pointer (bp) original value
        mov     rbp, rsp            ; use base pointer to access stack contents

        mov A_Vector_Size_sub , rsi ; Initilaize ==> A_Vector_Size
        mov byte [rbp-4] , 0
        mov B_Vector_Size_sub , rcx ; Initilaize ==> A_Vector_Size
        mov byte [rbp-12] , 0

        cmp r12 , 1
        je .r8_no_pointer
        mov rbx, [r8]
        jmp .r8_is_pointer
        .r8_no_pointer:
             mov rbx , r8
        .r8_is_pointer:
        mov r12 , 0 

        mov r15, 0x0            ; sets the boroow for first time
        mov r14, 0x0            ; sets the progress for first time
        mov r10, 0x0
        mov r11, 0x0
        .loop:
            mov r9, [rbx]       ; for testing
            mov r11b, [rdi+r14]         ; puts the LSB of operand1 to LSB of r11
            mov r10b, [rdx+r14]         ; puts the LSB of operand2 to LSB of r10
            cmp B_Vector_Size_sub, r14            ; checks whether we finished all the shorter number digits
            jbe .skipline           ; if we did, we skip the next line
            sub r11b, r10b          ; sub the LSB_1 to LSB_2

        .skipline:
                sub r11b, r15b          ; add the result with the borrow
                cmp r11b, 0x09          ; checks if negative
                ja .borrow               ; jump to make a borrow
                jmp .notborrow           ; jump in case there isnt a borrow

        .borrow:
                add r11b, 10        ; reduces by 10
                mov r15b, 0x1       ; saves borrow
                jmp .rest

        .notborrow:  
                mov r15b, 0x0       ; sets borrow to 0

        .rest:
                mov [rbx+r14], r11b     ; puts answer in the memory of "result" from c
                inc r14             ; increasing the counter to move forward
                cmp A_Vector_Size_sub, r14       ; checks wheter we finished all the shorter number digits
                mov r9, [rbx]       ; for testing
                ja .loop            ; if we didnt, we go do everything again
                mov [rbx+r14], r15b  ; putting the borrow in the last slot of the result


         mov rbx, [r8]       ; r8 points to an address that points to the beginning of the result char*. this gets the 1st adress
         
         mov     rsp, rbp
         pop     rbp
         ret 

struc locals_for_mult
.old_rbp: resq 1
.Loc0: resq 1
.Loc1: resq 1
endstruc

%define local_var(offset) qword [rbp - offset]

%define A_Vector_Size_mult local_var(locals_for_mult.Loc0)
%define B_Vector_Size_mult local_var(locals_for_mult.Loc1)

_mult:                              ; functions are defined as labels
        push    rbp                 ; save Base Pointer (bp) original value
        mov     rbp, rsp            ; use base pointer to access stack contents

        mov A_Vector_Size_mult , rsi ; Initilaize ==> A_Vector_Size
        mov byte [rbp-4] , 0
        mov B_Vector_Size_mult , rcx ; Initilaize ==> A_Vector_Size
        mov byte [rbp-12] , 0

        mov     r9, rdx             ; put the adress of shorter number in r9. we need rdx available
        xor     r10, r10            ; zero out r10. r10 is going to be counter for carry of adding
        xor     rax, rax            ; zero out rax
        xor     rdx, rdx            ; zero out rdx
        mov     rbx, [r8]           ; puts the adress of the address of the result chars in rbx
        xor     r14, r14            ; zero out r14 for later use, will count for longer number
        xor     r15, r15            ; zero out r15 for later use, will count for shorter number
        .big_loop:
                mov al, [rdi+r14]   ; taking rightmost digit from longer number to rax rightmost digit
                mov r11b, [r9+r15]  ; taking the rightmost digit from shorter number to r11 rightmost digit
                mul r11b       ; multiplying the 2 digits from the 2 bignums
                jmp .check_carry_of_multplying    ; checking if we got carry
                
        .check_carry_of_multplying:
                mov r11b, 10;
                idiv r11b           ;diving rax by 10. the full number stays in rax, while the remainder goes to rdx
                jmp .add_to_result  ; going to add to result the remainder and recursivly, the carry
        
        .add_to_result:
                add r14, r15;
                add r14, 1;
                add [rbx+r14], al ; adding the whole in the correct place in the array of the result
                sub r14, 1;
                shr rax, 8          ; moving the remainder to the lsb
                add [rbx+r14], al; adding the remainder part to whats really there.
                mov r10, r14;
                sub r14, r15;
                jmp .check_carry_of_adding ; gonna do a loop to move
                
        .check_carry_of_adding:
            cmp byte[rbx+r10], 0x09          ; checks if there is carry for each slot
            ja .carry                 ; going to move the carry
            add r10, 1;
            cmp byte[rbx+r10], 0x09          ; checks if there is carry for each slot
            ja .carry                 ; going to move the carry
            jmp .increase_longer
            
        .carry:
            sub byte[rbx+r10], 0x0a         ; reducing 10 from the the overflowing number
            add byte[rbx+r10+1], 0x01        ; adding the carry to the next slot
            add r10b, 1;
            jmp .check_carry_of_adding ;
            
        .increase_longer:
            add r14, 1; adding mult the next slot of longer number
            cmp r14, A_Vector_Size_mult ; check whether we finished with longer number
            je .increase_shorter ;
            jmp .big_loop;
            
        .increase_shorter:
            xor r14,r14 ; zero out r14
            add r15, 1 ;
            cmp r15, B_Vector_Size_mult ; checks whether we finished with shorter number also
            jne .big_loop; starting all over again to multyply next digits


.point:
         mov     rsp, rbp
         pop     rbp
         ret

 ; This part is from mayer class, Code for easy access to arguments & components
 ;****************************************************************
%define A0 rdi
%define A1 rsi
%define A2 rdx
%define A3 rcx
%define A4 r8
%define A5 r9

%define param(offset) qword [rbp + offset]

struc x86frame64bit
.old_rbp: resq 1
.ret_addr: resq 1
.A6: resq 1
.A7: resq 1
.A8: resq 1
.A9: resq 1
.A10: resq 1
.A11: resq 1
endstruc

%define old_rbp param(x86frame64bit.old_rbp)
%define ret_addr param(x86frame64bit.ret_addr)
%define A6 param(x86frame64bit.A6)
%define A7 param(x86frame64bit.A7)
%define A8 param(x86frame64bit.A8)
%define A9 param(x86frame64bit.A9)
%define A10 param(x86frame64bit.A10)
%define A11 param(x86frame64bit.A11)

struc x86frame64bitLocals
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

%define local_var(offset) qword [rbp - offset]

%define A_Vector local_var(x86frame64bitLocals.Loc0)
%define Orign_B_Vector local_var(x86frame64bitLocals.Loc1)
%define Copy_B_Vector local_var(x86frame64bitLocals.Loc2)

%define A_Vector_Size local_var(x86frame64bitLocals.Loc3)
%define Copy_B_Vector_Size local_var(x86frame64bitLocals.Loc4)

%define Help_Vector local_var(x86frame64bitLocals.Loc5)
%define Result_Vector local_var(x86frame64bitLocals.Loc6)

%define Help_Vector_Size local_var(x86frame64bitLocals.Loc7)
%define Orign_B_Vector_Size local_var(x86frame64bitLocals.Loc8)
%define Orign_A_Vector_Size local_var(x86frame64bitLocals.Loc9)
;*****************************************************************

;;int _shift_left(void * v1, int size)
;
;Parameters:
;   1) rdi - vector
;   2) rsi - size of vector
;Returns:
;      void

_shift_left:
        push    rbp                 ; save Base Pointer (bp) original value
        mov     rbp, rsp 
            mov r9 , rsi
            .loop1:
            cmp r9 , 0
            je .end_loop1
            mov r12b , [rdi + r9 - 1]
            mov [rdi + r9] , r12b
            dec r9
            jmp .loop1
            .end_loop1:
            mov r13 , 0
            mov [rdi + r9] , r13b

         mov     rsp, rbp
         pop     rbp
         ret

;;int _shift_right(void * v1, int size)
;
;Parameters:
;   1) rdi - vector
;   2) rsi - size of vector
;Returns:
;      void

_shift_right:
        push    rbp                 ; save Base Pointer (bp) original value
        mov     rbp, rsp 
            mov r9 , 0
            .loop1:
            cmp r9 , rsi
            je .end_loop1
            mov r12b , [rdi + r9 + 1]
            mov [rdi + r9] , r12b
            inc r9
            jmp .loop1
            .end_loop1:
            mov r13 , 0
            mov [rdi + rsi] , r13b

         mov     rsp, rbp
         pop     rbp
         ret

;A ==> Dividend
;B ==> Divisor
;Result ==> A/b
;
;int _div(void * v1, void * v2, void ** result)
;
;Parameters:
;   1) rdi - first vector
;   2) rdx - second vector
;   3) r8 - pointer to the result vector ( rbx holds the result address )
;Returns:
;      void


_div:                              ; functions are defined as labels
        push    rbp                 ; save Base Pointer (bp) original value
        mov     rbp, rsp            ; use base pointer to access stack contents

        .init_vars:                                           
             mov A_Vector , rdi     ; Initilaize ==> A_Vector

             mov Orign_B_Vector , rdx ; Initilaize ==> Orign_B_Vector

             
             mov A_Vector_Size , rsi ; Initilaize ==> A_Vector_Size
             mov Orign_A_Vector_Size , rsi ; Initilaize ==> A_Vector_Size
             mov byte [rbp-28] , 0
             mov byte [rbp-76] , 0


             mov Copy_B_Vector_Size , rcx ; Initilaize ==> B_Vector_Size
             mov Orign_B_Vector_Size , rcx
             mov byte [rbp-36] , 0
             mov byte [rbp-68] , 0

             mov rbx, [r8]          ; Initilaize ==> Result_Vector
             mov Result_Vector , rbx

             mov rbx, [r9]        ; Initilaize ==> Help_Vector
             mov Help_Vector , rbx

             mov rbx , [rbp+16] ; Initilaize ==> Copy_B_Vector
             mov rcx , [rbx]
             mov Copy_B_Vector , rcx
             
             mov r15 , Result_Vector ; *** Test ***
             mov r13 , [r15] ; *** Test ***

        .init_Help_Vector:
                mov rdx , Help_Vector 
                mov byte [rdx] , 1
                mov Help_Vector_Size , 1

        .Copy_Vectors1:
            mov rax , Orign_B_Vector ; Copy_B = Orign_B 
            mov rbx , Copy_B_Vector
            mov rcx , Copy_B_Vector_Size

            mov r9 , 0
            .loop1:
                cmp r9 , rcx
                je .end_loop1
                mov r12b , [rax + r9]
                mov [rbx + r9] , r12b
                inc r9
                jmp .loop1
                .end_loop1:

        .first_loop:
                mov r15 , Result_Vector ; *** Test ***
                mov r13 , [r15] ; *** Test ***
                mov r15 , A_Vector ; *** Test ***
                mov r13 , [r15] ; *** Test ***
                mov r15 , Copy_B_Vector ; *** Test ***
                mov r13 , [r15] ; *** Test ***

                mov rdi , A_Vector ; ==> Preapare params for _compare:
                mov rsi , Copy_B_Vector
                mov rdx , A_Vector_Size
                mov rcx , Copy_B_Vector_Size
                
                sub rsp , 88
                call _compare ; ==> Compares A and B:
                add rsp , 88

                cmp rax , (-1)
                je .B_is_bigger
                jmp .A_is_bigger

        .A_is_bigger: 
                ;************SHL TO B
                mov rdi , Copy_B_Vector ; ==> Preapare params for _shift_left:
                mov rsi , Copy_B_Vector_Size
                
                sub rsp , 88
                call _shift_left  ; ==> Shift left to the Divisor
                add rsp , 88
                inc Copy_B_Vector_Size

                ;************SHL TO HELP
                mov rdi , Help_Vector ; ==> Preapare params for _shift_left:
                mov rsi , Help_Vector_Size
                
                sub rsp , 88
                call _shift_left  ; ==> Shift left to the Divisor
                add rsp , 88
                inc Help_Vector_Size

                ;************

                jmp .first_loop

        .B_is_bigger:
                ;************SHR TO B
                 mov rdi , Copy_B_Vector ; ==> Preapare params for _shift_right:
                 mov rsi , Copy_B_Vector_Size

                 sub rsp , 88
                 call _shift_right
                 add rsp , 88
                 dec Copy_B_Vector_Size

                 ;************SHR TO HELP
                 mov rdi , Help_Vector ; ==> Preapare params for _shift_right:
                 mov rsi , Help_Vector_Size

                
                 mov r14b , [rdi]
                 cmp r14b , 1
                 je .skip_shift
                 sub rsp , 88
                 call _shift_right
                 add rsp , 88
                 dec Help_Vector_Size

                 .skip_shift:

                 ;************A=A-B
                 mov rdi , A_Vector ; ==> Preapare params for _sub:
                 mov rsi , A_Vector_Size
                 mov rdx , Copy_B_Vector
                 mov rcx , Copy_B_Vector_Size
                 mov r8 , A_Vector
                 mov r12 , 1
                 
                 sub rsp , 88
                 call _sub 
                 add rsp , 88

                 .point3:
                 mov r11, A_Vector_Size
                 mov r12 , 0
                 mov r12b , byte [rdi + r11 - 1]
                 cmp  r12, 0
                 je .decrease
                 jmp .not_Dec
                 .decrease:
                 dec A_Vector_Size
                 jmp .point3
                 .not_Dec:


                 .point1:
                ;************B_COPY=0
                 ;mov rdi , Copy_B_Vector ; ==> Preapare params for _sub:
                 ;mov rsi , Copy_B_Vector_Size
                ; mov rdx , Copy_B_Vector
                 ;mov rcx , Copy_B_Vector_Size
                ; mov r8 , Copy_B_Vector
                ; mov rbx , 1
                 ;
                 sub rsp , 88
                 ;call _sub 
                 ;add rsp , 88



                 ;************B_COPY=B_ORIGN
                .Copy_Vectors2:
                     mov rax , Copy_B_Vector 
                     mov rbx , Orign_B_Vector
                    mov rcx , Copy_B_Vector_Size
                    mov r10 , Orign_B_Vector_Size
                    mov Copy_B_Vector_Size , r10

                   mov r9 , 0
                     .loop2:
                      cmp r9 , rcx
                     je .end_loop2
                     mov r12b , [rbx + r9]
                     mov [rax + r9] , r12b
                     inc r9
                        jmp .loop2
                        .end_loop2:


                ;************result = result +help
                mov rdi , Result_Vector ; ==> Preapare params for _add:
                mov rsi , Orign_A_Vector_Size
                mov rdx , Help_Vector
                mov rcx , Orign_A_Vector_Size
                mov r12 , 1
                mov r8 , Result_Vector
.point2:
                
                sub rsp , 88
                call _add ; ==> Add 10 to the Result
                add rsp , 88

                
                ;;************help=1
                mov rdi , Help_Vector ; ==> Preapare params for _add:
                mov rsi , Help_Vector_Size
                mov rdx , Help_Vector
                mov rcx , Help_Vector_Size
                mov r12 , 1
                mov r8 , Help_Vector

                sub rsp , 88
                call _sub ; ==> Add 10 to the Result
                add rsp , 88
                mov rdx , Help_Vector 
                mov byte [rdx] , 1
                mov Help_Vector_Size , 1



                mov rbx , Result_Vector
                ; ==> Compares A and B:
                mov rdi , A_Vector ; ==> Preapare params for _compare:
                mov rsi , Orign_B_Vector
                mov rdx , A_Vector_Size
                ;dec rdx
                mov rcx , Orign_B_Vector_Size

                cmp rdx , rcx
                je .point0
                jmp .point
                .point0:

                .point:
                sub rsp , 88
                call _compare ; ==> Compares A and B:
                add rsp , 88

                cmp rax , (-1)
                je .ret
                jmp .first_loop

        .ret:
                mov r15 , Result_Vector ; *** Test ***
                mov r13 , [r15] ; *** Test ***

         mov     rsp, rbp
         pop     rbp
         ret


;Compares two vectors of digits
;
;;int compare(void * v1, void * v2, int n, int m)
;
;Parameters:
;   1) rdi - first vector
;   2) rsi - second vector
;   3) rdx - first vector length
;   4) rcx - second vector length
;Returns:
;   rax - sign of comparison : -1, 0 or 1

_compare:
        push    rbp                 ; save Base Pointer (bp) original value
        mov     rbp, rsp 
    mov rax , 0
    mov r10 , 0
    cmp rdx, rcx
    jne     .diff_lens  ;check ratio between length_1 and length_2
    
    mov     r9, rdx     ;if (length_1 = length_2) then compare one by one
.loop:
    cmp     r9, 0       ;if (vectors are empty)
    je      .equals     ;then vectors are equal
    dec     r9
    mov     al, [rdi + r9]     ;load digits from biggest to smallest
    mov     r10b, [rsi + r9]     ;and compare
    cmp     al, r10b
    ja      .first_gt
    jb      .second_gt
    jmp     .loop    

.diff_lens:
    cmp     rdx, rcx    ;if (length_1 != length_2)
    jg      .first_gt   ;then compare length_1 and length_2
    jmp     .second_gt

.first_gt:
    mov     rax, 1
    mov     rsp, rbp
         pop     rbp
         ret

.second_gt:
    mov     rax, (-1)
    mov     rsp, rbp
         pop     rbp
         ret

.equals:
    xor     rax, rax
    mov     rsp, rbp
         pop     rbp
         ret
