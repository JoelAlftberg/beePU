.org 0x0000

; load 0x1337 into r1
LLI r1, 0xFF
LUI r1, 0x80

; load 0x1010 into r3
LLI r3, 0x10
LUI r3, 0x10

; store the value of r3 (0x1010) at the address in r1 (0x1337) in RAM
STA r3, r1

; load the value stored at the address in r1 (0x1337) to r12
LDA r1, r12

; Push r1 (0x80FF) to the stack and call the subroutine
PUSH r1
CALL subroutine

; Load 0xFFFF into r7
LLI r2, 0x0A

; Load 0x0001 into r8
LLI r0, 0x01

; subtract until r0 is zero
loop:
	SUB r0, r2
BNE loop

; Load 0x0004 into r10 and then switch bank
LLI r0, 0x04
MOV r0, r10
BANK

; Load from address stored in r1 (0x1337) in bank 0x4 to r12
LDA r1, r12

; Pop the value we pushed earlier (0x80FF) into r6 
POP r6

end:
	HLT

; Subroutine to test CALL and RET
subroutine:
; load 0x2138 into r1 then copy it to r7
	LLI r1, 0x38
	LUI r1, 0x21
	MOV r1, r7
	RET