# bIS - beePU Instruction Set

The Instruction set for the beeCPU is called bIS which stands for *beePU instruction Set*  
The instructions are two bytes long, the layout of the opcodes are explained in detail below.

## Field layout

### Format selector bits  
The first two bits of the opcode `[15,14]` is used to select which format the instruction is, these are the available formats: 
* `00` - Two-register operations
* `01` - Immediate operations
* `10` - Jump operations
* `11` - Single-register operations

Bits `[9:8]`  are currently unused in all opcodes.

# Instructions

## ADD
`00 0001 -- xxxx xxxx`  

The ADD instruction adds the value of the source register to the value stored in the destination register.   
* `[7:4]` - Source register (value being added)
* `[3:0]` - Destination register (where result is to be stored)

## AND
`00 0101 -- xxxx xxxx`  

The AND instruction does a logical AND operation on the source and destination registers.  
The result of the operation is stored in the destination register.  
Sets the Z Flag in the flag register if the result is zero and the Sign flag if the MSB is 1.

* `[7:4]` - Source register
* `[3:0]` - Destination register (where result is to be stored)

## BEQ
`10 0010 xxxxxxxxxx`  

The BEQ instruction jumps to the specified *signed* offset if the Z flag is set.  
The offset has the range -512 to +511 bytes.
* `[9:0]` - Signed offset in bytes to jump on branch

## BGT
`10 0100 xxxxxxxxxx`  

The BGT instruction jumps to the specified *signed* offset if **neither** the Z flag or N flag is set.  
The offset has the range -512 to +511 bytes.
* `[9:0]` - Signed offset in bytes to jump on branch

## BLT
`10 0101  xxxxxxxxxx`  

The BLT instruction jumps to the specified *signed* offset if the N flag is set.  
The offset has the range -512 to +511 bytes.
* `[9:0]` - Signed offset in bytes to jump on branch

## BNE
`10 0011  xxxxxxxxxx`  

The BNE instruction jumps to the specified *signed* offset if the Z flag is **not** set.  
The offset has the range -512 to +511 bytes.
* `[9:0]` - Signed offset in bytes to jump on branch

## HLT
`11 0001 -- ---- ----`  

The HLT instruction halts the operation of the CPU permanently.  
* Bits `[7:0]` are ignored in this opcode.

## JMP
`10 0001 -- xxxx ----`  

The JMP instruction jumps (sets the PC) to the address stored in the target register  
* `[7:4]` - Target register
* Bits `[3:0]` are ignored in this opcode.

## LDA
`00 0011 -- xxxx xxxx`  

The LDA instruction loads the value at the *address* which the source register holds to the destination register.  
* `[7:4]` -  Source register, containing an address
* `[3:0]` - Destination register (where the data is to be stored)

## MOV
`00 0010 -- xxxx xxxx`  
The MOV instruction loads the value from the source to the destination register.  

* `[7:4]` - Source register (Register to fetch from)
* `[3:0]` - Destination register (where data is to be stored)

## NOP
`11 0010 -- ---- ----`  

The NOP instruction skips one cycle of processing.  
* Bits `[7:0]` are ignored in this opcode.

## NOT
`11 0011 -- xxxx ----`

The NOT instruction performs a logical NOT operation on the register specified, which inverts all it's bits.  
* `[7:4]` - Target register
* Bits `[3:0]` are ignored in this opcode.

## OR
`00 0100 -- xxxx xxxx`  

The OR instruction does a logical OR operation on the source and destination registers.  
The result of the operation is stored in the destination register.  
Sets the Z Flag in the flag register if the result is zero and the Sign flag if the MSB is 1.

* `[7:4]` - Source register
* `[3:0]` - Destination register (where result is to be stored)

## STA
`00 0110 -- xxxx xxxx`

The STA instruction reads value from a register and stores it at the address in the destination register.  
* `[7:4]` - Source register
* `[3:0]` - Destination register (containing address to store value at)

## SUB
`00 0111 -- xxxx xxxx`  

The SUB instruction subtractes the value of the source register from the value stored in the destination register.   
* `[9:8]`  - Unused
* `[7:4]` - Source register (value to subtract)
* `[3:0]` - Destination register (containing value to subtract from)