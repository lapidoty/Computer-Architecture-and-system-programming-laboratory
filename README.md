# Compute Complex Roots
" In this assignment, you shall write a program in x86 assembly language, to
find a single root of a polynomial of
complex
coefficients.
This assignment shall require you to work with the x87 subsystem, the
SSE register set, arrays, and interfacing with the C standard library. The
x87 does not support
complex numbers
, so you will need to implement the
representation for complex numbers and the functions defined over them
using support for floating-point numbers in the x87 subsystem. "

# Arbitrary Precision Calculator for Big Numbers

" In this assignment you shall write a postfix-based calculator for integer arith-
metic, somewhat similar to the dc program on Unix/Linux (read over the man page for dc).
The program will be written in a combination of C and x86/64-bit assembly language.
This version works with arbitrary-precision integers, providing functionality similar to that of bignums in Scheme, int in Python,
or LargeInteger in Java.
Your calculator will implement the following functionality:
•
Addition
•
Subtraction
•
Multiplication
•
Division "

# Virtual machine for SIC
" The SIC computer architecture has only one instruction:
SBN A, B, C
.
SBN
stands for
Subtract  &  Branch  if  Negative
.  This is an example of a
one-
instruction computer
, of which many are known. SIC is not a realistic com-
puter architecture, in the sense that it would be worth implementing it in
hardware, but a theoretical one, in the sense that it is Turing-complete.
For this assignment, you will write the following tools:
1. A virtual machine for SIC, emulating the
SBN
instruction. This shall
be written in x86 assembly language.
2. A second virtual machine for SIC, emulating the
SBN
instruction. This
shall be written in SIC assembly language, shall only use the instruction
SBN
, and shall run on top of the virtual machine that you wrote in x86
assembly language. This program is essentially a bunch of numbers
that we too could run on our own virtual machine for SIC.
You are given the code for computing Fibonacci numbers.
The finish-line for this assignment is to be able to run "a tower" consisting
of Fibonacci running on top of (2) running on top of (2) running on top of
(2), and so on, as many times as you like, running on top of (1). The ability
to run such a "tower" of virtual machines will be our demonstration that
your implementation of the virtual machine is correct."
