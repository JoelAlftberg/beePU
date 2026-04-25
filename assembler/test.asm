.org 0x0000
LLI r1, 0x37 ; hejtest
LUI r1, 0x13
LLI r3, 0x10
LUI r3, 0x10 ; lägger in 16 i R3
STA r3, r1
LDA r1, r12
SUB r12, r3
HLT