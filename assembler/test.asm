.org 0x0000

; load 0x1337 into r1
LLI r1, 0x37 
LUI r1, 0x13

; load 0x1010 into r3
LLI r3, 0x10
LUI r3, 0x10

; store the value of r3 (0x1010) at the address in r1 (0x1337) in RAM
STA r3, r1

; load the value stored at the address in r1 (0x1337) to r12
LDA r1, r12

; subtract the value of r12 from the value in r3
SUB r3, r12

; Load 0xFFFF into r7
LLI r2, 0x0A

; Load 0x0001 into r8
LLI r0, 0x01

; subtract until r0 is zero
loop:
	SUB r0, r2
	
BNE loop

end:

HLT